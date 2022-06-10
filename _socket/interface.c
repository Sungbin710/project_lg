#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "interface.h"

int send_to_server(int clnt_sock, char cmd, char *buf){

    int ret;
    char send_msg[BUFFER_SIZE];

    switch(cmd){
        case REGISTER:
            send_msg[0] = REGISTER;
            memcpy(&send_msg[1], buf, sizeof(UserInfo));
            break;
        case LOGIN:
            send_msg[0] = LOGIN;
            memcpy(&send_msg[1], buf, sizeof(int));
            break;
        case PLAY_MEDIA:
            send_msg[0] = PLAY_MEDIA;
            memcpy(&send_msg[1], buf, sizeof(int) + sizeof(MediaInfo));
            break;
        case END_MEDIA:
            send_msg[0] = END_MEDIA;
            memcpy(&send_msg[1], buf, sizeof(int) + sizeof(MediaInfo));
        case USER_INFO:
            send_msg[0] = USER_INFO;
            memcpy(&send_msg[1], buf, sizeof(int));
        default:
            break;

    }

    /* send 실패시 -1을 반환, errno 값을 변경함 */
    ret = send(clnt_sock, send_msg, sizeof(BUFFER_SIZE), 0);

    return ret;
}


int receive_from_server(int clnt_sock, char *buf, int timeout_ms){
    int recv_len = 0;
    struct timeval timeout;
    fd_set readFds;

    timeout.tv_sec = 0;
    timeout.tv_usec = timeout_ms * 1000;

    FD_ZERO(&readFds);
    FD_SET(fd, &readFds);
    select(fd+1, &readFds, NULL, NULL, &timeout);

    if(FD_ISSET(fd, &readFds))
    {
        recv_len = recv(fd, buf, recv_len, 0);
    }

	return recv_len;

}