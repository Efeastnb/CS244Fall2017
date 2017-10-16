#include "Arduino.h"
#include "ESP8266WiFi.h"
#include <ESP8266WiFiMulti.h> 
#include <ESP8266HTTPClient.h>

const char * host = "169.234.70.153";
const char *ssid = "UCInet Mobile Access";


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
}

void loop()

{
    digitalWrite(LED_BUILTIN,HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN,LOW);
    delay(500);

    const char server[] = "169.234.122.91"; 



   WiFiClient client;


    int Heartrate = 120;

    int Bloodpressure = 140;


  
   Serial.println("\nStarting connection to server..."); 
  
    if (client.connect(server, 80)) {
      Serial.println("connected to server");
      
      
     String data = "Heartrate="
            +                        (String) Heartrate
            +  "&Bloodpressure="  +(String)   Bloodpressure;
          
      int length = data.length();
           
      String postRequest =(String)("POST ") + "/demo.php" + "/ HTTP/1.1\r\n" +  
            "Content-Type:  application/x-www-form-urlencoded\r\n" +  
            "Host: " + host + ":" + 80 + "\r\n" +            
            "Content-Length: " + length + "\r\n" +  
            "Connection: Keep Alive\r\n\r\n" +  
            data+"\r\n";  
      Serial.println(postRequest);  
      client.print(postRequest);  
      delay(2000);
  
    } 

}





    


