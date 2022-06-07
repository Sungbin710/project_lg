#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "login.h"
#include "custom_struct.h"
#include "interface_cmd.h"

// before login : 1 / after login : 2
int user_state = 1;
unsigned int user_id;

int register_user(int clnt_sock){
    
    UserInfo *user = (UserInfo *)malloc(sizeof(UserInfo));

    printf("사용자 등록을 위한 정보를 입력해주세요.\n");
    printf("ID: "); scanf("%u, ", &user->id);
    printf("under 15 letters.\n");
    printf("name: "); scanf("%s, ", user->name);
    printf("age: "); scanf("%u, ", &user->age);
    printf("남자면 0, 여자면 1을 입력해주세요.\n");
    printf("sex: "); scanf("%u, ", &user->sex);
    
    char *send_msg = (char *)malloc(sizeof(UserInfo) + 1);
    send_msg[0] = REGISTER;
    memcpy(send_msg+1, user, sizeof(UserInfo));
    send(clnt_sock, send_msg, sizeof(UserInfo) + 1, 0);
    

    // send_msg 해제
    free(user);
    free(send_msg);

	printf("send complete\n");

    char *recv_msg = (char *)malloc(sizeof(int) + 1);
	memset(recv_msg, 0, sizeof(int) +1);
    
    recv(clnt_sock, recv_msg, sizeof(int) + 1, 0);
    char cmd = recv_msg[0];
    int is_register = -1;
    memcpy(&is_register, recv_msg+1, sizeof(int)); 
	
	free(recv_msg);
/*
	printf("recv complete\n");
	printf("cmd: %d \n", cmd);
	printf("is_register: %d\n", is_register);
*/
    // 정상 수신
    if(cmd == REGISTER){
        if(!is_register){
            printf("success register\n");
            return 0;
        }
        else{
            printf("failure register\n");
            return 1;
        }
    }
    else{
        printf("잘못된 값이 수신되었습니다.");
        return 1;
    }
	
    
}

int login(int clnt_sock){
    
	char msg[BUFFER_SIZE];	
	
    printf("로그인을 위해 ID를 입력해주세요.\n");
    printf("ID: "); scanf("%d, ", &user_id);

	msg[0] = LOGIN;
	memcpy(&msg[1], &user_id, sizeof(unsigned int));

    send(clnt_sock, msg, sizeof(unsigned int) + 1, 0);
	
	printf("login request send\n");

	memset(msg, 0, BUFFER_SIZE);

	recv(clnt_sock, msg, BUFFER_SIZE, 0);
	char cmd = msg[0];
	int is_login;
	memcpy(&is_login, &msg[1], sizeof(int));

	// normal receive
	if(cmd == LOGIN){

		if(!is_login){
			printf("login success\n");
			user_state = 2;
			return 0;
		}
		else{
			printf("login fail\n");
			return 1;
		}

	}
    else{
        printf("잘못된 값이 수신되었습니다.");
        return 1;
    }


}
