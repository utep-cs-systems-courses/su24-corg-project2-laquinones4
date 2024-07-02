#include <msp430.h>
#include "libTimer.h"
#include "buzzer.h"
#include "notes.h"
#include "switches.h"

//const int notesBYE[] = {10000, Cn, Cn, G, G, A, A, G, F, F, E, E, D, D, C, G, G, F, F, E};

//void playsong(int state){
  //  int count = 0;
  //  int notescount = 0;
  //  char p1val = P1IN;
  //  while (state){
    //    notescount++;
    //    buzzer_set_period(notesBYE[notescount]);
    //    state = (p1val & SW1) ? 1 : 0; 
    //  }

  //  if (!state)
    //    buzzer_set_period(0);
  //}

void buzzer_init()
{
  /* 
       Direct timer A output "TA0.1" to P2.6.  
        According to table 21 from data sheet:
          P2SEL2.6, P2SEL2.7, anmd P2SEL.7 must be zero
          P2SEL.6 must be 1
        Also: P2.6 direction must be output
  */
  timerAUpmode();/* used to drive speaker */
  P2SEL2 &= ~(BIT6 | BIT7);
  P2SEL &= ~BIT7;
  P2SEL |= BIT6;
  P2DIR = BIT6;/* enable output to speaker (P2.6) */
}

void buzzer_set_period(short cycles) /* buzzer clock = 2MHz.  (period of 1k results in 2kHz tone) */
{
  CCR0 = cycles;
  CCR1 = cycles >> 1;/* one half cycle */
}
