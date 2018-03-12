
#include <mqueue.h>
#include "keys.c"
#include <string.h>
#include <pthread.h>
#include <fcntl.h> /* flags */
#include <sys/stat.h> /* modes */
#include <stdio.h>


//make concurrent?

int main(void) {
	char *qname = "/CLIENT_1";
	
//concurrent clients...

int resp;

printf("New client1 opened. \n");	
		
int y = num_items(qname);
	printf("Number of items: %d \n", y);
	
	float fl = 2.3;
		int x = set_value( 1,"test1",fl ,qname);
		if(x==0){
	printf("set_value(1,test1,2,3) executed correctly \n");
	}else{
			printf("set_value() failure \n");
		}
		
	
		int r = set_value( 1,"test1",fl ,qname);
		if(r==0){
	printf("set_value(1,test1,2,3) executed correctly \n");
	}else{
			printf("set_value() failure \n");
		}
		
			 y = num_items(qname);
			
	printf("Number of items: %d \n", y);
	
		
		int deletetest = delete_key(1, qname);
		if(deletetest==0){
			
			printf("(Deleted succesfully \n");
			}else{
					printf("delete() failure \n");
				}
				
				 y = num_items(qname);
			
	printf("Number of items: %d \n", y);
				
	printf("Init()s \n", y);
	
	init(qname);
	
		
				 y = num_items(qname);
			
	printf("Number of items: %d \n", y);
	
				
	printf("Client 1 demo finished. \n");
				
}

