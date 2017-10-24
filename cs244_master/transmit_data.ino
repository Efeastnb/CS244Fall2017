//time_stamp IR RED HR RR SPO2
//Heart Rate is 60 to 120 for each person per minutes
//BandPass Filter 1HZ -- 2HZ
//Respiration Rate is 10 to 20 for each person per minutes
//BandPass Filter 1/6HZ -- 1/3HZ



#include "MAX30105.h" 
#include "ESP8266WiFi.h"
#include <Wire.h>
#include <ESP8266WiFiMulti.h> 
#include <ESP8266HTTPClient.h>

const char * host = "169.234.104.170";
const char server[] = "169.234.104.170";
const char *ssid = "UCInet Mobile Access";
uint32_t ppgnum = 0;
uint32_t count = 0;
//uint32_t _IR[4] ;
//uint32_t _RED[4] ;
uint32_t _IR[3000] ;
uint32_t _RED[3000] ;

MAX30105 particleSensor;//instantiation

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);  
  Serial.println("");  
  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());
  WiFi.begin(ssid); 
  Serial.print("Connecting..");  
  while (WiFi.status()!= WL_CONNECTED)  
  {  
    delay(500);  
    Serial.print(".");  
  }  
  Serial.println();  
  Serial.print("Connected,IP Address:");  
  Serial.println(WiFi.localIP());  

   // Initialize sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use GPIO0 and GPIO2 as I2C port, 400kHz speed
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }
  /*
  //Setup to sense a nice looking saw tooth on the plotter
  byte ledBrightness = 0x02; //Options: 0=Off to 255=50mA,0x02, 0.4mA;0x1F, 6.4mA;0x7F, 25.4mA
  byte sampleAverage = 8; //Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 3; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  byte sampleRate = 50; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 411; //Options: 69, 118, 215, 411
  int adcRange = 4096; //Options: 2048, 4096, 8192, 16384

  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with these settings
*/
  particleSensor.setup();
  Serial.println("Data collect starts"); 
  for(ppgnum = 0; ppgnum < 2999; ppgnum++)
  {
     _IR[ppgnum] = particleSensor.getIR();
     _RED[ppgnum] = particleSensor.getRed(); 
     //delay(20);
  }
  Serial.println("Data collect completed");  
}

void loop()

{
    if (count < 3000){
  
    WiFiClient client;
  
    Serial.println("\nStarting connection to server..."); 

    if (client.connect(server, 80)) {
      Serial.println("connected to server");}
    

      String data = "_IR0=" 
            +              (String) _IR[count]
            +  "&_RED0="  +(String) _RED[count];
           /* +  "&_IR1= "  +(String) _IR[count+1]
            +  "&_RED1="  +(String) _RED[count+1]
            +  "&_IR2= "  +(String) _IR[count+2]
            +  "&_RED2="  +(String) _RED[count+2]
            +  "&_IR3= "  +(String) _IR[count+3]
            +  "&_RED3="  +(String) _RED[count+3];*/
      int length = data.length();
         
      String postRequest =(String)("POST ") + "/cs244/tdata.php" + "/ HTTP/1.1\r\n" +  
            "Content-Type:  application/x-www-form-urlencoded\r\n" +  
            "Host: " + host + ":" + 80 + "\r\n" +            
            "Content-Length: " + length + "\r\n" +  
            "Connection: Keep Alive\r\n\r\n" +  
            data+"\r\n";  
      //Serial.println(postRequest);  
      client.print(postRequest);  
      //delay(20);//0.02S
      count = count + 1;
    }
}
/*
void loop()

{ 
  if (count < 1500){

  for(ppgnum = 0; ppgnum < 4; ppgnum++){
     _IR[ppgnum] = particleSensor.getIR();
     _RED[ppgnum] = particleSensor.getRed();
    }
    
    WiFiClient client;
  
    //Serial.println("\nStarting connection to server..."); 

    if (client.connect(server, 80)) {
      //Serial.println("connected to server");
    

      String data = "_IR0=" 
            +              (String) _IR[0]
            +  "&_RED0="  +(String) _RED[0]
            +  "&_IR1= "  +(String) _IR[1]
            +  "&_RED1="  +(String) _RED[1]
            +  "&_IR2= "  +(String) _IR[2]
            +  "&_RED2="  +(String) _RED[2]
            +  "&_IR3= "  +(String) _IR[3]
            +  "&_RED3="  +(String) _RED[3];
      int length = data.length();
         
      String postRequest =(String)("POST ") + "/cs244/tdata.php" + "/ HTTP/1.1\r\n" +  
            "Content-Type:  application/x-www-form-urlencoded\r\n" +  
            "Host: " + host + ":" + 80 + "\r\n" +            
            "Content-Length: " + length + "\r\n" +  
            "Connection: Keep Alive\r\n\r\n" +  
            data+"\r\n";  
      //Serial.println(postRequest);  
      client.print(postRequest);  
      //delay(20);//0.02S

      } 
   
    count++;
  }
}*/

