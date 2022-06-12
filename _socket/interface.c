#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "interface.h"

int send_to_server(int sockfd, char cmd, char *buf){

    char send_buf[BUFFER_SIZE];

    switch(cmd){
        case REGISTER:
            send_buf[0] = REGISTER;
            memcpy(&send_buf[1], buf, sizeof(UserInfo));
            break;
        case LOGIN:
            send_buf[0] = LOGIN;
            memcpy(&send_buf[1], buf, sizeof(int));
            break;
        case PLAY_MEDIA:
            send_buf[0] = PLAY_MEDIA;
            memcpy(&send_buf[1], buf, sizeof(int) + sizeof(MediaInfo));
            break;
        case END_MEDIA:
            send_buf[0] = END_MEDIA;
            memcpy(&send_buf[1], buf, sizeof(int) + sizeof(MediaInfo));
        case USER_INFO:
            send_buf[0] = USER_INFO;
            memcpy(&send_buf[1], buf, sizeof(int));
        default:
            break;

    }

    /* send 실패시 -1을 반환, errno 값을 변경함 */
    if(send(sockfd, send_buf, sizeof(BUFFER_SIZE), 0) == -1){
		perror("send: ");	
		return -1;
	}

    return 0;
}


int receive_from_server(int sockfd, int cmd,  char *buf, int timeout_ms){
	
	char recv_buf[BUFFER_SIZE];
    struct timeval timeout;
    fd_set readFds;

    timeout.tv_sec = 0;
    timeout.tv_usec = timeout_ms * 1000;
	
	/* readFds init */
    FD_ZERO(&readFds);
	/* Add sockfd to readFds */
    FD_SET(sockfd, &readFds);
	/* wait fd change within timeout */ 
    if(select(sockfd+1, &readFds, NULL, NULL, &timeout) == -1){
		perror("select error: ");
		return -1;
	}

    if(FD_ISSET(sockfd, &readFds))
    {
		/* recv 실패시 -1을 반환, errno 값을 변경함 */
        if(recv(sockfd, &recv_buf, BUFFER_SIZE, 0) == -1){
			perror("recv: ");
			return -1;
		}
		
		if(cmd != recv_buf[0]){
			printf("incorrect cmd is received");
			return -1;
		}	

		switch(cmd){
			case REGISTER:
				/* receive ID */
				memcpy(buf, &recv_buf[1], sizeof(unsigned int));
				break;
			case LOGIN:
				/* receive is_success */
				memcpy(buf, &recv_buf[1], sizeof(int));
				break;
			case PLAY_MEDIA:
				memcpy(buf, &recv_buf[1], sizeof(signed long));
				break;
			/*	
			case END_MEDIA:
				break;
			*/
			case USER_INFO:
				memcpy(buf, &recv_buf[1], sizeof(UserInfo));
				break;
			default:
				printf("not developed yet\n");
				break;

		}

		return 0;
	}

	printf("timeout occured\n");
	return -1;

}
