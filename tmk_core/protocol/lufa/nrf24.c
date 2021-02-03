#include "nrf24.h"
#include "nRF24L01.h"
// #include "lib/RF24.hpp"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "print.h"
#include <util/delay.h>
#include <inttypes.h>
// #include <alloca.h>
// #include "debug.h"
// #include "timer.h"
// #include "action_util.h"
// #include "ringbuffer.hpp"
// #include <string.h>
// #include "wait.h"
// #include "analog.h"

// These are the pin assignments for the XD60 rev3 PCB.
// You need to unsolder the 2 10ohm pullup resistors next
// to the I2C header on the back of the PCB and wire your
// NRF24L01+ module using SDA (F7) for CE and SCL (F4) for CS
// You may define them to something else in your config.h
// if yours is wired up differently.
#define NRF24_CE                    D1
#define NRF24_CS                    D0

// IRQ is not implemented yet.
#define NRF24_IRQ                   E6

#define NRF24_CHANNEL               5
#define NRF24_PAYLOAD_SIZE          22

// RF modules support 10 Mhz SPI bus speed
#define NRF24_SPI_CLOCK_SPEED       10000000  // SCK frequency
#define NRF24_SCK_DIVISOR           (F_CPU / NRF24_SPI_CLOCK_SPEED)

// bool p_variant; /* False for RF24L01 and true for RF24L01P */
uint8_t payload_size; /**< Fixed size of payloads */
bool dynamic_payloads_enabled; /**< Whether dynamic payloads are enabled. */
uint8_t pipe0_reading_address[5]; /**< Last address set on pipe 0 for reading. */
uint8_t addr_width = 5; /**< The address width to use - 3,4 or 5 bytes. */

uint8_t pairing_mac_address[]       = { 0xBB, 0x0A, 0xDC, 0xA5, 0x75 };
uint8_t reconnect_mac_address[]     = { 0x00, 0x00, 0x00, 0x00, 0x00 };

/* Pre-defined pairing packets */
uint8_t payload_pairing_1_long[22]  = {0x15, 0x5F, 0x01, 0x84, 0x5E, 0x3A, 0xA2, 0x57, 0x08, 0x10, 0x25, 0x04, 0x00, 0x01, 0x47, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xEC};
uint8_t payload_pairing_1_small[5]  = {0x15, 0x40, 0x01, 0x84, 0x26};
uint8_t payload_pairing_2_long[22]  = {0x00, 0x5F, 0x02, 0x00, 0x00, 0x00, 0x00, 0x58, 0x8A, 0x51, 0xEA, 0x01, 0x07, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
uint8_t payload_pairing_2_small[5]  = {0x00, 0x40, 0x02, 0x01, 0xbd};
uint8_t payload_pairing_3_long[22]  = {0x00, 0x5F, 0x03, 0x01, 0x00, 0x04, 0x4D, 0x35, 0x31, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB6};
uint8_t payload_pairing_3_small[5]  = {0x00, 0x5F, 0x03, 0x01, 0x0f};
uint8_t payload_pairing_4[10]       = {0x00, 0x0F, 0x06, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xEA};
/* Enf of pre-defined pairing packets */

uint8_t keyboard_payload[]          = {0x00, 0xC1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

uint8_t keepalive[]                 = {0x00, 0x40, 0x04, 0xB0, 0x0C};
uint8_t hello[]                     = {0x00, 0x4F, 0x00, 0x04, 0xB0, 0x10, 0x00, 0x00, 0x00, 0xED};

static const uint8_t child_pipe_enable[] PROGMEM = {
    ERX_P0, ERX_P1, ERX_P2, ERX_P3, ERX_P4, ERX_P5
};

static const uint8_t child_pipe[] PROGMEM = {
  RX_ADDR_P0, RX_ADDR_P1, RX_ADDR_P2, RX_ADDR_P3, RX_ADDR_P4, RX_ADDR_P5
};

static const uint8_t child_payload_size[] PROGMEM = {
  RX_PW_P0, RX_PW_P1, RX_PW_P2, RX_PW_P3, RX_PW_P4, RX_PW_P5
};

static struct {
    bool paired;
    bool initialized;
    bool configured;
} state;

/****************************************************************************/

#if defined(__AVR_ATmega16U2__) || defined(__AVR_ATmega32U2__) || defined(__AVR_ATmega16U4__) || defined(__AVR_ATmega32U4__) || defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB647__) || defined(__AVR_AT90USB1286__) || defined(__AVR_AT90USB1287__)
#    define SPI_SCK_PIN B1
#    define SPI_MOSI_PIN B2
#    define SPI_MISO_PIN B3
#elif defined(__AVR_ATmega32A__)
#    define SPI_SCK_PIN B7
#    define SPI_MOSI_PIN B5
#    define SPI_MISO_PIN B6
#elif defined(__AVR_ATmega328P__)
#    define SPI_SCK_PIN B5
#    define SPI_MOSI_PIN B3
#    define SPI_MISO_PIN B4
#endif

static void nrf24_spi_init(void) {
    setPinOutput(SPI_SCK_PIN);
    setPinOutput(SPI_MOSI_PIN);
    setPinInput(SPI_MISO_PIN);
}

static void nrf24_spi_deinit(void) {
    setPinInputHigh(SPI_SCK_PIN);
    setPinInputHigh(SPI_MOSI_PIN);
    setPinInputHigh(SPI_MISO_PIN);
}

static void nrf24_end_transaction(void) {
    writePinHigh(NRF24_CS);
}

static void nrf24_begin_transaction(void) {
    writePinLow(NRF24_CS);
}

/****************************************************************************/

// static spi_status_t nrf24_read_register_with_length(uint8_t reg, uint8_t* buf, uint8_t len) {
//   spi_status_t status;
//   nrf24_begin_transaction();
//   spi_write( R_REGISTER | ( REGISTER_MASK & reg ) );
//   status = spi_receive(buf, len);
//   nrf24_end_transaction();
//   return status;
// }

static spi_status_t nrf24_read_register(uint8_t reg) {
    spi_status_t result;

    nrf24_begin_transaction();
    spi_write( R_REGISTER | ( REGISTER_MASK & reg ) );
    result = spi_read();
    nrf24_end_transaction();

    return result;
}

static spi_status_t nrf24_write_register_with_length(uint8_t reg, const uint8_t* buf, uint8_t len) {
    uint8_t status;

    nrf24_begin_transaction();
    status = spi_write( W_REGISTER | ( REGISTER_MASK & reg ) );
    spi_transmit(buf, len);
    nrf24_end_transaction();

    return status;
}

static spi_status_t nrf24_write_register(uint8_t reg, uint8_t value) {
    spi_status_t status;

    nrf24_begin_transaction();
    status = spi_write( W_REGISTER | ( REGISTER_MASK & reg ) );
    spi_write(value);
    nrf24_end_transaction();

    return status;
}

static spi_status_t nrf24_spi_trans(uint8_t cmd) {
    spi_status_t status;

    nrf24_begin_transaction();
    status = spi_write( cmd );
    nrf24_end_transaction();

    return status;
}

/****************************************************************************/

static spi_status_t nrf24_flush_rx(void) {
    return nrf24_spi_trans( FLUSH_RX );
}

static spi_status_t nrf24_flush_tx(void) {
    return nrf24_spi_trans( FLUSH_TX );
}

spi_status_t nrf24_get_status(void) {
  return nrf24_spi_trans(NOP);
}

/* ---------- */

void nrf24_powerUp(void) {
    spi_status_t cfg = nrf24_read_register(NRF_CONFIG);

    // if not powered up then power up and wait for the radio to initialize
    if (!(cfg & _BV(PWR_UP))){
        nrf24_write_register(NRF_CONFIG, cfg | _BV(PWR_UP));

        // For nRF24L01+ to go from power down mode to TX or RX mode it must first pass through stand-by mode.
        // There must be a delay of Tpd2stby (see Table 16.) after the nRF24L01+ leaves power down mode before
        // the CEis set high. - Tpd2stby can be up to 5ms per the 1.0 datasheet
        wait_ms(5);
    }
    // dprintf("[NRF24] - powerUp\n");
}

void nrf24_toggleFeatures(void) {
    nrf24_begin_transaction();
	spi_write( ACTIVATE );
    spi_write( 0x73 );
	nrf24_end_transaction();
    // dprintf("[NRF24] - toggleFeatures\n");
}

void nrf24_setChannel(uint8_t channel) {
    const uint8_t max_channel = 125;
    nrf24_write_register(RF_CH,rf24_min(channel,max_channel));
    // dprintf("[NRF24] - setChannel\n");
}

bool nrf24_setDataRate(rf24_datarate_e speed) {
    bool result = false;
    uint8_t setup = nrf24_read_register(RF_SETUP) ;

    // HIGH and LOW '00' is 1Mbs - our default
    setup &= ~(_BV(RF_DR_LOW) | _BV(RF_DR_HIGH)) ;

    txDelay=85;
    if( speed == RF24_250KBPS ) {
        // Must set the RF_DR_LOW to 1; RF_DR_HIGH (used to be RF_DR) is already 0
        // Making it '10'.
        setup |= _BV( RF_DR_LOW ) ;
        txDelay=155;
    } else {
        // Set 2Mbs, RF_DR (RF_DR_HIGH) is set 1
        // Making it '01'
        if ( speed == RF24_2MBPS ) {
            setup |= _BV(RF_DR_HIGH);
            txDelay=65;
        }
    }

    nrf24_write_register(RF_SETUP, setup);

    setup = nrf24_read_register(RF_SETUP);

    // Verify our result
    if ( nrf24_read_register(RF_SETUP) == setup ) {
        result = true;
    }
    return result;
}

void nrf24_setRetries(uint8_t delay, uint8_t count) {
    nrf24_write_register(SETUP_RETR,(delay&0xf)<<ARD | (count&0xf)<<ARC);
    // dprintf("[NRF24] - setRetries\n");
}

void nrf24_disableCRC(void) {
    uint8_t disable = nrf24_read_register(NRF_CONFIG) & ~_BV(EN_CRC) ;
    nrf24_write_register( NRF_CONFIG, disable ) ;
    // dprintf("[NRF24] - disableCRC\n");
}

void nrf24_stopListening(void) {
    writePinLow(NRF24_CE);

    wait_us(NRF24_TX_DELAY_2MBPS);

    if(nrf24_read_register(FEATURE) & _BV(EN_ACK_PAY)){
        wait_us(NRF24_TX_DELAY_2MBPS);
        nrf24_flush_tx();
    }

    nrf24_write_register(NRF_CONFIG, ( nrf24_read_register(NRF_CONFIG) ) & ~_BV(PRIM_RX) );

    nrf24_write_register(EN_RXADDR,nrf24_read_register(EN_RXADDR) | _BV(pgm_read_byte(&child_pipe_enable[0]))); // Enable RX on pipe0
}

void nrf24_setAutoAck(bool enable) {
  if ( enable )
    nrf24_write_register(EN_AA, 0x3F);
  else
    nrf24_write_register(EN_AA, 0);
}

void nrf24_setPayloadSize(uint8_t size) {
  payload_size = rf24_min(size,32);
}

void nrf24_enableDynamicPayloads(void) {
    // Enable dynamic payload throughout the system
    nrf24_write_register(FEATURE,nrf24_read_register(FEATURE) | _BV(EN_DPL) );
    // Enable dynamic payload on all pipes
    nrf24_write_register(DYNPD,nrf24_read_register(DYNPD) | _BV(DPL_P5) | _BV(DPL_P4) | _BV(DPL_P3) | _BV(DPL_P2) | _BV(DPL_P1) | _BV(DPL_P0));
    dynamic_payloads_enabled = true;
}

void nrf24_enableAckPayload(void) {
    nrf24_write_register(FEATURE,nrf24_read_register(FEATURE) | _BV(EN_ACK_PAY) | _BV(EN_DPL) );
    // Enable dynamic payload on pipes 0 & 1
    nrf24_write_register(DYNPD,nrf24_read_register(DYNPD) | _BV(DPL_P1) | _BV(DPL_P0));
    dynamic_payloads_enabled = true;
}

void nrf24_enableDynamicAck(void) {
    nrf24_write_register(FEATURE,nrf24_read_register(FEATURE) | _BV(EN_DYN_ACK) );
}

bool nrf24_available(uint8_t* pipe_num) {
    if (!( nrf24_read_register(FIFO_STATUS) & _BV(RX_EMPTY) )){
        // If the caller wants the pipe number, include that
        if ( pipe_num ){
            uint8_t status = nrf24_get_status();
            *pipe_num = ( status >> RX_P_NO ) & 0x07;
        }
        return 1;
    }

    return 0;
}

spi_status_t nrf24_write_payload(const uint8_t* buf, uint8_t len, const uint8_t reg) {
    spi_status_t status;
    uint8_t i;
    uint8_t bufferSize = dynamic_payloads_enabled ? len : payload_size;
    uint8_t payload[bufferSize];

    for (i = 0; i < bufferSize; ++i) {
        payload[i] = 0;
    }

    for (i = 0; i < len; ++i) {
        payload[i] = buf[i];
    }

    nrf24_begin_transaction();
    status = spi_write(reg);
    spi_transmit(payload, bufferSize);
    nrf24_end_transaction();

    return status;
}

void nrf24_startFastWrite( const void* buf, uint8_t len, const bool multicast, bool startTx){ //TMRh20
	nrf24_write_payload( buf, len, multicast ? W_TX_PAYLOAD_NO_ACK : W_TX_PAYLOAD ) ;
	if(startTx){
		writePinHigh(NRF24_CE);
	}
    // dprintf("[NRF24] - nrf24_startFastWrite done %08b\n", status);
}

bool nrf24_write(const void* buf, uint8_t len, const bool multicast) {
    spi_status_t status;

    //Start Writing
    xprintf("[NRF24] - fast write\n");
	nrf24_startFastWrite(buf, len, multicast, true);

    xprintf("[NRF24] - wait for tx status\n");
    while( ! ( nrf24_get_status()  & ( _BV(TX_DS) | _BV(MAX_RT) ))) {
	}

    xprintf("[NRF24] - write pin low\n");
    writePinLow(NRF24_CE);

    xprintf("[NRF24] - register num retries\n");
    status = nrf24_write_register(NRF_STATUS, _BV(RX_DR) | _BV(TX_DS) | _BV(MAX_RT) );

    //Max retries exceeded
    if( status & _BV(MAX_RT)){
        xprintf("[NRF24] - max retries exceeded\n");
        nrf24_flush_tx(); //Only going to be 1 packet int the FIFO at a time using this method, so just flush
        return 0;
    }

    //TX OK 1 or 0
    return 1;
}

spi_status_t nrf24_read_payload(void* buf, uint8_t data_len) {
    spi_status_t status;
    uint8_t* current = (uint8_t*)(buf);

    if(data_len > payload_size) data_len = payload_size;
    uint8_t blank_len = dynamic_payloads_enabled ? 0 : payload_size - data_len;

    nrf24_begin_transaction();
    status = spi_write( R_RX_PAYLOAD );
    //blank_len = status - data_len;
    while ( data_len-- ) {
        *current++ = spi_read();
    }
    //printf("Excess data : ");
    while ( blank_len-- ) {
        spi_write(0xff);
    }
    //   Serial.println();
    nrf24_flush_rx();
    nrf24_end_transaction();

    return status;
}

spi_status_t nrf24_read(void* buf, uint8_t len ){
  uint8_t result;
  // Fetch the payload
  result = nrf24_read_payload( buf, len );

  //Clear the two possible interrupt flags with one command
  nrf24_write_register(NRF_STATUS,_BV(RX_DR) | _BV(MAX_RT) | _BV(TX_DS) );
  return result;
}

bool nrf24_pairingFlow(uint8_t *pairing_packet, uint8_t *pairing_packet_small, uint8_t *ack_payload) {
  bool keep_going = true;
  while (keep_going)
  {
    if (nrf24_available(NULL)) {
      break;
    }

    while (!nrf24_write(pairing_packet, 22, 0)) {
    }

    if (nrf24_available(NULL)) {
      break;
    }

    wait_ms(2);

    while (keep_going)
    {
      if (nrf24_write(pairing_packet_small, 5, 0)) {
        if (nrf24_available(NULL)) {
          keep_going = false;
        }
      }
    }
  }

  wait_ms(2);

  nrf24_read(ack_payload, 22);
  return 1;
}

void nrf24_openWritingPipe(const uint8_t* address, bool reverse) {
    uint8_t res[addr_width];

    if (reverse) {
        // reverse address before sending
        for (int i = 0; i < addr_width; i++) {
            res[i] = address[addr_width-1-i];
        }
    } else {
        for (int i = 0; i < addr_width; i++) {
            res[i] = address[i];
        }
    }

    nrf24_write_register_with_length(RX_ADDR_P0, res, addr_width);
    nrf24_write_register_with_length(TX_ADDR, res, addr_width);

    nrf24_write_register(RX_PW_P0, payload_size);
}

void nrf24_openReadingPipe(uint8_t child, const uint8_t* address, bool reverse) {

    uint8_t res[addr_width];

    if (reverse) {
        // reverse address before sending
        for (int i = 0; i < addr_width; i++) {
            res[i] = address[addr_width-1-i];
        }
    } else {
        for (int i = 0; i < addr_width; i++) {
            res[i] = address[i];
        }
    }

    // If this is pipe 0, cache the address.  This is needed because
    // openWritingPipe() will overwrite the pipe 0 address, so
    // startListening() will have to restore it.
    if (child == 0){
        memcpy(pipe0_reading_address, res, addr_width);
    }

    if (child <= 6)
    {
        // For pipes 2-5, only write the LSB
        if ( child < 2 ) {
            nrf24_write_register_with_length(pgm_read_byte(&child_pipe[child]), res, addr_width);
        } else {
            nrf24_write_register_with_length(pgm_read_byte(&child_pipe[child]), res, 1);
        }

        nrf24_write_register(pgm_read_byte(&child_payload_size[child]),payload_size);

        // Note it would be more efficient to set all of the bits for all open
        // pipes at once.  However, I thought it would make the calling code
        // more simple to do it this way.
        nrf24_write_register(EN_RXADDR,nrf24_read_register(EN_RXADDR) | _BV(pgm_read_byte(&child_pipe_enable[child])));
    }
}

void nrf24_setAddress(uint8_t *address) {
    // uint8_t address_device[5];
    // uint8_t address_dongle[5];
    // for (int i = 0; i < 5; i++) {
    //     address_device[i] = address[3 + (4 - i)];
    //     address_dongle[i] = address[3 + (4 - i)];
    // }
    // address_dongle[0] = 0;

    nrf24_stopListening();

    // Reopen Reading PIPE
    // nrf24_openReadingPipe(2, address_dongle, false);
    // nrf24_openReadingPipe(1, address_device, false);
    uint8_t txaddress[] = {0x0, 0x0, 0x0, 0x0, 0x1};
    nrf24_openWritingPipe(txaddress, false);
    nrf24_openReadingPipe(1, txaddress, false);
}
/* ---------- */

static bool nrf24_init(void) {
    state.initialized  = false;

    // Initialize pins
    setPinOutput(NRF24_CE);
    setPinOutput(NRF24_CS);

    spi_init();
    spi_start(NRF24_CS, false, 0, NRF24_SCK_DIVISOR);

    // Perform a hardware reset
    writePinLow(NRF24_CE);
    writePinHigh(NRF24_CS);

    // Must allow the radio time to settle else configuration bits will not necessarily stick.
    // This is actually only required following power up but some settling time also appears to
    // be required after resets too. For full coverage, we'll always assume the worst.
    // Enabling 16b CRC is by far the most obvious case if the wrong timing is used - or skipped.
    // Technically we require 4.5ms + 14us as a worst case. We'll just call it 5ms for good measure.
    // WARNING: Delay is based on P-variant whereby non-P *may* require different timing.
    wait_ms(5);

    // Reset NRF_CONFIG and enable 16-bit CRC.
    nrf24_write_register( NRF_CONFIG, 0x0C );

    // Set 1500uS (minimum for 32B payload in ESB@250KBPS) timeouts, to make testing a little easier
    // WARNING: If this is ever lowered, either 250KBS mode with AA is broken or maximum packet
    // sizes must never be used. See documentation for a more complete explanation.
    // nrf24_setRetries(5,15);

    // Then set data rate to 2MBPS
    nrf24_setDataRate( RF24_2MBPS ) ;

    // Disable dynamic payloads, to match dynamic_payloads_enabled setting - Reset value is 0
    nrf24_toggleFeatures();
    nrf24_write_register(FEATURE,0 );
    nrf24_write_register(DYNPD,0);

    // Reset current status
    // Notice reset and flush is the last thing we do
    nrf24_write_register(NRF_STATUS,_BV(RX_DR) | _BV(TX_DS) | _BV(MAX_RT) );

    // Set up default configuration.  Callers can always change it later.
    // This channel should be universally safe and not bleed over into adjacent
    // spectrum.
    nrf24_setChannel(NRF24_CHANNEL);

    // Flush buffers
    nrf24_flush_rx();
    nrf24_flush_tx();

    nrf24_powerUp(); //Power up by default when begin() is called

    // Enable PTX, do not write CE high so radio will remain in standby I mode ( 130us max to transition to RX or TX instead of 1500us from powerUp )
    // PTX should use only 22uA of power
    nrf24_write_register(NRF_CONFIG, ( nrf24_read_register(NRF_CONFIG) ) & ~_BV(PRIM_RX) );

    spi_status_t status = nrf24_get_status();

    // if status = 0x0E, then STATUS register is ready
    // we are clear to go
    state.initialized = status == 0x0E ? true : false;

    nrf24_spi_deinit();

    return state.initialized; //state.initialized;
}

static bool nrf24_configure(void) {

    state.configured = false;

    nrf24_spi_init();

    nrf24_stopListening();
    nrf24_setAutoAck(1);
    // nrf24_setRetries(3, 1);
    nrf24_setPayloadSize(NRF24_PAYLOAD_SIZE);
    uint8_t address[] = {0x0, 0x0, 0x0, 0x0, 0x1};
    nrf24_setAddress(address);
    nrf24_enableDynamicPayloads();
    nrf24_enableAckPayload();
    nrf24_enableDynamicAck();

    wait_ms(2);

    // // Open writing pipe
    // nrf24_openWritingPipe(pairing_mac_address, true);

    // Open Reading Pipe
    // nrf24_openReadingPipe(1, pairing_mac_address, true);

    nrf24_setChannel(NRF24_CHANNEL);
    nrf24_setDataRate(RF24_2MBPS);
    // nrf24_stopListening();

    // uint8_t readAddress[] = { 0xCF, 0x01, 0x2B, 0xCA, 0x32 };

    // nrf24_read_register_with_length(TX_ADDR, readAddress, addr_width);
    // // nrf24_dumpRegisters();

    spi_status_t status = nrf24_get_status();

    // if status = 0x0E, then STATUS register is ready
    // we are clear to go
    state.configured = status == 0x0E ? true : false;
    nrf24_spi_deinit();
    state.configured = true;
    return state.configured;
}

bool nrf24_pair(void) {
    state.paired = false;

    nrf24_spi_init();

    dprintf("[NRF24] - Starting pairing process\n");

    // spi_status_t status = 0;
    // uint8_t response_buffer[22];

    // nrf24_pairingFlow(payload_pairing_1_long, payload_pairing_1_small, response_buffer);
    // nrf24_setAddress((unsigned char *)"00001");

    // nrf24_pairingFlow(payload_pairing_2_long, payload_pairing_2_small, response_buffer);

    // nrf24_pairingFlow(payload_pairing_3_long, payload_pairing_3_small, response_buffer);

    // while (!nrf24_write(payload_pairing_4, 22, 0)) {
    // }

    // dprintf("[NRF24] - Keyboard paired\n");

    nrf24_spi_deinit();

    state.paired = true;
    return state.paired;
}

/* ---------- */

bool nrf24_is_paired(void) { return state.paired; }

void nrf24_task(void) {
    // xprintf("[NRF24] - preparing task\n");
    if (!state.configured && !nrf24_enable_keyboard()) {
        dprintf("[NRF24] - Could not enable keyboard\n");
        return;
    }
}

bool nrf24_enable_keyboard(void) {

    if (!state.initialized && !nrf24_init()) {
        dprintf("[NRF24] - Could not initialize NRF24 module\n");
        return false;
    }
    else{
        xprintf("[NRF24] - initialized\n");
    }

    if (!state.configured && !nrf24_configure()) {
        dprintf("[NRF24] - Could not configure NRF24 module\n");
        return false;
    }
    else{
        xprintf("[NRF24] - configured\n");
    }

    // if (!state.paired && !nrf24_pair()) {
    //     dprintf("[NRF24] - Pairing failed\n");
    //     return false;
    // }

    return true;
}

void nrf24_checksum(uint8_t *payload, uint8_t len) {
    uint8_t checksum = 0;

    for (uint8_t i = 0; i < (len - 1); i++) {
      checksum += payload[i];
    }

    payload[len - 1] = -checksum;
}

void nrf24_print_buffer(uint8_t *buf, uint8_t len) {
    for (int i = 0; i < len; i++) {
        dprintf("%02X ", buf[i]);
    }
    dprintf("\n");
}

bool nrf24_send_keys(uint8_t hid_modifier_mask, uint8_t *keys, uint8_t nkeys) {

    uint8_t payload[10];
    for (int i = 0; i < 10; i++) {
        payload[i] = 0x00;
    }
    payload[0] = 0x00;
    payload[1] = 0xC1;
    payload[2] = hid_modifier_mask;

    xprintf("[NRF24] - copy keys into payload\n");
    for (int i = 0; i < nkeys; i++) {
        payload[i+3] = keys[i];
    }

    xprintf("[NRF24] - checksum\n");
    nrf24_checksum(payload, 10);

    xprintf("[NRF24] - SPI init\n");
    nrf24_spi_init();

    xprintf("[NRF24] - sending keys\n");
    while (!nrf24_write(payload, 10, 0))
    {
    }

    xprintf("[NRF24] - flushing\n");
    nrf24_flush_rx();

    xprintf("[NRF24] - deinit\n");
    nrf24_spi_deinit();

    xprintf("[NRF24] - done\n");
    return true;
}
