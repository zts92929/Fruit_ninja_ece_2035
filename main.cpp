//=================================================================
// The main program file.
//
// Copyright 2020 Georgia Tech.  All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must not be made publicly available anywhere.
//==================================================================

// External libs
#include <stdlib.h>

// Project includes
#include "globals.h"
#include "hardware.h"
#include "compost_pile_public.h"
#include "fruit_public.h"
#include "player_public.h"

#define G 0x008000 //GREEN
#define R 0xF1C40F //YELLOW
#define r 0xFF0000 //RED
#define _ 0x000000 //BLACK

//For sound components
////AnalogOut DACout(p18);
//PwmOut pwmout(p26);
////PwmOut speaker(p25);

////wave_player waver(&DACout);
//SDFileSystem sd(p5, p6, p7, p8, "sd"); // mosi, miso, sck, cs

DigitalOut myled1(LED1);
DigitalOut myled2(LED2);
DigitalOut myled3(LED3);
DigitalOut myled4(LED4);

int explosion[12 * 15] = {
    r,r,_,_,r,_,r,r,_,R,_,_,R,r,_,
    R,R,r,r,_,R,r,r,R,_,_,r,r,R,_,
    r,_,r,R,r,R,r,_,R,_,r,R,_,r,r,
    _,r,R,R,R,R,R,R,_,R,R,R,_,R,_,
    r,r,r,_,R,R,_,R,R,R,R,R,R,R,r,
    _,r,R,R,_,R,R,R,R,_,R,R,r,r,_,
    r,R,r,R,R,R,_,R,R,R,R,R,_,r,r,
    _,_,R,R,R,R,R,R,_,R,R,R,r,R,_,
    _,R,r,r,R,_,R,R,R,R,r,r,R,r,_,
    r,r,r,R,_,R,R,r,R,_,R,_,_,_,r,
    r,r,_,_,r,r,R,R,r,_,_,r,R,_,_,
    _,_,_,r,_,_,r,r,r,_,_,_,r,r,_
};
int fruit_explosion[12 * 15] = {
    G,G,G,_,_,_,_,_,G,G,G,_,_,_,_,
    G,G,G,_,_,_,_,_,G,G,G,G,G,G,G,
    G,G,G,_,_,_,_,_,G,G,G,G,G,G,G,
    _,_,_,_,_,_,_,_,_,_,G,G,G,G,G,
    _,_,_,G,G,G,G,G,G,G,G,G,_,_,_,
    _,_,G,G,G,G,G,G,G,G,G,G,_,_,_,
    _,_,G,G,G,G,G,G,G,G,G,_,_,_,_,
    _,_,G,G,G,G,G,G,G,G,G,G,_,_,_,
    _,_,G,G,G,G,G,G,G,G,G,_,_,_,_,
    _,_,G,G,G,_,G,G,G,G,G,G,G,G,G,
    _,_,_,_,_,_,_,_,G,G,G,G,G,G,G,
    _,_,_,_,_,_,_,_,G,G,G,G,G,G,G
};
int blank[12 * 15] = {
_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,_,_,_,_
};

// ===User implementations start===
int fruit_contact(void); // iterate through fruits and see if any collided
int lives=3;
int strikes=0;
int sliced=0;
int score_mode=0;
void compost_pile_update(void);
void clear();
void DrawLives();

void playSound(char * wav);
void playNotes(void);
void set_random_seed(Timer);

int main()
{
    uLCD.baudrate(300000);
    wait(0.2);
    set_time(1256729737);
    GameInputs inputs; 
    // First things first: initialize hardware
    ASSERT_P(hardware_init() == ERROR_NONE, "Hardware init failed!");
    pc.printf("Program Starting\n");

    // ===User implementations start===
    // Game state variables
    
    // Timer to measure game update speed (secondarily used to generate random seed)
    Timer t;
    int dt; // delta time
    set_random_seed(t);
 
    //pwmout.period(1.0/4000.0);
    //playSound("/sd/wavfiles/BUZZER.wav");//test the sound convert to thread
    //wait(0.1);
    //playNotes();
    
    //initialize functions
    compost_pile_init();
    fruit_init();
    player_init();
    
    while(1)
    {
        uLCD.locate(0,6);
        uLCD.printf("Welcome to Fruit\n Ninja\n");
        uLCD.printf("Button 1 for Score Mode\n");
        uLCD.printf("Button 2 for Timed Mode\n");
        inputs = read_inputs();
        if(inputs.b1==0)
        {
            score_mode=1;
            break;
        }
        else if(inputs.b2==0)
        {
            break;
        }
        
    }
    wait(0.2);
    uLCD.cls();
    while(1)
    {
        uLCD.locate(0,5);
        uLCD.printf("Button 1 for Easy\n");
        uLCD.printf("Button 2 for Medium\n");
        uLCD.printf("Button 3 for Hard\n");
        inputs = read_inputs();
        if(inputs.b1==0)
        {
            set_speed(8);
            set_knife_speed(32);
            set_fruit_speed(3);
            break;
        }
        else if(inputs.b2==0)
        {
            set_speed(4);
            set_knife_speed(16);
            set_fruit_speed(5);
            break;
        }
        else if(inputs.b3==0)
        {
            set_speed(2);
            set_knife_speed(32);
            set_fruit_speed(7);
            break;
        }
     }           
    uLCD.cls();
    wait(1);
    time_t startTime = time(0);
    time_t timeNow = time(0);
    int difftime;
    int difftimecopy=0;
    while(1)
    {
        timeNow=time(0);
        difftime=timeNow-startTime+difftimecopy;
        t.start();

        // Draw fruits first
        //uLCD.printf("fruit_generator\n");
        fruit_generator();
        
        //Put score on screen
        uLCD.locate(0,1);
        uLCD.printf("Score: %d\n",get_player_score());
        uLCD.printf("Strikes: %d\n",get_strikes());
        uLCD.printf("Time: %d\n",(difftime));
        DrawLives();
        // NOTE: Text is 8 pixels tall

        //uLCD.printf("player_fruit_draw\n");
        player_draw(0x0000FF);
        player_knife_draw();
        
        
        
        //uLCD.printf("Reading inputs\n");
        inputs = read_inputs();
        
        if (inputs.b1==0){
            uLCD.cls();
            int pause=0;
            
            while (pause==0)
            {
               
               uLCD.locate(0,3);
               uLCD.printf("Game Paused\n");
               uLCD.printf("Press any button\n to resume\n");
               uLCD.printf("Score: %d\n",get_player_score());
               uLCD.printf("Strikes: %d\n",get_strikes());
               uLCD.printf("Lives: %d\n",lives);
               wait(0.3);
               inputs = read_inputs();
               
               if( inputs.b1==0 || inputs.b2==0 || inputs.b3==0)
               {
                   pause=1;
                }
            }
            startTime=time(0);
            difftimecopy=difftime;
            uLCD.cls();
            
            
        }

        //pc.printf("player motions\n");
        if (inputs.ay<=-0.3) player_moveUp();            
        if (inputs.ay>= 0.3) player_moveDown();                
        if (inputs.b3==0) player_throw();
        
        //uLCD.printf("compost_pile_update\n");
        compost_pile_update();
        
        //uLCD.printf("fruit_contact\n");
        fruit_contact();
        
        // TODO: Check end of the game
        if (lives==0 ||get_compost_tallest_height()>=63||get_strikes()==7)
        {
          uLCD.cls();
          uLCD.printf("You Lose");
          break; 
        }            
        //uLCD.printf("check endgame\n");
        
        // TODO: Check if game won
        if (score_mode==1){
        if(get_player_score()>=4)
        {
          uLCD.cls();
          uLCD.printf("You WIN");
          break; 
        }
        }
        else
        {
            if(difftime>=30)
            {
                uLCD.cls();
                uLCD.printf("Score: %d\n",get_player_score());
                break;
            }
        
        }
            

        // Compute update time
        t.stop();
        dt = t.read_ms();
        if (dt < 100) wait_ms(100 - dt);
    }
    pc.printf("out of main loop\n");
    destroyList(get_fruit_list());
    
    return 0;
    // ===User implementations end===
}

// ===User implementations start===

/* This function iterate through the fruit list, checking if anyone one of them is contacting 
*  with the player's knife. Don't forget to check if the "fruit" is the bomb, since we 
*  generated bombs using the fruit_generator. 
*/ 
int fruit_contact(void) {
    PLAYER p=player_get_info();
    int number=0;
    if (p.player_has_knife==0)
    {
        llnode_t* f=get_fruit_list()->head;
        while(f != NULL){
            
            FRUIT* num1= (FRUIT*)f->data;
            if(p.travely-(num1->box.bottomRight.y)<0 &&p.travely-(num1->box.topLeft.y)>0||(p.travely-3)-(num1->box.bottomRight.y)<0 &&(p.travely-3)-(num1->box.topLeft.y)>0)
            {
                if((p.travelx+12)-(num1->box.topLeft.x)>0&&(p.travelx-p.knife_speed)-(num1->box.topLeft.x)<0)
                {
                if(num1->type==0)
                {
                    lives-=1;
                    uLCD.BLIT(num1->box.topLeft.x, num1->box.topLeft.y,12 ,15, explosion);
                    wait(0.2);
                    uLCD.BLIT(num1->box.topLeft.x, num1->box.topLeft.y,12 ,15, blank);
                    num1->status=FRUIT_SLICED;
                    sliced=1;
                }
                else if(num1->type==4)
                {
                reduce_strikes();
                uLCD.BLIT(num1->box.topLeft.x, num1->box.topLeft.y,12 ,15, fruit_explosion);
                wait(0.2);
                uLCD.BLIT(num1->box.topLeft.x, num1->box.topLeft.y,12 ,15, blank);
                sliced=1;
                num1->status=FRUIT_SLICED;
                player_score_inc(number);
                number+=1;
                  }
                  
                else if(num1->type==3)
                {
                reduce_compost_height();
                uLCD.BLIT(num1->box.topLeft.x, num1->box.topLeft.y,12 ,15, fruit_explosion);
                wait(0.2);
                uLCD.BLIT(num1->box.topLeft.x, num1->box.topLeft.y,12 ,15, blank);
                sliced=1;
                num1->status=FRUIT_SLICED;
                player_score_inc(number);
                number+=1;  
                }  
                else{
                uLCD.BLIT(num1->box.topLeft.x, num1->box.topLeft.y,12 ,15, fruit_explosion);
                wait(0.2);
                uLCD.BLIT(num1->box.topLeft.x, num1->box.topLeft.y,12 ,15, blank);
                sliced=1;
                num1->status=FRUIT_SLICED;
                player_score_inc(number);
                number+=1;
                }
                }
            }
            f=f->next;
        }        
    } 
  return(0);
}

void clear() {uLCD.cls();}

/** Call compost_pile_update() repeatedly in your game-loop. ex: main()
    This function iterate through the fruit list. Add fruits that reach the bottom of the screen
    to the compost pile.
    @return  Number of remaining cities. You might end the game when all cities are demolished.
*/
void compost_pile_update(void){
    
    llnode_t* f=get_fruit_list()->head;
    sliced=0;
    while(f!=NULL)
    {
        FRUIT* num1= (FRUIT*)f->data;
        if(num1->box.bottomRight.y>128-get_compost_tallest_height()&& num1->status==FRUIT_ACTIVE)
        {
            double num=0;
        for(int i=0;i<10;i++)
        {
            if((num1->box.bottomRight.y>=128-get_compost_height(i)&&(num1->box.topLeft.x-num>0&&num1->box.topLeft.x-(num+12.8)<0))||(num1->box.bottomRight.y>=128-get_compost_height(i)&&(num1->box.bottomRight.x-num>0 &&num1->box.bottomRight.x-(num+12.8)<0)))
            {compost_add(i);
             num1->status=FRUIT_SLICED;
             break;}
             num+=12.8;
            
            
        }
        }
        f=f->next;       
    }
    draw_compost();
    
}

//fcn to play a wav
void playSound(char* wav)
{
    //open wav file
    FILE *wave_file;
    wave_file=fopen(wav,"r");
    
    if(wave_file != NULL) 
    {
        printf("File opened successfully\n");

        //play wav file
        printf("Sound playing...\n");
        waver.play(wave_file);
    
        //close wav file
        printf("Sound stopped...\n");
        fclose(wave_file);
        return;
    }
    
    printf("Could not open file for reading - %s\n", wav);
        return;
}

/* Wanna hear some annoying sounds?*/
void playNotes(void)
{
    int i;
// generate a 500Hz tone using PWM hardware output
    speaker.period(1.0/500.0); // 500hz period
    speaker =0.5; //50% duty cycle - max volume
    wait(3);
    speaker=0.0; // turn off audio
    wait(2);
// generate a short 150Hz tone using PWM hardware output
// something like this can be used for a button click effect for feedback
    for (i=0; i<10; i++) {
        speaker.period(1.0/150.0); // 500hz period
        speaker =0.25; //25% duty cycle - mid range volume
        wait(.02);
        speaker=0.0; // turn off audio
        wait(0.5);
    }

// sweep up in frequency by changing the PWM period
    for (i=0; i<8000; i=i+100) {
        speaker.period(1.0/float(i));
        speaker=0.25;
        wait(.1);
    }
    wait(2);

// two tone police siren effect -  two periods or two frequencies
// increase volume - by changing the PWM duty cycle
    for (i=0; i<26; i=i+2) {
        speaker.period(1.0/969.0);
        speaker = float(i)/50.0;
        wait(.5);
        speaker.period(1.0/800.0);
        wait(.5);
    }
    // decrease volume
    for (i=25; i>=0; i=i-2) {
        speaker.period(1.0/969.0);
        speaker = float(i)/50.0;
        wait(.5);
        speaker.period(1.0/800.0);
        wait(.5);
    }
    speaker =0.0;
    wait(2);
}

void set_random_seed(Timer t) {
    GameInputs inputs; 
    t.start();
    uLCD.printf("Push any button to start.\n");
    while(1){
      inputs = read_inputs();
      if (inputs.b1 || inputs.b2 || inputs.b3) break;
      }
    uLCD.cls();
    t.stop();
    int seed = t.read_ms();    
    srand(seed);
    }
    
void DrawLives()
{
    for(int i=112;i<112+5*lives;i+=5)
        uLCD.filled_rectangle(i, 0, i+3, 4, 0xFFFFFF);
    for(int i=112+5*lives;i<=127;i+=5)
        uLCD.filled_rectangle(i, 0, i+3, 4, 0x000000);
}

// ===User implementations end===
