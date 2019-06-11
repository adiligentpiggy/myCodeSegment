#include <stdio.h>
#include <sys/stat.h>
typedef unsigned char uint8;
typedef unsigned int uint32;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
#define  ARR_SIZE(a)  (sizeof((a)) / sizeof((a[0])))

//#define TEST_DATA 0
enum
{
    AT_INIT_STEP_AT = 1,
    AT_INIT_STEP_PIN,
    AT_INIT_STEP_UNLOCK_PIN,
    AT_INIT_STEP_CSQ,
    AT_INIT_STEP_CREG,
    AT_INIT_STEP_CONFIG_APN,
    AT_INIT_STEP_ACTIVATE_PDP,
    AT_INIT_STEP_SETUP_SOCKET,
    AT_INIT_STEP_CONNECT_BROKER_AT,
    AT_INIT_STEP_SUBSCRIBE_AT,
    AT_INIT_STEP_PUBLISH_AT,
    AT_INIT_STEP_PING_AT,
    AT_INIT_STEP_RESPONSE_AT,
    AT_INIT_STEP_SEND_CHECK,
    AT_INIT_STEP_CHECK_SOCKET_STATUS,
    AT_INIT_STEP_BUSINESS_READ,
    AT_INIT_STEP_BUSINESS_PARSE_DATA,
    AT_INIT_STEP_BUSINESS_IDLE,
    AT_INIT_STEP_CLOSE_SOCKET,
    AT_INIT_STEP_DEACTIVATE_PDP,
    AT_INIT_STEP_RESET,
    AT_INIT_STEP_WAITFOR_RESP,
};

static char* getStepName(uint8 step)
{
    uint8 index = step - AT_INIT_STEP_AT;
    char* pstr[] = {
        "---->>>>  AT_INIT_STEP_AT\r\n",
        "---->>>>  AT_INIT_STEP_PIN\r\n",
        "---->>>>  AT_INIT_STEP_UNLOCK_PIN\r\n",
        "---->>>>  AT_INIT_STEP_CSQ\r\n",
        "---->>>>  AT_INIT_STEP_CREG\r\n",
        "---->>>>  AT_INIT_STEP_CONFIG_APN\r\n",
        "---->>>>  AT_INIT_STEP_ACTIVATE_PDP\r\n",
        "---->>>>  AT_INIT_STEP_SETUP_SOCKET\r\n",
        "---->>>>  AT_INIT_STEP_CONNECT_BROKER_AT\r\n",
        "---->>>>  AT_INIT_STEP_SUBSCRIBE_AT\r\n",
        "---->>>>  AT_INIT_STEP_PUBLISH_AT\r\n",
        "---->>>>  AT_INIT_STEP_PING_AT\r\n",
        "---->>>>  AT_INIT_STEP_RESPONSE_AT\r\n",
        "---->>>>  AT_INIT_STEP_SEND_CHECK\r\n",
        "---->>>>  AT_INIT_STEP_CHECK_SOCKET_STATUS\r\n",
        "---->>>>  AT_INIT_STEP_BUSINESS_READ\r\n",
        "---->>>>  AT_INIT_STEP_BUSINESS_PARSE_DATA\r\n",
        "---->>>>  AT_INIT_STEP_BUSINESS_IDLE\r\n",
        "---->>>>  AT_INIT_STEP_CLOSE_SOCKET\r\n",
        "---->>>>  AT_INIT_STEP_DEACTIVATE_PDP\r\n",
        "---->>>>  AT_INIT_STEP_RESET\r\n",
        "---->>>>  AT_INIT_STEP_WAITFOR_RESP\r\n",
    };

    if (index >= ARR_SIZE(pstr))
        return "---->>>>  INVALID STEP \r\n";
    else
        return pstr[index];
}

void dfu_data_pkt_construct(uint8 * p_buf, uint32 * p_size)
{
    uint8 image_buf[512] = {0};
    uint8 * p_img_buf = image_buf;
    uint8 count = 20;//m_image_len / (DATA_PACKET_MAX_SIZE + 1) + 1;
    uint8 i = 0;
    uint32 read_bytes = 0;
    uint8 * p_esc = NULL;
    FILE * p_file = NULL;
    char* m_full_path_file_name = "/home/sunhongzhe/bin/tools/README.txt";
    struct stat buf;

    // set data packet type
    //set_pkt_type(DATA_PACKET,p_buf);
    //*p_size += PKT_TYPE_SIZE;
    //p_buf += PKT_TYPE_SIZE;

    // open dfu file, read 512 bytes
    p_file = fopen((char *)m_full_path_file_name, "rb+");
    if (p_file == NULL)
    {
        printf("open dfu file failed!");
        exit(1);
        return;
    }
    
    stat(m_full_path_file_name, &buf);
     printf("%s file size = %d\n", m_full_path_file_name, buf.st_size);

    for(; i < count; i++)
    {
    	  int j = 0;
    	  uint32 read_len = 0;
        if ((read_len = fread(p_img_buf, 1, 512, p_file)) <= 0)
        {
            printf("\nread failed, errno is %d  read_bytes=%d\n", ferror(p_file), read_bytes);
            fclose(p_file);
            return;
        }
        
        read_bytes += read_len;

         //printf("read length is %d %c\n", read_bytes, p_img_buf[0]);
         for (j = 0; j < read_len; j++)
         {
         	putchar(p_img_buf[j]);
         }
         
        //strncpy((char *)p_buf, (const char *)p_img_buf, read_bytes);
        //hci_transport_pkt_write(m_dfu_tx_buffer, read_bytes);
        //rx_ack_pkt();
    }

    fclose(p_file);
}

uint16_t crc16_compute(uint8_t const * p_data, uint32_t size, uint16_t const * p_crc)
{
	  uint32_t i = 0;
    uint16_t crc = (p_crc == NULL) ? 0xFFFF : *p_crc;

    for (i = 0; i < size; i++)
    {
        crc  = (uint8_t)(crc >> 8) | (crc << 8);
        crc ^= p_data[i];
        crc ^= (uint8_t)(crc & 0xFF) >> 4;
        crc ^= (crc << 8) << 4;
        crc ^= ((crc & 0xFF) << 4) << 1;
    }

    return crc;
}


int main()
{
	uint8 arr[] ={0xC9,  0x4E, 01, 0xE8, 03, 00, 00, 00, 04, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
0x9A, 02, 01, 00};
  uint16_t sum = crc16_compute(arr, ARR_SIZE(arr), NULL);
  printf("sum=%x \r\n", sum);
	#ifdef TEST_DATA
	printf("step =%d name=%s", 22, getStepName(22));
	#else
	printf("nothing done \r\n");
	#endif
	//dfu_data_pkt_construct(NULL, NULL);
	return 0;
}