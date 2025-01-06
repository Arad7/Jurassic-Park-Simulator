// gets the new amount of people for 9:00 - 10:59.59
    /*for(i=0; i < 120; i ++){
        pthread_mutex_lock(&minute_mutex);
        globalMinute++;
        pthread_mutex_unlock(&minute_mutex);
        sendPeople(poissonRandom(25));
        usleep(10000);
    }

    // gets the new amount of people for 11:00 - 13:59.59
    for(i=0; i < 180; i ++){
        pthread_mutex_lock(&minute_mutex);
        globalMinute++;
        pthread_mutex_unlock(&minute_mutex);
        sendPeople(poissonRandom(45));
        usleep(10000);
    }

    // gets the new amount of people for 14:00 - 15:59.59
    for(i=0; i < 120; i ++){
        pthread_mutex_lock(&minute_mutex);
        globalMinute++;
        pthread_mutex_unlock(&minute_mutex);
        sendPeople(poissonRandom(35));
        usleep(10000);
    }
    
    // gets the new amount of people for 16:00 - 18:59.59
    for(i=0; i < 180; i ++){
        pthread_mutex_lock(&minute_mutex);
        globalMinute++;
        pthread_mutex_unlock(&minute_mutex);
        sendPeople(poissonRandom(25));
        usleep(10000);
    }*/