/*
 * This is a proof of concept code for the InvenSense INMP441 microphone and the teensy 3.1 
 * Stereo microphone data is recorded at 48kHz and 32bit sampling per half-frame
 * (24bit real audio content, a odditiy of this microphone)
 *
 * This code is I2S RX only and pushes all the data via fast USB Serial to an computer, 
 * showing that this is possible for high-quality stereo data even without the use of DMA
 *
 * pins configurations:       
 * Rx pin 11 : I2S0_RX_BCLK
 * Rx pin 12 : I2S0_RX_FS   
 * Rx pin 13 : RX pin       
*/

// set 48kHz sampling rate
#define CLOCK_TYPE                  (I2S_CLOCK_48K_INTERNAL)

#include <Wire.h>
/* I2S digital audio */
#include <i2s.h>

// allocate  data buffer
const uint16_t buffersize = 4096; // 2048;
volatile int32_t buffer[buffersize];
uint32_t nTX = 0;
uint32_t nRX = 0;

boolean silent = true;
unsigned char bytes[4];

// extract the 24bit INMP441 audio data from 32bit sample
void extractdata_inplace(int32_t  *pBuf) {
  // set highest bit to zero, then bitshift right 7 times
  // do not omit the first part (!)
  pBuf[0] = (pBuf[0] & 0x7fffffff) >>7;
}


/* --------------------- Direct I2S Receive, we get callback to read 2 words from the FIFO ----- */

void i2s_rx_callback( int32_t *pBuf )
{
  // perform the data extraction for both channel sides
  extractdata_inplace(&pBuf[0]);
  extractdata_inplace(&pBuf[1]);

  // this does not buffer any data, but directly pushs it to USB
  // use a fast (!) program to dump and analyze this.
  // Minicom and Realterm are too slow.
  for(int i=0; i<2; i++) {
    /* 32 bit variant
    bytes[0] = (pBuf[i] >> 24) & 0xFF;
    bytes[1] = (pBuf[i] >> 16) & 0xFF;
    bytes[2] = (pBuf[i] >> 8) & 0xFF;
    bytes[3] = pBuf[i] & 0xFF;*/
    
    // 24 bit variant
    bytes[0] = (pBuf[i] >> 16) & 0xFF;
    bytes[1] = (pBuf[i] >> 8) & 0xFF;
    bytes[2] = pBuf[i] & 0xFF;
    
    // send the data over USB
    Serial.write(bytes,3);
  }  
}


/* --------------------- Direct I2S Transmit, we get callback to put 2 words into the FIFO ----- */

void i2s_tx_callback( int32_t *pBuf )
{
  // send the data
  pBuf[0] = buffer[nTX++];
  pBuf[1] = buffer[nTX++];
  if(nTX >= buffersize) nTX=0;
}

/* ----------------------- begin -------------------- */

void setup()
{
  // << nothing before the first delay will be printed to the serial
  delay(1500);  

  if(!silent){
	  Serial.print("Pin configuration setting: ");
	  Serial.println( I2S_PIN_PATTERN , HEX );
 	  Serial.println( "Initializing." );
  }

  if(!silent) Serial.println( "Initialized I2C Codec" );
  
  // prepare I2S RX with interrupts
  I2SRx0.begin( CLOCK_TYPE, i2s_rx_callback );
  if(!silent) Serial.println( "Initialized I2S RX without DMA" );
  
  // I2STx0.begin( CLOCK_TYPE, i2s_tx_callback );
  // Serial.println( "Initialized I2S TX without DMA" );
  
  // fill the buffer with something to see if the RX callback is activated at all
  buffer[0] = 0x42424242;
  
  delay(5000);
  // start the I2S RX
  I2SRx0.start();
  //I2STx0.start();
  if(!silent) Serial.println( "Started I2S RX" );
}

/* --------------------- main loop ------------------ */
void loop()
{
  // this is empty as all the heavy lifting is done in the i2s_rx_callback() function
  // which will send every audio sample to the USB
  
  // for debugging issues, look at the orange LED at pin 13
  // if data is received correctly from the microphone, it will glow
  // with just a little amount of flickering
}
