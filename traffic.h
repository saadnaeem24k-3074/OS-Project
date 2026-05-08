#ifndef TRAFFIC_H
#define TRAFFIC_H

#include <pthread.h>
#include <semaphore.h>

#define NUM_LANES    4        
#define GREEN_MS     4000    
#define YELLOW_MS    1200     
#define MAX_QUEUE    12     
#define GEN_MS       2000    
#define PED_INTERVAL 15000   
#define PED_MS       3500    

#define SIG_RED    0
#define SIG_YELLOW 1
#define SIG_GREEN  2


extern const char *DIR[];

typedef struct {
    int signal;             
    int queue;               
    int passed;  
} Lane;

extern Lane            g_lanes[NUM_LANES];
extern int             g_total_passed;  
extern int             g_cycle_count;    
extern int             g_ped_crossing;   
extern int             g_ped_count;     
extern int             g_running;        

extern pthread_mutex_t g_mtx;     
extern pthread_cond_t  g_cv;        
extern sem_t           g_sem;  

void *controller_thread  (void *arg);
void *lane_thread        (void *arg);
void *vehicle_gen_thread (void *arg);
void *pedestrian_thread  (void *arg);

void sleep_ms(int ms);

#endif 
