//
// Created by madhushika on 10/5/18.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <time.h>
#include "timer.h"

long n, m;
int mMemberOperations, mInsertOperations, mDeleteOperations;
double mMember, mInsert, mDelete;
double start, finish, elapsed;
int thread_count;

struct list_node_s {
    int data;
    struct list_node_s* next;
};

pthread_mutex_t lock ;
struct list_node_s* head = NULL;

int Insert(int value, struct list_node_s** head_p);
int Member(int value, struct list_node_s* head_p);
int Delete(int value, struct list_node_s** head_pp);
void Get_args(int argc, char* argv[]);
void Usage(char* prog_name);
void *operate(void* rank);

int main(int argc, char* argv[]) {
    // Get the command line args
    Get_args(argc, argv);

    int randValue;

    //generate number of each operations
    mMemberOperations = mMember * m;
    mInsertOperations = mInsert * m;
    mDeleteOperations = mDelete * m;


    head = malloc(sizeof(struct list_node_s));
    head->next = NULL;

    //creating the link list
    for (int index = 0; index < n; index++) {
        randValue = rand() % 65536;
        Insert(randValue, &head);
    }

    long thread;
    pthread_t* thread_handles;

    thread_handles = malloc(thread_count * sizeof(pthread_t));

    //start the timer
    GET_TIME(start);

    //create threads
    for(thread = 0; thread < thread_count; thread++) {
        pthread_create(&thread_handles[thread], NULL, &operate, (void*) thread);

    }

    for(thread = 0; thread < thread_count; thread++) {
        pthread_join(thread_handles[thread], NULL);

    }

    free(thread_handles);

    //stop the timer
    GET_TIME(finish);
    elapsed = finish - start;
    printf("Elapsed time %e seconds\n", elapsed);

    return 0;

}

void *operate(void* rank) {

    int operationSelector;

    while(m>0){
        //create a random value among 0, 1, 2
        operationSelector = rand() % 3;

        //create a random value between 0 - (2^16-1)
        int randomValue;
        randomValue = rand() % 65536;

        //lock the mutex
        pthread_mutex_lock(&lock);

        if(operationSelector == 0 && mMemberOperations > 0){
            Member(randomValue, head);
            mMemberOperations--;
            m--;
        }

        else if(operationSelector == 1 && mInsertOperations > 0){
            Insert(randomValue, &head);
            mInsertOperations--;
            m--;
        }

        else if(operationSelector == 2 && mDeleteOperations > 0){
            Delete(randomValue, &head);
            mDeleteOperations--;
            m--;
        }

        //unlock the mutex
        pthread_mutex_unlock(&lock);

    }
    return NULL;
}



int Insert(int value, struct list_node_s** head_p){
    struct list_node_s* current_p = *head_p;
    struct list_node_s* pred_p = NULL;
    struct list_node_s* temp_p;

    while(current_p != NULL && current_p->data < value){
        pred_p = current_p;
        current_p = current_p->next;
    }

    if(current_p == NULL || current_p->data > value){
        temp_p = malloc(sizeof(struct list_node_s));
        temp_p->data = value;
        temp_p->next = current_p;
        if(pred_p == NULL)
            *head_p = temp_p;
        else
            pred_p->next = temp_p;
        return 1;
    }else{
        return 0;
    }

};

int Member(int value, struct list_node_s* head_p){
    struct list_node_s* curr_p = head_p;

    while (curr_p != NULL && curr_p->data < value)
        curr_p = curr_p->next;

    if(curr_p == NULL || curr_p->data > value){
        return 0;
    }else{
        return 1;
    }
};

int Delete(int value, struct list_node_s** head_pp){
    struct list_node_s* curr_p = *head_pp;
    struct list_node_s* pred_p = NULL;

    while(curr_p != NULL && curr_p->data < value){
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if(curr_p != NULL && curr_p->data == value){
        if(pred_p == NULL){
            *head_pp = curr_p->next;
        }
        else{
            pred_p->next = curr_p->next;
        }
        free(curr_p);
        return 1;
    }
    else{
        return 0;
    }
};

void Get_args(int argc, char* argv[]) {
    if (argc != 7) Usage(argv[0]);
    if (atof(argv[3]) + atof(argv[4]) +atof(argv[5]) != 1) Usage(argv[1]);
    n = (int)strtol(argv[1], NULL, 10);
    m = (int)strtol(argv[2], NULL, 10);
    mMember = atof(argv[3]);
    mInsert = atof(argv[4]);
    mDelete = atof(argv[5]);
    thread_count = strtol(argv[6], NULL, 10);
};

void Usage(char* prog_name) {
    fprintf(stderr, "usage: %s <number of elements> <number of operations> <mMember> <mInsert> <mDelete> <numbr Of Threads>\n", prog_name);
    fprintf(stderr,"mMember + mInsert + mDelete = 1.\n",prog_name);
    exit(0);
}
