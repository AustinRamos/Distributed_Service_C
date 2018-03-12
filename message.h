#ifndef MESSAGE_H
#define MESSAGE_H  



struct request{
int key;
float flt;
char str[256];
char client_name[90]; //easiest way is to keep track of the client name here. hope this is ok.
};

#endif
