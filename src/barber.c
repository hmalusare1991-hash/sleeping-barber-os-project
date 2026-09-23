/*
 * Sleeping Barber Problem - Process/Thread Synchronization
 * Implemented in C using POSIX threads (pthreads) and semaphores.
 *
 * Compile: gcc barber.c -o barber -lpthread
 * Run:     ./barber
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define NUM_CHAIRS 3          // Number of waiting room chairs (N)
#define NUM_CUSTOMERS 10      // Total customers to simulate

sem_t customers;              // Counts waiting customers
sem_t barbers;                // Signals barber availability
pthread_mutex_t accessSeats;  // Protects freeSeats (critical section)

int freeSeats = NUM_CHAIRS;
int customers_served = 0;
int customers_turned_away = 0;

// ---------- Barber thread ----------
void *barber(void *arg) {
    while (1) {
        sem_wait(&customers);              // sleep until a customer arrives

        pthread_mutex_lock(&accessSeats);
        freeSeats++;                       // one chair becomes free
        pthread_mutex_unlock(&accessSeats);

        sem_post(&barbers);                // barber ready for this customer

        printf("Barber: cutting hair...\n");
        sleep(1);                          // simulate haircut duration
        printf("Barber: finished a haircut.\n");
    }
    return NULL;
}

// ---------- Customer thread ----------
void *customer(void *arg) {
    int id = *(int *)arg;

    pthread_mutex_lock(&accessSeats);
    if (freeSeats > 0) {
        freeSeats--;
        printf("Customer %d: arrived, took a seat. (Free seats: %d)\n", id, freeSeats);
        pthread_mutex_unlock(&accessSeats);

        sem_post(&customers);              // notify the barber
        sem_wait(&barbers);                // wait for barber to be ready

        printf("Customer %d: is getting a haircut.\n", id);
        customers_served++;
    } else {
        pthread_mutex_unlock(&accessSeats);
        printf("Customer %d: arrived, no free chairs -> left without a haircut.\n", id);
        customers_turned_away++;
    }

    return NULL;
}

int main() {
    pthread_t barber_thread;
    pthread_t customer_threads[NUM_CUSTOMERS];
    int ids[NUM_CUSTOMERS];

    sem_init(&customers, 0, 0);
    sem_init(&barbers, 0, 0);
    pthread_mutex_init(&accessSeats, NULL);

    srand(time(NULL));

    printf("=== Sleeping Barber Simulation (Chairs = %d, Customers = %d) ===\n\n",
           NUM_CHAIRS, NUM_CUSTOMERS);

    // Start the barber as a background (detached-style) thread
    pthread_create(&barber_thread, NULL, barber, NULL);

    // Spawn customers with randomized arrival delay
    for (int i = 0; i < NUM_CUSTOMERS; i++) {
        ids[i] = i + 1;
        usleep((rand() % 500 + 100) * 1000);  // random arrival delay (0.1s - 0.6s)
        pthread_create(&customer_threads[i], NULL, customer, &ids[i]);
    }

    // Wait for all customers to finish their attempt
    for (int i = 0; i < NUM_CUSTOMERS; i++) {
        pthread_join(customer_threads[i], NULL);
    }

    // Give the barber a moment to finish the last haircut in progress
    sleep(2);

    printf("\n=== Simulation Summary ===\n");
    printf("Customers served      : %d\n", customers_served);
    printf("Customers turned away : %d\n", customers_turned_away);

    pthread_mutex_destroy(&accessSeats);
    sem_destroy(&customers);
    sem_destroy(&barbers);

    // Note: barber_thread runs an infinite loop, so we exit the process directly.
    return 0;
}
