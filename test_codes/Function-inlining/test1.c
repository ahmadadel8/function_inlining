#include <stdio.h>

int f1(int x){
	return x+x;
}

int f2(int y){
	return y*y;
}


int main()
{
    int a = 2;
    int b = f1(a);
    int c = f2(5);
    return 0;
}
