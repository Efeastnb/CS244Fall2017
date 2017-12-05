#include "SparkFunLIS3DH.h"
#include "Wire.h"
//#include "SPI.h"
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include "MAX30105.h" 

int count = 0;
int loopcount = 0;
float X[50] ;
float Y[50] ;
float Z[50] ;
uint32_t Ir[50];
uint32_t Red[50];

char WIFINAME[] = "AE5D7E";
char WIFIPASSWORD[]="4CE363WW0176A";
const char server[]="192.168.0.3";

//unsigned long time_now;
long time_old = 0;
long startTime;
long samplesTaken = 0;

WiFiClient client;
LIS3DH myIMU; //Default constructor is I2C, addr 0x19.
MAX30105 particleSensor;//instantiation

void setup() {
  // put your setup code here, to run once:
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
  byte ledBrightness = 0x1f; //Options: 0=Off to 255=50mA,0x02, 0.4mA;0x1F, 6.4mA;0x7F, 25.4mA
  byte sampleAverage = 4; //Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 2; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  byte sampleRate = 200; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 411; //Options: 69, 118, 215, 411
  int adcRange = 4096; //Options: 2048, 4096, 8192, 16384

  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with these settings
  
  //particleSensor.setup();//default setup
  
  myIMU.settings.accelRange = 2;      //Max G force readable.  Can be: 2, 4, 8, 16
  myIMU.settings.accelSampleRate = 50;
  myIMU.begin();

}

void loop() {
  // put your main code here, to run repeatedly:
  if(samplesTaken < 30000){
    particleSensor.check(); //Check the sensor, read up to 3 samples
  
    while (particleSensor.available()) //do we have new data?
    {
      samplesTaken++;
      Serial.println(samplesTaken);   
      
      
      X[count] = myIMU.readFloatAccelX();
      Y[count] = myIMU.readFloatAccelY();
      Z[count] = myIMU.readFloatAccelZ();
      Ir[count]= particleSensor.getFIFOIR();
      Red[count]=particleSensor.getFIFORed();
      count++;
      particleSensor.nextSample(); //We're finished with this sample so move to next sample
    }

     if(samplesTaken % 50 == 0 && samplesTaken != 0 && millis() - time_old > 100){
      for (count=0; count< 50; count++){
        WiFiClient client;
        //Serial.println("\nStarting connection to server..."); 
      
        if (client.connect(server, 80)) {
           //Serial.println("connected to server");
      
           String data = "_X0=" 
           +                   (String) X[count]
           + "&_Y0="          +(String) Y[count]
           + "&_Z0="          +(String) Z[count]
           + "&_IR0="         +(String) Ir[count]
           + "&_RED0="        +(String) Red[count];
      
          int length = data.length();
      
          String postRequest =(String)("POST ") + "/cs244/tdata.php" + "/ HTTP/1.1\r\n" +  
              "Content-Type:  application/x-www-form-urlencoded\r\n" +  
              "Host: " + server + ":" + 80 + "\r\n" +            
              "Content-Length: " + length + "\r\n" +  
              "Connection: close\r\n\r\n" +  
              data+"\r\n";  
          client.print(postRequest);  
          //delay(100); 
          //Serial.print(postRequest);  
        }
      }
      count = 0;
      time_old = millis();
      //Serial.println((float)time_old, 2);   
    }
  }
}
