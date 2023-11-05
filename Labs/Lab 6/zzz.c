#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t* isBusyMutex;

pthread_mutex_t* isSleepingMutex;
int isSleeping; // bool

pthread_mutex_t* openChairsMutex;
int openChairs;

int hairsLeft;

void* customer(void* ptr) {
    int customerID = (int) ptr;
    /* Empty Customer Chair Check */

    sleep(customerID * 3);

    printf("Customer %d: Walked in\n", customerID);
    pthread_mutex_lock(openChairsMutex);
    if (openChairs > 0) {
        printf("Customer %d: Sat Down\n", customerID);
        openChairs -= 1;
    } else {
        pthread_mutex_unlock(openChairsMutex);
        printf("Customer %d: No Seats Available\n", customerID);
        return NULL;
    }
    pthread_mutex_unlock(openChairsMutex);

    /* Barber Checks */
    
    // Initial lock for busy of ordering of customers
    pthread_mutex_lock(isBusyMutex);
    
    // Barber Sleeping Check
    pthread_mutex_lock(isSleepingMutex);
    if (isSleeping) {
        printf("Customer %d: Woke up barber\n", customerID);
        isSleeping = 0;
    } else {
        printf("Customer %d: Barber not busy. Taking spot\n", customerID);
    }
    pthread_mutex_unlock(isSleepingMutex);

    // Get a hair cut from barber
    hairsLeft = customerID;

    // Do nothing while there are still hairs
    while (hairsLeft != 0) {
    }
    printf("Customer %d: Haircut finished\n", customerID);

    // Finished haircut. Barber is no longer busy
    pthread_mutex_unlock(isBusyMutex);

    // Give Chair Back
    pthread_mutex_lock(openChairsMutex);
    openChairs += 1;
    pthread_mutex_unlock(openChairsMutex);

}

int main() {
    /* Initialize */

    // Mutex
    isSleepingMutex = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(isSleepingMutex, NULL);

    isBusyMutex = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(isBusyMutex, NULL);

    openChairsMutex = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(openChairsMutex, NULL);

    // Variables
    isSleeping = 1; // true
    openChairs = 5;
    hairsLeft = -1;

    /* Queue Customers */
    int customerCount = 10;
    pthread_t* customers = malloc(sizeof(pthread_t) * customerCount);

    for (int i = 0 ; i < customerCount; i++) {
        customers[i] = (pthread_t) malloc(sizeof(pthread_t));
        pthread_create(&customers[i], NULL, customer, (void*) i);
    }

    // Barber

    while (1) {
        /* Sleeping Section */
        
        // If no more hairs are left, go back to sleep. 
        if (hairsLeft == 0) {
            pthread_mutex_lock(isSleepingMutex);
            printf("Barber: Yawn\n");
            isSleeping = 1;
            pthread_mutex_unlock(isSleepingMutex);
        }

        while (isSleeping == 1) {
            printf("Barber: Sleeping\n");
            pthread_mutex_unlock(isSleepingMutex);
            sleep(1);
            pthread_mutex_lock(isSleepingMutex);
        }
        pthread_mutex_unlock(isSleepingMutex);

        /* Cutting Section */
        printf("Barber: Cutting for customer %d\n", hairsLeft);

        while (hairsLeft != 0) {
            printf("Barber: Cutting Hair %d\n", hairsLeft);
            sleep(1);
            hairsLeft -= 1;
        }

        printf("Barber: Finished cutting customer's hair\n");
        sleep(1);
    }
    
    return 0;
}
