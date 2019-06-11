/*******************************************************************************
  * Copyright(C),BYD COMPANY LIMITED
  * FileName:        file_utils.c
  * Author:          You Zhong
  * version:         1.0
  * Date:            2017-11-22
  * Description:     file operation utils
  * History:
    1.Date:          2017-11-22
      Author:        You Zhong
      Modification:  First edition
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/statfs.h>
#include <sys/stat.h>
#include "zlog.h"
#include "file_utils.h"

/*******************************************************************************
  * Function Name: get_all_file_name_under_dir
  * Description: get all file name under directory
  * Input:    dir_path
  * Output:   None
  * Return:   None
*******************************************************************************/
void get_all_file_name_under_dir(char* dir_path)
{
    DIR* dir;
    struct dirent* ptr;

    dzlog_debug("get_upload_file_list entry ");
    dir = opendir(dir_path);
    if (dir == NULL)
    {
        dzlog_debug("open dir error ");
        return;
    }
    while ((ptr = readdir(dir)) != NULL)
    {
        //is directory
        if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0)
        {
            continue;
        }
        else if (ptr->d_type == DT_REG) //is file
        {
            dzlog_debug("upload file name = %s ", ptr->d_name);
        }
    }
    closedir(dir);
}

/*******************************************************************************
  * Function Name: get_file_size
  * Description: get file size
  * Input:    file path
  * Output:   None
  * Return:   file size
*******************************************************************************/
uint64 get_file_size(const char *path)
{
    struct stat statbuff;

    if(stat(path, &statbuff) < 0)
    {
        dzlog_debug("get file size error ");
        return 0;
    }
    else
    {
        dzlog_debug("path size = %ld ", statbuff.st_size);
        return statbuff.st_size;
    }
}

/*******************************************************************************
  * Function Name: get_file_modify_time
  * Description: get file modify time
  * Input:    file path
  * Output:   None
  * Return:   file modify time
*******************************************************************************/
time_t get_file_modify_time(const char* path)
{
    struct stat statbuff;

    if(stat(path, &statbuff) < 0)
    {
        dzlog_debug("get file modify time error ");
        return 0;
    }
    else
    {
        dzlog_debug("path size = %ld ", statbuff.st_mtime);
        return statbuff.st_mtime;
    }
}

/*******************************************************************************
  * Function Name: get_disk_free_space
  * Description: get disk free space
  * Input:    path
  * Output:   None
  * Return:   free size (MB)
*******************************************************************************/
int get_disk_free_space(char* path)
{
    unsigned long long freeDisk = 0;
    size_t mbFreedisk = 0;
    //unsigned long totalDisk = 0;
    //size_t mbTotalsize = 0;
    struct statfs diskInfo;

    statfs(path, &diskInfo);
    freeDisk = (diskInfo.f_bfree) * (diskInfo.f_bsize);
    mbFreedisk = freeDisk >> 20;

    //totalDisk = (diskInfo.f_blocks) * (diskInfo.f_bsize);
    //mbTotalsize = totalDisk >> 20;
    dzlog_debug("path: %s free space =%dMB\n", path, mbFreedisk);
    return mbFreedisk;
}

