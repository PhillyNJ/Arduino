
#include "nrf24.h"
const uint8_t PRX_ADDRESS[6] = { RX_ADDR_P0, RX_ADDR_P1, RX_ADDR_P2, RX_ADDR_P3, RX_ADDR_P4, RX_ADDR_P5 };
uint8_t lastPipeReceived;
uint8_t payload_len;
uint8_t ptx0[5] =	{0x78, 0x78, 0x78, 0x78, 0x78};
uint8_t ptx1[5] =	{0xF1, 0xB4, 0xB5, 0xB6, 0xB3};
uint8_t ptx2[5] =	{0xCD, 0xB4, 0xB5, 0xB6, 0xB3};
uint8_t ptx3[5] =	{0xA3, 0xB4, 0xB5, 0xB6, 0xB3};
uint8_t ptx4[5] =	{0x0F, 0xB4, 0xB5, 0xB6, 0xB3};
uint8_t ptx5[5] =	{0x05, 0xB4, 0xB5, 0xB6, 0xB3};

enum Node node = PRX; // default
enum Mode current_mode = RECEIVE; // default for start network

void nrf24_init(enum Node nd)
{
	node = nd;
	nrf24_setupPins();
	nrf2f_initSpi();
	ce_low;
	csn_high;
}

/* Init hardware SPI */
void nrf2f_initSpi(){
	
	// CS pin is not active
	SPI_PORT |= (1<<CSN);
	// Enable SPI, Master, set clock rate
	SPCR = (1<<SPE)|(1<<MSTR);	
	SPSR |= (1<<SPI2X);
	
}

/* configure the module */
void nrf24_config(uint8_t channel, uint8_t pay_length)
{
	/* Use static payload length ... */
	payload_len = pay_length;
	
	// CRC enable, 1 byte CRC length
	nrf24_configRegister(CONFIG, CONFIG_MASK); // 0x0B 0000‭ 1011‬
	
	// Auto Acknowledgment - all pipes
	nrf24_configRegister(EN_AA, 0x3F);
	
	nrf24_configRegister(FEATURE, 0x06); // Enable dynamic payload - testing
	
	
	// Enable RX addresses
	nrf24_configRegister(EN_RXADDR,0x3F); // all pipes

	// 5 byte addresses
	nrf24_configRegister(SETUP_AW, 0x03);
	
	// 15 retries
	nrf24_configRegister(SETUP_RETR, 0x25); // todo make retries global? 5 retries, 750us wait
	
	// Set RF channel
	nrf24_configRegister(RF_CH,channel);
	
	//set speed
	nrf24_configRegister(RF_SETUP, 0x0E); //0dBm 2Mbps

	nrf24_configRegister(DYNPD,0x3F); 	// Dynamic packet length on all pipes
	
	//nrf24_configRegister(RX_PW_P0, payload_len);
	//nrf24_configRegister(RX_PW_P1, payload_len);
	//nrf24_configRegister(RX_PW_P2, payload_len);
	//nrf24_configRegister(RX_PW_P3, payload_len);
	//nrf24_configRegister(RX_PW_P4, payload_len);
	//nrf24_configRegister(RX_PW_P5, payload_len);
	//
	switch(node){
		
		case PRX:
		nrf24_setRxDataPipe(ptx0, 0);
		nrf24_setRxDataPipe(ptx1, 1);
		nrf24_setRxDataPipe(ptx2, 2);
		nrf24_setRxDataPipe(ptx3, 3);
		nrf24_setRxDataPipe(ptx4, 4);
		nrf24_setRxDataPipe(ptx5, 5);
		break;
		case PTX1:
		nrf24_setDataPipe(ptx1);
		break;
		case PTX2:
		nrf24_setDataPipe(ptx2);
		break;
		case PTX3:
		nrf24_setDataPipe(ptx3);
		break;
		case PTX4:
		nrf24_setDataPipe(ptx4);
		break;
		case PTX5:
		nrf24_setDataPipe(ptx5);
		break;
		
	}	

	// Start listening
	nrf24_powerUpRx();
	ce_high;// I think this is wrong..need to delete


}
void nrf24_configure_star_network(uint8_t channel){

	nrf24_flushTx(); // clear out the TX Buffer
	freq_channel = channel;
	
	// CRC enable, 1 byte CRC length
	nrf24_configRegister(CONFIG, CONFIG_MASK); // 0x0B 0000‭ 1011‬
	
	// Auto Acknowledgment - all pipes
	nrf24_configRegister(EN_AA, 0x3F);
	
	nrf24_configRegister(FEATURE, 0x06); // Enable dynamic payload
	
	// Enable RX addresses
	nrf24_configRegister(EN_RXADDR,0x3F); // all pipes

	// 3 byte addresses
	nrf24_configRegister(SETUP_AW, 0x03);
	
	// 5 retries
	nrf24_configRegister(SETUP_RETR, 0x25);// todo make retries global? 5 retries, 750us wait
	
	// Set RF channel
	nrf24_configRegister(RF_CH,channel);
	
	//set speed
	nrf24_configRegister(RF_SETUP, 0x0E); //0dBm 2Mbps
	
	nrf24_configRegister(DYNPD,0x3F); 	// Dynamic packet length on all pipes
	
	// Start listening
	nrf24_powerUpRx();
	ce_high;

	// set up lookup table
	uint8_t nc = 1;
	for(uint8_t i = 0; i < 50; i++){

		for(uint8_t j = 0; j < 5; j++){
			lookup_table[i][j] = nc++;
			//printf("lookup_table[%d][%d] = %d\n\r", i,j, nc);
		}
	}
}

void nrf24_configure_star_node(uint8_t node_id, enum Mode md){

	current_mode = md;

	uint8_t pipeLength = 5;
	uint8_t i;
	uint8_t offset = 2;
	
	if(md == RECEIVE){
		
		for(i = 0; i < pipeLength; i++){
			ptx0[i] = node_id + 1;
		}
		
		ptx1[0] = (node_id * 5) + offset;
		offset++;
		ptx2[0] = (node_id * 5) + offset;
		offset++;
		ptx3[0] = (node_id * 5) + offset;
		offset++;
		ptx4[0] = (node_id * 5) + offset;
		offset++;
		ptx5[0] = (node_id * 5) + offset;
		uint8_t val = node_id + 1;
		for(i = 1; i < pipeLength; i++){

			ptx1[i] = val;
			ptx2[i] = val;
			ptx3[i] = val;
			ptx4[i] = val;
			ptx5[i] = val;
		}

		nrf24_setRxDataPipe(ptx0, 0);
		nrf24_setRxDataPipe(ptx1, 1);
		nrf24_setRxDataPipe(ptx2, 2);
		nrf24_setRxDataPipe(ptx3, 3);
		nrf24_setRxDataPipe(ptx4, 4);
		nrf24_setRxDataPipe(ptx5, 5);
	} else {

		
		// find node pipe & position
		uint8_t position = 0;
		uint8_t master = 0;

		for(uint8_t x = 0; x < 50; x++){
			for(uint8_t j = 0; j < 5; j++){
				if(lookup_table[x][j] == node_id){
					position = j ;
					master = x + 1;
				};
			}
		}
		
		// fill the pipe arrays
		ptx1[0] = node_id;
		ptx2[0] = node_id;
		ptx3[0] = node_id;
		ptx4[0] = node_id;
		ptx5[0] = node_id;
		
		for(uint8_t z = 1; z< 5;z++){
			
			ptx1[z] = master;
			ptx2[z] = master;
			ptx3[z] = master;
			ptx4[z] = master;
			ptx5[z] = master;
			
		}
		
		switch(position){
			case 1:
			nrf24_setDataPipe(ptx1);
			break;
			case 2:
			nrf24_setDataPipe(ptx2);
			break;
			case 3:
			nrf24_setDataPipe(ptx3);
			break;
			case 4:
			nrf24_setDataPipe(ptx4);
			break;
			case 5:
			nrf24_setDataPipe(ptx5);
			break;
		}

	}
	
}

/* Set the RX address */
void nrf24_rx_address(uint8_t * adr, uint8_t * to)
{
	csn_low;
	nrf24_write_register(RX_ADDR_P0,adr,nrf24_ADDR_LEN);
	csn_high;

	_delay_us(30);

	csn_low;
	nrf24_write_register(RX_ADDR_P1,to,nrf24_ADDR_LEN);
	csn_high;
	
}

void nrf24_tx_address(uint8_t* adr)
{
	csn_low;
	nrf24_write_register(TX_ADDR, adr,nrf24_ADDR_LEN);
	csn_high;
	
	_delay_us(30);
	
	csn_low;
	nrf24_write_register(RX_ADDR_P0, adr,nrf24_ADDR_LEN);
	csn_high;
}
void nrf24_setRxDataPipe(uint8_t * rxAddr, uint8_t configNode){
	
	if(configNode < 2){
	
		nrf24_write_register(PRX_ADDRESS[configNode], rxAddr,nrf24_ADDR_LEN);		
		
	}else{	
		
		uint8_t lsb[1] = {rxAddr[0]}; // only the LSB is needed		
		nrf24_write_register(PRX_ADDRESS[configNode], lsb,1);
	
	}
	
}

void nrf24_setDataPipe(uint8_t * addr){
	
	csn_low;
	nrf24_write_register(RX_ADDR_P0, addr,nrf24_ADDR_LEN);
	csn_high;
	_delay_us(30);
	csn_low;
	nrf24_write_register(TX_ADDR, addr,nrf24_ADDR_LEN);
	csn_high;
	
}


uint8_t nrf24_payload_length()
{
	return payload_len;
}

uint8_t nrf24_dataReady()
{
	lastPipeReceived = 0; // reset
	uint8_t status;
	csn_low;
	status = spi_transfer(NOP);               // Read status register
	csn_high;
	lastPipeReceived = (status >> RX_P_NO) & 0x07;
	
	return status & (1<<RX_DR);
}

uint8_t nrf24_rxFifoEmpty()
{
	uint8_t fifoStatus;

	nrf24_readRegister(FIFO_STATUS, &fifoStatus, 1);
	
	return (fifoStatus & (1 << RX_EMPTY));
}

uint8_t nrf24_getLastPipeReceived(){
	
	return lastPipeReceived;
}
uint8_t nrf24_getPacketLossCount()
{
	uint8_t val;
    nrf24_readRegister(OBSERVE_TX, &val, 1);
	uint8_t pl = val >> 4;
	if(pl >= 5){ // todo get from global
		//nrf24_configRegister(RF_CH,freq_channel);
	}
	return pl;
	
}
uint8_t nrf24_payloadLength()
{
	uint8_t status;
	csn_low;	
	spi_transfer( R_RX_PL_WID );           
	spi_transfer_sync_data(&status,&status, 1);		
	csn_high;
	return status;
}

/* Reads payload bytes into data array */
void nrf24_getData(uint8_t* data)
{
	csn_low;
	spi_transfer( R_RX_PAYLOAD );            // Send cmd to read rx payload
	spi_transfer_sync_data(data,data, payload_len);		// Read payload
	csn_high;
	
	nrf24_configRegister(STATUS,(1<<RX_DR)); // Reset status register	
	
}

void nrf24_getPayLoad(void * buf, uint8_t len){
	
	csn_low;
	spi_transfer( R_RX_PAYLOAD );            // Send cmd to read rx payload
	spi_transfer_sync_data(buf,buf, len);		// Read payload
	csn_high;
	
	nrf24_configRegister(STATUS,(1<<RX_DR)); // Reset status register
	
}

void nrf24_setAckPayLoad(void * buf, uint8_t pipe, uint8_t len){

	csn_low;
	spi_transfer(W_ACK_PAYLOAD | pipe);
	spi_transfer_sync_data(buf, buf,len);
	csn_high;
}

/* Returns the number of retransmissions occurred for the last message */
uint8_t nrf24_retransmissionCount()
{
	uint8_t rv;
	nrf24_readRegister(OBSERVE_TX, &rv, 1);
	rv = rv & 0x0F;
	return rv;
}

void nrf24_sendPayLoad(void * buf, uint8_t len, uint8_t *ack_data){
	
	ce_low;
	nrf24_powerUpTx();
	nrf24_flushTx();
	
	csn_low; // start spi
	spi_transfer(W_TX_PAYLOAD); // Write cmd to write payload
	spi_transmit_data(buf,len);   // Write payload
	csn_high; // done sending SPI
	
	ce_high; /* Start the transmission - Pulse High  */
	_delay_us(15);
	ce_low;	

	while(nrf24_isSending(ack_data)){}; // wait to finish or time out
}

uint8_t nrf24_isSending(uint8_t * ack_data)
{
	uint8_t status = nrf24_getStatus();
	if(status & (1 << MAX_RT)){
		//printf("Max RT Hit\n\r"); // just for trouble shooting
	}
	/* if sending successful (TX_DS) or max retries exceeded (MAX_RT). */
	if((status & ((1 << TX_DS)  | (1 << MAX_RT))))
	{
		
		ce_low; // start listening 
		
		// get ack payload
		uint8_t ack = nrf24_rxFifoEmpty();
		
		if(ack == 0){ // rx has data when 0
			*ack_data = 1; // we have data :)			
			nrf24_getPayLoad(&ack_payload, sizeof ack_payload);
			nrf24_flushRx();
		}	
		
		nrf24_powerUpRx();
		
		return 0; /* false */
	}

	return 1; /* true */

}

uint8_t nrf24_getStatus()
{
	uint8_t rv;
	csn_low;
	rv = spi_transfer(NOP);
	csn_high;
	return rv;
}

uint8_t nrf24_txIsFull(){
	
	uint8_t rv;

	rv = nrf24_getStatus();

	/* Transmission went OK */
	if((rv & ((1 << TX_FULL))))
	{
		return 0;
	}
	
	return 1;
	
}

void nrf24_flushRx(){
	
	csn_low;
	spi_transfer(FLUSH_RX);
	csn_high;
}
void nrf24_flushTx(){
	
	csn_low;
	spi_transfer(FLUSH_TX);
	csn_high;
}

void nrf24_powerUpRx()
{
	nrf24_flushRx(); // clean out
	ce_low;
	// reset the status register so we can start fresh
	nrf24_configRegister(CONFIG, RF24_CONFIG | ( (1<<PWR_UP) | (1<<PRIM_RX) ) );
	ce_high;
	
	nrf24_configRegister(STATUS,(1 << TX_DS) | (1 << MAX_RT));
}

void nrf24_powerUpTx()
{
	nrf24_configRegister(CONFIG, RF24_CONFIG | ( (1<<PWR_UP) | (0<<PRIM_RX) ) );
	
}

void nrf24_powerDown()
{
	ce_low;
	nrf24_configRegister(CONFIG,(1<<PRIM_RX)); // need to test this
	_delay_ms(2); // let settle at least 1.5 ms
}


uint8_t nrf24_available(uint8_t* pipe_num)
{
	uint8_t status = nrf24_getStatus();
	
	uint8_t result = ( status & (1 << RX_DR) );

	if (result == 1)
	{
		// If the caller wants the pipe number, include that
		if ( pipe_num )
		*pipe_num = ( status >> RX_P_NO ) & 0X07;

		nrf24_configRegister(STATUS, (1 << RX_DR) );

		// Handle ack payload receipt
		if ( status & (1 << TX_DS) )
		{
			nrf24_configRegister(STATUS, (1 << TX_DS));
		}
	}

	return result;
}


/* hardware spi routine */
uint8_t spi_transfer(uint8_t data)
{
	
	SPDR = data;
	
	while(!(SPSR & (1<<SPIF)));

	return(SPDR);
}

uint8_t spi_transfer_register_and_value(uint8_t addr, uint8_t data)
{
	SPDR = addr;
	
	while(!(SPSR & (1<<SPIF)));

	SPDR = data;
	
	while(!(SPSR & (1<<SPIF)));

	return(SPDR);
}


/* Send one byte into the given nrf24 register */
void nrf24_configRegister(uint8_t reg, uint8_t value)
{
	csn_low;
	spi_transfer(W_REGISTER | (REGISTER_MASK & reg));
	spi_transfer(value);
	csn_high;
}

/* Read single register from nrf24 */
void nrf24_readRegister(uint8_t reg, uint8_t * value, uint8_t len)
{
	csn_low;
	spi_transfer(R_REGISTER | (REGISTER_MASK & reg));
	spi_transfer_sync_data(value,value,len);
	csn_high;

}
void spi_transfer_sync_data (uint8_t * dataout, uint8_t * datain, uint8_t len)
// Shift full array through target device
{
	uint8_t i;
	for (i = 0; i < len; i++) {
		SPDR = dataout[i];
		while((SPSR & (1<<SPIF))==0);
		datain[i] = SPDR;
	}
}
/* Write to a single register of nrf24 */
void nrf24_write_register(uint8_t reg, uint8_t * value, uint8_t len)
{
	csn_low;

	spi_transfer(W_REGISTER | (REGISTER_MASK & reg));
	spi_transmit_data(value,len);
	
	csn_high;
}

void spi_transmit_data(uint8_t * dataout, uint8_t len)
{
	uint8_t i;
	for (i = 0; i < len; i++) {
		SPDR = dataout[i];
		while((SPSR & (1<<SPIF))==0);
		
	}

}