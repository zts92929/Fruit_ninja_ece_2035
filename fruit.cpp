//=================================================================
// Implementation of fruit module.
//
// Copyright 2020 Georgia Tech.  All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must not be made publicly available anywhere.
//==================================================================

#include "fruit_private.h"
#include "doubly_linked_list.h"
#include "compost_pile_private.h"

int fruit_tick=0;

//Create a DLL for fruits
DLinkedList* fruitDLL = NULL;

void fruit_init(void)
{
//Create a new doubly linked list of fruits
    fruitDLL = create_dlinkedlist();
}

void fruit_generator(void){
    fruit_tick++;
    // only fire the fruit at certain ticks
    if((fruit_tick % FRUIT_INTERVAL)==0 || fruit_tick==0){
        //uLCD.printf("fruit_create()");
        fruit_create();
    }        
    // update the fruits and draw them
   // uLCD.printf("fruit_update()\n");
    fruit_update_position();
}

void fruit_create(void){
    //uLCD.printf("create\n");
    FRUIT* M = (FRUIT*)malloc(sizeof(FRUIT));
    // M->y = 0;
    //each fruit has its own tick
    M->type = rand() % 5;
    M->size= rand() % 7;
    M->size+=6;
    switch (M->type)
    {
    case 0:
        M->draw = draw_bomb;
        break;
    case 1:
        M->draw = draw_orange;
        break;
    case 2:
        M->draw = draw_banana;
        break;
    case 3:
        M->draw = draw_trash;
        break;
    case 4:
        M->draw = draw_strike;
        break;
    default:
        break;
    }
    M->tick = 0;
    //set a random source for the fruit
    M->direction = rand() % 3;
    if (M->direction == 0){
        M->source = rand() % (SIZE_X - M->size - PLAYER_SPACE);
    //set a random target for the fruit
        M->target = rand() % (SIZE_X - M->size - PLAYER_SPACE);
    //the fruit starts at its source
        M->box.topLeft.x = M->source + PLAYER_SPACE;
        M->box.topLeft.y = 0; // = {M->source + PLAYER_SPACE, 0};
        M->box.bottomRight.x = M->source + M->size + PLAYER_SPACE;
        M->box.bottomRight.y = M->size;
        //M->box.bottomRight = {M->source + FRUIT_SIZE + PLAYER_SPACE, FRUIT_SIZE};
        double diagnal = sqrt((M->source - M->target)*(M->source - M->target) + SIZE_Y*SIZE_Y);
        M->delta_x = (M->target - M->source) / diagnal;
        M->delta_y = fabs(SIZE_Y / diagnal);
    }
    else if(M->direction == 1){
        M->source = rand() % (SIZE_Y - M->size);
    //set a random target for the fruit
        M->target = rand() % (SIZE_Y - M->size);
        M->box.topLeft.x = PLAYER_SPACE;
        M->box.topLeft.y = M->source;
        //M->box.topLeft = {PLAYER_SPACE, M->source};
        M->box.bottomRight.x = PLAYER_SPACE + M->size;
        M->box.bottomRight.y = M->source + M->size;
        //M->box.bottomRight = {PLAYER_SPACE + FRUIT_SIZE, M->source + FRUIT_SIZE};
        double diagnal = sqrt((M->source - M->target)*(M->source - M->target) + (SIZE_X - PLAYER_SPACE)*(SIZE_X - PLAYER_SPACE));
        M->delta_x = (SIZE_X - PLAYER_SPACE) / diagnal; 
        M->delta_y = fabs((M->target - M->source) / diagnal);
    }else{
        M->source = rand() % (SIZE_Y - M->size);
    //set a random target for the fruit
        M->target = rand() % (SIZE_Y - M->size);
        M->box.topLeft.x = PLAYER_SPACE + SIZE_X - M->size;
        M->box.topLeft.y = M->source;
        //M->box.topLeft = {PLAYER_SPACE + SIZE_X - FRUIT_SIZE, M->source};
        M->box.bottomRight.x = PLAYER_SPACE + SIZE_X;
        M->box.bottomRight.y = M->source + M->size;
        //M->box.bottomRight = {PLAYER_SPACE + SIZE_X, M->source + FRUIT_SIZE};
        double diagnal = sqrt((M->source - M->target)*(M->source - M->target) + (SIZE_X - PLAYER_SPACE)*(SIZE_X - PLAYER_SPACE));
        M->delta_x = (PLAYER_SPACE - SIZE_X) / diagnal; 
        M->delta_y = fabs((M->target - M->source) / diagnal);
    }
        
    
    M->status = FRUIT_ACTIVE;
    
    for (int i=0; i<10; i++)
    {
        double num2=0;
        int number=get_compost_height(i);
        if (M->box.bottomRight.y>128-number)
            {M->status = FRUIT_SLICED;}
        else if(M->box.bottomRight.y>128-number-5)
        {   if(M->box.topLeft.x-10<num2 || M->box.bottomRight.x+10>num2)
            {M->status = FRUIT_SLICED;}
        }
        num2+=12.8;
    }
    
    insertHead(fruitDLL, M);
}

void fruit_update_position(void){

    
    //controls how fast the fruit will move
    int rate = FRUIT_SPEED;
    //delta_x and delta_y account for the slope of the fruit
    DrawFunc draw = NULL;
    LLNode* current = fruitDLL->head;
    FRUIT* newFruit;
    //iterate over all fruits
    while(current)
    {   newFruit = (FRUIT*) current->data;
        if(newFruit->status == FRUIT_SLICED ||
            newFruit->box.topLeft.x > 127 ||
            newFruit->box.topLeft.y > 127 ||
            newFruit->box.bottomRight.x < 0   ||
            newFruit->box.bottomRight.y < 0)
        {
            //cover the last fruit location
            draw_nothing(newFruit->box);
            // clear the fruit on the screen
            draw = NULL;           
            // Remove it from the list
            //pc.printf("deleting fruit node...\n");
            deleteNode(fruitDLL, current);
            //pc.printf("fruit node deleted.\n");
        }
        else 
        {
            //cover the last fruit location
            draw_nothing(newFruit->box);

            // update fruit position
            
            //pc.printf("%f, %f\n", newFruit->delta_x, newFruit->delta_y);
            newFruit->box.topLeft.x += rate*newFruit->delta_x;
            newFruit->box.topLeft.y += rate*newFruit->delta_y;
            newFruit->box.bottomRight.x += rate*newFruit->delta_x;
            newFruit->box.bottomRight.y += rate*newFruit->delta_y;
            //pc.printf(" %f, %f", newFruit->delta_x, newFruit->delta_y);
            // draw fruit
            draw = newFruit->draw;
            //update fruit's internal tick
            newFruit->tick++;
            //current->data = (void*) newFruit;            
        }       
        // Advance the loop
        if(draw) draw(newFruit->box);
        current = current->next;
    }
}

DLinkedList* get_fruit_list() {
 return(fruitDLL);
}

void set_fruit_speed(int num){
    FRUIT_SPEED=num;
}
    
