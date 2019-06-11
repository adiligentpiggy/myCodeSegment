/************************************************************************
  * Copyright(C),BYD COMPANY LIMITED.
  * All rights reserverd.
  * Author:          sun.hongzhe@byd.com
  * version:         1.0
  * Date:            2017-12-01
  * Description:     primi_types.h
  * History:
    1.Date & Author: sun.hongzhe@byd.com
      Modification:  First edition (frame version)
************************************************************************/
#ifndef PRIMITIVE_TYPES_H__
#define PRIMITIVE_TYPES_H__

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define dzlog_debug printf
#define dzlog_info printf
#define dzlog_error printf

typedef unsigned char  uint8;
typedef unsigned short uint16;
typedef unsigned int   uint32;
typedef unsigned long   uint64;

typedef  char  int8;
typedef  short int16;
typedef  int   int32;
typedef  long  int64;
typedef long long int   llint;

//#define DEBUG(x,y...)   {printf("[ %s : %s : %d] ",__FILE__, __func__, __LINE__); printf(x,##y); printf("\n");}
//#define ERROR(x,y...)   {printf("[ %s : %s : %d] ",__FILE__, __func__, __LINE__); printf(x,##y); printf("\n");}
#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))
#define MAX_PATH_LEN  256
#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))

#endif // PRIMITIVE_TYPES_H__
