#include <stdio.h>
#include <unistd.h>

#include "ledisplay.h"
void demo_1(int fd,int ledid);
void demo_eth(int fd,int ledid);
int main(void)
{
    int i;
    int ledid = LED_Init(0xffffffff);
	int fd = InitSock("192.168.192.60");
	if (fd < 0)
    {
        printf("LED_Init error!\n");
    }
    printf("ledid = %d\n", ledid);
	
    demo_eth(fd,ledid);
	
    close(fd)  ;
    LED_UnInit(ledid);
    //LED_SetBrightness(ledid, 15);

    return 0;
}

void demo_1(int fd,int ledid)
{
	ContentArray1 a = {'\0'};
    a.ContentCount = 0x03;
    a.ContentArray[0].SerialNum = 0;
    a.ContentArray[0].ContentType = 1;
    a.ContentArray[0].X = 0;
    a.ContentArray[0].Y = 16;
    a.ContentArray[0].Width = 320;
    a.ContentArray[0].Height = 16;

    a.ContentArray[1].SerialNum = 1;
    a.ContentArray[1].ContentType = 3;
    a.ContentArray[1].X = 0;
    a.ContentArray[1].Y = 0;
    a.ContentArray[1].Width = 320;
    a.ContentArray[1].Height = 16;

    a.ContentArray[2].SerialNum = 2;
    a.ContentArray[2].ContentType = 2;
    a.ContentArray[2].X = 0;
    a.ContentArray[2].Y = 32;
    a.ContentArray[2].Width = 320;
    a.ContentArray[2].Height = 32;

    int a1= LED_SetLayout(fd, ledid, a);
    printf("LED_SetLayout = %d\n", a1);

    ClockProperty clock_property;
    clock_property.WordSize = 16;
    clock_property.WordColoro = 0xf00fff;
    clock_property.DateFormat = 2;
    clock_property.TimeFormat = 1;
	clock_property.WordFont = 0;
	clock_property.EnableCheck = 0xff;
    clock_property.YearMode = 0;
    clock_property.HourMode = 0;
    clock_property.IsPosiUserDefined=1;
    clock_property.DateX = 0;
    clock_property.DateY = 16;
    clock_property.WeekX = 100;
    clock_property.WeekY = 16;
    clock_property.TimeX = 256;
    clock_property.TimeY = 16;
    int a2 = LED_SetClockStyle(fd, ledid, 0, clock_property);
	
	unsigned char test1[] = {0xC9, 0xCC, 0xC3,0xB3,  0xB3,0xC7,  0xB7,0xBD};

	TextProperty one;
    one.PlayStyle = 1;
    one.PlaySpeed = 1;
    one.WordCount = sizeof(test1);
	printf("one.WordCount = %d\n", one.WordCount);
    one.WordSize = 16;
    one.WordFont = 0;
    one.WordColoro = 0xff0000;
    one.WordSpacing = 2;
    one.StandingTime = 0;
	unsigned char second[2] = {0xb1,0xb1};
    TextProperty two;
    two.PlayStyle = 1;
    two.PlaySpeed = 0;
    two.WordCount = sizeof(second);
    printf("two.WordCount = %d\n", two.WordCount);
    two.WordSize = 32;
    two.WordFont = 1;
    two.WordColoro = 0x00ffff;
    two.WordSpacing = 10;
    two.StandingTime = 0;

  
    int aa = LED_SetFontData(fd, ledid, 1, one, test1);
    printf("aa= %d\n", aa);
    int bb =   LED_SetFontData(fd, ledid, 2, two, second);
    printf("bb = %d\n", bb);

    int a3 = LED_AdjustTime(fd, ledid);
    printf("LED_AdjustTime = %d\n", a3);
}


void demo_eth(int fd,int ledid)
{
	ContentArray1 a = {'\0'};
	ClockProperty clock_property;
	TextProperty one;
	TextProperty two;
	PictureProperty three;
	int a1,a2,aa,bb,a3;
	int i=0,j=0;
	unsigned char test1[] = {0xb7, 0xC4, 0xd6,0xaf,  0xB3,0xC7, 0x3c,0x2d,0x2d,0x3e, 0xBa,0xf3,0xCe,0xc0,0xd5,0xaf};
	//unsigned char second[] = {0xb1,0xb1,0xb9,0xfa,0xd6,0xae,0xb4,0xba};
	unsigned char second[] = {0xce,0xf7,0xb0,0xb2,0xb5,0xd8,0xcc,0xfa,0x31,0xba,0xc5,0xcf,0xdf};
	unsigned int third[256];
	unsigned int fourth[64*64];
	unsigned char *p=(unsigned char *)third;
	unsigned char *q=(unsigned char *)fourth;
	for(j=0; j<8; j++)
	{
		for(i=0;i<5;i++)
		{
			third[i*16+j] = 0xff0000ff;
		}
		for(i=5;i<10;i++)
		{
			third[i*16+j] = 0xffff0000;
		}
		for(i=10;i<16;i++)
		{
			third[i*16+j] = 0xff00ff00;
		}
	}
	for(j=8; j<16; j++)
	{
		for(i=0;i<5;i++)
		{
			third[i*16+j] = 0xffff00ff;
		}
		for(i=5;i<10;i++)
		{
			third[i*16+j] = 0xffffff00;
		}
		for(i=10;i<16;i++)
		{
			third[i*16+j] = 0xff00ffff;
		}
	}

	for(j=0; j<32; j++)
	{
		for(i=0;i<20;i++)
		{
			fourth[i*64+j] = 0xff0000ff;
		}
		for(i=20;i<40;i++)
		{
			fourth[i*64+j] = 0xffff0000;
		}
		for(i=40;i<64;i++)
		{
			fourth[i*64+j] = 0xff00ff00;
		}
	}
	for(j=32; j<64; j++)
	{
		for(i=0;i<20;i++)
		{
			fourth[i*64+j] = 0xffff00ff;
		}
		for(i=20;i<40;i++)
		{
			fourth[i*64+j] = 0xffffff00;
		}
		for(i=40;i<64;i++)
		{
			fourth[i*64+j] = 0xff00ffff;
		}
	}

	
	a.ContentCount = 0x04;
	a.ContentArray[0].SerialNum = 0;
	a.ContentArray[0].ContentType = 1;
	a.ContentArray[0].X = 272;//120;
	a.ContentArray[0].Y = 16;
	a.ContentArray[0].Width = 48;//200;
	a.ContentArray[0].Height = 16;

	a.ContentArray[1].SerialNum = 1;
	a.ContentArray[1].ContentType = 3;
	a.ContentArray[1].X = 0;
	a.ContentArray[1].Y = 0;
	a.ContentArray[1].Width = 320;
	a.ContentArray[1].Height = 16;

	a.ContentArray[2].SerialNum = 2;
	a.ContentArray[2].ContentType = 2;
	a.ContentArray[2].X = 0;
	a.ContentArray[2].Y = 32;
	a.ContentArray[2].Width = 320;
	a.ContentArray[2].Height = 32;
	
	a.ContentArray[3].SerialNum = 3;
	a.ContentArray[3].ContentType = 5;
	a.ContentArray[3].X = 256;
	a.ContentArray[3].Y = 16;
	a.ContentArray[3].Width = 16;
	a.ContentArray[3].Height = 16;
	
	a1= LED_SetLayout(fd, ledid, a);
	
	clock_property.WordSize = 16;
	clock_property.WordColoro = 0xffa0ff;
	clock_property.DateFormat = 1;
	clock_property.TimeFormat = 0;
	clock_property.WordFont = 0;

	clock_property.EnableCheck = 0x03;//0xff;
	clock_property.YearMode = 0;
	clock_property.HourMode = 1;

	a2 = LED_SetClockStyle(fd, ledid, 0, clock_property);
	
	three.PlaySpeed=0;
	three.PlayStyle=0;
	three.StandingTime=1;
	three.TotalFrameNum=1;
	a3 = LED_SetPicture(fd, ledid,3,three, p);


	one.PlayStyle = 1;
	one.PlaySpeed = 1;
	one.WordCount = sizeof(test1);
	one.WordSize = 16;
	one.WordFont = 0;
	one.WordColoro = 0xffff00;
	one.WordSpacing = 2;
	one.StandingTime = 0;

	two.PlayStyle = 1;
	two.PlaySpeed = 0;
	two.WordCount = sizeof(second);
	//printf("two.WordCount = %d\n", two.WordCount);
	two.WordSize = 32;
	two.WordFont = 1;
	two.WordColoro = 0x00ffff;
	two.WordSpacing = 10;
	two.StandingTime = 0;

	aa = LED_SetFontData(fd, ledid, 1, one, test1);
	bb = LED_SetFontData(fd, ledid, 2, two, second);

	
	/*
	a.ContentCount = 0x01;
	a.ContentArray[0].SerialNum = 0;
	a.ContentArray[0].ContentType = 4;
	a.ContentArray[0].X = 256;
	a.ContentArray[0].Y = 0;
	a.ContentArray[0].Width = 64;
	a.ContentArray[0].Height = 64;

	a1= LED_SetLayout(fd, ledid, a);
	three.PlaySpeed=0;
	three.PlayStyle=0;
	three.StandingTime=1;
	three.TotalFrameNum=1;
	a3 = LED_SetPicture(fd, ledid,0,three, q);
	*/
	LED_AdjustTime(fd, ledid);
	LED_SetBrightness(fd,ledid,0);
}
