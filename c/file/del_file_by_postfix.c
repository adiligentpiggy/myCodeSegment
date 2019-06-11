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

  
void del_files_by_postfix(const char *path, int daycount)  
{  
    struct dirent* ent = NULL;  
    DIR *pDir;  
    char dir[512];  
    struct stat statbuf;  
  
//打开目录  
    if ((pDir = opendir(path)) == NULL)  
    {  
        myLog("Cannot open directory:%s\n", path);  
        return;  
    }  
//读取目录  
    while ((ent = readdir(pDir)) != NULL)  
    { //统计当前文件夹下有多少文件（不包括文件夹）  
      //得到读取文件的绝对路径名  
        snprintf(dir, 512, "%s/%s", path, ent->d_name);  
        //得到文件信息  
        lstat(dir, &statbuf);  
        myLog("item:%s\n", dir);
        //判断是目录还是文件  
        if (S_ISDIR(statbuf.st_mode))  
        {  
        	myLog("dir:%s\n", dir);
            continue;  
        }
        if (strstr(ent->d_name, "ps_log") == NULL) {
        	myLog("111\n");
        	continue;
        }

        char* s = strrchr(ent->d_name, '.');
        if(s == NULL) {
        	myLog("222\n");
        	continue;
        }
        s++;
        int day_elapse = atoi(s);
        myLog("day_elapse:%d\n", day_elapse);
        if(day_elapse < daycount) {
        	remove(dir);
        	myLog("delete file:%s\n", dir);
        }
    } 
//关闭目录  
    closedir(pDir);  
}

int main()
{
   int i = 0, j = 0;
   del_files_by_postfix("/tmp", 11111);

   return(0);
}