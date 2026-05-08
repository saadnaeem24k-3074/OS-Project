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

    // dashed yellow lines
    for(int x=rx; x<cx-road; x+=28){
        DrawRectangle(x,cy-2,18,4,Color{255,255,100,120});
    }

    for(int x=cx+road+10; x<W; x+=28){
        DrawRectangle(x,cy-2,18,4,Color{255,255,100,120});
    }

    for(int y=0; y<cy-road; y+=28){
        DrawRectangle(cx-2,y,4,18,Color{255,255,100,120});
    }

    for(int t=cy+road+10; y<H; y+=28){
        DrawRectangle(cx-2,y,4,18,Color{255,255,100,120});
    }

    //just some variables to store the copies and would later be used to draw the screen
    int sig[NUM_LANES], queue[NUM_LANES], passed[NUM_LANES];
    int total, cycles, ped_on, ped_count;

    pthread_mutex_lock(&g_mtx);//lock the shared data
    for(int i=0 ;i<NUM_LANES ; i++){
        sig[i]=g_lane[i].signal;
        queue[i]=g_lane[i].queue;
        passed[i]=g_lanes[i].passed;
    }

    total= g_total_passed;
    cycles=g_cycle_count;
    ped_on=g_ped_crossing;
    ped_count=g_ped_count;
    pthread_mutex_unlock(&g_mtx);

    if(ped_on){
        //nort strips
        for (int s = 0; s < 4; s++) {
            int sx = cx - road + s * (road*2/4);
            DrawRectangle(sx, cy-road-22, road*2/4-4, 16, WHITE);
        }
        //South arm stripes 
        for (int s = 0; s < 4; s++) {
            int sx = cx - road + s * (road*2/4);
            DrawRectangle(sx, cy+road+6,  road*2/4-4, 16, WHITE);
        }
        // West arm stripes 
        for (int s = 0; s < 4; s++) {
            int sy = cy - road + s * (road*2/4);
            DrawRectangle(cx-road-22, sy, 16, road*2/4-4, WHITE);
        }
        // East arm stripes
        for (int s = 0; s < 4; s++) {
            int sy = cy - road + s * (road*2/4);
            DrawRectangle(cx+road+6, sy, 16, road*2/4-4, WHITE);
        }

        DrawText("PEDESTRIAN CROSSING", cx-90, cy-road-42, 15,
                 (Color){255,220,0,255});
    }

    //singal position for each lane
    int lx[]={cx,cx+road+28,cx,cx-road-20};
    int ly[]={cy-road-28,cy,cy+road+28,cy};

    for(int i=0 ; i< NUM_LANES; i++){
        DrawCircle(lx[i],ly[i],17,BLACK); //circle outer color
        DrawCircle(lx[i],ly[i],12,sig_color(sig[i])); //circle inner color
        
        if(sig[i]==SIG_GREEN){
            DrawCircle(lx[i],ly[i],18,(Color){0,255,0,40}); //circle inner color
        }
            int draw_count=queue[i] > 8 ? 8 : queue[i];

            for(int j=0 ; j<draw_count; j++){
                int bx=qx[i]+qdx[i]*j;
                int by=qy[i]+qdy[i]*j;
                DrawRectangle(bx,by,11,11,SKYBLUE);
                DrawRectangleLines(bx,by,11,11,(Color){100,180,255,255});
                
            }

            if(queue[i] > 8){
                char extra[8];
                snprintf(extra,sizeof(extra,"+%d",queue[i]-8));
                DrawText(extra,qx[i]+qdx[i]*8,qy[i],qdy[i]*8,11,YELLOW);
            }
    }

    //labesl
    DrawText("N", cx-6,  14,    18, LIGHTGRAY);
    DrawText("S", cx-6,  H-22,  18, LIGHTGRAY);
    DrawText("W", rx+8,  cy-9,  18, LIGHTGRAY);
    DrawText("E", W-22,  cy-9,  18, LIGHTGRAY);


    //stats panle
    int px=10;
    int py=10;
    int pw=panel_w-14;

    //background
    DrawRectangle(0,0,panel_w-4,H,(Color){30,32,36,255});
    DrawRectangleLines(0,0,panel_w-4,H,(Color){80,80,80,255});

    //title
    DrawText("Traffic Controller",px,py,15,WHIYE);
    py+=12;
    DrawLine(px,py,px+pw,GRAY);
    py+=26

    DrawText("LANES",px,py,14,GRAY);
    py+=20;

    //used to convert signal number into text
    const char *sig_names[]={"RED","YLW","GRN"};
    Color sig_lc[] = { RED, YELLOW, GREEN };

    for(int i=0 ;i<NUM_LANES;i++){
        if(sig[i]==SIG_GREEN){
            DrawRectangle(px-4,py-2,pw,20,(Color){0,60,0,180});
        }

        DrawText(DIR[i],px,py,14,LIGHTGRAY);
        DrawText(sig_names[sig[i]],px+56,py,14,sig_lc[sig[i]]);

        char buf[32];
        snprintf(buf,sizeof(buf,"Q:%2d  P:%d",queue[i],passed[i]));
        DrawText(buf,px+90,py,14,LIGHTGRAY);
        py+=22;
    }

    py+=8;
    DrawLine(px,py,px+pw,py,GRAY);
    py+=12;
    
    DrawText("STATISTICS",px,py,14,LIGHTGRAY);
    py+=20;

    //prints stats
    char buf[64];
    snprintf(buf, sizeof(buf), "Total passed : %d", total);
    DrawText(buf, px, py, 14, LIGHTGRAY);
     py += 20;

    snprintf(buf, sizeof(buf), "Signal cycles: %d", cycles);
    DrawText(buf, px, py, 14, LIGHTGRAY); 
    py += 20;

    snprintf(buf, sizeof(buf), "Ped crossings: %d", ped_count);
    DrawText(buf, px, py, 14, LIGHTGRAY); 
    py += 20;

    //check if peds line on!
    if (ped_on) {
        DrawRectangle(px-4, py, pw, 22, (Color){180,0,0,200});
        DrawText("PEDESTRIAN CROSSING!", px, py+4, 14, WHITE);
        py += 26;
    }

    py += 8;
    DrawLine(px, py, px+pw, py, GRAY); py += 12;

    //color legend
    DrawText("LEGEND", px, py, 14, GRAY); py += 18;
    DrawCircle(px+8, py+7, 7, GREEN);
    DrawText("Green signal",  px+20, py, 13, LIGHTGRAY); py += 20;
    DrawCircle(px+8, py+7, 7, YELLOW);
    DrawText("Yellow signal", px+20, py, 13, LIGHTGRAY); py += 20;
    DrawCircle(px+8, py+7, 7, RED);
    DrawText("Red signal",    px+20, py, 13, LIGHTGRAY); py += 20;
    DrawRectangle(px+2, py+2, 11, 11, SKYBLUE);
    DrawText("Car in queue",  px+20, py, 13, LIGHTGRAY); py += 20;
    DrawRectangle(px+2, py+2, 18, 10, WHITE);
    DrawText("Ped crossing",  px+20, py, 13, LIGHTGRAY);


}