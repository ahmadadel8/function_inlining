#include <stdio.h>

void scan_int(int *x)
{
    scanf("%d", x);
    return;
}

int poly(int x)
{
    int y;
    y = x*3;
    return y;
}


void print_int(int x)
{
    printf("%d \n", x);
    return;
}

int main()
{
    int a;
    scan_int(&a);
    int p;
    p = poly(3);
    int r = a + p;
    print_int(r);
    return 0;
}
