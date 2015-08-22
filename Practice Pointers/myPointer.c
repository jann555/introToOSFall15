#include <stdio.h>
#include <string.h>

int main(int argc, char **argv){
  
  int x=1,y=2, z[30];
  int *ip; /*This is a pointer to int*/
	z[0]=34;
	z[1]=25;
	z[2]=36;	
	char s;
	char m[100];
  	printf("This is a test \n");
	printf("This is x original value %d \n", x);
	ip=&x;
	printf("This is ip %d Pointing to X %d \n", ip[0], x);
	x =12;
	printf("This is ip %d Pointing to X after changing value of X %d \n", ip[0], x);
	printf("This is Y original value %d \n",y);
	y=*ip;
	*ip=5;
	printf("This is a y %d pointing to *ip \n", y);
	ip=&z[0];
	printf(" x= %d, y=%d, z[0]=%d \n", x,y,z[0]);
	printf("This is ip= %d pointing to z[0] \n", ip[0]);
	y=*(ip+1);
	printf("This is y= %d pointing to z[0+1] \n", y);
	printf("This is x current value %d \n", x);
	*ip=*ip+15;
	printf("This is ip(34) +15 = %d \n", z[0]);
	y =*ip +1;
	printf("This is a y %d pointing to *ip +1 \n", y);
	char *pmessage ="This is a message from a pointer";
	strcpy(m, pmessage);
	printf("Message: %s \n", m);


  
  return 0;
}
