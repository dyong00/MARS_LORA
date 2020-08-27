#include <SoftwareSerial.h>
#include "SNIPE.h"

#define PING  1
#define PONG  2

#define CODE  PING    /* Please define PING or PONG */

#define TXpin D9
#define RXpin D8
#define ATSerial Serial

//16byte hex key
String lora_app_key = "11 22 33 44 55 66 77 88 99 aa bb cc dd ee ff 00";  

SoftwareSerial DebugSerial(RXpin,TXpin);
SNIPE SNIPE(ATSerial);

void setup() {
  ATSerial.begin(115200);

  // put your setup code here, to run once:
  while(ATSerial.read()>= 0) {}
  while(!ATSerial);

  DebugSerial.begin(115200);

  /* SNIPE LoRa Initialization */
  if (!SNIPE.lora_init()) {
    DebugSerial.println("SNIPE LoRa Initialization Fail!");
    while (1);
  }

  /* SNIPE LoRa Set Appkey */
  if (!SNIPE.lora_setAppKey(lora_app_key)) {
    DebugSerial.println("SNIPE LoRa app key value has not been changed");
  }
  
  /* SNIPE LoRa Set Frequency */
  if (!SNIPE.lora_setFreq(LORA_CH_1)) {
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
  if (!SNIPE.lora_setRxtout(5000)) {
    DebugSerial.println("SNIPE LoRa Rx Timout value has not been changed");
  }  
    
  DebugSerial.println("SNIPE LoRa PingPong Test");
}

void loop() {
  
  /*필요 변수 생성*/
  double lat; //위도 <- 뭔가 소수점이 나올 수도 있을것같아서 실수형으로 해놨어요! 한조 코드 보고 자료형 수정 가능!
  double lon; //경도
  String send = "marsvest/"; //전달할 문자열 (드론과 조끼 구분을 위한 상위 구분자 기본 초기화)
  
  /*gps 값 초기화*/
  //위도와 경도 저장 (위도 경도 값을 반환하는 함수를 생성해 사용하면 더 간단해질듯! <- 한조에게 말하기)
  lat = 25; //일단 임의의 값 저장
  lon = 45;
  
  /*전달할 문자열 생성*/
  send.append("/"+Double.toString(lat)); //+위도 
  send.append("/"+Double.toString(lon)); //+경도
  
#if CODE == PING //조끼
        if (SNIPE.lora_send(send)) //최종 문자열을 전달
        {
          DebugSerial.println("send success");
          
          /* 다른 보드로부터 받는 부분 / 조끼는 보내기만 하므로 주석 처리함
          String ver = SNIPE.lora_recv();
          DebugSerial.println(ver);

          if (ver != "AT_RX_TIMEOUT" && ver != "AT_RX_ERROR")
          {
            DebugSerial.println("recv success");
            DebugSerial.println(SNIPE.lora_getRssi());
            DebugSerial.println(SNIPE.lora_getSnr());            
          }
          else
          {
            DebugSerial.println("recv fail");
            delay(500);
          }*/
        }
       delay(1000);
       
#elif CODE == PONG //드론
        String ver = SNIPE.lora_recv();
        delay(300);

        DebugSerial.println(ver);
        
        if (ver != "AT_RX_TIMEOUT" && ver != "AT_RX_ERROR")
        {
          DebugSerial.println("recv success");
          DebugSerial.println(SNIPE.lora_getRssi());
          DebugSerial.println(SNIPE.lora_getSnr());

          if(SNIPE.lora_send("lora pong"))
          {
            DebugSerial.println("send success");
          }
          else
          {
            DebugSerial.println("send fail");
            delay(500);
          }
        }
       delay(1000);
#endif
}