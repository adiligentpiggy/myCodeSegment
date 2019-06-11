#include <stdio.h>

void func_test_switch(int id)
{
    int i = 0;

    switch (id)
    {
    if(i > 0)
    {
        case 1:
            printf("do 1\n");
            break;
        case 2:
            printf("do 2\n");
            break;
        case 3:
            printf("do 3\n");
            break;
        case 4:
            printf("do 4\n");
            break;
    }
        case 5:
            printf("do 5\n");
            break;
        case 6:
            printf("do 6\n");
            break;
        default:
            printf("default, not do\n");
            break;
    }
}

void main()
{
    int j = 0;

    for (; j < 7; j++)
    {
        func_test_switch(j);
    }
}
