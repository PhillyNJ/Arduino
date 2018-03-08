/*  Author: PhillyNJ
    Date: 3-6-2018
    nRF24L01+ Arduino Uno and AT Mega 2560 Example
    The library supports a simple 1 receiver and 5 transitter configurations
    as well as a star network that can support up-to 250 nodes
    By default, ACK is enabled and will send back a ACK payload to node PTX1 (simple Configuration)

  Pin Defines

  nRF24L01+ Mega  Uno   MEGA_PIN  UNO_PIN
  ________________________________________________________________________
  CE        PB4   PB1     10      9   // CE output
  CSN       PB0   PB2     53      10  // CSN output SS
  SCK       PB1   PB5     52      13  // SCK output
  MOSI      PB2   PB3     51      11  // MOSI output
  MISO      PB3   PB4     50      12  // MISO input
  ________________________________________________________________________

   Instructions
   The library suports running in 2 configurations -

   Simple Configuration Example:
 * * Set myNode to either PRX (receiver) or PTX1 - PTX5 (Transmitter)
 * * To Run this example, set myNode to PRX and upload the sketch
 * * Then change myNode to PTX1 and upload to another Arduino
 * * The receiver will send a ACK payload to transmitter PTX1. You can can this by changing #define SEND_ACK_TO PTX1
 * * Payload data is sent as a strut nrf24_packet_t. The payload sends the node of id of the transmitter and a random number between 0-255 
 * * Payload lengths are dynamic. The library will know the data length
*/


#include <stdint.h>
#include <stdarg.h>
#include "nRF24L01.h"
#include "nrf24.h"
#define F_CPU 16000000L
#define MAXSTRINGSIZE 80
#define INTERVAL 2 // every 2 seconds send data
#define SEND_ACK_TO PTX1 // the receiver will send a ACK payload to transmitter PTX1

extern uint8_t data_array[4];// 32 bytes max
extern uint8_t ack_payload[4];
uint8_t ack_pd[3];

extern uint8_t lastPipeReceived;
extern uint8_t ptx1[5];

volatile int elapsedSeconds = 0;
volatile bool send_data = false;

void run_simple_example(void);

void init_timer(void);
void printDetails();
void printRegister(uint8_t reg);
void xprint(const char* fmt, ... );

Node myNode = PTX1; //Set Node to either PRX (receiver) or PTX1 - PTX5 (Transmitter) - for simple configuration only

nrf24_packet_t pck;
uint8_t ackval = 0;
uint16_t fakeval = 0;

void setup() {

  Serial.begin(9600);
  xprint("nRF24L01+ Arduino Examples * Simple* \n\r");
  send_data = false;
  elapsedSeconds = 0;
  init_timer();
  
  nrf24_init(myNode);
  nrf24_config(71, sizeof(pck));
 
  printDetails();
  delay(2000);
  nrf24_flushTx();
  nrf24_powerUpRx();
}

void loop() {

  run_simple_example();

}

/*
  Basic example for a simple configuration (1 reveiver and 5 transmitters
*/
void run_simple_example() {

  if (ackval > 250) {
    ackval = 0;
  }

  if (myNode == PRX) {

    if (nrf24_dataReady())
    {
     // nrf24_flushTx();
      // send back some ack data
      xprint("Sending - ");
      for (uint8_t z = 0; z < sizeof data_array; z++) { // preload ack data
        data_array[z] = ackval++; // fill with dummy data     
        xprint(" %d ", data_array[z]);
      }
      xprint("\n\r");
      nrf24_setAckPayLoad(&data_array, PTX1, sizeof data_array); // need to set the pipe number for Ack payload

      uint8_t payLoadLength = nrf24_payloadLength();

      nrf24_getPayLoad(&pck, sizeof(pck));

      xprint("From Pipe %d with length: %d\n\r", lastPipeReceived, payLoadLength);

      if (lastPipeReceived > 5) {

        xprint("Invalid data received\n\r");
        lastPipeReceived = 0;
      } else {

        xprint("%d-%d\n\r", pck.id, pck.data);

      }

    }

    elapsedSeconds = 0; // not needed

  } else {

    if (send_data && elapsedSeconds % INTERVAL == 0) {

      uint8_t ack_data_status;
      send_data = false;

      int r = rand() % 255;

      pck.id = myNode;
      pck.data = r;

      nrf24_sendPayLoad(&pck, sizeof(pck), &ack_data_status);
      xprint("Sending >>>\n\r");

      xprint("%d-%d\n\r", pck.id, pck.data);

      if (ack_data_status == 1) {
        
        uint8_t payLoadLength = nrf24_payloadLength();
        
       // nrf24_getPayLoad(&data_array, sizeof(data_array));
       // nrf24_flushRx();
       
        xprint("ACK Data length: %d [", payLoadLength);
        for (uint8_t i = 0; i < payLoadLength ; i++) {
          xprint(" %d ", ack_payload[i]);
          
        }
        xprint("]\n\r");
      }

      if (elapsedSeconds > 1000) {
        elapsedSeconds = 0; // restart
      }
    }
  }
}

void printRegister(uint8_t reg) {
  uint8_t val;
  nrf24_readRegister(reg, &val, 1);
  xprint("0X%X\n\r", val);
}

void printDetails() {
  uint8_t val;

  xprint("STATUS:\t\t0x%X\n\r", nrf24_getStatus());

  xprint("CONFIG:\t\t");
  printRegister(CONFIG);

  xprint("RF_CH:\t\t");
  printRegister(RF_CH);

  xprint("RF SETUP:\t");
  printRegister(RF_SETUP);

  xprint("EN_AA:\t\t");
  printRegister(EN_AA);

  xprint("EN_RXADDR:\t");
  printRegister(EN_RXADDR);

  xprint("FEATURE:\t");
  printRegister(FEATURE);

  xprint("DYNPD:\t\t");
  printRegister(DYNPD);

  xprint("OBSERVE_TX:\t");
  printRegister(OBSERVE_TX);

  // end read TX
  if (myNode == PRX) {

    for (int x = 0; x < 6; x++) {

      switch (x) {
        case 0:
          xprint("RX_ADDR_P0:\t");
          break;
        case 1:
          xprint("RX_ADDR_P1:\t");
          break;
        case 2:
          xprint("RX_ADDR_P2:\t");
          break;
        case 3:
          xprint("RX_ADDR_P3:\t");
          break;
        case 4:
          xprint("RX_ADDR_P4:\t");
          break;
        case 5:
          xprint("RX_ADDR_P5:\t");
          break;
      }

      if (x < 2) {

        nrf24_csn_digitalWrite(LOW);
        spi_transfer(R_REGISTER + PRX_ADDRESS[x]);
        for (int i = 0; i < 5; i++) {
          val = spi_transfer(0xFF);
          xprint("0x%X ", val);
        }

        nrf24_csn_digitalWrite(HIGH);
        xprint("\n\r");

      } else {

        uint8_t val;
        nrf24_readRegister(PRX_ADDRESS[x], &val, 1);
        xprint("0x%X ", val);

        for (int z = 1; z < 5; z++) {
          xprint("0x%X ", ptx1[z]);
        }
        xprint("\n\r");

      }
    }

  } else {

    xprint("TX_ADDR:\t");
    nrf24_csn_digitalWrite(LOW);

    // Start read TX
    spi_transfer(R_REGISTER + TX_ADDR);

    for (int i = 0; i < 5; i++) {

      val = spi_transfer(TX_ADDR);
      xprint("0x%X ", val);

    }
    xprint("\n\r");
    nrf24_csn_digitalWrite(HIGH);
    xprint("RX_ADDR_P0:\t");
    nrf24_csn_digitalWrite(LOW);
    // Start read TX
    spi_transfer(R_REGISTER + RX_ADDR_P0);
    for (int i = 0; i < 5; i++) {

      val = spi_transfer(RX_ADDR_P0);
      xprint("0x%X ", val);
    }
    xprint("\n\r");

    nrf24_csn_digitalWrite(HIGH);
  }

}

// Serial wrapper function to make it easy to use STDIO specifiers
void xprint(const char* fmt, ... ) {

  va_list arglist;
  char dest[MAXSTRINGSIZE];
  va_start( arglist, fmt );
  vsprintf(dest, fmt, arglist);
  va_end( arglist );
  Serial.print(dest);
}


void init_timer() {

  // initialize Timer1
  cli();          // disable global interrupts
  TCCR1A = 0;     // set entire TCCR1A register to 0
  TCCR1B = 0;     // same for TCCR1B

  // set compare match register to desired timer count:
  OCR1A = 15624;
  // turn on CTC mode:
  // Set CS10 and CS12 bits for 1024 prescaler:
  TCCR1B |=  (1 << WGM12) | (1 << CS10) | (1 << CS12);
  // enable timer compare interrupt:
  TIMSK1 |= (1 << OCIE1A);
  // enable global interrupts:
  sei();
}
ISR(TIMER1_COMPA_vect) {

  elapsedSeconds++;
  send_data = true;
}
