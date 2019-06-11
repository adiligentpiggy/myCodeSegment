
unsigned char EncryKey[16] = {21, 34, 55, 43, 44, 55, 66, 75, 65, 74, 23, 45, 67, 89, 76, 55};

static void printHexData(unsigned char *data, int size)
{
    int i = 0;
    char temp[100] = {0};
    snprintf(temp, 100, "####### PrintHexData size=%d\r\n", size);
    printf(temp);
    for(i = 0; i < size; i++)
    {
        char str[20] = {0};
        sprintf(str,"%02x ", data[i]);
        printf(str);
    }
    printf("\r\n");
}

static char DataEncryptAndDecrypt(unsigned char *bufin,unsigned short len,unsigned char *bufout)
{
	int i = 0;
//	GetMD5String(EncryKey, VinNum, 17);//Éú³ÉÃÜÂëÖµ
	if(len <= 0)
	{
		return 0;
	}
	for(i = 0;i < len;i++)
	{
		bufout[i] = bufin[i] ^ EncryKey[i % 16];
	}
	return 1;
}

int main()
{
	int intt = 0x12345678;
	int restore_intt = 0;
	unsigned char EncryptData[4] = {0};
	unsigned char toSend[4] = {0};
	unsigned char restored[4] = {0};
	EncryptData[0] = (unsigned char)(intt >> 24);
		EncryptData[1] = (unsigned char)(intt >> 16);
		EncryptData[2] = (unsigned char)(intt >> 8);
		EncryptData[3] = (unsigned char)(intt);
		DataEncryptAndDecrypt(EncryptData,4, toSend);
		printHexData(toSend, 4);
		DataEncryptAndDecrypt(toSend,4, restored);
		printHexData(restored, 4);
		restore_intt = (restored[0] << 24) + (restored[1] << 16) + (restored[2] << 8) + (restored[3]);
		
		printf("intt =%d restore_intt=%d", intt, restore_intt);
}