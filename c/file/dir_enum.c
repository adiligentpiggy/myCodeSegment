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

//��ȡָ��Ŀ¼�µ������ļ��б� author:wangchangshaui jlu  
char** getFileNameArray(const char *path, int* fileCount)  
{  
    int count = 0;  
    char **fileNameList = NULL;  
    struct dirent* ent = NULL;  
    DIR *pDir;  
    char dir[512];  
    struct stat statbuf;  
  
//��Ŀ¼  
    if ((pDir = opendir(path)) == NULL)  
    {  
        myLog("Cannot open directory:%s\n", path);  
        return NULL;  
    }  
//��ȡĿ¼  
    while ((ent = readdir(pDir)) != NULL)  
    { //ͳ�Ƶ�ǰ�ļ������ж����ļ����������ļ��У�  
//�õ���ȡ�ļ��ľ���·����  
        snprintf(dir, 512, "%s/%s", path, ent->d_name);  
        //�õ��ļ���Ϣ  
        lstat(dir, &statbuf);  
        //�ж���Ŀ¼�����ļ�  
        if (!S_ISDIR(statbuf.st_mode))  
        {  
            count++;  
        }  
    } //while  
//�ر�Ŀ¼  
    closedir(pDir);  
//  myLog("��%d���ļ�\n", count);  
  
//�����ַ�ָ�����飬������һ���Ŀ��������ļ����ַ����Ŀռ�  
    if ((fileNameList = (char**) myMalloc(sizeof(char*) * count)) == NULL)  
    {  
        myLog("Malloc heap failed!\n");  
        return NULL;  
    }  
  
//��Ŀ¼  
    if ((pDir = opendir(path)) == NULL)  
    {  
        myLog("Cannot open directory:%s\n", path);  
        return NULL;  
    }  
//��ȡĿ¼  
    int i;  
    for (i = 0; (ent = readdir(pDir)) != NULL && i < count;)  
    {  
        if (strlen(ent->d_name) <= 0)  
        {  
            continue;  
        }  
        //�õ���ȡ�ļ��ľ���·����  
        snprintf(dir, 512, "%s/%s", path, ent->d_name);  
        //�õ��ļ���Ϣ  
        lstat(dir, &statbuf);  
        //�ж���Ŀ¼�����ļ�  
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
            myLog("��%d���ļ�:%s\n", i, ent->d_name);  
            i++;  
        }  
    } //for  
//�ر�Ŀ¼  
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
   
   //printf("getDayno��%d\n", getDayno);
   
   
   return(0);
}