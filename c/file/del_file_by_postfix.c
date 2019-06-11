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
  
//��Ŀ¼  
    if ((pDir = opendir(path)) == NULL)  
    {  
        myLog("Cannot open directory:%s\n", path);  
        return;  
    }  
//��ȡĿ¼  
    while ((ent = readdir(pDir)) != NULL)  
    { //ͳ�Ƶ�ǰ�ļ������ж����ļ����������ļ��У�  
      //�õ���ȡ�ļ��ľ���·����  
        snprintf(dir, 512, "%s/%s", path, ent->d_name);  
        //�õ��ļ���Ϣ  
        lstat(dir, &statbuf);  
        myLog("item:%s\n", dir);
        //�ж���Ŀ¼�����ļ�  
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
//�ر�Ŀ¼  
    closedir(pDir);  
}

int main()
{
   int i = 0, j = 0;
   del_files_by_postfix("/tmp", 11111);

   return(0);
}