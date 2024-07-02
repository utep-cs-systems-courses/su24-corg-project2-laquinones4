#include <msp430.h>
#include "libTimer.h"
#include "buzzer.h"
#include "led.h"
#include "switches.h"

int main(void) {
  configureClocks();
  
  buzzer_init();
 

  switch_init();
  led_init();
  
  or_sr(0x18);          // CPU off, GIE on
}
