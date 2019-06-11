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

void* receive_pcm_thread(void *args)
{
    struct sockaddr_in server_addr;

    //int    client_addr_len;
    struct ip_mreq mreq;
    struct hostent *group;
    struct in_addr ia;
    play_pcm_params *params = (play_pcm_params *)args;
    int rate = params->rate;
    uint16 format = params->format;
    uint16 channels = params->channels;
    const char* grp_ip = get_live_broadcast_ip();
    uint32 port = get_live_broadcast_port();

    if (params->cb != NULL) {
        (*params->cb)(params->arg);
    }
    free(params);

    //if((sock_handle = socket(AF_INET,SOCK_STREAM, 0))<0)
    if((sock_handle = socket(AF_INET,SOCK_DGRAM, 0))<0)
    {
        dzlog_error("socket error: %s", strerror(errno));
        return NULL;
    }
    dzlog_debug("receive pcm from group ip:%s port:%d", grp_ip, port);
    /* ¨¦¨¨??¨°a?¨®¨¨?¡Á¨¦2£¤¦Ì?¦Ì??¡¤ */
    memset(&mreq, 0, sizeof(mreq));

    if ((group = gethostbyname(grp_ip)) == (struct hostent *) 0) {
        dzlog_error("gethostbyname %s", strerror(errno));
        exit(errno);
    }
    memcpy(&ia, group->h_addr, group->h_length);
    /* ¨¦¨¨??¡Á¨¦¦Ì??¡¤ */
    memcpy(&mreq.imr_multiaddr.s_addr, &ia, sizeof(ia));
    /* ¨¦¨¨??¡¤¡é?¨ª¡Á¨¦2£¤???¡é¦Ì??¡ä?¡Â?¨²¦Ì?¦Ì??¡¤D??¡é */
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    /* ¡ã?¡À??¨²?¨®¨¨?¡Á¨¦2£¤¦Ì??¡¤¡ê??¡ä¡À??¨²¨ª??¡§¡Á¡Â?a¡Á¨¦2£¤3¨¦?¡À¡ê???¨®D?¨®¨¨?¡Á¨¦2??¨¹¨º?¦Ì?¡Á¨¦2£¤???¡é */
    if (setsockopt
            (sock_handle, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq,
             sizeof(struct ip_mreq)) == -1) {
        dzlog_error("setsockopt %s", strerror(errno));
        exit(SOCKET_ERROR);
    }

    int set = 1;
    if (setsockopt(sock_handle, SOL_SOCKET, SO_REUSEADDR, &set, sizeof(int)) < 0) {
        dzlog_error("setsockopt %s", strerror(errno));
        exit(SOCKET_ERROR);
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
      /*nb = listen(sock_handle, 1);
      if(nb == -1) {
          perror("listen error\n");
          return NULL;
      }

      dzlog_info("listen the port:");

      client_addr_len = sizeof(client_addr);*/
    pcminfo = get_free_pcm_handle(NULL, get_event_pri_by_index(LIVE_INDEX),
        ABANDON_FOCUS_AFTER_END, decode_buf.data);
    if (NULL == pcminfo) {
        dzlog_error("get_free_pcm_handle failed");
        return NULL;
    }

    if (0 != snd_wav_set_params(pcminfo, rate,
            format/*SND_PCM_FORMAT_S16_LE*/, channels)) {
        dzlog_error("snd_wav_set_params failed");
        return NULL;
    }

#ifdef SAVE_PCM_FILE
    WAVContainer_t wav;
    char *filename = get_pcm_file_name();

    if ((pcm_fd = open(filename, O_WRONLY | O_CREAT, 0644)) == -1) {
        dzlog_error("Error open: %s", filename);
        goto Done;
    }
    SNDWAV_PrepareWAVParams(&wav, channels, rate, 16, 60*60*24/*/ 24 hours*/);
    if (WAV_WriteHeader(pcm_fd, &wav) < 0) {
        goto Done;
    }
#endif

    play_pcm = true;
    pthread_create(&play_pcm_thread_id, NULL, play_pcm_data, NULL);
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
        exit(NO_MEM);
    }

    while(1)
    {
        dzlog_debug("waiting for pcm data\n");
        int buf_len = 0;
        /*cli_sockfd = accept(sock_handle, (struct sockaddr *)&client_addr, &client_addr_len);
        if(cli_sockfd == -1) {
            perror("accept error\n");
            goto Done;
        }*/

        //while (1) {
            //buf_len = recv(cli_sockfd, sock_data, rcv_size, 0);
            struct sockaddr_in addr;
            socklen_t addrlen = sizeof(struct sockaddr_in);
            buf_len = recvfrom(sock_handle, sock_data, rcv_size, 0, (struct sockaddr * )&addr,&addrlen);
            //dzlog_info("%d bytes pcm data got", buf_len);
            if ((buf_len == 1 && sock_data[0] == LIVE_TALK_END_MARK)
                || buf_len == 0
                || buf_len == -1)
            {
                dzlog_info("pcm stream finished");
                goto Done;
            }
            pthread_mutex_lock (&buf_mutex);
            int cplen = min(buf_len, ARRAY_SIZE(recv_buf.data) - recv_buf.count);
            memcpy(recv_buf.data + recv_buf.count, sock_data, cplen);
            recv_buf.count += cplen;
            dzlog_debug("recv_buf.count =%d", recv_buf.count);
            pthread_mutex_unlock(&buf_mutex);
            //char buf[10];
            //send(cli_sockfd, buf, 10, 0);
       //}
    }
Done:
    play_pcm = false;
    free(sock_data);
    pthread_join(play_pcm_thread_id, NULL);
    snd_pcm_drain(pcminfo->handle);
    snd_pcm_close(pcminfo->handle);
    reset_pcm_handle(pcminfo);
    if (cli_sockfd != -1) {
        close(cli_sockfd);
    }
    close(sock_handle);
#ifdef SAVE_PCM_FILE
    close(pcm_fd);
#endif
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
