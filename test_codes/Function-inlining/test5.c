#include <stdio.h>

int f1(int x){
	return  f2(2);
	
}

int f2(int y){
	return 1;
}


int main()
{
    int c= f1(5);
    return 0;
}
