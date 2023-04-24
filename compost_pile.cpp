//=================================================================
// The file is for module "compost pile"
//
// Copyright 2020 Georgia Tech.  All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must not be made publicly available anywhere.
//==================================================================

#include "compost_pile_private.h"

COMPOST arr[10];
int tallest_pile_height;



// See the comments in compost_pile_public.h
void compost_pile_init() {
    int xpos=0;
    int ypos=0;
    for (int i=0; i<10;i++)
    {
        
        arr[i].x=xpos;
        arr[i].y=ypos;
        arr[i].width=12.8;
        arr[i].height=0;
        xpos+=12.8;
        tallest_pile_height=0;
    }
    
}

COMPOST compost_get_info(int index){
    return arr[index];
}

void compost_add(int index) {
    arr[index].height+=12.8;
    if (arr[index].height>tallest_pile_height)
    {
        tallest_pile_height=arr[index].height;
    }
}

void draw_compost(void){
    
    for(int i=0;i<10;i++)
    {
        uLCD.filled_rectangle(arr[i].x, 128-arr[i].height, arr[i].x+12.8, 128, 0x00FF00);
    }    
}

int get_compost_tallest_height() {
    return(tallest_pile_height);
}

int get_compost_height(int index) {

    return(arr[index].height);
}

void reduce_compost_height()
{
    for(int i=0;i<10;i++)
    {
        uLCD.filled_rectangle(arr[i].x, 128-arr[i].height, arr[i].x+12.8, 128, 0x000000);
    }  
    for(int i=0;i<10;i++)
    {
        if(arr[i].height==0)
        {continue;}
        else
        {arr[i].height-=12.8;}
    }
} 