/* mipslabwork.c

	 This file written 2015 by F Lundevall

	 This file should be changed by YOU! So add something here:

	 This file modified 2015-12-24 by Ture Teknolog 

	 Latest update 2015-08-28 by F Lundevall

	 For copyright and licensing, see file COPYING */

#include <stdint.h>	 /* Declarations of uint_32 and the like */
#include <pic32mx.h>	/* Declarations of system-specific addresses etc */
#include "mipslab.h"	/* Declatations for these labs */

#define TMR2PERIOD ((80000000 / 256) / 10) /* 100ms */
#if TMR2PERIOD > 0xffff
#error "TimerPeriodIsTooBig"
#endif

int mytime = 0x0000;
int timeoutcount = 0;
int game_started = 1; // Flag indicating if game is started. 
int direction = 0; // Direction of blinking, 0 - Left, 1 - Right

int left_score = 0;
int right_score = 0;

volatile int *porte = (volatile int *) 0xbf886110;

char textstring[] = "";

/* Interrupt Service Routine */

void blink(void) {

	if (direction) {
		*porte = *porte / 2;
	} else {
		*porte = *porte * 2;	
	}
}

void score(int player) {
    /* 0 - right player, 1 - left player */

    char text[] = "Point";

    if (player) {
    /* Left player */

    } else {
    /* Right player */
    }

    /* A player has reached 3 points, game is stopped. */
    if (left_score >= 3) {
        display_string(0, "Left wins");
        display_update();
        game_started = 0;

    } else if (right_score >=3){
        display_string(0, "Right wins wins");
        display_update();
        game_started = 0;
    }



    display_string(0, text);
    display_update();

}

void user_isr (void) {


	if (IFS(0) & 0x100) {
		// Timer interrupt
		timeoutcount++;
		IFS(0) = 0;
	
		if (timeoutcount == 10) {

		blink();

/*
		time2string(textstring, mytime);
		display_string(3, textstring);
        display_string(0, "");
		display_update();
		tick( &mytime);
*/
		timeoutcount = 0;
		}

	}
	if (IFS(0) & (1 << 15)){

		// Switch 3 interrupt
		*porte = *porte * 2;
		IFS(0) = 0;
	}

}

/* Lab-specific initialization goes here */
void labinit( void ) {


	// Set *E to address of TRISE.
	volatile int *E = (volatile int *) 0xbf886100;
	
	// Set last 8 bits to zero, i.e. sets them as output pins.
	*E = *E & 0xff00;

	// Initialize port D, set bits 11-5 as inputs.
	TRISD = TRISD & 0x0fe0;

	// Start with second rightmost LED
	*porte = 2;
	
	/*
	Set 0x70, 0111 000 for 1:256 prescaling.
	Set timeperiod.
	Reset timer.
	Start the timer by setting the 'ON' bit to '1', bit #15
	in T2CON.
	Heavily inspired by exercise 2, question 4.
	*/	

	T2CONSET = 0x70;
	PR2 = TMR2PERIOD;
	TMR2 = 0;
	T2CONSET = 0x8000;

	// Enable interrupts from TMR2
	IPC(2) = 4;
	IEC(0) = 0x100;
	// Enable interrupts for SW3
	IPC(3) = 0x1c000000;
	IEC(0) = IEC(0) | (1 << 15);
    // TODO: Enable interrupts for BTN4





	// Enable interrupts globally
	enable_interrupt();
	
	return;
}

/* This function is called repetitively from the main program */
void labwork( void ) {

    int btn = getbtns();

	if (game_started) {


        /* Incorrect hits */
        if ((btn == 1) && *porte != 1) {
            
            //Point to left player
            score(1);

        }

        if ((btn == 4) && *porte != 128) {

            //Point to right player            
            score(0);
        }

        /* Missed hits */
        if (*porte == 0 && direction)
            //Point to right player            
            score(0);
        if (*porte == 0 && !direction)
            //Point to left player
            score(1);

        /* Correct hits */
        if ((btn == 1) && *porte == 1)
		    direction = 0;

	    if ((btn == 4) && *porte == 128)
		    direction = 1;



	} else {		


	}
}





