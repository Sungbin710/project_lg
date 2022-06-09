#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <errno.h>

#include "login.h"
#include "custom_struct.h"
#include "interface_cmd.h"
#include "connect.h"

// before login : 1 / after login : 2
int user_state = 1;
unsigned int user_id;

int register_user(int clnt_sock){
    
    /* 전송할 구조체 메모리 할당 */
    UserInfo *user = (UserInfo *)malloc(sizeof(UserInfo));
    if(user == NULL){
        printf("FAIL: Memory Allocation\n");
        return -1;
    }

    /* 사용자로부터 사용자 정보 입력 */
    printf("사용자 등록을 위한 정보를 입력해주세요.\n");
    printf("ID: "); scanf("%u, ", &user->id);
    printf("under 16 letters.\n");
    printf("name: "); scanf("%16s, ", user->name);
    printf("age: "); scanf("%u, ", &user->age);
    printf("남자면 0, 여자면 1을 입력해주세요.\n");
    printf("sex: "); scanf("%u, ", &user->sex);
    
    /* 서버로 전송 */
    char *send_msg = (char *)malloc(sizeof(UserInfo) + 1);
    send_msg[0] = REGISTER;
    memcpy(send_msg+1, user, sizeof(UserInfo));
    /* send 실패시 -1을 반환하고, errno값을 변경함 */
    if(send(clnt_sock, send_msg, sizeof(UserInfo) + 1, 0) == -1){
        perror("send: ");
    }
    
    /* 할당한 메모리 해제 */
    free(user);
    free(send_msg);

    /* 수신할 메모리 버퍼 할당 */
    char *recv_msg = (char *)malloc(sizeof(int) + 1);
	memset(recv_msg, 0, sizeof(int) +1);
    
    /* recv 실패시 -1을 반환하고, errno값을 변경함 */
    // if(recv(clnt_sock, recv_msg, sizeof(int) + 1, 0) == -1){
    //     perror("recv: ");
    // }
    if(recv_with_timeout(clnt_sock, recv_msg, sizeof(int) + 1, TIMEOUT) == -1){
        perror("recv: ");
        return 1;
    }

    /* 수신한 데이터 파싱 */
    char cmd = recv_msg[0];
    int is_register;
    memcpy(&is_register, recv_msg+1, sizeof(int)); 
	
    /* 할당한 메모리 해제 */
	free(recv_msg);

    /* 정상 수신 */
    if(cmd == REGISTER){
        /* is_register이 0이면 success, 그 외에는 failure */
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
        printf("false value is received\n");
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
