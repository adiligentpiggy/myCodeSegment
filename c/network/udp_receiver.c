#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
//#include <alsa/asoundlib.h>
#include<sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "primi_types.h"
//#include "zlog.h"

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
    dzlog_debug("receive pcm from group ip:%s port:%d\n", grp_ip, port);
    /* ¨¦¨¨??¨°a?¨®¨¨?¡Á¨¦2£¤¦Ì?¦Ì??¡¤ */
    memset(&mreq, 0, sizeof(mreq));

    if ((group = gethostbyname(grp_ip)) == (struct hostent *) 0) {
        dzlog_error("gethostbyname %s", strerror(errno));
        exit(errno);
    }
    memcpy(&ia, group->h_addr, group->h_length);
    
    memcpy(&mreq.imr_multiaddr.s_addr, &ia, sizeof(ia));
    
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);//inet_addr("10.4.1.233");
    
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
	
	/*struct timeval tv_out;
    tv_out.tv_sec = 10;// Wait for 30s.
    tv_out.tv_usec = 0;
    // setsockopt to avoid the case that land PA control unit does not notify that Live PA is ended.
    int ret = setsockopt(sock_handle, SOL_SOCKET, SO_RCVTIMEO, &tv_out, sizeof(tv_out));
    if (ret < 0) {
        dzlog_error("setsockopt %s", strerror(errno));
        exit(1);
    }*/

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
    dzlog_debug("get socket rcv buf size =%d\n", rcv_size);
    uint8 *sock_data = malloc(rcv_size);
    if (sock_data == NULL) {
        dzlog_error("malloc failed\n");
        exit(1);
    }

    while(1)
    {
        dzlog_debug("waiting for pcm data\n");
        int buf_len = 0;
		memset(sock_data, 0, rcv_size);
      
            struct sockaddr_in addr;
            socklen_t addrlen = sizeof(struct sockaddr_in);
            buf_len = recvfrom(sock_handle, sock_data, rcv_size, 0, (struct sockaddr * )&addr,&addrlen);
            dzlog_info("%d bytes pcm data got: %s\n", buf_len, sock_data);
			if (-1 == buf_len) goto Done;
    

            //dzlog_debug("rcv_size =%d", rcv_size);

          
    }
Done:
    //play_pcm = false;
    free(sock_data);
    
   
    close(sock_handle);

    dzlog_info("receive_pcm_thread end\n");
    return NULL;
}

int main(int argc, char *argv[]) {
	const char* multi_broadcast_ip = NULL;
    //const char* multi_broadcast_port = NULL;
    int port = 0;
    
	if (argc != 3) {
		dzlog_info("argc:%d !=3\n", argc);
        exit(1);
    }
    
    multi_broadcast_ip = argv[1];
    port = atoi(argv[2]);
    
    //const char* grp_ip = "224.1.0.1";//"239.0.0.1";//get_live_broadcast_ip();
    //uint32 port = 8861;//get_live_broadcast_port();
    receive_pcm_thread(multi_broadcast_ip, port);
    
    return 0;
}
