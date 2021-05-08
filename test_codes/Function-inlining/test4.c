#include <stdio.h>

int f1(int x){
	int a = 2;
	int b = f2(a);
	return 1;
	
}

int f2(int y){
	return y*y;
}


int main()
{
    int c= f1(5);
    return 0;
}
