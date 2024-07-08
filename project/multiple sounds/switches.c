#include <msp430.h>
#include "switches.h"
#include "led.h"
#include "buzzer.h"
#include "notes.h"

char switch_down = 0;

const int notesBYE[] = {Cn, Cn, G, G, A, A, G, F, F, E, E, D, D, Cn, G, G, F, F, E, E, D, G, G, F, F, E, E, D, Cn, Cn, G, G, A, A, G, F, F, E, E, D, D, Cn};

char switch_state_down;//, switch_state_changed; /* effectively boolean */
unsigned int count = 0;

static char switch_update_interrupt_sense()
{
  char p1val = P2IN;
  
  P2IES |= (p1val & SWITCHES);	/* if switch up, sense down */
  P2IES &= (p1val | ~SWITCHES);	/* if switch down, sense up */
  return p1val;
}

void 
switch_init()			/* setup switch */
{  
  P2REN |= SWITCHES;		/* enables resistors for switches */
  P2IE |= SWITCHES;		/* enable interrupts from switches */
  P2OUT |= SWITCHES;		/* pull-ups for switches */
  P2DIR &= ~SWITCHES;		/* set switches' bits for input */
  switch_update_interrupt_sense();
}

void switch_interrupt_handler(){
  count = 0;
  int notescount = 0;
  char p1val = switch_update_interrupt_sense();
  switch_state_down = 0;

  if (~p1val & SW1){
    switch_down |= SW1;
    switch_state_down = 1;
  }else if (~p1val & SW2){
    P1OUT |= LED_RED;
    switch_down |= SW2;
    switch_state_down = 1;
  }else if (~p1val & SW3){
    P1OUT |= LED_GREEN;
    switch_down |= SW3;
    switch_state_down = 1;
  }else if (~p1val & SW4){
    P1OUT |= LED_GREEN;
    P1OUT |= LED_RED;
    switch_down |= SW4;
    switch_state_down = 1;
  }
  
  while (switch_state_down){
    count++;
    if (count >= 35000){
      count = 0;
      notescount++;
    }
    buzzer_set_period(notesBYE[notescount]);    
    if (notescount > 41){
      notescount = 0;
    }
     p1val = switch_update_interrupt_sense();
     if (p1val & switch_down){
       P1OUT &= ~LEDS;
       switch_down &= ~SWITCHES;
       switch_state_down = 0;
       break;
     }     
  }
  buzzer_set_period(0);
}
