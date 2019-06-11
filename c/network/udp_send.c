#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
//#include <alsa/asoundlib.h>
#include<sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "primi_types.h"

int pa_send(const uint8 * p_buf, uint32 buf_len, const char* dst_ip, uint32_t port)
{
    int fd;
    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        dzlog_error("socket error:%s", strerror(errno));
        return false;
    }
    int optval = 1;
    setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &optval,
            sizeof(optval));
    struct sockaddr_in addrs;
    memset(&addrs, 0, sizeof(struct sockaddr_in));
    addrs.sin_family = AF_INET;
    addrs.sin_addr.s_addr = inet_addr(dst_ip);
    addrs.sin_port = htons(port);

    int sendBytes;
    if ((sendBytes = sendto(fd, p_buf, buf_len, 0, (struct sockaddr *) &addrs,
            sizeof(struct sockaddr))) == -1)
    {
        dzlog_error("sendto fail, errno=%s", strerror(errno));
        goto Err;
    }
    printf("%s has been sent to port :%d\n", p_buf, port);
Err:
    close(fd);
    return true;
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
	
	
	while (1)
	{
		char buffer[100] = {0}; 
		int bytes_read = read(0, buffer, 100);  
		if (bytes_read > 0)
		{
			pa_send(buffer, bytes_read, multi_broadcast_ip, port);
		}
	}	
    
    
    return 0;
}
