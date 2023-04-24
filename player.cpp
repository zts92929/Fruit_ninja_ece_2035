//=================================================================
// Implementation for the player module
//
// Copyright 2020 Georgia Tech.  All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must not be made publicly available anywhere.
//==================================================================
#include "player_private.h"
#include "compost_pile_public.h"

PLAYER player; // structure of player

PLAYER player_get_info(void){
    return(player);
}

void player_init(void) {    
    player.y=64;
    player.height=60;
    player.score=0;
    player.scorecopy=0;
    player.knife_length=4;
    player.player_has_knife=1;
    player.travelx=0;
    
}

void player_moveDown(void) { 
    if(player.y+4>(128-get_compost_tallest_height()))
    {  
    }
    else
    {
        uLCD.filled_rectangle(4, player.y, 16, player.y-6, BLACK);
        uLCD.filled_rectangle(0, player.y, 12, player.y-12, BLACK);  
        player.y+=player.speed;
        player.height+=player.speed;
     }      
}


void player_moveUp(void) { 
    if(player.height==0)
    {}
    else
    {
        uLCD.filled_rectangle(4, player.y, 16, player.y-6, BLACK);
        uLCD.filled_rectangle(0, player.y, 12, player.y-12, BLACK); 
        player.y-=player.speed;
        player.height-=player.speed;
    }
        
        
}

void player_throw(void) { 
    player.player_has_knife=0;
    uLCD.filled_rectangle(4, player.y, 16, player.y-6, BLACK);
    player.travely=player.y;
}

void player_knife_draw(void) {
    if (player.player_has_knife==0&&player.travelx<129)
    {
        player.scorecopy=player.score;
        uLCD.filled_rectangle(player.travelx-player.knife_speed, player.travely, player.travelx-(player.knife_speed-4), player.y-3, BLACK);
        uLCD.filled_rectangle(player.travelx, player.travely, player.travelx+4, player.y-3, 0xFFFF00);
        player.travelx+=player.knife_speed;
    }
    else if(player.player_has_knife==0&&player.travelx>129)
    {
        if (player.scorecopy-player.score==0)
        {
            player.strikes+=1;
        }
        player.score=player.scorecopy;
        player_knife_return();
    }
    else{
    uLCD.filled_rectangle(12, player.y, 16, player.y-3, 0xFFFF00);
    player.travelx=0;
    }
}

void player_knife_return(void) {
    player.player_has_knife=1;
}

// ==== player_private.h implementation ====
void player_draw(int color) {
    uLCD.filled_rectangle(0, player.y, 12, player.y-12, BLUE); 
}

void player_score_inc(int num) {
    player.scorecopy= player.scorecopy+num+1;
}

int get_player_score(){
    return (player.score);
}

int get_strikes()
{
    return(player.strikes);
}

void set_speed(int num)
{
    player.speed=num;
}
void set_knife_speed(int num)
{
    player.knife_speed=num;
}

void reduce_strikes()
{
    player.strikes-=1;
}


