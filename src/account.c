#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <glib.h>

#include "account.h"
#include "interface.h"

static Account_Callbacks account_callbacks = { 0 };

static GCond register_cond;
static GMutex register_lock;

static GCond login_cond;
static GMutex login_lock;

static GCond request_cond;
static GMutex request_lock;

static GCond userinfo_cond;
static GMutex userinfo_lock;

static GCond popular_list_cond;
static GMutex popular_list_lock;

// before login : 1 / after login : 2
static int user_state = 1;
static int user_id = -1;

int get_user_state()
{
	return user_state;
}

int get_user_id()
{
	return user_id;
}

static void ClearLineFromReadBuffer(void)
{
	while (getchar() != '\n') ;	//  문자열의 끝 '엔터키'가 들어올때까지 계속 Read가 되어짐. 즉, 버퍼의 값을 사용하지 않고 그냥 버려짐.
}

static int input_userid(int *id)
{

	/* 초기화 */
	*id = -1;

	printf("아이디를 입력해주세요.\n");
	printf("id: ");
	scanf("%d", id);
	if (*id < 0) {
		printf("잘못 입력하셨습니다.\n");
		ClearLineFromReadBuffer();
		return 1;
	}

	return 0;
}

static int input_username(char *name)
{

	printf
	    ("이름은 15글자이하로 공백없이 입력해주세요.\n");
	printf("이름: ");
	scanf("%15s", name);
	ClearLineFromReadBuffer();
	name[15] = '\0';

	return 0;
}

static int input_userage(unsigned int *age)
{

	printf
	    ("나이를 입력해주세요.\n");
	printf("나이: ");
	scanf("%u", age);
	if (*age < 0 || *age > 150) {
		printf("잘못 입력하셨습니다.\n");
		ClearLineFromReadBuffer();
		return 1;
	}
	return 0;
}

static int input_usersex(unsigned int *sex)
{

	printf("남자면 0, 여자면 1을 입력해주세요.\n");
	printf("성별: ");
	scanf("%u", sex);
	if (!(*sex == 0 || *sex == 1)) {
		printf("잘못된 값입니다.\n");
		ClearLineFromReadBuffer();
		return 1;
	}

	return 0;
}

int input_userinfo(char *name, unsigned int *age, unsigned int *sex)
{

	printf("===================================\n");
	printf("사용자 등록을 위한 정보를 입력해주세요.\n");

	while (input_username(name) != 0) ;

	while (input_userage(age) != 0) ;

	while (input_usersex(sex) != 0) ;

	printf("정상적으로 사용자 정보가 입력되었습니다.\n");

	return 0;
}

static gboolean get_unique_id_cb(int *id)
{
	if (*id < 0) {
		printf("Error\n");
		return FALSE;
	}
	printf("your id: %d \n", *id);

	g_mutex_lock(&register_lock);
	g_cond_signal(&register_cond);
	g_mutex_unlock(&register_lock);

	return TRUE;
}

/* callback 등록 */
static void register_init()
{
	account_callbacks.get_unique_id = get_unique_id_cb;
	account_install_callbacks(&account_callbacks, NULL);
}

int register_user(int clnt_sock)
{

	char name[16];
	unsigned int age;
	unsigned int sex;

	/* install callback */
	register_init();

	while (input_userinfo(name, &age, &sex) != 0) ;

	/* 서버에 전송하기 전에 lock을 걸어 */
	/* g_cond_wait()보다 callback 함수의 g_cond_signal()이 먼저 호출되는 경우를 방지한다. */
	g_mutex_lock(&register_lock);

	register_user_to_server(clnt_sock, name, age, sex);

	g_cond_wait(&register_cond, &register_lock);
	g_mutex_unlock(&register_lock);

	return 0;

}

static gboolean allow_login_cb(int *is_success, int *id)
{

	if (!(*is_success)) {
		user_id = *id;
		user_state = 2;
		printf("ID: %d login success\n", user_id);
	} else {
		printf("login fail\n");
	}

	g_mutex_lock(&login_lock);
	g_cond_signal(&login_cond);
	g_mutex_unlock(&login_lock);

	return TRUE;
}

/* callback 등록 */
static void login_init(int *id)
{

	// Account_Callbacks account_callbacks = { 0 };
	account_callbacks.allow_login = allow_login_cb;
	account_install_callbacks(&account_callbacks, id);
}

int login(int clnt_sock)
{

	int try_id;
	login_init(&try_id);

	while (input_userid(&try_id) != 0) ;
	/* 서버에 전송하기 전에 lock을 걸어 */
	/* g_cond_wait()보다 callback 함수의 g_cond_signal()이 먼저 호출되는 경우를 방지한다. */
	g_mutex_lock(&login_lock);

	login_to_server(clnt_sock, try_id);

	g_cond_wait(&login_cond, &login_lock);
	g_mutex_unlock(&login_lock);

}

static gboolean get_recent_list_cb(char file[][256], int *list_size)
{
	// printf("login_cb called\n");

	int len = *list_size;

	if (len <= 0) {
		printf("There is no list\n");
	}

	if (len > 5) {
		len = 5;
	}

	printf("========최근 재생 목록========\n");
	for (int i = 1; i <= len; ++i) {
		printf("%d. %s\n", i, file[i - 1]);
	}
	printf("===================\n");

	g_mutex_lock(&request_lock);	// (3)
	g_cond_signal(&request_cond);
	g_mutex_unlock(&request_lock);	// (4)

	return TRUE;
}

static void request_recent_list_init()
{

	account_callbacks.get_recent_list = get_recent_list_cb;
	account_install_callbacks(&account_callbacks, NULL);
}

int request_recent_list(int sockfd)
{

	int list_size = LIST_SIZE;

	request_recent_list_init();

	/* 서버에 전송하기 전에 lock을 걸어 */
	/* g_cond_wait()보다 callback 함수의 g_cond_signal()이 먼저 호출되는 경우를 방지한다. */
	g_mutex_lock(&request_lock);

	request_recent_list_to_server(sockfd, get_user_id(), list_size);

	g_cond_wait(&request_cond, &request_lock);
	g_mutex_unlock(&request_lock);

}

static gboolean
get_userinfo_cb(char *name, unsigned int *age, unsigned int *sex)
{
	printf("이름: %s\n", name);
	printf("나이: %u\n", *age);
	if (*sex == 0)
		printf("성별: 남\n");
	else if (*sex == 1)
		printf("성별: 여\n");

	g_mutex_lock(&userinfo_lock);	// (3)
	g_cond_signal(&userinfo_cond);
	g_mutex_unlock(&userinfo_lock);	// (4)

	return TRUE;
}

static void request_userinfo_init()
{

	account_callbacks.get_userinfo = get_userinfo_cb;
	account_install_callbacks(&account_callbacks, NULL);
}

int request_userinfo(int sockfd)
{

	request_userinfo_init();

	/* 서버에 전송하기 전에 lock을 걸어 */
	/* g_cond_wait()보다 callback 함수의 g_cond_signal()이 먼저 호출되는 경우를 방지한다. */
	g_mutex_lock(&userinfo_lock);

	request_userinfo_to_server(sockfd, get_user_id());

	g_cond_wait(&userinfo_cond, &userinfo_lock);
	g_mutex_unlock(&userinfo_lock);

}

static gboolean get_popular_list_cb(char file[][256], int *list_size)
{

	int len = *list_size;
	if (len <= 0) {
		printf("There is no list\n");
	}

	if (len > 5) {
		len = 5;
	}

	printf("========인기 순위========\n");
	for (int i = 1; i <= len; ++i) {
		printf("%d. %s\n", i, file[i - 1]);
	}
	printf("===================\n");

	g_mutex_lock(&popular_list_lock);
	g_cond_signal(&popular_list_cond);
	g_mutex_unlock(&popular_list_lock);

	return TRUE;
}

static void request_popular_list_init()
{

	account_callbacks.get_popular_list = get_popular_list_cb;
	account_install_callbacks(&account_callbacks, NULL);
}

int request_popular_list(int sockfd)
{

	request_popular_list_init();
	int choice;
	int list_size = LIST_SIZE;

	printf("1. 남성 인기 미디어 컨텐츠\n");
	printf("2. 여성 인기 미디어 컨텐츠\n");
	printf("3. 전체 인기 미디어 컨텐츠\n");
	printf("선택: ");
	scanf("%d", &choice);

	if (choice < 1 || choice > 3) {
		printf("incorrect number\n");
		return 0;
	}

	/* 서버에 전송하기 전에 lock을 걸어 */
	/* g_cond_wait()보다 callback 함수의 g_cond_signal()이 먼저 호출되는 경우를 방지한다. */
	g_mutex_lock(&popular_list_lock);

	request_popular_list_to_server(sockfd, choice - 1, list_size);

	g_cond_wait(&popular_list_cond, &popular_list_lock);
	g_mutex_unlock(&popular_list_lock);

}

int logout()
{
	user_state = 1;
	return 0;
}
