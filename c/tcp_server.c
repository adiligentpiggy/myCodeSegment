#include <sys/types.h>  
#include <sys/socket.h>  
#include <stdio.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include <unistd.h>  
#include <string.h>  
#include <stdlib.h>  
#include <fcntl.h>  
#include <sys/shm.h>  
  
#define PORT  8890  
#define QUEUE_SIZE   10  
#define BUFFER_SIZE 1024  
  
void str_echo(int sockfd)    
{    
    char buffer[BUFFER_SIZE];  
    pid_t pid = getpid();  
    while(1)  
    {  
        memset(buffer,0,sizeof(buffer));  
        int len = recv(sockfd, buffer, sizeof(buffer),0);  
        if(strcmp(buffer,"exit\n")==0)  
        {  
        printf("child process: %d exited.\n",pid);  
            break;  
        }  
    printf("pid:%d receive:\n",pid);  
        fputs(buffer, stdout);  
        send(sockfd, buffer, len, 0);  
    }  
    close(sockfd);  
}  
  
int main(int argc, char **argv)   
{  
    //����IPV4��TCP���ӵ��׽���������  
    int server_sockfd = socket(AF_INET,SOCK_STREAM, 0);  
  
    //����sockaddr_in  
    struct sockaddr_in server_sockaddr;  
    server_sockaddr.sin_family = AF_INET;  
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);  
    server_sockaddr.sin_port = htons(PORT);  
  
    //bind�ɹ�����0��������-1  
    if(bind(server_sockfd,(struct sockaddr *)&server_sockaddr,sizeof(server_sockaddr))==-1)  
    {  
        perror("bind");  
        exit(1);//1Ϊ�쳣�˳�  
    }  
    printf("bind success.\n");    
  
    //listen�ɹ�����0��������-1������ͬʱ֡����������ΪQUEUE_SIZE  
    if(listen(server_sockfd,QUEUE_SIZE) == -1)  
    {  
        perror("listen");  
        exit(1);  
    }  
    printf("listen success.\n");  
  
    for(;;)  
    {  
        struct sockaddr_in client_addr;  
        socklen_t length = sizeof(client_addr);   
        //����������accept�ϣ��ɹ����طǸ������֣�������-1  
        int conn = accept(server_sockfd, (struct sockaddr*)&client_addr,&length);   
        if(conn<0)  
        {  
            perror("connect");  
            exit(1);  
        }  
        printf("new client accepted.\n");  
          
        pid_t childid;  
        if(childid=fork()==0)//�ӽ���  
        {  
            printf("child process: %d created.\n", getpid());  
            close(server_sockfd);//���ӽ����йرռ���    
            str_echo(conn);//�������������    
            exit(0);  
        }  
    }  
  
    printf("closed.\n");  
    close(server_sockfd);  
    return 0;  
}  