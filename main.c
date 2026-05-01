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

static void draw(int W,int H){

    int panel_w=270; //left panel width
    int road=55; //half width of each road arm
    int rx = panel_w; //roads starts from here i.e 270px
    int rw=W-panel_w; //total road withd
    int cx=rx+rw/2; //x-intercept
    int cy=H/2 //y intercept

    ClearBackground((Color){20,22,25,255}); //called every frame to reset canvas
    //green rectangles
    DrawRectangle(rx,0,cx-road-rx,cy-road,(Color){34,85,34,255});
    DrawRectangle(cx+road,0,W-(cx+road),cy-road,(Color){34,85,34,255});
    DrawRectangle(rx,cy+road,cx-road-rx,H-(cy+road),(Color){34,85,34,255});
    DrawRectangle(cx+road,cy+road,W-(cx+road),H-(cy+road),(Color){34,85,34,255});

    //road for E and W
    DrawRectangle(rx,cy-road,rw,road*2,DARKGRAY);
    DrawRectangle(cx-road,0,road*2,H,DARKGRAY);

    //intersection road
    DrawRectangle(cx-road,cy-road,road*2,road*2,GRAY);
    

}