/*******************************************************************************
  * Copyright(C),BYD COMPANY LIMITED
  * FileName:        file_utils.h
  * Author:          You Zhong
  * version:         1.0
  * Date:            2017-11-22
  * Description:     file_utils.h
  * History:
    1.Date:          2017-11-22
      Author:        You Zhong
      Modification:  First edition
*******************************************************************************/
#ifndef _FILE_UTILS_H_
#define _FILE_UTILS_H_

#include "primi_types.h"

void get_all_file_name_under_dir(char* dir_path);
uint64 get_file_size(const char *path);
time_t get_file_modify_time(const char* path);
int get_disk_free_space(char* path);
#endif
