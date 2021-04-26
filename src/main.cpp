#include <mbed.h>

#include <USBSerial.h>
USBSerial serial;


// button interrupt
InterruptIn buttonInterrupt(USER_BUTTON);

// counter 
volatile uint8_t counter = 0;

// global system tick
volatile uint32_t myTick = 0;


// count the button pushes
void push_counter(void) {
  // static variable - has a local scope but is initialised on startup
  volatile static uint32_t lastTick = 0;

  // check if 300ms has passed since the last real press
  if (myTick - lastTick > 300) {

    // increment the counter
    counter++;

    // save the time of the real press
    lastTick = myTick;
  }
}

// system tick ISR
void myTicker(void) {
  myTick++;
}


int main() {
  
  // Setup the system tick
  Ticker t;
  t.attach_us(&myTicker, 1000);

  // set the interrupt to fire on a rising edge
  buttonInterrupt.rise(&push_counter);


  while (1) {
    
    // print out the counter value
    for (int i  = 0; i < 10; i++) {
      serial.printf("count: %d\n\r", counter);
      wait_ms(1000);
    }
    
    // reset every so often
    counter = 0;
    serial.printf("RESET\n\r");
  }
}