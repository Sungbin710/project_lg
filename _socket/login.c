#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>

#include "login.h"
#include "interface.h"

// before login : 1 / after login : 2
int user_state = 1;
unsigned int user_id = -1;

int register_user(int clnt_sock){
    
	UserInfo user;
	
    printf("사용자 등록을 위한 정보를 입력해주세요.\n");
//    printf("ID: "); scanf("%u, ", &user.id);
    printf("under 16 letters.\n");
    printf("name: "); scanf("%16s, ", user.name);
    printf("age: "); scanf("%u, ", &user.age);
    printf("남자면 0, 여자면 1을 입력해주세요.\n");
    printf("sex: "); scanf("%u, ", &user.sex);

	if(send_to_server(clnt_sock, REGISTER, (char *)&user) != 0){
		printf("error int send_to_server function\n");	
		return -1;
	}
	
	unsigned int receive_id;
	if(receive_from_server(clnt_sock, REGISTER, (char *)&receive_id, TIMEOUT_MS) != 0){
		printf("error int receive_from_server function\n");	
		return -1;
	}

	printf("===================\n");
	printf("your ID is %d\n", receive_id); 
	printf("===================\n");
	return 0;
    
}


int login(int clnt_sock){
	printf("login success\n");
	user_id = 1;
	user_state = 2;
	return 0;
}

/*
int login(int clnt_sock){
 
	int try_id;
    printf("로그인을 위해 ID를 입력해주세요.\n");
    printf("ID: "); scanf("%d, ", &try_id);

	if(send_to_server(clnt_sock, LOGIN, (char *)&try_id) != 0){
		printf("error int send_to_server function\n");	
		return -1;
	}

	int is_success;
	if(receive_from_server(clnt_sock, LOGIN, (char *)&is_success, TIMEOUT_MS) != 0){
		printf("error int receive_from_server function\n");	
		return 1;
	}

	if(!is_success){
		printf("login success\n");
		user_id = try_id;
		user_state = 2;
		return 0;
	}
	else{
		printf("login fail\n");
		return 1;
	}

}
*/
