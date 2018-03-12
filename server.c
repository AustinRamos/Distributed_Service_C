
#include <stdlib.h>
#include "message.h"
#include "vector.h"
#include <mqueue.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h> /* flags */
#include <sys/stat.h> /* modes */
#include <stdio.h>


//sudo sh -c 'echo 512 > /proc/sys/fs/mqueue/msg_max'


pthread_mutex_t mutex_msg;
int msg_not_copied = 1; /* TRUE = 1 */
pthread_cond_t cond_msg;
vector messages;

//helper: returns the index of the struct in the messages vector with the corresponding key.
int vector_index(int key){
	
	int i;
	
	
	for(i = 0;i<vector_total(&messages);i++){
	 //simple linear search.
      struct request * temp = malloc(sizeof(*temp));
     temp =  (struct request * )   vector_get(&messages, i);
     if(key==temp->key){
		 return i;
		 }
		}
		
	return -1;
	}



//thread function for init()

void process_message_init(struct request *msg){

 struct request *msg_local =  malloc(sizeof( struct request)); /* local message */
 
 mqd_t q_client; /* client queue */
int  result = 0;



pthread_mutex_lock(&mutex_msg);
msg_local = msg;
pthread_cond_signal(&cond_msg); 
pthread_mutex_unlock(&mutex_msg);


q_client = mq_open(msg_local->str, O_WRONLY);
if (q_client == -1){
result = -1; //so client knows the process failed.
perror("Can’t open client queue");
}
else {
mq_send(q_client, (char *) &result, sizeof(result), 0);
mq_close(q_client);

}
pthread_exit(0);
	}

void process_message_set_value(struct request *msg){

	 struct request *msg_local =  malloc(sizeof( struct request)); 
 
 mqd_t q_client; 
int  result = 0;




pthread_mutex_lock(&mutex_msg);

msg_local = msg;
int temp =  vector_total(&messages); 
 vector_add(&messages, (void*)msg);
 int temp1 =  vector_total(&messages);
pthread_cond_signal(&cond_msg); 
pthread_mutex_unlock(&mutex_msg);
if(temp==temp1){ //did not add vector succesfully. compares size before and after
	result = -1;
	printf("Not succesful. Size: %d \n", vector_total(&messages));
	}


q_client = mq_open(msg_local->client_name, O_WRONLY);
if (q_client == -1){
result = -1;
perror("Can’t open client queue");
}
else {

mq_send(q_client, (char *) &result, sizeof(result), 0);
mq_close(q_client);

}


pthread_exit(0);
	
}
void process_message_num_items(struct request *msg)
{
 struct request *msg_local =  malloc(sizeof( struct request));
 
 mqd_t q_client;
int  result = 0;




pthread_mutex_lock(&mutex_msg);

msg_local = msg;
result = vector_total(&messages);
pthread_cond_signal(&cond_msg); 
pthread_mutex_unlock(&mutex_msg);

printf("Alternate thread message: %s \n", msg_local->str);

q_client = mq_open(msg_local->client_name, O_WRONLY);
if (q_client == -1){
int  result = -1;
perror("Can’t open client queue");

}
else {
mq_send(q_client, (char *) &result, sizeof(result), 0);
mq_close(q_client);
}
}

void process_message_delete_value(struct request *msg){
	
struct request *msg_local =  malloc(sizeof( struct request)); 
 
 mqd_t q_client; 
int  result = 0;

pthread_mutex_lock(&mutex_msg);
msg_local = msg;
int temp =  vector_total(&messages);

int index = vector_index(msg_local->key);

if(index==-1){
	printf("Error, key not found. \n");
	}
vector_delete(&messages, index);
printf("After deletion, total number of elements: %i \n", vector_total(&messages));
int temp1 =  vector_total(&messages);
 
 
pthread_cond_signal(&cond_msg); 
pthread_mutex_unlock(&mutex_msg);
if(temp==temp1 || index==-1){ //did not delete vector succesfully.
	result = -1;
	printf("Not succesful. Size: %d \n", vector_total(&messages));
	}

q_client = mq_open(msg_local->client_name, O_WRONLY);
if (q_client == -1){
	result = -1;
perror("Can’t open client queue");

}
else {

mq_send(q_client, (char *) &result, sizeof(result), 0);
mq_close(q_client);

}
pthread_exit(0);	
}

void process_message_get_value(struct request *msg){
struct request *msg_local =  malloc(sizeof( struct request)); 
 
 mqd_t q_client; 
struct request *response =  malloc(sizeof( struct request));


pthread_mutex_lock(&mutex_msg);

msg_local = msg;

int index = vector_index(msg->key);
response = (struct request * )   vector_get(&messages, index);


pthread_mutex_unlock(&mutex_msg);



q_client = mq_open(msg_local->client_name, O_WRONLY);
if (q_client == -1){
perror("Can’t open client queue");

}
else {
mq_send(q_client, (char *) response, sizeof(*response), 0);
mq_close(q_client);
}
	
	}

int main(void)
{
//have to init vector:	
    vector_init(&messages);
	
	
mqd_t q_server; /* server queue */
 struct request *msg = (struct request *) malloc(sizeof( *msg)); /* message to receive */
struct mq_attr q_attr; /* queue atributes */
pthread_attr_t t_attr[6]; /* thread atributes */


q_attr.mq_maxmsg = 20;
q_attr.mq_msgsize = sizeof(*msg);


pthread_t thid[6]; //need reference to threads

q_server = mq_open("/server",  O_RDONLY | O_CREAT, 0777 , &q_attr);

if (q_server == -1) {
perror("Can't create server queue");
return 1;
}

pthread_mutex_init(&mutex_msg, NULL);
pthread_cond_init(&cond_msg, NULL);
int i;
//initialize 1 thread for each thread process function
for( i = 0;i<6;i++){
pthread_attr_setdetachstate(&t_attr[i], PTHREAD_CREATE_DETACHED);
pthread_attr_init(&t_attr[i]);
}

while (1) {
	
	 struct request *msg_type = (struct request *) malloc(sizeof( *msg_type));
	
	
	printf(" waiting for Struct Type (where 1<=k<=6 ) \n");

ssize_t ret = mq_receive(q_server, (char *)msg_type, sizeof( *msg_type), 0); 

if(ret==-1){
	perror("Error on recieve: ");
	}
	
	//We have recieved a type, and will go into one of thee corresponding branches:
if(msg_type->key==1){
	//all i have to do is free the vector here.
	
	
int x = pthread_create(&thid[0], &t_attr[0], (void *)process_message_init, msg_type );

pthread_join(thid[0],NULL);

if(x!=0){
	perror("Error creating init thread!");
	}

	}
	//set_value()
	else if(msg_type->key==2){
		//wait for second message, containing struct to be inserted
		ssize_t ret = mq_receive(q_server, (char *)msg_type, sizeof( *msg_type), 0); 
		if(ret==-1){
			perror("mq_revieve error: ");
			}
		
		int x = pthread_create(&thid[1], &t_attr[1], (void *)process_message_set_value, msg_type );

pthread_join(thid[1],NULL);
		}
		//get_value():
		else if(msg_type->key==3){

			//wait for second message with key value of desired strucy
			ssize_t ret = mq_receive(q_server, (char *)msg_type, sizeof( *msg_type), 0); 
			int x = pthread_create(&thid[2], &t_attr[2], (void *)process_message_get_value, msg_type );

pthread_join(thid[2],NULL);
			
			}
			else if(msg_type->key==4){
				//modify value(just delete and than insert. not even necessary on this end, I just make the call
			// on keys.c. will keep here to avoid confusion.
				}
				else if(msg_type->key==5){
				//delete
				
				//we wait for second message with actual key value
				ssize_t ret = mq_receive(q_server, (char *)msg_type, sizeof( *msg_type), 0); 
				
				int x = pthread_create(&thid[4], &t_attr[4], (void *)process_message_delete_value, msg_type );

pthread_join(thid[4],NULL);
			
				}
	else if(msg_type->key==6){
	//all i have to do is free the vector here.
	
int test =	pthread_create(&thid[5], &t_attr[5], (void *)	process_message_num_items, msg_type );
if(test!=1){
	perror("error on pthread: ");
	}
pthread_join(thid[5],NULL);
	printf("Number of messages - messages cleared \n");

	}
	//**everything else will take place in individual ifs like this, so while(1) loop is always waiting for key values
	else{
	
		
printf("waiting for real message \n");

printf("Main thread Message: %s \n", msg->str);
if(ret==-1){
	printf("MESSAGE SIZE: %ld",sizeof(&msg));
	
	perror("ERR:");

	}
//pthread_create(&thid, &t_attr, (void *)process_message, msg);
//pthread_join(thid,NULL);




//msg_not_cpied first init to true. set to 0 in process message(), after message is succesfully copied.

/* wait for thread to copy message */

pthread_mutex_lock(&mutex_msg);

//work out message not copied stuff they are all off
while (msg_not_copied==1){
printf("copying message \n");
 pthread_cond_wait(&cond_msg, &mutex_msg);
 printf("after con wait \n");
msg_not_copied = 0;

pthread_mutex_unlock(&mutex_msg);
 printf("pthread_mutex_unlock(&mutex_msg); \n");

}

printf("**** WILL NOW WAIT FOR NEXT MESSAGE \n");
}
}
//Main done 
}
//** most current**** stuck on only being able to send 2 messages.






