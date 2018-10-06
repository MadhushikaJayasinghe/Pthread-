#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <time.h>
#include "timer.h"

// input args
long n, m;
double mMember, mInsert, mDelete;
int thread_count;

int mMemberOps, mInsertOps, mDeleteOps;

struct list_node_s {
    int data;
    struct list_node_s *next;
};
struct list_node_s *head = NULL;

pthread_rwlock_t lock;

void get_args(int argc, char **argv);

void usage(char *prog_name);

int insert(int value, struct list_node_s **head_p);

int member(int value, struct list_node_s *head_p);

int delete(int value, struct list_node_s **head_pp);

void *operate();

void get_args(int argc, char **argv) {
    if (argc != 7) {
        usage(argv[0]);
    } else {
        n = (int) strtol(argv[1], NULL, 10);
        m = (int) strtol(argv[2], NULL, 10);
        mMember = atof(argv[3]);
        mInsert = atof(argv[4]);
        mDelete = atof(argv[5]);
        thread_count = (int) strtol(argv[6], NULL, 10);

        if (mMember + mInsert + mDelete != 1) {
            usage(argv[0]);
        } else {
            mMemberOps = (int) (mMember * m);
            mInsertOps = (int) (mInsert * m);
            mDeleteOps = (int) (mDelete * m);
        }
    }
}

void usage(char *prog_name) {
    fprintf(stderr,
            "Usage: %s <number of elements> <number of operations> <mMember> <mInsert> <mDelete> <num Of Threads>\n",
            prog_name);
    fprintf(stderr, "Sum of mMember, mInsert and mDelete should be equal to 1.\n");
    exit(0);
}

int insert(int value, struct list_node_s **head_p) {
    struct list_node_s *current_p = *head_p;
    struct list_node_s *pred_p = NULL;
    struct list_node_s *temp_p;

    while (current_p != NULL && current_p->data < value) {
        pred_p = current_p;
        current_p = current_p->next;
    }

    if (current_p == NULL || current_p->data > value) {
        temp_p = malloc(sizeof(struct list_node_s));
        temp_p->data = value;
        temp_p->next = current_p;
        if (pred_p == NULL)
            *head_p = temp_p;
        else
            pred_p->next = temp_p;
        return 1;
    } else {
        return 0;
    }
}

int member(int value, struct list_node_s *head_p) {
    struct list_node_s *curr_p = head_p;

    while (curr_p != NULL && curr_p->data < value)
        curr_p = curr_p->next;

    if (curr_p == NULL || curr_p->data > value) {
        return 0;
    } else {
        return 1;
    }
}

int delete(int value, struct list_node_s **head_pp) {
    struct list_node_s *curr_p = *head_pp;
    struct list_node_s *pred_p = NULL;

    while (curr_p != NULL && curr_p->data < value) {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if (curr_p != NULL && curr_p->data == value) {
        if (pred_p == NULL) {
            *head_pp = curr_p->next;
        } else {
            pred_p->next = curr_p->next;
        }
        free(curr_p);
        return 1;
    } else {
        return 0;
    }
}

void *operate() {
    int operationSelector;

    while (m > 0) {
        // create a random value among 0, 1, 2 to select the operation
        operationSelector = rand() % 3;

        // create a random value between 0 - (2^16-1)
        int randomValue;
        randomValue = rand() % 65536;


        if (operationSelector == 0 && mMemberOps > 0) {
            // lock the read lock
            pthread_rwlock_rdlock(&lock);

            member(randomValue, head);
            mMemberOps--;
            m--;

            // unlock the read lock
            pthread_rwlock_unlock(&lock);

        } else if (operationSelector == 1 && mInsertOps > 0) {
            // lock the write lock
            pthread_rwlock_wrlock(&lock);

            insert(randomValue, &head);
            mInsertOps--;
            m--;

            // unlock the write lock
            pthread_rwlock_unlock(&lock);
        } else if (operationSelector == 2 && mDeleteOps > 0) {
            // lock the write lock
            pthread_rwlock_wrlock(&lock);

            delete(randomValue, &head);
            mDeleteOps--;
            m--;

            // unlock the write lock
            pthread_rwlock_unlock(&lock);
        }
    }
    return 0;
}

int main(int argc, char *argv[]) {
    // get the command line args
    get_args(argc, argv);

    head = malloc(sizeof(struct list_node_s));
    head->next = NULL;

    // create the link list
    int randValue;
    for (int index = 0; index < n; index++) {
        randValue = rand() % 65536;
        insert(randValue, &head);
    }

    long thread;
    pthread_t *thread_handles;
    thread_handles = malloc(thread_count * sizeof(pthread_t));

    double start, finish, elapsed;
    // start the timer
    GET_TIME(start);

    // create threads
    for (thread = 0; thread < thread_count; thread++) {
        pthread_create(&thread_handles[thread], NULL, &operate,
                       (void *) thread);
    }
    for (thread = 0; thread < thread_count; thread++) {
        pthread_join(thread_handles[thread], NULL);
    }
    free(thread_handles);

    // stop the timer
    GET_TIME(finish);
    elapsed = finish - start;
    printf("%f", elapsed);
    return 0;
}