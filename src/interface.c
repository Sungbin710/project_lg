#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <glib.h>

#include "interface.h"

static GMutex g_callback_lock;

static const Playback_Callbacks *playback_callbacks = NULL;
static void *playback_user_data = NULL;

static const Account_Callbacks *account_callbacks = NULL;
static void *account_user_data = NULL;

static void recv_response(char *buf)
{

	gboolean ret = FALSE;
	char cmd = buf[0];

	g_mutex_lock(&g_callback_lock);

	switch (cmd) {
	case REGISTER:
		if (account_callbacks->get_unique_id) {
			int *received_id = (int *)malloc(sizeof(int));
			memcpy(received_id, buf + 1, sizeof(int));
			ret = account_callbacks->get_unique_id(received_id);
			free(received_id);
		}
		break;
	case LOGIN:
		if (account_callbacks->allow_login) {
			int *is_success = (int *)malloc(sizeof(int));
			memcpy(is_success, &buf[1], sizeof(int));
			ret =
			    account_callbacks->allow_login(is_success,
							   account_user_data);
			free(is_success);
		}
		break;
	case PLAY_MEDIA:
		if (playback_callbacks->get_start_pos) {
			gint64 *received_position =
			    (gint64 *) malloc(sizeof(gint64));
			memcpy(received_position, &buf[1], sizeof(gint64));
			ret =
			    playback_callbacks->
			    get_start_pos(playback_user_data,
					  received_position);
			free(received_position);
		}
		break;
	case USER_INFO:
		if (account_callbacks->get_userinfo) {
			UserInfo *user = (UserInfo *) malloc(sizeof(UserInfo));
			memcpy(user, &buf[1], sizeof(UserInfo));
			ret =
			    account_callbacks->get_userinfo(user->name,
							    &user->age,
							    &user->sex);
			free(user);
		}
		break;
	case RECENT_LIST:
		if (account_callbacks->get_recent_list) {
			Filelist *list = (Filelist *) malloc(sizeof(Filelist));
			memcpy(list, &buf[1], sizeof(Filelist));
			ret =
			    account_callbacks->get_recent_list(list->file_list,
							       &list->size);
			free(list);
		}
		break;
	case POPULAR_LIST:
		if (account_callbacks->get_popular_list) {
			Recommed *reco = (Recommed *) malloc(sizeof(Recommed));
			memcpy(reco, &buf[1], sizeof(Recommed));
			ret =
			    account_callbacks->get_popular_list(reco->file_list,
								&reco->size);
			free(reco);
		}
		break;
	default:
		printf("no cmd\n");
		break;

	}

	g_mutex_unlock(&g_callback_lock);

}

void
playback_install_callbacks(const Playback_Callbacks * callbacks,
			   void *user_data)
{
	playback_callbacks = callbacks;
	playback_user_data = user_data;
}

void
account_install_callbacks(const Account_Callbacks * callbacks, void *user_data)
{
	account_callbacks = callbacks;
	account_user_data = user_data;
}

void *receive_from_server(void *arg)
{

	int clnt_sock = *((int *)arg);
	int cmd;
	char buf[BUFFER_SIZE] = { 0, };

	while (1) {

		if (recv(clnt_sock, buf, BUFFER_SIZE, 0) == -1) {
			printf("recv error\n");
			exit(1);
		}
		recv_response(buf);
		memset(buf, 0, BUFFER_SIZE);

	}
}

int
register_user_to_server(int sockfd, char name[16], unsigned int age,
			unsigned int sex)
{

	char send_buf[BUFFER_SIZE] = { 0, };
	char cmd = REGISTER;
	UserInfo user;
	/* 임의 값 */
	user.id = -1;
	strcpy(user.name, name);
	user.age = age;
	user.sex = sex;

	send_buf[0] = cmd;
	memcpy(&send_buf[1], &user, sizeof(UserInfo));

	/* send 실패시 -1을 반환, errno 값을 변경함 */
	if (send(sockfd, send_buf, BUFFER_SIZE, 0) == -1) {
		perror("send: ");
		return 1;
	}

	return 0;
}

int login_to_server(int sockfd, int id)
{

	char send_buf[BUFFER_SIZE] = { 0, };
	char cmd = LOGIN;

	send_buf[0] = cmd;
	memcpy(&send_buf[1], &id, sizeof(int));

	/* send 실패시 -1을 반환, errno 값을 변경함 */
	if (send(sockfd, send_buf, BUFFER_SIZE, 0) == -1) {
		perror("send: ");
		return 1;
	}

	return 0;
}

int request_userinfo_to_server(int sockfd, int id)
{

	char send_buf[BUFFER_SIZE] = { 0, };
	char cmd = USER_INFO;

	send_buf[0] = cmd;
	memcpy(&send_buf[1], &id, sizeof(int));

	/* send 실패시 -1을 반환, errno 값을 변경함 */
	if (send(sockfd, send_buf, BUFFER_SIZE, 0) == -1) {
		perror("send: ");
		return 1;
	}

	return 0;
}

int
request_last_playback_to_server(int sockfd, int id, char *file_name,
				long file_size)
{

	char send_buf[BUFFER_SIZE] = { 0, };
	char cmd = PLAY_MEDIA;
	MediaInfo media = { 0, };

	media.id = id;
	strcpy(media.file_name, file_name);
	media.file_size = file_size;
	/* 임의 값 */
	media.played = 0;

	send_buf[0] = cmd;
	memcpy(&send_buf[1], &media, sizeof(MediaInfo));

	/* send 실패시 -1을 반환, errno 값을 변경함 */
	if (send(sockfd, send_buf, BUFFER_SIZE, 0) == -1) {
		perror("send: ");
		return 1;
	}

	return 0;
}

int
update_last_playback_to_server(int sockfd, int id, char *file_name,
			       long file_size, gint64 last_pos)
{

	char send_buf[BUFFER_SIZE] = { 0, };
	char cmd = END_MEDIA;
	MediaInfo media = { 0, };

	media.id = id;
	strcpy(media.file_name, file_name);
	media.file_size = file_size;
	media.played = last_pos;

	send_buf[0] = cmd;
	memcpy(&send_buf[1], &media, sizeof(MediaInfo));

	/* send 실패시 -1을 반환, errno 값을 변경함 */
	if (send(sockfd, send_buf, BUFFER_SIZE, 0) == -1) {
		perror("send: ");
		return 1;
	}

	return 0;
}

int request_recent_list_to_server(int sockfd, int id, int size)
{

	char send_buf[BUFFER_SIZE] = { 0, };
	char cmd = RECENT_LIST;

	send_buf[0] = cmd;
	memcpy(&send_buf[1], &id, sizeof(int));
	memcpy(&send_buf[5], &size, sizeof(int));

	/* send 실패시 -1을 반환, errno 값을 변경함 */
	if (send(sockfd, send_buf, BUFFER_SIZE, 0) == -1) {
		perror("send: ");
		return 1;
	}

	return 0;

}

int request_popular_list_to_server(int sockfd, int sex, int size)
{

	char send_buf[BUFFER_SIZE] = { 0, };
	char cmd = POPULAR_LIST;

	send_buf[0] = cmd;
	memcpy(&send_buf[1], &sex, sizeof(int));
	memcpy(&send_buf[5], &size, sizeof(int));

	/* send 실패시 -1을 반환, errno 값을 변경함 */
	if (send(sockfd, send_buf, BUFFER_SIZE, 0) == -1) {
		perror("send: ");
		return 1;
	}

	return 0;
}
