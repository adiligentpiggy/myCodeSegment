#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/stat.h>
#include<arpa/inet.h>
#include<errno.h>
#define MAXBUF 256

int main()
{
    int ssock;
    int clen;
    struct sockaddr_in client_addr,server_addr;
    char buf[MAXBUF];
    strcpy(buf,"I miss you already!\n");
    if((ssock = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP))<0){
        perror("socket error:\n");
        exit(1);
    }

    int optval = 1;
    setsockopt(ssock, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval));

    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family  =AF_INET;
    server_addr.sin_addr.s_addr=inet_addr("127.0.0.1"/*"10.4.20.255"*/);
    server_addr.sin_port=htons(7758);

    long i = 0;
    while (1)
    {
        if (-1 == sendto(ssock,(void *)buf,MAXBUF,0,(struct sockaddr*)&server_addr,sizeof(server_addr)))
        {
            printf("sendto fail, errno=%s\n", strerror(errno));
        }
        printf("%ld Send message:%s\n", i, buf);
        i++;
        sleep(2);
    }

    /*strcpy(buf, "");
    sendto(ssock,(void *)buf,strlen(buf),0,(struct sockaddr*)&server_addr,sizeof(server_addr));
    printf("%d Send message:%s\n", i, buf);*/
    close(ssock);
    return 0;
}
