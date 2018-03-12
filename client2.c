
#include <mqueue.h>
#include "keys.c"
#include <string.h>
#include <pthread.h>
#include <fcntl.h> /* flags */
#include <sys/stat.h> /* modes */
#include <stdio.h>


int main(void) {
	char *qname = "/CLIENT_2";


int resp;
	
	float fl = 2.3;
		int x = set_value( 1,"test1",fl ,qname);
		if(x==0){
	printf("set_value SUCCESS \n");
	}else{
			printf("set_value failure \n");
		}
		
		fl = 2.012;
		 x = set_value( 2,"test2",fl ,qname);
		if(x==0){
	printf("set_value SUCCESS \n");
	}else{
			printf("set_value failure \n");
		}
		
	
		
			int y = num_items(qname);
	printf("Number of items: %d \n", y);
	
	
	char * c; float f;
	get_value(1,&c,&f, qname);	
	//*weird pointer shit going on here... 
	printf("client side key==1 float: %f \n", f);
	printf("client side key==2 str: %s \n", c);
				
				float *flt;
				*flt = 9.9;
				modify_value(1,"new string", flt, qname);
				
				get_value(1,&c,&f, qname);	

	printf("client side key==1 new float: %f \n", f);
	printf("client side  key==1 new str: %s \n", c);
				
}

