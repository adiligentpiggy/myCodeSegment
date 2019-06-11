#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
//#include <alsa/asoundlib.h>
#include<sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "primi_types.h"

#define dzlog_debug printf
#define dzlog_error printf
#define dzlog_info printf

void* receive_pcm_thread(const char* grp_ip, uint32 port)
{
    struct sockaddr_in server_addr;

    //int    client_addr_len;
    struct ip_mreq mreq;
    struct hostent *group;
    struct in_addr ia;

    int sock_handle = 0;
    /*if (params->cb != NULL) {
        (*params->cb)(params->arg);
    }
    free(params);*/

    //if((sock_handle = socket(AF_INET,SOCK_STREAM, 0))<0)
    if((sock_handle = socket(AF_INET,SOCK_DGRAM, 0))<0)
    {
        dzlog_error("socket error: %s", strerror(errno));
        return NULL;
    }
    dzlog_debug("receive pcm from group ip:%s port:%d", grp_ip, port);
    /* ����??��a?����?����2����?��??�� */
    memset(&mreq, 0, sizeof(mreq));

    if ((group = gethostbyname(grp_ip)) == (struct hostent *) 0) {
        dzlog_error("gethostbyname %s", strerror(errno));
        exit(errno);
    }
    memcpy(&ia, group->h_addr, group->h_length);
    /* ����??������??�� */
    memcpy(&mreq.imr_multiaddr.s_addr, &ia, sizeof(ia));
    /* ����??����?������2��???���??��?��?����?��??��D??�� */
    mreq.imr_interface.s_addr = inet_addr("10.4.1.112");//htonl(INADDR_ANY);
    /* ��?��??��?����?����2����??����??���??����??������?a����2��3��?����???��D?����?����2??����?��?����2��???�� */
    if (setsockopt
            (sock_handle, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq,
             sizeof(mreq)) == -1) {
        dzlog_error("setsockopt %s", strerror(errno));
        exit(1);
    }

    int set = 1;
    if (setsockopt(sock_handle, SOL_SOCKET, SO_REUSEADDR, &set, sizeof(int)) < 0) {
        dzlog_error("setsockopt %s", strerror(errno));
        exit(1);
    }

    int nb = 0;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);

    if(bind(sock_handle, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        dzlog_error("bind error:%s", strerror(errno));
        return NULL;
    }
 
    int rcv_size = 10000;
    socklen_t optlen;
    optlen = sizeof(rcv_size);
    nb = getsockopt(sock_handle, SOL_SOCKET, SO_RCVBUF, &rcv_size, &optlen);
    if (nb < 0) {
        dzlog_error("get socket rcv buf size failed");
    }
    dzlog_debug("get socket rcv buf size =%d", rcv_size);
    uint8 *sock_data = malloc(rcv_size);
    if (sock_data == NULL) {
        dzlog_error("malloc failed");
        exit(1);
    }

    while(1)
    {
        dzlog_debug("waiting for pcm data\n");
        int buf_len = 0;
      
            struct sockaddr_in addr;
            socklen_t addrlen = sizeof(struct sockaddr_in);
            buf_len = recvfrom(sock_handle, sock_data, rcv_size, 0, (struct sockaddr * )&addr,&addrlen);
            dzlog_info("%d bytes pcm data got", buf_len);
    

            //dzlog_debug("rcv_size =%d", rcv_size);

          
    }
Done:
    //play_pcm = false;
    free(sock_data);
    
   
    close(sock_handle);

    dzlog_info("receive_pcm_thread end\n");
    return NULL;
}

#define LOCALPORT 12378
#define LINE_HELLO_GROUP "239.0.0.1"
#define  LUDP_MAX  1024

int receiveHandler(const char* local_ip){
    int sockListen = 0;
    struct ip_mreq mreq = {0};
    if((sockListen = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
        dzlog_info("receiveHandler socket fail\n");
        return -1;
    }
    int set = 1;
    setsockopt(sockListen, SOL_SOCKET, SO_REUSEADDR, &set, sizeof(int));
    struct sockaddr_in recvAddr;
    memset(&recvAddr, 0, sizeof(struct sockaddr_in));

    recvAddr.sin_family = AF_INET;
    recvAddr.sin_port = htons(LOCALPORT);
    recvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    // ??����????��????????|????��??3???��???
    if(bind(sockListen, (struct sockaddr *)&recvAddr, sizeof(struct sockaddr)) == -1){
        dzlog_info("receive port:%d, bind fail\n", LOCALPORT);
        return -1;
    }

    /* use setsockopt() to request that the kernel join a multicast group */
    mreq.imr_multiaddr.s_addr=inet_addr(LINE_HELLO_GROUP);
    //?-��?��????��???���� INADDR_ANY ????��1?o??��???��?????o??��??����?????????����??��??? IP ??��??�?�?
    //??|???????????o??��???????��-??��?��?????��3????????��??�� ?�?
    mreq.imr_interface.s_addr= inet_addr(local_ip);

    if (setsockopt(sockListen, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0)
    {
        //perror("setsockopt");
        dzlog_info("%s, add line group cast fail:%s\n", __FUNCTION__, strerror(errno));
        return -1;
    }

    int recvbytes;
    int recvlen = 1500; //UDP max len.
    char recvbuf[recvlen];
    //char *tokseps = ",";
    //char *pt;
    uint32 addrLen = sizeof(struct sockaddr_in);
    uint16 msgid = 0;
    uint16 msgContentLen = 0;

    //media_data *pdata = NULL;

    while(1) {
        memset(recvbuf, 0, sizeof(recvbuf));
        if((recvbytes = recvfrom(sockListen, recvbuf, recvlen, 0,
            (struct sockaddr *)&recvAddr, &addrLen)) != -1)
        {
            //?��a��??????????��??��??��?????��?�?
            if(recvbytes >= LUDP_MAX)
            {
                dzlog_info("recv message too long :%d\n", recvbytes);
                continue;
            }
            recvbuf[recvbytes] = 0; //clip more data.

        }else{
            dzlog_info("recvfrom fail\n");
        }

        //��??????o??��???�C?????1?�?
        //pthread_testcancel();
    }
    setsockopt(sockListen, IPPROTO_IP, IP_DROP_MEMBERSHIP, &mreq, sizeof(mreq));
    close(sockListen);
    dzlog_info("%s exit line:%d\n", __FUNCTION__, __LINE__);
    return 0;
}

int main(int argc, char *argv[]) {
	const char* multi_broadcast_ip = NULL;
    //const char* multi_broadcast_port = NULL;
    int port = 0;
#if 0    
	if (argc != 3) {
		dzlog_info("argc:%d !=3\n", argc);
        exit(1);
    }
    
    multi_broadcast_ip = argv[1];
    port = atoi(argv[2]);
    
    //const char* grp_ip = "224.1.0.1";//"239.0.0.1";//get_live_broadcast_ip();
    //uint32 port = 8861;//get_live_broadcast_port();
    receive_pcm_thread(multi_broadcast_ip, port);
#else
    if (argc != 2) {
		dzlog_info("argc:%d !=2\n", argc);
        exit(1);
    }
    receiveHandler(argv[1]);
#endif
    
    return 0;
}
