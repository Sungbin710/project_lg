#include <gtk/gtk.h>
#include <gst/gst.h>
#include <gst/video/videooverlay.h>
#include <gst/pbutils/pbutils.h>

#include <dirent.h>

#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/stat.h>		/* stat -> file size */
#include "account.h"
#include "interface.h"
#include "playback.h"
#include "discover.h"
#include "playlist.h"

void error_handling(char *message);

int connect_to_server(int *argc, char *argv[]);

int main(int argc, char *argv[], char *envp[])
{
	int state;
	int clnt_sock;
	if ((clnt_sock = connect_to_server(&argc, argv)) == 0) {
		printf("서버와의 연결에 실패했습니다.\n");
	}

	/* make recv thread */
	gchar *name = "receive_thread";
	GThread *receive_thread =
	    g_thread_new(name, receive_from_server, (void *)&clnt_sock);

	while (1) {
		state = get_user_state();

		if (state == 1) {

			int choice_num = 0;
			printf("===================\n");
			printf("enter the number\n");
			printf("1. 아이디 발급\n");
			printf("2. 로그인\n");
			printf("3. 종료\n");
			printf("===================\n");
			printf("선택: ");
			scanf("%d", &choice_num);

			switch (choice_num) {
			case 1:
				register_user(clnt_sock);
				break;
			case 2:
				login(clnt_sock);
				break;
			case 3:
				printf("프로그램을 종료합니다.\n");
				close(clnt_sock);
				exit(0);
			default:
				printf
				    ("잘못된 번호를 선택하셨습니다.\n");
				break;
			}

		} else if (state == 2) {

			int choice_num = 0;
			printf("===================\n");
			printf("enter the number\n");
			printf("1. 미디어 컨텐츠 재생\n");
			printf("2. 미디어 컨텐츠 상세 정보 보기\n");
			printf("3. 사용자 정보 보기\n");
			printf("4. 최근 재생 목록\n");
			printf("5. 인기 미디어 컨텐츠 추천\n");
			printf("6. 로그아웃\n");
			printf("7. 종료\n");
			printf("===================\n");
			printf("선택: ");
			scanf("%d", &choice_num);

			switch (choice_num) {
			case 1:
				playback_media(clnt_sock);
				break;
			case 2:
				print_media_info();
				break;
			case 3:
				request_userinfo(clnt_sock);
				break;
			case 4:
				request_recent_list(clnt_sock);
				break;
			case 5:
				request_popular_list(clnt_sock);
				break;
			case 6:
				logout();
				break;
			case 7:
				printf("프로그램을 종료합니다.\n");
				close(clnt_sock);
				exit(0);
			default:
				break;
			}
		}

	};

	close(clnt_sock);
	return 0;
}

int connect_to_server(int *argc, char *argv[])
{

	int clnt_sock;

	struct sockaddr_in serv_addr;

	//TCP연결지향형이고 ipv4 도메인을 위한 소켓을 생성
	clnt_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (clnt_sock == -1) {
		error_handling("socket error");
	}
	//인자로 받은 서버 주소 정보를 저장
	memset(&serv_addr, 0, sizeof(serv_addr));
	//서버주소체계는 IPv4이다
	serv_addr.sin_family = AF_INET;
	//서버주소 IP저장해주기(인자로 받은거 넘겨주기)
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	//서버주소 포트번호 인자로 받은거 저장해주기
	serv_addr.sin_port = htons(atoi(argv[2]));

	//클라이언트 소켓부분에 서버를 연결!
	if (connect(clnt_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr))
	    == -1)
		error_handling("connect error");

	return clnt_sock;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
