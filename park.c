#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include "random437.h"

FILE *output;
int CARNUM, MAXPERCAR;
int totalArrivals, totalRiders, totalRejected, totalWaitingTime;
double averageWaitingTime;
int worstLineLength, worstLineTime, waitingQueue, globalMinute;
// passenger mutex protects the variables associated with passengers arriving and in the waiting queue.
// loading mutex prevents explorers from taking on passengers simultaneously.
// minute mutex protects the global minute variable from receiving simultaneous changes.
pthread_mutex_t passenger_mutex, minute_mutex, sync_mutex;
pthread_cond_t start_signal;

void* runExplorer(){
    for(int i = 0; i < 600; i++){
        pthread_mutex_lock(&sync_mutex);
        while(globalMinute != i){
            pthread_cond_wait(&start_signal, &sync_mutex);
        }
        pthread_mutex_lock(&passenger_mutex);
        if(waitingQueue < MAXPERCAR){
            totalRiders = totalRiders + waitingQueue;
            waitingQueue = 0;
        }else{
            totalRiders = totalRiders + MAXPERCAR;
            waitingQueue = waitingQueue - MAXPERCAR;
        }
        pthread_mutex_unlock(&passenger_mutex);
        pthread_mutex_unlock(&sync_mutex);
    }
}

// method that used in passengerHandler in order to either accept or reject incoming people 
void sendPeople(int incomingPeople){
    int accepted, rejected, hours, minutes;
    //////////////////////////////////////////////////////////////
    // WE CAN ALSO LOCK AND UNLOCK BEFORE AND AFTER THE CHECKING OF WIATING Queue (and use verrible here) //
    // AND ALSO WHEN ADDING AT THE END INSTEAD OF BLOCKING THE WHOLE SECTION //
    ///////////////////////////////
    pthread_mutex_lock(&passenger_mutex);

    if (waitingQueue >= 800){
        accepted = 0;
        rejected = incomingPeople;
    }
    else if (waitingQueue + incomingPeople >= 800)
    {
        accepted = 800 - waitingQueue;
        rejected = incomingPeople - accepted;
    }
    else{
        accepted = incomingPeople;
        rejected = 0;
    }
    waitingQueue += accepted;
    if(waitingQueue > worstLineLength){
        worstLineLength = waitingQueue;
        worstLineTime = globalMinute;
    }
    totalArrivals = totalArrivals + incomingPeople;
    totalRejected = totalRejected + rejected;
    hours = 9 + (globalMinute / 60);
    minutes = (globalMinute % 60);
    fprintf(output, "%d arrive %d reject %d wait-line %d at %02d:%02d:00\n", globalMinute, incomingPeople, rejected, waitingQueue, hours, minutes);
    pthread_mutex_unlock(&passenger_mutex);
}

void* passengerHandler(){
    //add passengers to wait queue or send away
    //int incomingPeople;
    for(int i = 0; i < 600; i++){
        pthread_mutex_lock(&sync_mutex);
        pthread_mutex_lock(&minute_mutex);
        globalMinute = i;
        if(i < 120){
            sendPeople(poissonRandom(25));
        }else if(i < 300){
            sendPeople(poissonRandom(45));
        }else if(i < 420){
            sendPeople(poissonRandom(35));
        }else{
            sendPeople(poissonRandom(25));
        }
        pthread_mutex_unlock(&minute_mutex);
        pthread_cond_broadcast(&start_signal);
        pthread_mutex_unlock(&sync_mutex);
        usleep(10000);
        //Count the number of people still in line after one minute of simulated rides.
        totalWaitingTime = totalWaitingTime + waitingQueue;
    }
}

int main(int argc, char** argv){
    int i, j;
    if(argc == 5){
        CARNUM = atoi(argv[2]);
        MAXPERCAR = atoi(argv[4]);
        totalArrivals = totalRejected = totalRiders = totalWaitingTime = worstLineLength = worstLineTime = waitingQueue = 0;
        //Initialize global minute to -1 to hold all vehicle thread before first passenger arrival.
        globalMinute = -1;
        printf("CARNUM: %d, MAXPERCAR: %d\n", CARNUM, MAXPERCAR);
        pthread_t tid[CARNUM + 1];
        output = fopen("output.txt", "w");
        pthread_mutex_init(&passenger_mutex, NULL);
        pthread_mutex_init(&minute_mutex, NULL);
        pthread_mutex_init(&sync_mutex, NULL);
        pthread_cond_init(&start_signal, NULL);
        
        for(i = 0; i < (CARNUM + 1); i++){
            if(i == 0){
                //create passenger handler thread before engaging vehicles
                pthread_create(&tid[i], NULL, passengerHandler, NULL);
            }else{
                //create vehicle threads
                pthread_create(&tid[i], NULL, runExplorer, NULL);
            }
        }

        for(j = 0; j < (CARNUM + 1); j++){
            pthread_join(tid[j],NULL);
        }

        averageWaitingTime = (double)totalWaitingTime/(double)totalRiders;
        int hours = 9 + (worstLineTime / 60);
        int minutes = (worstLineTime % 60);
        printf("Total Arrivals: %d total riders: %d total rejected: %d average waiting time (mins): %f worst line length: %d at %02d:%02d:00\n",
        totalArrivals, totalRiders, totalRejected, averageWaitingTime, worstLineLength, hours, minutes);
        fclose(output);
        pthread_mutex_destroy(&passenger_mutex);
        pthread_mutex_destroy(&minute_mutex);
        pthread_mutex_destroy(&sync_mutex);
        pthread_cond_destroy(&start_signal);
    }
    return 0;
}