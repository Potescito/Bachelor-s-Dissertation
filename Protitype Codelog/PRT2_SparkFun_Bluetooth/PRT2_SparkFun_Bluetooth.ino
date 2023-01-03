// Prototype suitable for implementation (need to be updated)
// DCSR-JAHP

# include <SoftwareSerial.h>

SoftwareSerial port(7,8); //  RX, TX (avoid using tx and rx ports of arduino)

// Vector with the data Tx
float data[1];
float v;            // Voltaje
float f = 0;        // Flux in L/m
float volumen = 0; 
float f1;  // Flux value after sampling
float f_old = 0;
float v_old = 0;
float sample_time = 0.00333333;  // 0.01ms -> minutes -> L/m; // 1/samples
int flag_1 = 0;
int resp_rate = 0;
int resp_rate_2 = 0;
float dif = 0;
unsigned long time, time2 = 0;

void setup()
{

  Serial.begin(9600);
  port.begin(9600); // Starts communication data rate for Bluetooth ( 38400 bd in configuration mode )
  calibr();
  port.println("*********Receiving Data**********");
  pinMode(9,INPUT);
}
 
void loop()
{ 
  //load data in data var
  data[0] = analogRead(A0);
  //port.println(data[0]);
  //port.println(data[0]*5/1023.0);
  //Serial.println(data[0]);
  f = 0;
  v = (data[0]*5/1023.0)+dif;

  time = millis();
  time2 = time - time2;

  if(v > 2.8){
    f = (11.449 * v * v * v) - (122.89 * v * v) + (443.83 * v) - 527.89;
  }
  else if ( v < 2.6){
    f = -((-10.695 * v * v * v) + (60.129 * v * v) + (-116.63 * v) + 89.218);
  }
  if( f > 15) f = 15;
  if(f < -15 ) f = -15;
  
  volumen = (sample_time * (f + f_old) + 2 * v_old) / 2;  // Discrete volumen

  Serial.print(v);
  Serial.print("\t");
  Serial.print(f);
  Serial.print("\t");
  Serial.println(volumen);
  port.println(volumen);

  delay(100); //En milisegundos
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
