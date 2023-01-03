// Data Transmission Prototype NRF24L01 Transceiver ( It uses the 2.4 GHz band ) 
// Max: 100 meters in open space ( 125 different channels )
// JAHP

#include <SPI.h>  // SPI Communication Standard Library
#include <nRF24L01.h>  // Device version library
#include <RF24.h> // This makes the programming less difficult 

// Declare pins CE and CSN
#define CE_PIN 9 // Digital pin CE for SPI Communication ( Uno/Nano = 9 )
#define CSN_PIN 10 // Digital pin CSN SPI Communication ( Uno/Nano = 10 )
 
// Variable with the address of the channel where the data is going to be transmitted
byte address[5] ={'c','a','n','a','l'}; // A simple pipe

// Create the radio object (NRF24L01)
RF24 radio(CE_PIN, CSN_PIN);

// Vector with the data Tx
float data[1];

void setup()
{
  // Initialize the NRF24L01
  radio.begin();
  // Initialize the serial port
  Serial.begin(9600); // Or 115200 for more data rate
  radio.setRetries(15,15); // Set the number and delay of retries upon failed submit
// Open a writing channel
  radio.openWritingPipe(address);
 
}
 
void loop()
{ 
  //cargamos los datos en la variable datos[]
  data[0] = analogRead(A0);

  // Send the data
  bool ok = radio.write(data, sizeof(data));
  // Report the sent data through the serial port
  if(ok)
  {
    Serial.println(data[0]);
  }
  else
  {
    Serial.println("Restart");
  }
  delay(100);
}
