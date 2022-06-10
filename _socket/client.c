#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "custom_struct.h"
#include "login.h"


#include <bsd/unistd.h>


extern int user_state;

void error_handling(char* message);


int main(int argc, char* argv[], char *envp[])
{
	setproctitle_init(argc, argv, envp);
	int clnt_sock;
	/*
	int clnt_sock;
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
	
			printf("enter the number\n");
			printf("1. register\n");
			printf("2. login\n");
			printf("3. exit\n");
			scanf("%d", &choice_num);

			switch(choice_num){
				case 1:
					register_user(clnt_sock);
					break;
				case 2:
					login(clnt_sock);	
					break;
				case 3:
					printf("exit program\n");
					exit(0);
				default:
					break;
			}
			

		}
		else if(user_state == 2){
			
			int choice_num;		

			printf("enter the number\n");
			printf("1. play media\n");
			printf("2. print user information\n");
			printf("3. logout\n");
			printf("4. exit\n");
			scanf("%d: ", &choice_num);

			char *playlist[1000];
			int play_num = -1;
			int ret = -1;
			switch(choice_num){
				case 1:
					// 자료구조
					
					ret = print_playlist(playlist, &play_num);
					if(ret == 0){ // SUCCESS
						//파일 이름 입력 받기
  						setproctitle("%s",playlist[play_num]);	
						play_media(clnt_sock, playlist[play_num]);
					}
					// for(int i=0; i<1000; ++i)
					// 	free(playlist[i]);
						
					break;
				case 2:
					Print_UserInfo();
					break;
				case 3:
					printf("ID: %u is logout\n");
					user_state = 1; 
					break;
				case 4:
					printf("exit program\n");
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
