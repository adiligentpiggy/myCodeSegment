#include <stdio.h>
#include "file_utils.h"

int g_var = 0;

static int _add(int a, int b) {
    printf("_add callad, a:%d, b:%d\n", a, b);
    return a+b;
}

void output_integer(time_t t) {
	printf("t=%ld\n", t);
}

void output_llinteger(llint t) {
	printf("llint=%lld\n", t);
}

int main(void) {
    int n = 1;
	time_t modify_time = get_file_modify_time("./a.sh");
	printf("modify_time=%ld\n", modify_time);
	
	printf("modify_time*1000=%lld\n", (llint)modify_time*1000);
	
	
	output_integer(modify_time);
	output_integer(modify_time*1000);
	output_llinteger((llint)modify_time*1000);
	
    /*printf("sizeof int:%d, pointer:%d\n", sizeof(int), sizeof(int*));
	printf("sizeof constant string:%d %d\n", sizeof("10.4.2"), sizeof('a'));
    printf("sizeof unsigned long long:%d, long:%d\n", sizeof(unsigned long long), sizeof(long));
	printf("sizeof constant string:%d %d\n", sizeof("10.4.2"), sizeof('a'));
    
    printf("one n=%d, g_var=%d\n", n, g_var);
    ++n;
    --n;
    
    g_var += 20;
    g_var -= 10;
    n = _add(1, g_var);
    printf("two n=%d, g_var=%d\n", n, g_var);*/
    
    return 0;
}
