#ifndef __LEDISPLAY_H__
#define __LEDISPLAY_H__


#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <fcntl.h>

#define DEBUG

#define REC_RESPOND_TIME_SEC	0
#define REC_RESPOND_TIME_USEC   250000
#define	REC_RESPONE_COUNT	1
#define PORT_BROAD		8888

#define MAX_CONTENT_NUM 10

typedef struct	_TextProperty
{
    unsigned int    WordFont; //0:宋体 1:楷体 2:黑体
    unsigned int    WordColoro;
    unsigned short   WordCount;
    unsigned char   WordSize;
    unsigned char   PlayStyle; // 0:静止 1:左移 2:右移 3:上移 4:下移
    unsigned short  PlaySpeed; // 0 - 255, 0为最快
    unsigned char   StandingTime; // 滞留时间
    unsigned char   WordSpacing; //滚动时文字间隔
} TextProperty;

typedef struct	_ClockProperty
{
    unsigned char   WordSize;
    unsigned int    WordFont;
    unsigned int    WordColoro;
    unsigned char   DateFormat;//年月日的布局格式，0：2016年04月08日， 1：2016-04-08， 2：2016/04/08
    unsigned char   TimeFormat; //时分秒的布局格式， 0: 17:45:30   1:17时45分30秒
    unsigned char   EnableCheck;//每位表示是否显示各子模块，包括半日、年、月、日、星期、时、分、秒
    unsigned char   YearMode;//0表示2位年，1表示4位年
    unsigned char   HourMode;//0表示12小时制，1表示24小时制 
	unsigned char	IsPosiUserDefined;//是否自定义各个子模块的位置 0：使用默认位置，1：用户自定义位置
	unsigned short  DateX; //年月日起始X坐标，IsUsernDefined为0时不用关心
	unsigned short	DateY; //年月日起始Y坐标，IsUsernDefined为0时不用关心
	unsigned short  WeekX; //星期起始X坐标，IsUsernDefined为0时不用关心
	unsigned short  WeekY; //星期起始Y坐标，IsUsernDefined为0时不用关心
	unsigned short  TimeX; //时分秒起始X坐标，IsUsernDefined为0时不用关心
	unsigned short  TimeY; //时分秒起始Y坐标，IsUsernDefined为0时不用关心

	
} ClockProperty;

typedef struct _PictureProperty
{
    unsigned char TotalFrameNum;
    unsigned char PlaySpeed;
    unsigned char PlayStyle;
    unsigned char StandingTime;
}PictureProperty;

typedef struct _ContentRegion
{
    unsigned char SerialNum;   //0 - 9
    unsigned char ContentType; //1:digitalclock  2: font
    unsigned int  X;
    unsigned int  Y;
    unsigned int  Width;
    unsigned int  Height;
} ContentRegion;


typedef struct _ContentArray
{
    unsigned char  ContentCount;
    ContentRegion ContentArray[MAX_CONTENT_NUM];

} ContentArray1;


int LED_Init(int Sn);
int LED_UnInit(int LedID);

int LED_SetLayout(int fd, int LedID, ContentArray1 LedContenArray);

int LED_SetBrightness(int fd, int LedID, int Value);
int LED_AdjustTime(int fd, int LedID);
int LED_SetPower(int fd, int LedID, int Power);// power:1：开屏  2：关屏


int LED_SetFontData(int fd, int LedID, unsigned char SerialNum, TextProperty textProperty,  unsigned char *WordData);
int LED_SetClockStyle(int fd, int LedID, unsigned char SerialNum, ClockProperty clockProperty);
int LED_SetPicture(int fd, int LedID, unsigned char SerialNum, PictureProperty pictureProperty, unsigned char * WordData);

#endif
