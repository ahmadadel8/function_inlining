#include <stdio.h>

int f1(int x){
	int b = f2(2);
	return x+x;
	
}

int f2(int y){
	return y*y;
}


int main()
{
    int c= f1(5);
    return 0;
}
