#include <msp430.h>
#include "switches.h"
#include "led.h"
#include "buzzer.h"
#include "notes.h"

const int notesBYE[] = {Cn, Cn, G, G, A, A, G, F, F, E, E, D, D, Cn, G, G, F, F, E, E, D, G, G, F, F, E, E, D, Cn, Cn, G, G, A, A, G, F, F, E, E, D, D, Cn};

char switch_state_down, switch_state_changed; /* effectively boolean */
int count = 0;

static char 
switch_update_interrupt_sense()
{
  char p1val = P1IN;
  /* update switch interrupt to detect changes from current buttons */
  P1IES |= (p1val & SWITCHES);	/* if switch up, sense down */
  P1IES &= (p1val | ~SWITCHES);	/* if switch down, sense up */
  return p1val;
}

void 
switch_init()			/* setup switch */
{  
  P1REN |= SWITCHES;		/* enables resistors for switches */
  P1IE |= SWITCHES;		/* enable interrupts from switches */
  P1OUT |= SWITCHES;		/* pull-ups for switches */
  P1DIR &= ~SWITCHES;		/* set switches' bits for input */
  switch_update_interrupt_sense();
  led_update();
}

void switch_interrupt_handler(){
  count = 0;
  int notescount = 0;
  char p1val = switch_update_interrupt_sense();
  switch_state_down = (p1val & SW1) ? 0 : 1; /* 0 when SW1 is up */
  switch_state_changed = 1;
  led_update();
  
  while (switch_state_down){
    //enableWDTInterrupts();
    count++;
    if (count >= 30000){
      count = 0;
      notescount++;
    }
    buzzer_set_period(notesBYE[notescount]);    
    if (notescount > 41){
      notescount = 0;
    }
     p1val = switch_update_interrupt_sense();
     if (p1val & SW1)
       break;
     //playsong(1);
  }
  buzzer_set_period(0);
}
