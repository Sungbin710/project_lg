#include <glib.h>
#include <glib-object.h>

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>

void error_handling(char *message);
void *receive_from_server(void *arg);

int main(int argc, char* argv[])
{

    // int sts;
	// pthread_t thread_id;

    int clnt_sock;

	//connect_to_server();
	
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
	
    // if((sts=pthread_create((&thread_id),NULL,receive_from_server,&clnt_sock))!=0){
	// 	perror("error\n\n");
	// 	exit(1);
	// }
    
    gchar *name = "receive_mod"; 
    // gpointer receive_from_server(void *arg);
    GThread *thread1 = g_thread_new(name, receive_from_server, (void *)&clnt_sock);
    
    while(1){

    }
	//통신 후 소켓 클로우즈
	close(clnt_sock);
	return 0;
}

static void print_buf(){
    // printf("buf: %s\n", buf);
    printf("callback\n");
}

void *receive_from_server(void *arg)
{

    int clnt_sock = *((int *)arg);
    // printf("sockfd: %d\n", clnt_sock);
    int i=0;
    char buf[512];  
    GString *buffer = g_string_new(NULL);
    g_signal_connect(G_OBJECT(buffer),"notify", (GCallback)print_buf, NULL);
    // g_signal_connect(G_OBJECT(&i),"notify", (GCallback)print_buf, NULL);


    while(1){
        printf("%d\n", ++i);
        
        // sleep(1);
        if(recv(clnt_sock, buf, 512, 0) == 0){
            
        }
        
        buffer = g_string_assign(buffer, buf);
        g_print("%s", buffer->str);
        // printf("buffer: %s\n", buffer);
        // memset(buffer,0, sizeof(buffer));
    }

    // for(int i=0; i<5; ++i){
    //     printf("%d\n", i);
    // }
    
}


void error_handling(char *message)
{
    fputs(message,stderr);
    fputc('\n', stderr);
    exit(1);
}