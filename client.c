
#include <mqueue.h>
#include "keys.c"
#include <string.h>
#include <pthread.h>
#include <fcntl.h> /* flags */
#include <sys/stat.h> /* modes */
#include <stdio.h>


int main(void) {
	char *qname = "/CLIENT_1";
	//gcc -g -Wall -pthread -o client client.c -lrt -L. -lkeys
	// gcc client.o -L. -lutil -o prog 
	
	//gcc -g -Wall -pthread -o client client.c -lrt -L. -lkeys
	
/*mqd_t q_server; //server message queue 
mqd_t q_client; //client message queue 
struct request req;
int res;
struct mq_attr attr;
attr.mq_maxmsg = 1;
attr.mq_msgsize = sizeof(int);
req.key = 1; strcpy(req.str, "/CLIENT_ONE"); req.flt = 1.1;
printf("%s",req.str);

q_client = mq_open("/CLIENT_ONE",  O_RDONLY|O_CREAT, 0777, &attr);
if(q_client==-1){
	perror("Client error: ");
	}
q_server = mq_open("/server", O_WRONLY);
if(q_server==-1){
	perror("Server error: ");
	}
// fill in request 

mq_send(q_server, (char *)&req, sizeof(struct request), 0);
printf("1st sent \n");
mq_send(q_server, (char *)&req, sizeof(struct request), 0);
printf("2nd sent \n");
mq_send(q_server, (char *)&req, sizeof(struct request), 0);
printf("3rd sent \n");
mq_receive(q_client, (char *)&res, sizeof(int), 0);
mq_close(q_server);
mq_close(q_client);
mq_unlink("/CLIENT_ONE");
*/



//ar -cvq libkeys.a keys.c
//q - dictionary.o


int resp;
/*resp = init(qname);
if(resp==1){
	printf("INIT SUCCESS \n");
	}else{
			printf("INIT failure \n");
		}
		
	*/	
		
	//int y = num_items(qname);
	//printf("Number of items: %d \n", y);
	
	float fl = 2.3;
		int x = set_value( 1,"test1",fl ,qname);
		if(x==0){
	printf("set_value SUCCESS \n");
	}else{
			printf("set_value failure \n");
		}
		
	
		
			//int y = num_items(qname);
	//printf("Number of items: %d \n", y);
	char * c; float f;
	get_value(1,&c,&f, qname);	
	//*weird pointer shit going on here... 
	printf("client side float: %f \n", f);
	printf("client side str: %s \n", c);
		
		int deletetest = delete_key(1, qname);
		if(deletetest==0){
			printf("DELETE SUCCESS \n");
			}else{
					printf("DELETE Failure \n");
				}
				
				float *flt;
				*flt = 9.9;
				modify_value(1,"new string", flt, qname);
				
				get_value(1,&c,&f, qname);	

	printf("client side float: %f \n", f);
	printf("client side str: %s \n", c);
				
}
