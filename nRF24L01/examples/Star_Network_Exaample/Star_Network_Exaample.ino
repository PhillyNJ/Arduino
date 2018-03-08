 /*  Author: PhillyNJ
  *  Date: 3-6-2018
  *  nRF24L01+ Arduino Uno and AT Mega 2560 Example 
  *  The library supports a simple 1 receiver and 5 transitter configurations
  *  as well as a star network that can support up-to 250 nodes
  *  By default, ACK is enabled and will send back a ACK payload to node PTX1 (simple Configuration)

Pin Defines

nRF24L01+ Mega  Uno   MEGA_PIN  UNO_PIN
________________________________________________________________________
CE        PB4   PB1     10      9   // CE output 
CSN       PB0   PB2     53      10  // CSN output SS
SCK       PB1   PB5     52      13  // SCK output
MOSI      PB2   PB3     51      11  // MOSI output
MISO      PB3   PB4     50      12  // MISO input
________________________________________________________________________

  * Instructions 
  * The library suports running in 2 configurations -  
  * 
  * Simple Configuration Example: 
  * * Set myNode to either PRX (receiver) or PTX1 - PTX5 (Transmitter) 
  * * uncomment #define SIMPLE_EXAMPLE
  * 
  * Star Configuration Example
  * 
  * * Set the default enum Mode c_mode = RECEIVE; This is the default setting. 
  * * uint8_t node_number = 0; // set your node Id
  * * comment out the line #define SIMPLE_EXAMPLE 
  * * * Star Network Requirements
  * * * Valid Nodes:
  * * * Node 0 - Always the base node
  * * * Node 1 - Not used reserved for Pipe 0 ACK
  * * * Node 2-49 - Can receive from child nodes and transmit to its parent
  * * * Nodes 50 - 251 can only transmit 
  * 
*/


#include <stdint.h>
#include <stdarg.h>
#include "nRF24L01.h"
#include "nrf24.h"
#define F_CPU 16000000L
#define MAXSTRINGSIZE 80

extern uint8_t data_array[4];// 32 bytes max
extern uint8_t receive_data[4];
extern uint8_t ack_payload[4];
extern uint8_t lastPipeReceived;
extern uint8_t ptx1[5];

volatile int elapsedSeconds = 0;
volatile bool send_data = false;

void run_network_example(void);
void network_receive_data(void);
void network_relay_data(void);
void network_send_data(void);
void init_timer(void);
void printDetails();
void printRegister(uint8_t reg);
void xprint(const char* fmt, ... );

Node myNode = PRX; //Set Node to either PRX (receiver) or PTX1 - PTX5 (Transmitter) - for simple configuration only

enum Mode c_mode = RECEIVE; // star configuration only
uint8_t node_number = 0; // star configuration only

nrf24_packet_t pck;
uint16_t ackval = 0;
uint16_t fakeval = 0;

void setup() {
  
  Serial.begin(9600);
  xprint("nRF24L01+ Arduino Examples * Simple* \n\r");
  send_data = false;
  elapsedSeconds = 0;
  init_timer();
  nrf24_init(myNode);

  nrf24_configure_star_network(71);
  nrf24_configure_star_node(node_number, c_mode);

  printDetails();
  delay(2000);
  nrf24_flushTx();
  nrf24_powerUpRx();
}

void loop() {

  run_network_example();

}
/*
  Star network example 
*/
void run_network_example() {

  if (node_number == 0) { // base always receives
    network_receive_data();
  } else if (node_number < 50) {
    network_relay_data();
  } else if (send_data && elapsedSeconds % 1 == 0) {
    network_send_data();
    xprint("Packet Loss: %d\n\r", nrf24_getPacketLossCount());
    // todo reset packet loss 
  }
}

void network_receive_data(void) {

  if (ackval > 120) {
    ackval = 0; // reset
  }

  if (nrf24_dataReady())
  {
    nrf24_flushTx();
    // preload ack data
    for (uint8_t z = 0; z < 4; z++) { // todo make this global
      ack_payload[z] = ackval++; // fill with dummy data
    }
    nrf24_setAckPayLoad(&ack_payload, PTX1, 4); // todo make this global
    // get the payload size
    uint8_t payLoadsize = nrf24_payloadLength();

    if (payLoadsize > 32) {
      xprint("Invalid payload length received: %d\n\r", payLoadsize); // should implement a WDT to reset if this happens
      return;
    }
    xprint("Payload Size: %d\n\r", payLoadsize);

    nrf24_getPayLoad(&pck, sizeof pck);

    xprint("Packet ID: %d\n\r", pck.id);
    xprint("Packet Val: %d\n\r", pck.data);

    xprint("\n\r<<< Data Complete From Radio Pipe %d>>>\n\r", pck.id);

  }

}
void network_relay_data(void) {

  if (nrf24_dataReady())
  {

    uint8_t payLoadsize = nrf24_payloadLength();

    if (payLoadsize > 32) {
      xprint("Invalid payload length received: %d\n\r", payLoadsize); // should implement a WDT to reset if this happens
      return;
    }
    xprint("Payload Size: %d\n\r", payLoadsize);

    nrf24_getPayLoad(&pck, sizeof(pck));

    xprint("Packet ID: %d\n\r", pck.id);
    xprint("Packet Val: %d\n\r", pck.data);
    xprint("\n\r<<< Data Complete From Radio # %d >>>\n\r",  pck.id);
    xprint("\n\r<<< Resending to Base  >>>\n\r");

    c_mode = TRANSMIT;
    nrf24_configure_star_node(node_number, c_mode);

    uint8_t ack_data_status;
    nrf24_sendPayLoad(&pck, sizeof(pck), &ack_data_status);
    xprint("Packet Loss: %d\n\r", nrf24_getPacketLossCount());

    c_mode = RECEIVE;
    nrf24_configure_star_node(node_number, c_mode);

  }

}
void network_send_data(void) {
  uint8_t ack_data_status;
  send_data = false;
  if (fakeval > 120) {
    fakeval = 0; // reset
  }
  pck.id = node_number; // testing
  pck.data = fakeval++;

  xprint("Sending Payload size: %d\n\r", sizeof(pck));

  nrf24_sendPayLoad(&pck, sizeof(pck), &ack_data_status);

  if (ack_data_status == 1) {

    for (uint8_t i = 0; i < 4 ; i++) { // todo set as global
      xprint("%d ", ack_payload[i]);
    }
    xprint("\n\r");
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
