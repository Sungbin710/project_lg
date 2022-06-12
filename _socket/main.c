#include <gtk/gtk.h>
#include <gst/gst.h>
#include <gst/video/videooverlay.h>
#include <gst/pbutils/pbutils.h>

#include <sys/types.h>
#include <bsd/unistd.h>
#include <dirent.h>

#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/stat.h> /* stat -> file size */
#include "login.h"
#include "interface.h"
#include "play_media.h"
#include "discover_media.h"

extern int user_state;
extern unsigned int user_id;

int main(int argc, char* argv[], char *envp[])
{
	setproctitle_init(argc, argv, envp);
	int clnt_sock;
	

	//connect_to_server();
	/*
	struct sockaddr_in serv_addr;

	//TCP연결지향형이고 ipv4 도메인을 위한 소켓을 생성
	clnt_sock = socket(PF_INET, SOCK_STREAM, 0);
	if(clnt_sock == -1)
		error_handling("socket error");

	//인자로 받은 서버 주소 정보를 저장
	memset(&serv_addr, 0, sizeof(serv_addr));
	//서버주소체계는 IPv4이다
	serv_addr.sin_family = AF_INET;                  
	//서버주소 IP저장해주기(인자로 받은거 넘겨주기)
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);  
	//서버주소 포트번호 인자로 받은거 저장해주기
	serv_addr.sin_port = htons(atoi(argv[2]));

	//클라이언트 소켓부분에 서버를 연결!
	if(connect(clnt_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("connect error");
	*/

    
	while(1){
		
		if(user_state == 1){
		
			int choice_num;		
			printf("===================\n");
			printf("enter the number\n");
			printf("1. register\n");
			printf("2. login\n");
			printf("3. exit\n");
			printf("===================\n");
			printf("choice: "); scanf("%d", &choice_num);

			switch(choice_num){
				case 1:
					register_user(clnt_sock);
					break;
				case 2:
					login(clnt_sock);	
					break;
				case 3:
					printf("exit program\n");
					close(clnt_sock);
					exit(0);
				default:
					break;
			}
			

		}
		else if(user_state == 2){
			
			int choice_num;		
			printf("===================\n");
			printf("enter the number\n");
			printf("1. play media\n");
			printf("2. print media information\n");
			printf("3. print user information\n");
			printf("4. logout\n");
			printf("5. exit\n");
			printf("===================\n");
			printf("choice: "); scanf("%d", &choice_num);


			
			switch(choice_num){
				case 1:
					play_media(clnt_sock);

					break;
				case 2:
					print_media_info();
					// Print_UserInfo();
					break;
				case 3:
					
					// Print_UserInfo();
					break;
				case 4:
					printf("ID: %u is logout\n", user_id);
					user_state = 1; 
					break;
				case 5:
					printf("exit program\n");
					close(clnt_sock);
					exit(0);
				default:
					break;
			}
		}


	};

	
	//통신 후 소켓 클로우즈
	close(clnt_sock);
	return 0;
}
void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
