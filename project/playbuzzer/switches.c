#include <msp430.h>
#include "switches.h"
#include "led.h"
#include "buzzer.h"
#include "notes.h"

const int notesBYE[] = {C, EM, C, EM, D7, C, EM, D7, C, EM, D7, C, C, EM, D7, C, EM, D7};

char switch_state_down, switch_state_changed; /* effectively boolean */

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

int count = 0;
int notescount = 0;

void switch_interrupt_handler()
{
  char p1val = switch_update_interrupt_sense();
  switch_state_down = (p1val & SW1) ? 0 : 1; /* 0 when SW1 is up */
  switch_state_changed = 1;
  led_update();
  
  if (switch_state_down){
    count++;
    if (count >= 125){
      count = 0;
      notescount++;
    }
    if (notescount >= 10)
      notescount = 0;
    buzzer_set_period(notesBYE[notescount]);    
  }else{
    buzzer_set_period(0);
  }
}
