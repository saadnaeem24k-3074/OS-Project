#define _POSIX_C_SOURCE 199309L
#include "traffic.h"
#include <stdlib.h>
#include <time.h>

void *vehicle_gen_thread(void *arg) {
    (void)arg;
    srand((unsigned int)time(NULL) + 777);  

    while (g_running) {

        sleep_ms(GEN_MS);       

        if (!g_running) break;  

        pthread_mutex_lock(&g_mtx);

        int lane  = rand() % NUM_LANES;       
        int count = (rand() % 3) + 1;         

        if (g_lanes[lane].queue + count > MAX_QUEUE)
            count = MAX_QUEUE - g_lanes[lane].queue;

        if (count > 0)
            g_lanes[lane].queue += count;

        pthread_mutex_unlock(&g_mtx);
    }
    return NULL;
}

void *pedestrian_thread(void *arg) {
    (void)arg;

    while (g_running) {

        sleep_ms(PED_INTERVAL); 

        if (!g_running) break;  

        pthread_mutex_lock(&g_mtx);
        g_ped_crossing = 1;
        pthread_mutex_unlock(&g_mtx);

    }
    return NULL;
}
