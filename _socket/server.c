#include <stdio.h>
#include <stdlib.h> //atoi를 사용하려면 있어야함
#include <string.h> // memset 등
#include <unistd.h> //sockaddr_in, read, write 등
#include <arpa/inet.h>  //htnol, htons, INADDR_ANY, sockaddr_in 등
#include <sys/types.h>
#include <sys/socket.h>


typedef struct UserInfo{
    unsigned int id;
    char name[16];
    unsigned int age;
    unsigned int sex;
} UserInfo;


void error_handling(char * message);

int main(int argc, char* argv[])
{    
    int serv_sock;
    int clnt_sock;
   
    //sockaddr_in은 소켓 주소의 틀을 형셩해주는 구조체로 AF_INET일 경우 사용
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr; //accept함수에서 사용됨.
    socklen_t clnt_addr_size;
   
    //TCP연결지향형이고 ipv4 도메인을 위한 소켓을 생성
    serv_sock=socket(PF_INET, SOCK_STREAM, 0); 
    if(serv_sock == -1)
         error_handling("socket error");
    
    //주소를 초기화한 후 IP주소와 포트 지정
    memset(&serv_addr, 0, sizeof(serv_addr)); 
    serv_addr.sin_family=AF_INET;                //타입: ipv4
    serv_addr.sin_addr.s_addr=htonl(INADDR_ANY); //ip주소
    serv_addr.sin_port=htons(atoi(argv[1]));     //port
    
    //소켓과 서버 주소를 바인딩
    if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr))==-1)
    	error_handling("bind error");
    
    //연결 대기열 5개 생성 
    if(listen(serv_sock, 5)==-1)
    	error_handling("listen error");
    
    //클라이언트로부터 요청이 오면 연결 수락
    clnt_addr_size = sizeof(clnt_addr);
    clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
    if(clnt_sock==-1)
        error_handling("accept error");
        
    /*-----데이터 전송-----*/

    printf("연결 되었습니다\n");

    char recv_msg[128];

    // write(clnt_sock, msg, sizeof(UserInfo) + 1);
    recv(clnt_sock, recv_msg, sizeof(UserInfo) + 1, 0);
    
    int cmd = recv_msg[0];


	// register 
	/*
    UserInfo *user = (UserInfo *)(recv_msg+1);
    
    printf("cmd: %d \n", cmd);
    printf("id: %u \n", user->id);
    printf("name: %s \n", user->name);
    printf("age: %u \n", user->age);
    printf("sex: %u \n", user->sex);
	
	free(user);

	char send_msg[128];
    send_msg[0] = 0;
    int success = 0;
    memcpy(&send_msg[1], &success, sizeof(int));

    send(clnt_sock, send_msg, sizeof(int) + 1, 0);

	printf("send complete\n");
	*/
	
/*
	// login
	unsigned int *id = (unsigned int *)(recv_msg+1);
	
	//
	// manage in server
	printf("cmd: %d\n", cmd);
	printf("id: %u\n", *id);

	//

	char send_msg[128];
    send_msg[0] = 1;
    int success = 0;
    memcpy(&send_msg[1], &success, sizeof(int));

    send(clnt_sock, send_msg, sizeof(int) + 1, 0);

	printf("send complete\n");
*/
	while(1){

	};

    //소켓들 닫기
    close(clnt_sock);
    close(serv_sock);
    
    return 0;
}
void error_handling(char *message)
{
    fputs(message,stderr);
    fputc('\n', stderr);
    exit(1);
}
