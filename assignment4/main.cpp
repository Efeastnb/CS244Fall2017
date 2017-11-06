#include "SparkFunLIS3DH.h"
#include "Wire.h"
#include "SPI.h"
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>

uint32_t count = 0;
float X[500] ;
float Y[500] ;
float Z[500] ;

char WIFINAME[] = "UCInet Mobile Access";
char WIFIPASSWORD[]= "";
const char server[]="169.234.52.161";

WiFiClient client;
LIS3DH myIMU; //Default constructor is I2C, addr 0x19.

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
  myIMU.settings.accelRange = 16;      //Max G force readable.  Can be: 2, 4, 8, 16
  myIMU.settings.accelSampleRate = 50;
  myIMU.begin();
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
  }
  count = 0;

}


void loop()
{
  if(count<500){
    
    WiFiClient client;

    if (client.connect(server, 80)) {
       //Serial.println("connected to server");

       String data = "_X0=" 
       +                   (String) X[count]
       + "&_Y0="          +(String) Y[count]
       + "&_Z0="          +(String) Z[count];

      int length = data.length();
 
      String postRequest =(String)("POST ") + "/xyz.php" + "/ HTTP/1.1\r\n" +  
          "Content-Type:  application/x-www-form-urlencoded\r\n" +  
          "Host: " + server + ":" + 80 + "\r\n" +            
          "Content-Length: " + length + "\r\n" +  
          "Connection: Keep Alive\r\n\r\n" +  
          data+"\r\n";  
      client.print(postRequest);  
      //Serial.print(postRequest);  

  }
  count = count + 1;
  }
}
