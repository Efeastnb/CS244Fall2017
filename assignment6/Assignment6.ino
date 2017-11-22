#include "SparkFunLIS3DH.h"
#include "Wire.h"
//#include "SPI.h"
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include "MAX30105.h" 

//uint32_t count = 0;
//float X[500] ;
//float Y[500] ;
//float Z[500] ;

char WIFINAME[] = "AE5D7E";
char WIFIPASSWORD[]= "4CE363WW0176A";
const char server[]="192.168.0.3";

unsigned long time_now;
unsigned long time_old = 0;

double X0;
double Y0;
double Z0;
uint32_t IR0;
uint32_t RED0;

WiFiClient client;
LIS3DH myIMU; //Default constructor is I2C, addr 0x19.
MAX30105 particleSensor;//instantiation

void setup() {
  pinMode(BUILTIN_LED,OUTPUT);
  Serial.begin(115200);
  delay(1000);
  
  Serial.print("Connecting");
  Serial.println(WIFINAME);
  WiFi.begin(WIFINAME,WIFIPASSWORD);
  while (WiFi.status()!=WL_CONNECTED)
  {
      delay(500);
      Serial.print(".");
  }
  Serial.println("");

  Serial.print("Connected,IP address:");
  Serial.println(WiFi.localIP());
  
  //Call .begin() to configure the IMU

  // Initialize sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use GPIO0 and GPIO2 as I2C port, 400kHz speed
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }

  //Setup to sense a nice looking saw tooth on the plotter
  byte ledBrightness = 0x1F; //Options: 0=Off to 255=50mA,0x02, 0.4mA;0x1F, 6.4mA;0x7F, 25.4mA
  byte sampleAverage = 4; //Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 3; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  byte sampleRate = 400; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 411; //Options: 69, 118, 215, 411
  int adcRange = 4096; //Options: 2048, 4096, 8192, 16384

  //particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with these settings
  
  particleSensor.setup();//default setup
  
  myIMU.settings.accelRange = 2;      //Max G force readable.  Can be: 2, 4, 8, 16
  myIMU.settings.accelSampleRate = 50;
  myIMU.begin();
  /*
  for (count=0; count< 500; count++){
    X[count] = myIMU.readFloatAccelX();
    Serial.print(" X = ");
    Serial.println(X[count],4);
    Y[count] = myIMU.readFloatAccelY();
    Serial.print(" Y = ");
    Serial.println(Y[count],4);
    Z[count] = myIMU.readFloatAccelZ();
    Serial.print(" Z = ");
    Serial.println(Z[count],4);
  }*/

}


void loop()
{
  /*
  //Get all parameters
  Serial.print("\nAccelerometer:\n");
  Serial.print(" X = ");
  Serial.println(myIMU.readFloatAccelX(), 4);
  Serial.print(" Y = ");
  Serial.println(myIMU.readFloatAccelY(), 4);
  Serial.print(" Z = ");
  Serial.println(myIMU.readFloatAccelZ(), 4);
  
  //Serial.println(particleSensor.getIR());
  //Serial.println(particleSensor.getRed());
  */
  time_now = millis();
  if (time_now - time_old >= 100){
    time_old = time_now;
    Serial.print(" Timestamp = ");
    Serial.println(time_now);
    
    X0 = myIMU.readFloatAccelX();
    Y0 = myIMU.readFloatAccelY();
    Z0 = myIMU.readFloatAccelZ();
    IR0 = particleSensor.getIR();
    RED0 = particleSensor.getRed();
  
    //delay(1000);
    
    WiFiClient client;
  
    if (client.connect(server, 80)) {
       //Serial.println("connected to server");
  
       String data = "_X0=" 
       +                   (String) X0
       + "&_Y0="          +(String) Y0
       + "&_Z0="          +(String) Z0
       + "&_IR0="         +(String) IR0
       + "&_RED0="        +(String) RED0;
  
      int length = data.length();
  
      String postRequest =(String)("POST ") + "/cs244/tdata.php" + "/ HTTP/1.1\r\n" +  
          "Content-Type:  application/x-www-form-urlencoded\r\n" +  
          "Host: " + server + ":" + 80 + "\r\n" +            
          "Content-Length: " + length + "\r\n" +  
          "Connection: Keep Alive\r\n\r\n" +  
          data+"\r\n";  
      client.print(postRequest);  
      Serial.print(postRequest);  
    }
  }
}
