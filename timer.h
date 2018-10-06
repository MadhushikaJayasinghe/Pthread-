//
// Created by madhushika on 10/5/18.
//

#ifndef PTHREAD_TIMER_H
#define PTHREAD_TIMER_H

#include <sys/time.h>

/* The argument now should be a double (not a pointer to a double) */

#define GET_TIME(now) { \
   struct timeval t; \
   gettimeofday(&t, NULL); \
   now = t.tv_sec + t.tv_usec/1000000.0; \
}

#endif
//PTHREAD_TIMER_H
