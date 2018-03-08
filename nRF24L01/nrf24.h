/*
* ----------------------------------------------------------------------------
* “THE COFFEEWARE LICENSE” (Revision 1):
* <ihsan@kehribar.me> wrote this file. As long as you retain this notice you
* can do whatever you want with this stuff. If we meet some day, and you think
* this stuff is worth it, you can buy me a coffee in return.
* -----------------------------------------------------------------------------
* This library is based on this library: 
*   https://github.com/aaronds/arduino-nrf24l01
* Which is based on this library: 
*   http://www.tinkerer.eu/AVRLib/nRF24L01
* -----------------------------------------------------------------------------
*/

/** -----------------------------------------------------------------------------
*	Mega
*	MISO =	50 = PB3
	MOSI =	51 = PB2
	SCK =	52 = PB1
	SS/CS =	53 = PB0
*	Uno
	MISO =	12 = PB4
	MOSI =	11 = PB3
	SCK =	13 = PB5
	SS/CS =	10 = PB2
	
SPI			Mega	Uno		MEGA_PIN	UNO_PIN
________________________________________________________________________
CE			PB4		PB1		10			9	// CE output Pin 10 on Mega
CSN			PB0		PB2		53			10	// CSN output SS
SCK			PB1		PB5		52			13	// SCK output
MOSI		PB2		PB3		51			11	// MOSI output
MISO		PB3		PB4		50			12	// MISO input
________________________________________________________________________
*/



#ifdef  __cplusplus
extern "C" {
#endif
#ifndef NRF24
#define NRF24

#define F_CPU 16000000L

#include "nRF24L01.h"
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

#define SPI_PORT	PORTB

uint8_t lookup_table[50][5];
uint8_t freq_channel;

enum Mode {
	
	TRANSMIT = 0,
	RECEIVE = 1,
};

enum Node{
	
	PRX	 = 0, // receiver
	PTX1 = 1, // transmitter
	PTX2 = 2, // transmitter
	PTX3 = 3,
	PTX4 = 4,
	PTX5 = 5	
};

typedef struct __attribute__ ((packed)) {
	
	uint8_t id;
	uint16_t data;

} nrf24_packet_t ;


extern const uint8_t PRX_ADDRESS[6] ;
uint8_t data_array[4];// 32 bytes
uint8_t ack_payload[4];
uint8_t receive_data[4];	
#define set_bit(reg,bit) reg |= (1<<bit)
#define clr_bit(reg,bit) reg &= ~(1<<bit)
#define check_bit(reg,bit) (reg&(1<<bit))

#define LOW 0
#define HIGH 1


// define the SPI Pins
#if defined (__AVR_ATmega2560__)
#define CE		PB4  // 10 on mega
#define MISO	PB3  // 50
#define MOSI	PB2  // 51
#define SCK		PB1	 // 52
#define CSN		PB0  // 53 Control Select!
#elif defined (__AVR_ATmega328P__)
// UNO
#define CE		PB1 // 9
#define MISO	PB4 // 12
#define MOSI	PB3 // 11
#define SCK		PB5 // 13
#define CSN		PB2 // 10
#endif


#define nrf24_ADDR_LEN 5
#define nrf24_CONFIG ((1<<EN_CRC)|(0<<CRCO))

#define NRF24_TRANSMISSON_OK 0
#define NRF24_MESSAGE_LOST   1

#define ce_high		nrf24_ce_digitalWrite(HIGH);
#define ce_low		nrf24_ce_digitalWrite(LOW);
#define csn_high	nrf24_csn_digitalWrite(HIGH);
#define csn_low		nrf24_csn_digitalWrite(LOW);

/* adjustment functions */
void    nrf24_init(enum Node nd);
 // single pipe setup
void	nrf24_rx_address(uint8_t * adr, uint8_t * to);
void    nrf24_tx_address(uint8_t* adr);

// for multi-pipe config
void	nrf24_setRxDataPipe(uint8_t * rxAddr, uint8_t configNode);
void	nrf24_setDataPipe(uint8_t * addr);


void    nrf24_config(uint8_t channel, uint8_t pay_length);
void	nrf2f_initSpi();
void    nrf24_configure_star_network(uint8_t channel);
void    nrf24_configure_star_node(uint8_t node_id, enum Mode md);

/* state check functions */
uint8_t nrf24_dataReady();
uint8_t nrf24_isSending(uint8_t * ack_data);
uint8_t nrf24_getStatus();
uint8_t nrf24_rxFifoEmpty();
uint8_t nrf24_txIsFull();
uint8_t nrf24_getLastPipeReceived();
uint8_t nrf24_available(uint8_t* pipe_num);
uint8_t nrf24_getPacketLossCount(void);
/* core TX / RX functions */

void    nrf24_getData(uint8_t* data);
void    nrf24_getPayLoad(void * buf, uint8_t len);
void    nrf24_sendPayLoad(void * buf, uint8_t len, uint8_t *ack_data);
void	nrf24_setAckPayLoad(void * buf, uint8_t pipe, uint8_t len);
/* use in dynamic length mode */
uint8_t nrf24_payloadLength();

/* post transmission analysis */

uint8_t nrf24_retransmissionCount();

/* Returns the payload length */
uint8_t nrf24_payload_length();

/* power management */
void    nrf24_powerUpRx();
void    nrf24_powerUpTx();
void    nrf24_powerDown();

/* flush methods*/
void nrf24_flushRx();
void nrf24_flushTx();

/* low level interface ... */

uint8_t  spi_transfer(uint8_t data);
uint8_t  spi_transfer_register_and_value(uint8_t addr,uint8_t data);
void	 nrf24_write_register(uint8_t reg, uint8_t * value, uint8_t len);
void     nrf24_configRegister(uint8_t reg, uint8_t value);
void	 nrf24_readRegister(uint8_t reg, uint8_t * value, uint8_t len);
void	 spi_transfer_sync_data (uint8_t *dataout, uint8_t *datain, uint8_t len);
void	 spi_transmit_data (uint8_t * dataout, uint8_t len);

extern void nrf24_setupPins();

/* -------------------------------------------------------------------------- */
/* nrf24 CE pin control function
 *    - state:1 => Pin HIGH
 *    - state:0 => Pin LOW     */
/* -------------------------------------------------------------------------- */
extern void nrf24_ce_digitalWrite(uint8_t state);

/* -------------------------------------------------------------------------- */
/* nrf24 CE pin control function
 *    - state:1 => Pin HIGH
 *    - state:0 => Pin LOW     */
/* -------------------------------------------------------------------------- */
extern void nrf24_csn_digitalWrite(uint8_t state);

/* -------------------------------------------------------------------------- */
/* nrf24 SCK pin control function
 *    - state:1 => Pin HIGH
 *    - state:0 => Pin LOW     */
/* -------------------------------------------------------------------------- */
extern void nrf24_sck_digitalWrite(uint8_t state);

/* -------------------------------------------------------------------------- */
/* nrf24 MOSI pin control function
 *    - state:1 => Pin HIGH
 *    - state:0 => Pin LOW     */
/* -------------------------------------------------------------------------- */
extern void nrf24_mosi_digitalWrite(uint8_t state);

/* -------------------------------------------------------------------------- 
 nrf24 MISO pin read function
 returns: Non-zero if the pin is high 
*/
extern uint8_t nrf24_miso_digitalRead();



#endif
#ifdef  __cplusplus
}
#endif

