Readme



About

This is a demonstration of a concurrent distributed service, which allows for the storage of triple values. the server side code is meant to be concurrent(multiple clients can be sending requests at the same time), and the interprosces communication between the server and client(s) is handled using POSIX Message queues. The entire project is written in C. 

The client side has access to a static library with the following functions. with a static library api so that clients can make the following calls without having to worry directly about interprocess communication at all: 

1 – init() 

2 – set_value()

3 – get_value()

4 – modify_value()

5 – delete_key()

6 - num_items()





How to generate and run the executables:

There are two demonstrations of clients using the api described above to make calls.

To build the two executables, all that is necessary is to run the command “make” in the linux terminal. This will compile the server executable, compile the keys.c code, archive it into a static library, and handle all the static library and pthread linking. The demos would then be available by running “./server” on one terminal, and “./client#” on another.





Client1 will do the following:

check the the number of items is correctly 0 on start.

add 1 tuple, show that the method on the server side returned that it was a success.

check the new total number of elements stored( which will be 1)

It then adds another tuple. 

check the new total number of elements stored( which will be 2)

It deletes 1 tuple 

check the new total number of elements stored( which will be 1)

Then it will init()

And show server side has been reset and totalnum is at 0 again.





Client2 will do the following: 

Will add 2 tuples. 

Will get 1 tuple and print it out.

Will change a tuple and print it out.


