#include <stdlib.h>
#include "message.h"
#include <mqueue.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h> /* flags */
#include <sys/stat.h> /* modes */
#include <stdio.h>



//to recompile static lib:

// gcc -g -c -Wall -pthread -o keys -l keys.c 
// ar -cvq libkeys.a keys




//  initialization of the system.  all triples
//that were stored are destroyed. returns 0 on success , -1 on error.
int init(char * qname){
	printf("INIT STARTED");
	mqd_t q_server; /* server message queue */
mqd_t q_client; /* client message queue */

struct request req;
req.key = 1; strcpy(req.str, qname); req.flt = 1.1;strcpy(req.client_name, qname); //1 signifies INIT - others are garbage values.
int resp;
struct mq_attr attr;
attr.mq_maxmsg = 1;
attr.mq_msgsize = sizeof(int);

//open the queues
printf("\n sending the following name: %s \n", req.client_name );
q_client = mq_open(qname,  O_RDONLY|O_CREAT, 0777, &attr);
if(q_client==-1){
	perror("Client error: ");
	}
q_server = mq_open("/server", O_WRONLY);
if(q_server==-1){
	perror("Server error: ");
	}
	//send the int message telling the server to clear 
	struct mq_attr serv_attr; //= malloc(sizeof(*serv_attr));
	struct mq_attr new_serv_attr;
	 mq_getattr(q_server,  &serv_attr);
	 new_serv_attr = serv_attr;
	 new_serv_attr.mq_msgsize = sizeof(struct request);
	 mq_setattr(q_server, &new_serv_attr, &serv_attr);
int temp =	mq_send(q_server,(char *)&req, sizeof(struct request), 0);
	if(temp==-1){
		
		perror("Coudlnt send: ");
		}
	
	//get response, foward it 

	int x = mq_receive(q_client, (char *)&resp, sizeof(int), 0);
	if(x!=1){
		perror("Error recieving: ");
		}

	if(resp==0){
		printf("success keys.c\n");
		}
		if(resp==-1){
				printf("error keys.c");
			}
	//close the queues, unlink client
	mq_close(q_server);
mq_close(q_client);
mq_unlink(qname);

//return resp, 0 for success, -1 for failure
return resp;
	}
	


//inserts tuple element <key, value1, value2>.  returns 0 if it was inserted successfully and -1 in
// error. it is an error try to insert a key that already exists	
	
int set_value(int key, char *value1, float value2, char * qname){

	mqd_t q_server; /* server message queue */
mqd_t q_client; /* client message queue */

struct request req;
//will first send dummy struct telling it it is a set_value type (key==2)
req.key = 2; strcpy(req.str, value1); req.flt = value2; strcpy(req.client_name, qname);  //1 signifies INIT - others are garbage values.
//now here is real struct
printf("***Storing vector with String: %s, key: %i, float: %f \n", req.str, req.key, req.flt);
int resp;
struct mq_attr attr;
attr.mq_maxmsg = 1;
attr.mq_msgsize = sizeof(int);

//open the queues

q_client = mq_open(qname,  O_RDONLY|O_CREAT, 0777, &attr);
if(q_client==-1){
	perror("Client error: ");
	}
q_server = mq_open("/server", O_WRONLY);
if(q_server==-1){
	perror("Server error: ");
	}
	//send the int message telling the server to clear 
	int x = mq_send(q_server,(char *)&req, sizeof(struct request), 0);
	if(x!=1){
		perror("mq_send set_value error: ");
		}

	req.key = key;
	printf("***Storing vector with String: %s, key: %i, float: %f \n", req.str, req.key, req.flt);
mq_send(q_server,(char *)&req, sizeof(struct request), 0);

	mq_receive(q_client, (char *)&resp, sizeof(int), 0);
	
			
	//close the queues, unlink client
	mq_close(q_server);
mq_close(q_client);
mq_unlink(qname);
return resp;
		}
	//obtain the values associated with the key. **The values are returned in value1 and value2.**
//returns 0 on success and -1 on error, for example, if there is no element with that key	
int get_value(int key, char **value1, float *value2, char * qname){
	
	mqd_t q_server; /* server message queue */
mqd_t q_client; /* client message queue */

int result = 0;
struct request req;
req.key = 3; strcpy(req.str, qname); req.flt = 1.1;strcpy(req.client_name, qname); //1 signifies INIT - others are garbage values.
//for this example, struct wil be response rather than int.
struct request *resp = malloc(sizeof(*resp));
struct mq_attr attr;
attr.mq_maxmsg = 5;
attr.mq_msgsize = sizeof(struct request);

//open the queues

q_client = mq_open(qname,  O_RDONLY|O_CREAT, 0777, &attr);
if(q_client==-1){
	result = -1;
	perror("Client error: ");
	}
q_server = mq_open("/server", O_WRONLY);
if(q_server==-1){
	result = -1;
	perror("Server error: ");
	}
	//send the int message telling the server to clear 
	struct mq_attr serv_attr; //= malloc(sizeof(*serv_attr));
	struct mq_attr new_serv_attr;
	 mq_getattr(q_server,  &serv_attr);
	 new_serv_attr = serv_attr;
	 new_serv_attr.mq_msgsize = sizeof(struct request);
	 mq_setattr(q_server, &new_serv_attr, &serv_attr);

int temp =	mq_send(q_server,(char *)&req, sizeof(struct request), 0);
	if(temp==-1){
		perror("Coudlnt send: ");
	result = 0;
		}
	
	//now send second struct with int key as key. 
	req.key = key;
	
	int temp1 =	mq_send(q_server,(char *)&req, sizeof(struct request), 0);
	
	//get response, foward it 

	int x = mq_receive(q_client, (char *)resp, sizeof(struct request), 0);
	
	if(x==-1){
		perror("Error recieving: ");
		result = 0;
		}
			
			
			//here we assign the arg values the values in the struct
			*value2 = resp->flt;
		
			*value1 = resp->str;
			
			
			
	//close the queues, unlink client
	mq_close(q_server);
mq_close(q_client);
mq_unlink(qname);

//return resp, 0 for success, -1 for failure
return result;
	
	} 
	


//modify the values associated with a key(reference w parameters). returns 0 on success and -1 on error,
//for example, if there is no element with that key.	
int modify_value(int key, char *value1, float *value2, char* qname){
	//modify can just all delete and insert.
	
	
	int delete_success = delete_key(key, qname);
	int set_success = set_value(key,value1,*value2,qname );
	if(delete_success==1 && set_success==1){
		return 0;
		}else{
			return -1;
			}
	return -1;
	}
	
	// delete the element whose key is key.
// returns 0 on success and -1 on error. In case the key does not exist, -1 is also
//returned.
int delete_key(int key, char* qname){
		
	mqd_t q_server; /* server message queue */
mqd_t q_client; /* client message queue */

struct request req;
req.key = 5; strcpy(req.str, qname); req.flt = 1.1;strcpy(req.client_name, qname); //1 signifies INIT - others are garbage values.
int resp;
struct mq_attr attr;
attr.mq_maxmsg = 1;
attr.mq_msgsize = sizeof(int);

//open the queues

q_client = mq_open(qname,  O_RDONLY|O_CREAT, 0777, &attr);
if(q_client==-1){
	perror("Client error: ");
	}
q_server = mq_open("/server", O_WRONLY);
if(q_server==-1){
	perror("Server error: ");
	}
	//send the int message telling the server to clear 
	struct mq_attr serv_attr; //= malloc(sizeof(*serv_attr));
	struct mq_attr new_serv_attr;
	 mq_getattr(q_server,  &serv_attr);
	 new_serv_attr = serv_attr;
	 new_serv_attr.mq_msgsize = sizeof(struct request);
	 mq_setattr(q_server, &new_serv_attr, &serv_attr);
	
int temp =	mq_send(q_server,(char *)&req, sizeof(struct request), 0);
	if(temp==-1){
		perror("Coudlnt send: ");
		}
		
		//sending actual key value:
		req.key = key;
	mq_send(q_server,(char *)&req, sizeof(struct request), 0);
	
	
	//get response, foward it 

	int x = mq_receive(q_client, (char *)&resp, sizeof(int), 0);
	if(x==-1){
		perror("Error recieving: ");
		}

	if(resp==0){

		}
		if(resp==-1){
				printf("error keys.c");
			}
	//close the queues, unlink client
	mq_close(q_server);
mq_close(q_client);
mq_unlink(qname);

//return resp, 0 for success, -1 for failure
return resp;
		}
		
		
	//returns the number of items stored on the server. The call
//returns -1 in case of error. 
 int num_items(char* qname){
	 mqd_t q_server; 
mqd_t q_client; 

struct request req;
req.key = 6; strcpy(req.str, qname); req.flt = 1.1; strcpy(req.client_name, qname);//1 signifies INIT - others are garbage values.
int resp;
struct mq_attr attr;
attr.mq_maxmsg = 1;
attr.mq_msgsize = sizeof(int);

//open the queues
q_client = mq_open(qname,  O_RDONLY|O_CREAT, 0777, &attr);
if(q_client==-1){
	perror("Client error: ");
	}
q_server = mq_open("/server", O_WRONLY);
if(q_server==-1){
	perror("Server error: ");
	}
	//send the int message telling the server to clear 
	mq_send(q_server,(char *)& req, sizeof(struct request), 0);
	//get response, foward it 
	mq_receive(q_client, (char *)&resp, sizeof(int), 0);

	
		if(resp==-1){
				printf("error getting number of items. keys.c");
			}
	//close the queues, unlink client
	mq_close(q_server);
mq_close(q_client);
mq_unlink(qname);

//return resp, 0 for success, -1 for failure
return resp;
	 
	 } 
 

	

