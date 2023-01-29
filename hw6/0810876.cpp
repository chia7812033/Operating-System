#include <pthread.h>
#include <semaphore.h>
#include <sys/reg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

int chairSize;
int customerSize;
int availableChairNum;
int servedCustomerCount;

pthread_t simulationThread;
pthread_t barberThread;

sem_t customer_sem;
sem_t barber_sem;
pthread_mutex_t available_chair_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t cout_mutex = PTHREAD_MUTEX_INITIALIZER;

void addCustomer(int index);
void *barberThreadFunc(void *arg);
void *customerThreadFunc(void *arg);


void *barberThreadFunc(void *arg) {
    while (servedCustomerCount < customerSize) {
        sem_wait(&customer_sem);

        pthread_mutex_lock(&available_chair_mutex);
        availableChairNum++;
        pthread_mutex_unlock(&available_chair_mutex);
        servedCustomerCount++;

        sem_post(&barber_sem);
    }

    pthread_exit(nullptr);
}

void *customerThreadFunc(void *arg) {
    pthread_mutex_lock(&available_chair_mutex);
    if (availableChairNum > 0) {
        availableChairNum--;
        pthread_mutex_unlock(&available_chair_mutex);

        sem_post(&customer_sem);

        pthread_mutex_lock(&cout_mutex);
        cout << "Customer " << *(int *)arg << " is cutting hair" << endl;
        pthread_mutex_unlock(&cout_mutex);

        sem_wait(&barber_sem);
    } else {
        pthread_mutex_unlock(&available_chair_mutex);
        addCustomer(*(int *)arg);
    }

    delete arg;

    pthread_exit(nullptr);
}

void addCustomer(int index) {
    int *index_ptr = new int(index);
    pthread_t customerThread;
    pthread_create(&customerThread, nullptr, customerThreadFunc, index_ptr);
    pthread_join(customerThread, NULL);
}

int main(int argc, char **argv) {
    customerSize = atoi(argv[1]);
    chairSize = atoi(argv[2]);
    if (argv[3])
    {
        cout << "Usage: ./barber <CUSTOMER_NUM> <CHAIR_NUM>" << endl;
        return 0;
    }
    if (customerSize > 50)
    {
        cout << "Usage: <CUSTOMER_NUM> no more than 50" << endl;
        return 0;
    }
    if (customerSize == 0)
    {
        cout << "No customer today, end the program!" << endl;
        return 0;
    }
    availableChairNum = chairSize;
    servedCustomerCount = 0;

    sem_init(&customer_sem, 0, 0);
    sem_init(&barber_sem, 0, 0);

    pthread_create(&barberThread, nullptr, barberThreadFunc, nullptr);

    for (int i = 0; i < customerSize; i++) {
        addCustomer(i + 1);
    }

    pthread_join(barberThread, NULL);

    return 0;
}