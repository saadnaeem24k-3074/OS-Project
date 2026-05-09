
#define _POSIX_C_SOURCE 199309L
#include "traffic.h"
#include <stdlib.h>
#include <time.h>

Lane            g_lanes[NUM_LANES];
int             g_total_passed = 0;
int             g_cycle_count  = 0;
int             g_ped_crossing = 0;
int             g_ped_count    = 0;
int             g_running      = 1;



pthread_mutex_t g_mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  g_cv  = PTHREAD_COND_INITIALIZER;
sem_t           g_sem;


void sleep_ms(int ms) {
    struct timespec t = { ms / 1000, (ms % 1000) * 1000000L };
    nanosleep(&t, NULL);
}


void *controller_thread(void *arg) {
    (void)arg;
    int idx = 0;  

    while (g_running) {

      
        pthread_mutex_lock(&g_mtx);
        int ped = g_ped_crossing;
        pthread_mutex_unlock(&g_mtx);

        if (ped) {
            
            pthread_mutex_lock(&g_mtx);
            for (int i = 0; i < NUM_LANES; i++)
                g_lanes[i].signal = SIG_RED;
            pthread_cond_broadcast(&g_cv); 
            pthread_mutex_unlock(&g_mtx);

            sleep_ms(PED_MS);   

        
            pthread_mutex_lock(&g_mtx);
            g_ped_crossing = 0;
            g_ped_count++;
            pthread_cond_broadcast(&g_cv);
            pthread_mutex_unlock(&g_mtx);
            continue;           
        }

      
        sem_wait(&g_sem);
        if (!g_running) { sem_post(&g_sem); break; }

 
        pthread_mutex_lock(&g_mtx);
        g_lanes[idx].signal = SIG_GREEN;
        pthread_cond_broadcast(&g_cv);   
        pthread_mutex_unlock(&g_mtx);

        sleep_ms(GREEN_MS);   

        
        pthread_mutex_lock(&g_mtx);
        g_lanes[idx].signal = SIG_YELLOW;
        pthread_cond_broadcast(&g_cv);
        pthread_mutex_unlock(&g_mtx);

        sleep_ms(YELLOW_MS);  


        pthread_mutex_lock(&g_mtx);
        g_lanes[idx].signal = SIG_RED;
        g_cycle_count++;     
        pthread_cond_broadcast(&g_cv);
        pthread_mutex_unlock(&g_mtx);

      
        sem_post(&g_sem);

        idx = (idx + 1) % NUM_LANES;   
        sleep_ms(300);                  
    }
    return NULL;
}

/* ════════════════════════════════════════════════════════════════════ */
void *lane_thread(void *arg) {
    int idx = *((int *)arg);
    srand((unsigned int)time(NULL) + idx * 37); 
    while (g_running) {

        
        pthread_mutex_lock(&g_mtx);
        while (g_lanes[idx].signal != SIG_GREEN && g_running)
            pthread_cond_wait(&g_cv, &g_mtx); 
        pthread_mutex_unlock(&g_mtx);

        if (!g_running) break;

        
        while (g_running) {
            pthread_mutex_lock(&g_mtx);

            
            if (g_lanes[idx].signal != SIG_GREEN) {
                pthread_mutex_unlock(&g_mtx);
                break;
            }

           
            int drain = (rand() % 2) + 1;
            if (drain > g_lanes[idx].queue)
                drain = g_lanes[idx].queue;

            g_lanes[idx].queue   -= drain;  
            g_lanes[idx].passed  += drain;  
            g_total_passed       += drain;  

            pthread_mutex_unlock(&g_mtx);
            sleep_ms(600);  
        }
    }
    return NULL;
}
