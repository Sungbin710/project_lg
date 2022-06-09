#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>


int recv_with_timeout(int fd, char *buf, int buf_size, int timeout_ms)
{
    int rx_len = 0;
    struct    timeval  timeout;
    fd_set  	readFds;

    // recive time out config
    // Set 1ms timeout counter
    timeout.tv_sec  = 0;
    timeout.tv_usec = timeout_ms*1000;   

    FD_ZERO(&readFds);
    FD_SET(fd, &readFds);
    select(fd+1, &readFds, NULL, NULL, &timeout);

    if(FD_ISSET(fd, &readFds))
    {
        rx_len = recv(fd, buf, buf_size, 0);
    }

	return rx_len;
}