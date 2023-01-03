// Data Transmission Prototype Bluetooth module ( HM-10 B4.0 / HC-05 B2.0 )
// JAHP

# include <SoftwareSerial.h>

SoftwareSerial port(7,8); //  RX, TX (avoid using tx and rx ports of arduino)

// Vector with the data Tx
float data[1];
float val;
int st = 0;

void setup()
{

  Serial.begin(9600);
  port.begin(9600); // Starts communication data rate for Bluetooth ( 38400 bd in configuration mode )
  calibr()
  port.println("*********Receiving Data**********");
 
}
 
void loop()
{ 
  //load data in data var
  data[0] = analogRead(A0)-80;
  //port.println(data[0]);
  port.println(data[0]*5/1023.0);
  //Serial.println(data[0]);
  Serial.println(data[0]*5/1023.0);

  if( port.available() ){
    Serial.write(port.read());
  }
  if( Serial.available() ){
    port.write(Serial.read());
  }

  float tol = -0.02;
  float voltage = data[0]*5/1023.0 +val;
  float pressure = ((((voltage)/5)-0.5)/0.018)+tol;
  float pressure_ampl = pressure/11 - 2.5/11;

  //Serial.println(pressure);
  Serial.println(voltage);
  Serial.println(pressure_ampl);
  port.println(voltage);
  port.println(pressure_ampl);

  //=========================================0

  //venturi
  int insp = 0;
  int expi = 0;
  float speedpn = 0;

  if( pressure_ampl > 0 ){
    insp = 1;
  }
  else insp = 0;

  if( pressure_ampl < -0.2 ){
    expi = 1;
  }
  else expi = 0;
  
  if( insp ){
    speedpn = sqrt(2*(pressure_ampl/1.225))*100;
  }
  if( expi ){
    speedpn = -sqrt(2*(-pressure_ampl/1.225))*100;
  }
  
  float area = 3.1415926535897932384626433832795*(0.013/2)*(0.013/2);
  float flux  = area*speedpn*0.01*60;

  Serial.print("Speed(cm/s): ");
  Serial.println(speedpn);
  port.print("Speed(cm/s): ");
  port.println(speedpn);

  Serial.print("FLUX(L/m): ");
  Serial.println(flux);
  port.print("FLUX(L/m): ");
  port.println(flux);

  delay(200);
}

int calibr(){
  port.println("*******Calibration setup********");
  delay(10000); // Avoiding noise
  float val = 0;
  for (int i =0; i<50; i++) {
    val += (analogRead(A0)*5/1023.0);
  }
  val = val/50;
  dif = 2.735 - val;
  return dif;
}
