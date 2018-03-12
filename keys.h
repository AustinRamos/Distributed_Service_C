#ifndef KEYS_H_   /* Include guard */
#define KEYS_H_

//methods declared with signaturees adn parameter as specified in write up.



int init(char *qname);  

int set_value(int key, char *value1, float value2);

int get_value(int key, char *value1, float *value2);

int modify_value(int key, char *value1, float *value2);

int delete_key(int key);

int num_items();


#endif // KEYS_H_
