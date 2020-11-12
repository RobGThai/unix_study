#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

struct node {
    int n_number;
    struct node *n_next;
} *head = NULL;

static void cleanup_handler(void *arg) {
    free(arg);
    (void) pthread_mutex_unlock(&mtx);
}

static void *thread_func(void *arg) {
    struct node *p;

    pthread_cleanup_push(cleanup_handler, p);
    while(true) {
        pthread_mutex_lock(&mtx);
        while (head == NULL)
            pthread_cond_wait(&cond, &mtx);

        p = head;
        head = head->n_next;
        printf("Got %d from front of queue\n", p->n_number);
        free(p);
        pthread_mutex_unlock(&mtx);
    }
    pthread_cleanup_pop(false);
    return (void *)true;
}

int main(void) {
    pthread_t tid;
    int i;
    struct node *p;

    pthread_create(&tid, NULL, thread_func, NULL);

    for (i = 0; i < 10; i++) {
        p = malloc(sizeof(struct node));
        p->n_number = i;
        pthread_mutex_lock(&mtx);
        p->n_next = head;
        head = p;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mtx);
        sleep(1);
    }
    
    printf("Joining...\n");
    pthread_cancel(tid);
    pthread_join(tid, NULL);
    printf("All done -- exiting\n");
    return EXIT_SUCCESS;
}

