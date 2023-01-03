// Data Reception Prototype NRF24L01 Transceiver ( It uses the 2.4 GHz band ) 
// Max: 100 meters in open space ( 125 different channels )
// JAHP

#include <SPI.h> // SPI Communication Standard Library
#include <nRF24L01.h> // Device version library
#include <RF24.h> // This makes the programming less difficult 
 
// Declare pins CE and CSN
#define CE_PIN 9 // Digital pin CE for SPI Communication ( Uno/Nano = 9 )
#define CSN_PIN 10 // Digital pin CSN SPI Communication ( Uno/Nano = 10 )
// Variable with the address of the channel where the data is going to be received
byte direccion[5] ={'c','a','n','a','l'}; // A simple pipe

// Create the radio object (NRF24L01)
RF24 radio(CE_PIN, CSN_PIN);

// Vector with the data Rx
float data[1];

void setup()
{
  // Initialize the NRF24L01 
  radio.begin();
  // Initialize the serial port
  Serial.begin(9600); // Or 115200 for more data rate ( 115200 for more than 7000 data )
  
  // Open a reading channel
  radio.openReadingPipe(1, direccion); // Channel 0-5
  
  Serial.setTimeout(10); //Sets the maximum milliseconds to wait for serial data
  
  // We started reading from the channel
  radio.startListening();
 
}
 
void loop() {
 uint8_t numero_canal;
 //if ( radio.available(&numero_canal) )
 if ( radio.available() )
 {    
     // Read the data and store it in the variable data []
     radio.read(data,sizeof(data));
     // Report the received data through the serial port
     Serial.println(data[0]);
 }
 else
 {
     Serial.println("No data");
 }
 delay(100);
}
