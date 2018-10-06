# Pthread-
---------------------------
How to compile the program?
---------------------------

gcc -g -Wall -o <output_file_name> <c_file_name>.c -lpthread -lm

Example: gcc -g -Wall -o Serail Serail.c -lpthread


------------------------
How to run the program?
------------------------

------------------------
Mutex & Lock
------------------------

./<output_file_name> <number of elements> <number of operations> <mMember> <mInsert> <mDelete> <number Of Threads>

	mMember	    - Fraction of Member operations
	mInsert	    - Fraction of Insert operations
	mDelete	    - Fraction of Delete operations

Example: ./Lock 100 10 0.50 0.30 0.20 8
         ./Mutex 100 10 0.50 0.30 0.20 8

------------------------
Serial
------------------------

./<output_file_name> <number of elements> <number of operations> <mMember> <mInsert> <mDelete>


Example: ./Serial 100 10 0.50 0.30 0.2
