#include <stdio.h>

int pow2(int x)
{
    int y;
    y = x*x;
    return y;
}

int poly(int y)
{
    int p2;
    p2 = pow2(7);
    p2 = p2 + 10;
    p2 = p2 - y;
    return p2;
}

void print_int(int x)
{
    printf("%d \n", x);
    return;
}

int main()
{
    int p;
    p = poly(3);
    print_int(p);
    return 0;
}
