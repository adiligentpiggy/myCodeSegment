#include <stdio.h>
#include <string.h>
#include <pthread.h>
#define MAX_CMD_BUF 256

char ip_addr[16] = "0.0.0.0";
int execute_shell_cmd(const char *cmd, char * buf, int buflen)
{
    FILE *pp = popen(cmd, "r");

    if (!pp) {
        return -1;
    }

    while(fgets(buf, buflen, pp) != NULL)
    {
        printf("---%s\n", buf);
    }
    pclose(pp);

    return 0;
}

int get_my_ip_addr(char* my_ip_addr)
{
    int res = 0;
    if (strcmp(ip_addr, "0.0.0.0") == 0)
    {
        char buf[MAX_CMD_BUF];
        char cmd[MAX_CMD_BUF];
        stpcpy(cmd,
                "/sbin/ifconfig -a|grep inet|grep -v 127.0.0.1|grep -v inet6|awk '{print $2}'|tr -d '地址:'|tr -d 'addr:'");
        res = execute_shell_cmd(cmd, buf, sizeof(buf));
        strcpy(ip_addr, buf);
    }
    printf(ip_addr);
    strcpy(my_ip_addr, ip_addr);
    return res;
}

int main(int argc, char **argv) {
	char my_ip_addr[16];
	get_my_ip_addr(my_ip_addr);
	printf(my_ip_addr);
	return 0;
}