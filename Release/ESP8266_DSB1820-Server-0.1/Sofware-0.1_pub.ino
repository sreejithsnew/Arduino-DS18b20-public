<!-- /*------------------------------------
Title: ESP8266_DSB1820 Server
Author: Sreejith.S <VU3BIF> <sreejithsnew@gmail.com>
Dsb1820 temp reading set to Http response
Thanks anvar<123anvar@gmail.com > for helping to solve ajax calling stuffs
-------------------------------------*/ -->
/*
Integrated DSB working code into working server code.. Ajax calling is working 
in this server code.... Integration done..... Working perfectly when connecting in station mode... AP mode is not stable
*/
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
//#include <ESP8266mDNS.h>
#include <OneWire.h>
#include <DallasTemperature.h>
//#define DEBUG          //Comment this to avoid serial output logs
#define ONE_WIRE_BUS1 2

OneWire oneWire1(ONE_WIRE_BUS1);
DallasTemperature sensor(&oneWire1);

DeviceAddress Probe01 = { 0x28, 0xFF, 0x7C, 0x53, 0x82, 0x15, 0x03, 0x88 };   //Address of the DBS1820 modules using
DeviceAddress Probe02 = { 0x28, 0xFF, 0x52, 0x0B, 0x83, 0x15, 0x01, 0x01 };
DeviceAddress Probe03 = { 0x28, 0xFF, 0x4A, 0x51, 0x82, 0x15, 0x03, 0x97 };
DeviceAddress Probe04 = { 0x28, 0xFF, 0xC3, 0x02, 0x83, 0x15, 0x02, 0x0A };
DeviceAddress Probe05 = { };
DeviceAddress Probe06 = { };
DeviceAddress Probe07 = { };	//Update this when ds18b20 is available
DeviceAddress Probe08 = { };

String finaloutput;

//MDNSResponder mdns;

ESP8266WebServer server ( 80 );

void handleTest()
{  
  //queryStringValue();  
  server.send ( 200, "text/plain",  server.arg(0));
}

void debugout(String str) {
#ifdef DEBUG
  Serial.println(str);
#endif
}

/* void queryStringValue() {
 // Print how many properties we received and then print their names
 // and values.
 //Serial.println("Number of query properties: " + String(webServer.args()));
// int i;
// for (i=0; i<server.args(); i++) {
// Serial.println(" - " + server.argName(i) + " = " + server.arg(i));
Serial.println(server.arg(0));
// }
}
 */
void handleRoot() {
  server.send ( 200, "text/html", '$'+finaloutput+'$' );//Ouput is like $sensor1,sensor2,....sensor7$
}

void setup_wifi(){
  {
  const char *ssid = "your ssid here";              //This will connect to exsting wi-fi
  const char *password = "your password here";  

  WiFi.begin ( ssid, password );  

  // Wait for connection
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );   
  }
  }
/*  else if(mode=="AP"){              //This will set the AP mode to create hotspot (have some issue in this mode)
  char AP_NameChar[] = "ESP-WiFi";
  const char WiFiAPPSK[] = "myesp1234";
  
  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_NameChar, WiFiAPPSK);
  } */
}

void setup ( void ) {
  Serial.begin ( 115200 );
  sensor.begin();
  delayMicroseconds(10);
  setup_wifi();
/*   if ( mdns.begin ( "esp8266", WiFi.localIP() ) )
    {}
 */
  server.on ( "/", handleRoot );
  //server.on ( "/test.svg", drawGraph );
  server.on ( "/inline", []() {
    server.send ( 200, "text/plain", "this works as well" );
  } );
  server.on("/test",handleTest );
  //server.onNotFound ( handleNotFound );
  server.begin(); 
  delay (100);    

  sensor.setResolution(Probe01, 12);        //Set to max resolution, lower resolution (10) will be faster
  sensor.setResolution(Probe02, 12);
  sensor.setResolution(Probe03, 12);
  sensor.setResolution(Probe04, 12);
  sensor.setResolution(Probe05, 12);
  sensor.setResolution(Probe06, 12);
  sensor.setResolution(Probe07, 12);
  sensor.setResolution(Probe08, 12);
}

void loop ( void ) {

  sensor.requestTemperatures();
  delayMicroseconds(10);
  debugout("\nRequesting temperatures... \n");
  delay(500);                                         //Set the freq of measurement... This set the wifi writing too
  //Serial.print(sensor1.getTempCByIndex(0));

  String output1(sensor.getTempC(Probe01), 2);        //Covertion float -> String
  delayMicroseconds(10);
  String output2(sensor.getTempC(Probe02), 2);
  delayMicroseconds(10);
  String output3(sensor.getTempC(Probe03), 2);
  delayMicroseconds(10);
  String output4(sensor.getTempC(Probe04), 2);
  delayMicroseconds(10);
  String output5(sensor.getTempC(Probe04), 2);
  delayMicroseconds(10);
  String output6(sensor.getTempC(Probe04), 2);
  delayMicroseconds(10);
  String output7(sensor.getTempC(Probe04), 2);
  delayMicroseconds(10);
  String output8(sensor.getTempC(Probe04), 2);
  delayMicroseconds(10);
  
  debugout(output1);                           //Only for debugging purpose
  debugout(output2);
  debugout(output3);
  debugout(output4);
  debugout(output5);
  debugout(output6);
  debugout(output7);
  debugout(output8);

  finaloutput = output1 + ',' + output2 + ',' + output3 + ',' + output4 + ',' + output5 + ',' + output6 + ',' + output7 + ',' + output8;
  debugout(finaloutput);
  
  //mdns.update(); //use this if need dns :)
  server.handleClient();
}