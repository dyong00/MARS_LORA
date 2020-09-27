/*
    This sketch sends data via HTTP GET requests to data.sparkfun.com service.

    You need to get streamId and privateKey at data.sparkfun.com and paste them
    below. Or just customize this script to talk to other HTTP servers.

*/
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include "SNIPE.h" 

#define ATSerial Serial

//16byte hex key
String lora_app_key = "11 22 33 44 55 66 77 88 99 aa bb cc dd ee ff 00";

//와이파이
const char* ssid     = "KT_GiGA_Twosome";
const char* password = "twosome0731";

//서버 -> client.connect(server, httpPort)
IPAddress server(211,218,150,109);
const int httpPort = 5001;

#define TXpin D9
#define RXpin D8

SoftwareSerial DebugSerial(RXpin,TXpin);
SNIPE SNIPE(ATSerial);

// Use WiFiClient class to create TCP connections
WiFiClient client; //서버 연결 객체 

// Sample query 
char INSERT_SQL[] = "INSERT INTO `arduino_test`(`Humidity`, `Temperature`, `DateTime`) VALUES (60, 23, '2017-02-02 20:34:20')"; 

void setup() {
  ATSerial.begin(115200);
  // put your setup code here, to run once:
  while(ATSerial.read()>= 0) {}
  while(!ATSerial);

  DebugSerial.begin(115200);

  if (!SNIPE.lora_init()) {
    DebugSerial.println("SNIPE LoRa Initialization Fail!");
    while (1);
  }  

  /* SNIPE LoRa Set AppKey */
  if (!SNIPE.lora_setAppKey(lora_app_key)) {
    DebugSerial.println("SNIPE LoRa app key value has not been changed");
  }

  /* SNIPE LoRa Set Frequency */
  if (!SNIPE.lora_setFreq(LORA_CH_2)) {
    DebugSerial.println("SNIPE LoRa Frequency value has not been changed");
  }

  /* SNIPE LoRa Set Spreading Factor */
  if (!SNIPE.lora_setSf(LORA_SF_7)) {
    DebugSerial.println("SNIPE LoRa Sf value has not been changed");
  }

  /* SNIPE LoRa Set Rx Timeout 
   * If you select LORA_SF_12, 
   * RX Timout use a value greater than 5000  
  */
  /* SNIPE LoRa Set Rx Timeout */
  if (!SNIPE.lora_setRxtout(3000)) {
    DebugSerial.println("SNIPE LoRa Rx Timout value has not been changed");
  }    
    
  DebugSerial.println("Simple LoRa Gateway StartUP");

  // We start by connecting to a WiFi network

  DebugSerial.println();
  DebugSerial.println();
  DebugSerial.print("Connecting to ");
  DebugSerial.println(ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    DebugSerial.print(".");
  }

  DebugSerial.println("");
  DebugSerial.println("WiFi connected");
  DebugSerial.println("IP address: ");
  DebugSerial.println(WiFi.localIP());

  if (!client.connect(server, httpPort)) {
    DebugSerial.println("connection failed");
    return;
  }

  DebugSerial.print("connecting to Server");
}

int addr_loop = 1;
int retry_cnt = 0;
#define MAX_USER  12

void loop() {

  //char addr[2];
  //String msg;
  
  /* If communication is not successful, 
   * retry 5 times 
  */
  retry_cnt = 5;

  /* 15 configured nodes */
//  if(addr_loop > MAX_USER)
//    addr_loop = 1;
//
//  sprintf( addr, "%d", addr_loop );
//
//  //DebugSerial.println("send [Addr]: ");
//  //DebugSerial.print(addr_loop);
//  //DebugSerial.println("----------//");
//
//  addr_loop++;
//  
//  msg = (String)addr;

  //while(retry_cnt--)
  //{
  
        String ver = "GET /testlora/set_process.php?lon=4&lat=1&al=10 HTTP/1.0\r\n\r\n";
        
        client.print(ver);
        DebugSerial.println("yes");
  //}
 delay(1000);
}
