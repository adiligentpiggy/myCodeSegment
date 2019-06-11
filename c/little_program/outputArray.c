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
        //exit(1);
        return;
    }
    
    stat(m_full_path_file_name, &buf);
     printf("%s file size = %d\n", m_full_path_file_name, (int)buf.st_size);

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

//置换IP表
const static char IP_Table[64] = {
        58,50,42,34,26,18,10,2,
        60,52,44,36,28,20,12,4,
        62,54,46,38,30,22,14,6,
        64,56,48,40,32,24,16,8,
        57,49,41,33,25,17,9,1,
        59,51,43,35,27,19,11,3,
        61,53,45,37,29,21,13,5,
        63,55,47,39,31,23,15,7
};
//逆置换IP-1表
const static char IPR_Table[64] = {
        40,8,48,16,56,24,64,32,
        39,7,47,15,55,23,63,31,
        38,6,46,14,54,22,62,30,
        37,5,45,13,53,21,61,29,
        36,4,44,12,52,20,60,28,
        35,3,43,11,51,19,59,27,
        34,2,42,10,50,18,58,26,
        33,1,41,9,49,17,57,25
};
//E位选择表(扩展置换表)
static const char E_Table[48] = {
        32,1,2,3,4,5,4,5,
        6,7,8,9,8,9,10,11,
        12,13,12,13,14,15,16,17,
        16,17,18,19,20,21,20,21,
        22,23,24,25,24,25,26,27,
        28,29,28,29,30,31,32,1
};
//P换位表(单纯换位表)
const static char P_Table[32] = {
        16,7,20,21,29,12,28,17,
        1,15,23,26,5,18,31,10,
        2,8,24,14,32,27,3,9,
        19,13,30,6,22,11,4,25
};

//PC1选位表(密钥生成置换表1)
const static char PC1_Table[56] = {
        57,49,41,33,25,17,9,
        1, 58,50,42,34,26,18,
        10,2, 59,51,43,35,27,
        19,11,3, 60,52,44,36,
        63,55,47,39,31,23,15,
        7,62,54,46,38, 30,22,
        14,6,61,53,45,37, 29,
        21,13,5,28,20,12, 4
};
//PC2选位表(密钥生成置换表2)
const static char PC2_Table[48] = {
        14,17,11,24,1,5,3,28,
        15,6,21,10,23,19,12,4,
        26,8,16,7,27,20,13,2,
        41,52,31,37,47,55,30,40,
        51,45,33,48,44,49,39,56,
        34,53,46,42,50,36,29,32
};




//初始置换表IP  
char IP_Table1[64] = {  57,49,41,33,25,17,9,1,  
                                 59,51,43,35,27,19,11,3,  
                                 61,53,45,37,29,21,13,5,  
                                 63,55,47,39,31,23,15,7,  
                                 56,48,40,32,24,16,8,0,  
                                 58,50,42,34,26,18,10,2,  
                                 60,52,44,36,28,20,12,4,  
                                 62,54,46,38,30,22,14,6};   
//逆初始置换表IP^-1  
char IP_1_Table1[64] = {39,7,47,15,55,23,63,31,  
           38,6,46,14,54,22,62,30,  
           37,5,45,13,53,21,61,29,  
           36,4,44,12,52,20,60,28,  
           35,3,43,11,51,19,59,27,  
           34,2,42,10,50,18,58,26,  
           33,1,41,9,49,17,57,25,  
           32,0,40,8,48,16,56,24};  
  
//扩充置换表E  
char E_Table1[48] = {31, 0, 1, 2, 3, 4,  
                  3,  4, 5, 6, 7, 8,  
                  7,  8,9,10,11,12,  
                  11,12,13,14,15,16,  
                  15,16,17,18,19,20,  
                  19,20,21,22,23,24,  
                  23,24,25,26,27,28,  
                  27,28,29,30,31, 0};  
  
//置换函数P  
char P_Table1[32] = {15,6,19,20,28,11,27,16,  
                  0,14,22,25,4,17,30,9,  
                  1,7,23,13,31,26,2,8,  
                  18,12,29,5,21,10,3,24};  
                  
//置换选择1  
char PC_11[56] = {56,48,40,32,24,16,8,  
              0,57,49,41,33,25,17,  
              9,1,58,50,42,34,26,  
              18,10,2,59,51,43,35,  
              62,54,46,38,30,22,14,  
              6,61,53,45,37,29,21,  
              13,5,60,52,44,36,28,  
              20,12,4,27,19,11,3};  
  
//置换选择2  
char PC_21[48] = {13,16,10,23,0,4,2,27,  
              14,5,20,9,22,18,11,3,  
              25,7,15,6,26,19,12,1,  
              40,51,30,36,46,54,29,39,  
              50,44,32,46,43,48,38,55,  
              33,52,45,41,49,35,28,31};                  

static void printfArray(const char *arr, int len)
{
	int i = 0;
	for (i = 0; i < len; i++)
	{
		if (i != 0 && i % 8 == 0) printf("\n");
	    printf("%d, ", (arr[i]-1) & 0xFF);
	}
	printf("\n\n\n");
}

static void printfNativeArray(const char *arr, int len)
{
	int i = 0;
	for (i = 0; i < len; i++)
	{
		if (i != 0 && i % 8 == 0) printf("\n");
	    printf("%d, ", arr[i] & 0xFF);
	}
	printf("\n\n\n");
}


int main()
{
	/*uint8 arr[] ={0xC9,  0x4E, 01, 0xE8, 03, 00, 00, 00, 04, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
0x9A, 02, 01, 00};
  uint16_t sum = crc16_compute(arr, ARR_SIZE(arr), NULL);
  printf("sum=%x \r\n", sum);
	#ifdef TEST_DATA
	printf("step =%d name=%s", 22, getStepName(22));
	#else
	printf("nothing done \r\n");
	#endif
	//dfu_data_pkt_construct(NULL, NULL);
	*/
	printfArray(IP_Table, ARR_SIZE(IP_Table));
	printfArray(IPR_Table, ARR_SIZE(IPR_Table));
	printfArray(E_Table, ARR_SIZE(E_Table));
	printfArray(P_Table, ARR_SIZE(P_Table));
	printfArray(PC1_Table, ARR_SIZE(PC1_Table));
	printfArray(PC2_Table, ARR_SIZE(PC2_Table));
	//printfArray(IP_Table, ARR_SIZE(IP_Table));
	printf("another version\n\n\n");
	printfNativeArray(IP_Table1, ARR_SIZE(IP_Table1));
	printfNativeArray(IP_1_Table1, ARR_SIZE(IP_1_Table1));
	printfNativeArray(E_Table1, ARR_SIZE(E_Table1));
	printfNativeArray(P_Table1, ARR_SIZE(P_Table1));
	printfNativeArray(PC_11, ARR_SIZE(PC_11));
	printfNativeArray(PC_21, ARR_SIZE(PC_21));
	return 0;
}