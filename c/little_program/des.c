#include "memory.h"
#include "stdio.h"

typedef unsigned char bool;
#define ARR_SIZE(a) (sizeof(a) / sizeof(a[0]))

enum    {ENCRYPT,DECRYPT};// ENCRYPT:加密,DECRYPT:解密
void Des_Run(char Out[8], char In[8], bool Type);
void Des_SetKey(const char Key[8]);// 设置密钥
static void F_func(bool In[32], const bool Ki[48]);// f 函数
static void S_func(bool Out[32], const bool In[48]);// S 盒代替
static void Transform(bool *Out, bool *In, const char *Table, int len);// 变换
static void Xor(bool *InA, const bool *InB, int len);// 异或
static void RotateL(bool *In, int len, int loop);// 循环左移
static void ByteToBit(bool *Out, const char *In, int bits);// 字节组转换成位组
static void BitToByte(char *Out, const bool *In, int bits);// 位组转换成字节组
static void printDataAsHEXString(char data[], int size);

#ifdef NEW_TABLE
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

// S盒
const static char S_Box[8][4][16] = {
        // S1 
        14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7,
        0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8,
        4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0,
        15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13,
        //S2
        15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10,
        3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5,
        0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15,
        13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9,
        //S3
        10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8,
        13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1,
        13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7,
        1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12,
        //S4
        7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15,
        13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9,
        10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4,
        3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14,
        //S5
        2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9,
        14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6,
        4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14,
        11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3,
        //S6
        12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11,
        10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8,
        9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6,
        4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13,
        //S7
        4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1,
        13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6,
        1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2,
        6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12,
        //S8
        13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7,
        1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2,
        7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8,
        2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11
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
//左移位数表 
const static char LOOP_Table[16] = {
    1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1
};
#else
 //置换IP表
const static char IP_Table[64] = {
        57,49,41,33,25,17,9,1,  
                                 59,51,43,35,27,19,11,3,  
                                 61,53,45,37,29,21,13,5,  
                                 63,55,47,39,31,23,15,7,  
                                 56,48,40,32,24,16,8,0,  
                                 58,50,42,34,26,18,10,2,  
                                 60,52,44,36,28,20,12,4,  
                                 62,54,46,38,30,22,14,6
};
//逆置换IP-1表
const static char IPR_Table[64] = {
        39,7,47,15,55,23,63,31,  
           38,6,46,14,54,22,62,30,  
           37,5,45,13,53,21,61,29,  
           36,4,44,12,52,20,60,28,  
           35,3,43,11,51,19,59,27,  
           34,2,42,10,50,18,58,26,  
           33,1,41,9,49,17,57,25,  
           32,0,40,8,48,16,56,24
};
//E位选择表(扩展置换表)
static const char E_Table[48] = {
        31, 0, 1, 2, 3, 4,  
                  3,  4, 5, 6, 7, 8,  
                  7,  8,9,10,11,12,  
                  11,12,13,14,15,16,  
                  15,16,17,18,19,20,  
                  19,20,21,22,23,24,  
                  23,24,25,26,27,28,  
                  27,28,29,30,31, 0
};
//P换位表(单纯换位表)
const static char P_Table[32] = {
        15,6,19,20,28,11,27,16,  
                  0,14,22,25,4,17,30,9,  
                  1,7,23,13,31,26,2,8,  
                  18,12,29,5,21,10,3,24
};

// S盒
const static char S_Box[8][4][16] = {
        // S1 
        14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7,
        0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8,
        4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0,
        15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13,
        //S2
        15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10,
        3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5,
        0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15,
        13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9,
        //S3
        10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8,
        13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1,
        13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7,
        1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12,
        //S4
        7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15,
        13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9,
        10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4,
        3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14,
        //S5
        2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9,
        14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6,
        4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14,
        11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3,
        //S6
        12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11,
        10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8,
        9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6,
        4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13,
        //S7
        4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1,
        13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6,
        1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2,
        6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12,
        //S8
        13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7,
        1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2,
        7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8,
        2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11
};

//PC1选位表(密钥生成置换表1)
const static char PC1_Table[56] = {
        56,48,40,32,24,16,8,  
              0,57,49,41,33,25,17,  
              9,1,58,50,42,34,26,  
              18,10,2,59,51,43,35,  
              62,54,46,38,30,22,14,  
              6,61,53,45,37,29,21,  
              13,5,60,52,44,36,28,  
              20,12,4,27,19,11,3
};
//PC2选位表(密钥生成置换表2)
const static char PC2_Table[48] = {
        13,16,10,23,0,4,2,27,  
              14,5,20,9,22,18,11,3,  
              25,7,15,6,26,19,12,1,  
              40,51,30,36,46,54,29,39,  
              50,44,32,47,43,48,38,55,  
              33,52,45,41,49,35,28,31
};
//左移位数表 
const static char LOOP_Table[16] = {
    1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1
};
#endif
static bool SubKey[16][48];// 16圈子密钥

//����  
int DES_Swap(char left[32], char right[32]){  
    char temp[32];  
    memcpy(temp,left,32);     
    memcpy(left,right,32);    
    memcpy(right,temp,32);  
    return 0;  
}  
 
void Des_Run(char Out[8], char In[8], bool Type)
{
	  int i=0;
    static bool M[64], Tmp[32], *Li = &M[0], *Ri = &M[32];

    ByteToBit(M, In, 64);// 字节组转换成位组
    Transform(M, M, IP_Table, 64);// 变换
    if( Type == ENCRYPT ){
        for(i=0; i<16; i++) {
            memcpy(Tmp, Ri, 32);
            F_func(Ri, SubKey[i]);
            Xor(Ri, Li, 32);// 异或
            memcpy(Li, Tmp, 32);
            if (i == 1 || i == 14)
                printDataAsHEXString(M, 64);
            if (i == 15) {
                //DES_Swap(Li, Ri);
            }
        }
    }else{
        for(i=15; i>=0; i--) {
            memcpy(Tmp, Li, 32);
            F_func(Li, SubKey[i]);
            Xor(Li, Ri, 32);// 异或
            memcpy(Ri, Tmp, 32);
            if (i == 0) {
                //DES_Swap(Li, Ri);
            }
        }
    }
    Transform(M, M, IPR_Table, 64);// 变换
    BitToByte(Out, M, 64);
}
 
 
void Des_SetKey(const char Key[8])
{
	  int i=0;
    static bool K[64], *KL = &K[0], *KR = &K[28];
    ByteToBit(K, Key, 64);// 字节组转换成位组
    Transform(K, K, PC1_Table, 56);// 变换
    for(i=0; i<16; i++) 
    {
        RotateL(KL, 28, LOOP_Table[i]);// 循环左移
        RotateL(KR, 28, LOOP_Table[i]);// 循环左移
        Transform(SubKey[i], K, PC2_Table, 48);// 变换
    }

    /*for (i = 0; i < 16; i++)
    {
        printf("DES_MakeSubKeys i = %d\n", i);
        printDataAsHEXString(SubKey[i], 48);
        printf("\n");
    }*/
}
 
 
 
void F_func(bool In[32], const bool Ki[48])// f 函数
{
    static bool MR[48];
    static int i = 0;
    int mark = 1;
    Transform(MR, In, E_Table, 48);// 变换
    if (i == mark) {
    printf("after Transform \n");
    printDataAsHEXString(MR, 48);
    }
    Xor(MR, Ki, 48);// 异或
    if (i == mark) {
    printf("after Xor \n");
    printDataAsHEXString(MR, 48);
    }
    S_func(In, MR);// S 盒代替
    if (i == mark) {
    printf("after S_func \n");
    printDataAsHEXString(In, 32);
    }
    Transform(In, In, P_Table, 32);// 变换
    if (i == mark) {
    printf("after next Transform \n");
    printDataAsHEXString(In, 32);
    }
    i++;
}
 
void S_func(bool Out[32], const bool In[48])// S 盒代替,输入6位的数，输出4位的数，将第一六位对应的十进制数作为行，第二三四五为的对应十进制数作为列
{
    char i=0,j,k;
    char output = 0;
    for(i=0; i<8; i++,In+=6,Out+=4) 
    {
        j = (In[0]<<1) + In[5];
        k = (In[1]<<3) + (In[2]<<2) + (In[3]<<1) + In[4];
        //ByteToBit(Out, &S_Box[i][j][k], 4);
        output = S_Box[i][j][k];
        //��Ϊ2����  
        Out[0] = (output & 0X08) >> 3;  
        Out[1] = (output & 0X04) >> 2;  
        Out[2] = (output & 0X02) >> 1;  
        Out[3] = output & 0x01; 
    }
}
 
void Transform(bool *Out, bool *In, const char *Table, int len)//变换
{
	  int i = 0;
    static bool Tmp[256];
    for(i = 0; i < len; i++)
    {
    	#ifdef NEW_TABLE
        Tmp[i] = In[ Table[i] - 1 ];
        #else
        Tmp[i] = In[ Table[i] ];
        #endif
    }
    memcpy(Out, Tmp, len);
}
 
 
 
void Xor(bool *InA, const bool *InB, int len)//异或
{
	  int i = 0;
    for(i = 0; i < len; i++)
    {
        InA[i] ^= InB[i];
    }
}
 
void RotateL(bool *In, int len, int loop)//循环左移
{
    static bool Tmp[256];
    memcpy(Tmp, In, loop);
    memcpy(In, In+loop, len-loop);
    memcpy(In+len-loop, Tmp, loop);
}
 
void ByteToBit(bool *Out, const char *In, int bits)//字节组转换成位组
{
	  int i = 0;
    for(i = 0; i < bits; i++)
    {
        Out[i] = (In[i/8] >> (i%8)) & 1;
    }
}
 
void BitToByte(char *Out, const bool *In, int bits)
{
	  int i = 0;
    memset(Out, 0, (bits+7)/8);
    for(i = 0; i < bits; i++)
    {
        Out[i/8] |= In[i] << (i%8);
    }
}

static void printDataAsHEXString(char data[], int size)
{
	int i = 0;
	for(i = 0; i < size; i++)
    {
        if (i != 0 && i % 8 == 0) printf("\n");
        printf("0x%x ", data[i] & 0xFF);
  
    }
    printf("\n");
}
 
 
int main(int argc, char *argv[])
{
    char key[8] = "19809172";
    //char key[56] = "g993h3hc83j=+fmcmsksc~!@#$$53234dgdfrr4r";
    char str[] = "Hello";
    char ciphertext[800] = {0};
    int i = 0;
    int dataSize = strlen(str);
    int extra = dataSize % 8;
    int loop = (dataSize + 7) / 8 + (extra == 0 ? 1 : 0);
    /*char miwen[] = "5d38ce95fa773dba0d294305c463850907cbcca60a790b556d7eb3d99cdf0a69caa83c8d123145f1"
"b0a0c5b42e06d5747a9c096095c3ccf774128ef25e8eb35ba4784d4528db2fac007437ba5447c56c"
"c3a10245e19844d61b551cca0b1db3abb69995407eebf77ea6e04d5007b786aa15033d6b04e0b8bc"
"e2adb62063b10f93ff0de9fe55ded0409f0d7f967fb3f3f53a2a4f67b78c151504c4b271301307c4"
"e4fd87b4bcbaef8e4876217f14c093c75ba4aea3f62ff5efe59c6e645f493c7b57a45ddfb49a7c60"
"e792e058f36cfe538df450d169d2661b7e2442bd2b96faeea72884d8df8bdceb00ec762973d4b8cd";*/
    //printf("Before encrypting\n");
    //printf("%s\n",str);
    Des_SetKey(key);
    
    extra = 8 - extra;
    for (i = 0; i < loop; i++)
    {
       char temp[8] = {0};
       char *pData = &str[i * 8];
       char *pCipher = ciphertext + i * 8;
       if (i == loop - 1)
       {
           memcpy(temp, &str[i * 8], 8 - extra);
           memset(temp + 8 - extra, extra, extra); 
           pData = temp ;
       }
     
        printf("Before encrypting\n");
        printDataAsHEXString(pData, 8);
        Des_Run(pCipher, pData, ENCRYPT);
        printf("After encrypting\n");
        printDataAsHEXString(pCipher, 8);
 
    }
    
    printf("\n");
    printf("After decrypting\n");
    for (i = 0; i < loop; i++)
    {
      Des_Run(ciphertext + i * 8, ciphertext + i * 8, DECRYPT);
      printDataAsHEXString(ciphertext + i * 8, 8);
    }
 
    return 0;
 
}