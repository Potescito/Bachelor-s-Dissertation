// Last possible prototype (due dates reached due to KOSPIE Scholarship)
// JAHP-DCSR

#include <SoftwareSerial.h>  // Library for BT

#define ReadPort 9   // Analog port
#define samples 100  // Value in [Hz]
#define CT      10   // Calibration time [seconds] (Transducer readout stabilization)

SoftwareSerial port(7,8); // RX, TX (avoid using tx and rx ports when using arduino boards)

float data[1]; 
float v;              // Voltage
float f = 0;          // Flux in L/m
float volumen = 0;
float f1;             // Flux value after sampling
float f_old = 0;
float v_old = 0;
float sample_time = 0.0001666667//0.00333333;  // 0.01ms -> minutes -> L/m; // 1/samples
int flag_1 = 0;
int resp_rate = 0;
int resp_rate_2 = 0;
float dif = 0;
unsigned long time, time2 = 0;


void setup() {
  Serial.begin(9600);
  port.begin(9600);
  pinMode(ReadPort, INPUT);
  calibr();
}

void loop() {
  data[0] = analogRead(ReadPort);
  f = 0;
  v = (data[0] * 5 / 1023.0) + dif;
  time = millis();
  time2 = time - time2;

  /*
  pressure = ((((voltage)/5)-0.5)/0.018)+tol;
  //pressure_ampl = pressure/11 - 2.5/11;
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
 */

  if (v > 2.8) {
    f = (11.449 * v * v * v) - (122.89 * v * v) + (443.83 * v) - 527.89;  // Excel lab model
  } else if (v < 2.62) {
    f = -((-10.695 * v * v * v) + (60.129 * v * v) + (-116.63 * v) + 89.218);  // Excel lab model
  }
  if (f > 15) f = 15;
  if (f < -15) f = -15;

  if ((f < 0) & (f_old > 0)) {
    flag_1 = 1;
  }

  if (flag_1) {
    resp_rate = resp_rate + 1;
    flag_1 = 0;
    if (time2 > 20000) {
      resp_rate_2 = resp_rate * 3;
      time2 = time;
      resp_rate = 0;
    }
  }

  volumen = (sample_time * (f + f_old) + 2 * v_old) / 2;  // discrete volumen
  Serial.print(0);                                        // To freeze the lower limit
  Serial.print(" ");
  //Serial.print(10);  // To freeze the upper limit
  //Serial.print(" ");


  //port.println( data[0] );
  port.println( volumen );
  //Serial.println(data[0]);
  //Serial.println(f);
  //Serial.println( resp_rate_2 );
  //Serial.println(volumen);
  Serial.print("Flow: "); 
  Serial.print(f);  
  Serial.print("/ Volume: "); 
  Serial.print(volumen);
  Serial.print("/ Resp_Rate: "); 
  Serial.println(resp_rate_2);  

  f_old = f;
  v_old = volumen;
  delay(1000 / samples);  // milliseconds
}

int calibr() {
  Serial.println("calibration setup");
  port.println("calibration setup");
  delay(int(CT * 1000));  // Avoiding noise
  float val = 0;
  for (int i = 0; i < 50; i++) {
    val += (analogRead(ReadPort) * 5 / 1023.0);
  }
  val = val / 50;
  dif = 2.735 - val;  // 2.735 is the initial value of data set (experimental model)
  Serial.println("Device calibrated");
  port.println("Device calibrated");
  return dif;
}