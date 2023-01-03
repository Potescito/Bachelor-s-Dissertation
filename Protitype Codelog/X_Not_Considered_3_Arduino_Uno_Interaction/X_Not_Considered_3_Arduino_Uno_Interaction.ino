# include <SoftwareSerial.h> // for BT
# include <string.h> // for strcasecmp


# define ReadPort A0
# define CT 1 // Calibration time (seconds)
# define CI 12 // Inactivity time (seconds)
# define samples 10 // Number of samples per sec

SoftwareSerial port(7,8); //  RX, TX (avoid using tx and rx ports of arduino)

// Vector with the data Tx
float data[1];
float v; // Voltage
float f = 0; // Flux in L/m 
float volumen = 0;
float f1; // Flux value after sampling
float dif;
const char* psw = "OK"; // Password to start sending the data to BT
const char* pswn = "NO"; // Password to start sending the data to BT
char start[3]; // final character null
int flag = 0;
int act = 0;
unsigned long T1;
unsigned long T2;

//char buffer[32];

void setup()
{

  Serial.begin(9600);
  port.begin(9600); // Starts communication data rate for Bluetooth ( 38400 bd in configuration mode )
  calibr();
  Serial.println("Receiving Data: OK -> Start");
  port.println("Receiving Data: OK-> Start");
}
 
void loop()
{ 
  data[0] = analogRead(ReadPort);
  f = 0;
  v = (data[0]*5/1023.0)+dif;

  if(v > 2.8){
    f = (11.449 * v * v * v) - (122.89 * v * v) + (443.83 * v) - 527.89; // Excel lab model
  }
  else if ( v < 2.62){
    f = -((-10.695 * v * v * v) + (60.129 * v * v) + (-116.63 * v) + 89.218); // Excel lab model
  }
  if( f > 15) f = 15;
  if(f < -15 ) f = -15;
  
  f1 = f/60*samples;
  //volumen = volumen + f1;

  // Debugging
  //Serial.println(v); 
  ///Serial.print("\t"); 
  //Serial.print(f); 
  //Serial.print("\t"); 
  //Serial.println(volumen);
  
  if( port.available() ){
    (port.readStringUntil('\n')).toCharArray(start,sizeof(psw)+1);
    if( strcasecmp(start,psw) == 0 ){
      flag = 1; //I do it this way because the pirate arduino nano with Old bootloader doesn't work well with StringObjects:'(
      //T1 = millis();
    }
    else if( strcasecmp(start,pswn) == 0 ) flag = 0;
   
    /*size_t l = port.readBytesUntil('\n', buffer, sizeof(buffer - 1));
    if (buffer[l - 1] == '\r') {
      l--;
    }
    buffer[l] = 0; // the terminating zero
    Serial.println(buffer);
    if (strcmp(buffer, a) == 0) {
      Serial.println("hola");
    } else {
      Serial.println("chao");
    }*/
  }
  
  if( flag==1 ){
    volumen = volumen + f1;
    port.println(volumen); // Data BT transmission 
    Serial.println(volumen);
    inactive(f);
  }
  //T2 = millis()-T1; // Time elapsed since OK
  
  delay(1000*(1/samples)); //En milisegundos
}

void inactive(float flux){
  if ( flux == 0 && act == 0){
    T1 = millis(); // new timestamp
    act = 1;
  }
  else if( flux != 0 ){
    act = 0;
  }
  T2 = millis() - T1;
  if( flux == 0 && T2 > (CI*1000) && act == 1){
    flag = 0;
    Serial.println("Inactivity: Type OK->Start again");
    T2 = 0;
    act = 0;
  }
}

int calibr(){
  Serial.println("Calibration setup");
  port.println("Calibration setup");
  delay(int(CT*1000)); // Avoiding noise
  float val = 0;
  for (int i =0; i<50; i++) {
    val += (analogRead(ReadPort)*5/1023.0);
  }
  val = val/50;
  dif = 2.735 - val; // 2.735 is the initial value of data set (experimental model)
  return dif;
  Serial.println("Device calibrated");
  port.println("Device calibrated");
}
