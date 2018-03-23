/*************************************************************************************
*   Home Automation with Android App and NodeMCU
*
*  Groups of devices
*   ==> All Devices ON/OFF
*   
*  Voice activation and response
*
*   Developed by Marcelo Rovai on 27March17
*   Visit my blog: https://MJRoBot.org 
*   
*   WARNING: When uploading a code ==> Power-off Relays
************************************************************************************/
#include <ESP8266WiFi.h>
WiFiClient client;
WiFiServer server(80);
const char* ssid = "Lenovo A6000";
const char* password = "88888888";
String  command =""; // Command received from Android device

// Set Relay Pins
int light1 = D0;
int light2 = D1;

void setup()
{
  Serial.begin(115200);

  pinMode(light1, OUTPUT); 
  pinMode(light2, OUTPUT);    
  
  Serial.println("Connecting to WIFI");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print("..");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  delay(300);
    //server begin
  server.begin();
  Serial.println("Server Started");
  Serial.println("NodeMCU Local IP is : ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.print("/");
}
void loop()
{
    client = server.available();
    if (!client) return; 
    command = checkClient ();

         if (command == "r1on"  || command == "one on"    || command == "want on")        digitalWrite(light1,HIGH);
    else if (command == "r1off" || command == "one-off" || command == "one off")     digitalWrite(light1,LOW);
    else if (command == "r2on"  || command == "two on"    || command == "2 on")      digitalWrite(light2,HIGH);     
    else if (command == "r2off" || command == "two off" || command == "2 off")   digitalWrite(light2,LOW);
    else if (command == "allon" || command == "nyalakan semua" || command == "all lights on") 
    {
      digitalWrite(light1,HIGH);
      digitalWrite(light2,HIGH);
    }
    else if (command == "alloff" || command == "matikan semua" || command == "all lights off") 
    {
      digitalWrite(light1,LOW);
      digitalWrite(light2,LOW);
    }
    sendBackEcho(command); // send command echo back to android device
    command = "";
}

/* check command received from Android Device */
String checkClient (void)
{
  while(!client.available()) delay(1); 
  String request = client.readStringUntil('\r');
  request.remove(0, 5);
  request.remove(request.length()-9,9);
  return request;
}

/* send command echo back to android device */
void sendBackEcho(String echo)
{
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println(echo);
  client.println("</html>");
  client.stop();
  delay(1);
}
