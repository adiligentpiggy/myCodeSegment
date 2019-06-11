#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/stat.h>
#include<arpa/inet.h>
#include <errno.h>
#define MAXBUF 256

int main()
{
    int ssock;
    int clen;
    struct sockaddr_in client_addr,server_addr;
    char buf[MAXBUF];
    if((ssock=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP))<0){
        perror("socket error:");
        exit(1);
    }
    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    server_addr.sin_port=htons(7758);

    int opt = 1;
    setsockopt( ssock, SOL_SOCKET,SO_REUSEADDR, (const void *)&opt, sizeof(opt) );
    if(bind(ssock,(struct sockaddr *)&server_addr,sizeof(server_addr))<0){
        perror("bind error:");
        exit(1);
    }
    clen=sizeof(client_addr);
    int recv_len = 0;

    struct timeval tv_out;
    tv_out.tv_sec = 30;// Wait for 30s.
    tv_out.tv_usec = 0;
    // setsockopt to avoid the case that land PA control unit does not notify that Live PA is ended.
    int ret = setsockopt(ssock, SOL_SOCKET, SO_RCVTIMEO, &tv_out, sizeof(tv_out));
    if (ret < 0) {
        printf("setsockopt %s\r\n", strerror(errno));
        exit(-1);
    }

    long i = 0;
    while (1)
    {
        recv_len = recvfrom(ssock,(void *)buf,MAXBUF,0,(struct sockaddr*)&client_addr,&clen);
        printf("The %ldst times recv_len = %d recv buf:%s\r\n", i, recv_len, buf);

        if (recv_len <= 0)
        {
            printf("I will be back! error:%s\r\n", strerror(errno));
            break;
        }
        i++;
    }

    close(ssock);
    return 0;
}
