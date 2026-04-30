#define _POSIX_C_SOURCE 199309L
#include "traffic.h"
#include "raylib.h"
#include<stdio.h>
#include<string.h>


const char *DIR[]={"North","Eash","South","West"};

static Color sig_color(int sig){
    if(sig== SIG_GREEN) return GREEN;
    if(sig== SIG_YELLOW) return YELLOW;
    return RED;
}