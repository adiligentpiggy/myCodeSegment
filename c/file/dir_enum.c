#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#define myLog printf
#define myMalloc malloc

//获取指定目录下的所有文件列表 author:wangchangshaui jlu  
char** getFileNameArray(const char *path, int* fileCount)  
{  
    int count = 0;  
    char **fileNameList = NULL;  
    struct dirent* ent = NULL;  
    DIR *pDir;  
    char dir[512];  
    struct stat statbuf;  
  
//打开目录  
    if ((pDir = opendir(path)) == NULL)  
    {  
        myLog("Cannot open directory:%s\n", path);  
        return NULL;  
    }  
//读取目录  
    while ((ent = readdir(pDir)) != NULL)  
    { //统计当前文件夹下有多少文件（不包括文件夹）  
//得到读取文件的绝对路径名  
        snprintf(dir, 512, "%s/%s", path, ent->d_name);  
        //得到文件信息  
        lstat(dir, &statbuf);  
        //判断是目录还是文件  
        if (!S_ISDIR(statbuf.st_mode))  
        {  
            count++;  
        }  
    } //while  
//关闭目录  
    closedir(pDir);  
//  myLog("共%d个文件\n", count);  
  
//开辟字符指针数组，用于下一步的开辟容纳文件名字符串的空间  
    if ((fileNameList = (char**) myMalloc(sizeof(char*) * count)) == NULL)  
    {  
        myLog("Malloc heap failed!\n");  
        return NULL;  
    }  
  
//打开目录  
    if ((pDir = opendir(path)) == NULL)  
    {  
        myLog("Cannot open directory:%s\n", path);  
        return NULL;  
    }  
//读取目录  
    int i;  
    for (i = 0; (ent = readdir(pDir)) != NULL && i < count;)  
    {  
        if (strlen(ent->d_name) <= 0)  
        {  
            continue;  
        }  
        //得到读取文件的绝对路径名  
        snprintf(dir, 512, "%s/%s", path, ent->d_name);  
        //得到文件信息  
        lstat(dir, &statbuf);  
        //判断是目录还是文件  
        if (!S_ISDIR(statbuf.st_mode))  
        {  
            if ((fileNameList[i] = (char*) myMalloc(strlen(ent->d_name) + 1))  
                    == NULL)  
            {  
                myLog("Malloc heap failed!\n");  
                return NULL;  
            }  
            memset(fileNameList[i], 0, strlen(ent->d_name) + 1);  
            strcpy(fileNameList[i], ent->d_name);  
            myLog("第%d个文件:%s\n", i, ent->d_name);  
            i++;  
        }  
    } //for  
//关闭目录  
    closedir(pDir);  
  
    *fileCount = count;  
    return fileNameList;  
}

int main()
{
   int i = 0, j = 0;
   char** files = getFileNameArray("/", &i);
   for (j = 0; j < i; j++) {
   	  char* f_name = files[j];
   	  myLog(f_name);
   	  myLog("\n");
   }
   
   //printf("getDayno：%d\n", getDayno);
   
   
   return(0);
}