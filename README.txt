=============
Index numbers
=============
    140250D
    140684K

===================
Compile the program
===================
    gcc -g -Wall -o serial serial.c -lpthread -lm
    gcc -g -Wall -o parallel_lock parallel_lock.c -lpthread -lm
    gcc -g -Wall -o parallel_mutex parallel_mutex.c -lpthread -lm

===============
Run the program
===============
    ----------
    For serial
    ----------

        ./<output_file_name> <number of elements> <number of operations> <mMember> <mInsert> <mDelete>
        mMember	    - Fraction of Member operations
        mInsert	    - Fraction of Insert operations
        mDelete	    - Fraction of Delete operations

        Example: ./serial 100 10 0.50 0.30 0.2

    ----------------------------------
    For parallel_mutex & parallel_lock
    ----------------------------------

        ./<output_file_name> <number of elements> <number of operations> <mMember> <mInsert> <mDelete> <number Of Threads>
        mMember	    - Fraction of Member operations
        mInsert	    - Fraction of Insert operations
        mDelete	    - Fraction of Delete operations

        Examples: ./parallel_mutex 100 10 0.50 0.30 0.20 8
                  ./parallel_lock 100 10 0.50 0.30 0.20 8