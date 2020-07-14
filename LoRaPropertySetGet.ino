/*아두이노 로라 쉴드의 설정 값들을 알아보고, 설정할 수 있는 코드!!*/
/*일반 입출력 데이터 핀을 RX, TX핀으로 동작할 수 있게 해주는 라이브러리*/
#include <SoftwareSerial.h>
/*로라 쉴드를 사용하려면 다음의 라이브러리를 추가하자!
https://github.com/codezoo-ltd/SNIPE */
#include "SNIPE.h" 

//USB to TTL을 이용해서 디버그 메시지를 보기 위한 tx, rx 통신 핀 지정 
#define TXpin 11
#define RXpin 10

//로라 아두이노 쉴드와 아두이노 보드가 서로 통신하기 위해 Serial 클래스를 ATSerial 이란 이름으로 선언
#define ATSerial Serial

//로라 아두이노 쉴드끼리 통신 하는 암호 키 설정 <나만의 암호키 만들기!>
//16byte hex key
String lora_app_key = "11 22 33 44 55 66 77 88 99 aa bb cc dd ee ff 00";  

//USB to TTL을 이용해서 Debug를 시리얼에 나타내 디버그 메시지를 보기 위해 선언
//1) SoftwareSerial을 DebugSerial이란 이름으로 2)TXpin 11번 핀, RXpin 10번 핀으로 설정/선언(객체 생성)
SoftwareSerial DebugSerial(RXpin,TXpin);

/*SNIPE 객체 생성*/
//AT커맨드로 만들어진 로라 펌웨어를 아두이노 보드에서 동작시키기 위해 
SNIPE SNIPE(ATSerial);

void setup() { //전송 속도를 맞추고, 통신 초기에 쓰레기 값이 읽혀지는 것을 방지하기 위한 초기화 작업을 거침
  ATSerial.begin(115200); //아두이노 보드에서도 115200bps로 속도를 동일하게 설정해야 서로 통신할 수 있음.

  // put your setup code here, to run once:
  /* Serial을 안전하게 초기화하는 코드
통신할때 초기에 쓰레기값이 읽혀지는 것을 방지하기 위해 Serial값이 읽혀지지 않을때까지 읽어서 초기화합니다. 
Serial에 이상이 있어서 클래스가 선언되지 않았을 경우 프로그램을 더이상 진행 시키지 않습니다.*/
  while(ATSerial.read()>= 0) {}
  while(!ATSerial);

/*디버그 Serial 속도 설정: PC의 USB to TTL에 연결할 아두이노 보드의 Serial 속도를 115200bps로 설정했습니다. 
 PC의 테라텀 프로그램에서도 115200bps로 속도를 동일하게 설정해야 서로 통신할 수 있습니다.*/
  DebugSerial.begin(115200);

 /*로라 아두이노 쉴드 초기화 : 아두이노 보드에서 로라 아두이노 쉴드로 초기화 명령을 보내서 로라쉴드가 정상동작 하는지 확인 합니다.
정상동작 하지 않을 경우 10회 재시도한 후 그래도 동작되지않으면 프로그램을 더이상 진행시키지 않습니다.*/
  if (!SNIPE.lora_init()) {
    /* PC 디버그 화면에서 “ SNIPE LoRa Initialization Fail! ” 메시지가 출력될 경우, 로라 아두이노 쉴드와
      아두이노 보드를 분리 후 재결합 시켜보시기 바랍니다. 핀이 휘어져서 꼽혀있지 않거나 소켓에 덜 꼽
      혀 있어서 제대로 연결되지 않을 경우, 해당 메시지가 출력되는 경우가 있습니다 */ 
    DebugSerial.println("SNIPE LoRa Initialization Fail!");
    while (1);
  }  
  //초기화 작업이 무사히 끝나면 loop문 gogo
  DebugSerial.println("SNIPE LoRa StartUP");
}

void loop() { //아래의 get~ 함수는 반환 값이 모두 String이다. 
  DebugSerial.println("--- SNIPE LoRaPropertySetGet loop ---"); //Lora 특성값 (설정값)을 확인하고 셋팅함 

  DebugSerial.println(SNIPE.lora_getFreq()); //로라 주파수 반환
  delay(500);

  DebugSerial.println(SNIPE.lora_setFreq(LORA_CH_2)==true?"TRUE":"FALSE"); //로라 주파수를 채널2(922.1MHz)로 설정, 설정에 성공한다면 "TRUE" 출력
  delay(500);

  DebugSerial.println(SNIPE.lora_getTxp()); //로라 출력파워를 String으로 반환
  delay(500);

  DebugSerial.println(SNIPE.lora_setTxp(5)==true?"TRUE":"FALSE"); //출력 파워를 5dpm으로 설정, 설정에 성공한다면 "TRUE" 출력
  delay(500); //Tx Power는 0~14까지 설정 가능(단, 국내 전파법에 의해 채널1의 출력파워는 0~10까지만 허용) , TX Power: 거리를 측정해 자신의 위치를 추정하는데 사용
   
  DebugSerial.println(SNIPE.lora_getSf()); //SF(확산계수) 반환
  delay(500);

  DebugSerial.println(SNIPE.lora_setSf(LORA_SF_12)==true?"TRUE":"FALSE"); //SF(확산계수)를 12로 설정, 설정에 성공한다면 "TRUE" 출력
  delay(500);
  
  DebugSerial.println(SNIPE.lora_getRxtout()); //Rx Timeout(수신 대기 시간) 반환
  delay(500);

  DebugSerial.println(SNIPE.lora_setRxtout(1000)==true?"TRUE":"FALSE"); // 수신 대기 시간을 1000ms로 설정, 설정에 성공한다면 "TRUE" 출력
  delay(500); //(단, 0ms 이상) 설정한 시간동안 로라 아두이노 쉴드는 데이터가 수신되길 대기

  DebugSerial.println(SNIPE.lora_getTxtout()); //Tx Timeout(송신 대기 시간) 반환
  delay(500);

  DebugSerial.println(SNIPE.lora_setTxtout(1000)==true?"TRUE":"FALSE"); //로라 송신 대기 시간을 1000ms로 설정, 설정에 성공한다면 "TRUE" 출력
  delay(500); //(단, 0ms 이상 4000ms이하) 설정한 시간동안 로라 데이터를 송신


/*★로라 암호화/복호화 기능을 활성화 , 비활성화 할지 결정
LORA_AES_OFF일 경우 비활성화, LORA_AES_ON일 경우 활성화
로라 아두이노 쉴드는 기본값이 암호화/복호화 기능 사용으로 설정되어 있으므로 LORA_AES_OFF로 설정 했을 경우에만 원래대로
기능을 설정하는 용도로 사용합니다.*/
  DebugSerial.println(SNIPE.lora_setAESMode(LORA_AES_OFF)==true?"TRUE":"FALSE"); //★로라 암호화/복호화 기능 비활성화(미사용)
  delay(500); //활성화의 경우 LORA_AES_ON이라 설정하자! (테스트용 이외에는 암호화/복호화를 하는 것이 좋다)
  
  DebugSerial.println(SNIPE.lora_getAppKey()); //앱 키 반환
  delay(500);

  DebugSerial.println(SNIPE.lora_setAppKey(lora_app_key)==true?"TRUE":"FALSE"); // 앱 키를 위에 define한 값으로 설정, 설정에 성공한다면 "TRUE" 출력
  delay(500); // 각각 송수신할 로라 노드는 서로 동일한 앱 키가 지정되어야 한다! 다를 경우 서로를 찾지 못해 데이터를 주고 받지 못하기 때문!
      
  DebugSerial.println(SNIPE.lora_getVersion()); //어두이노 로라 모듈의 펌웨어 버전을 반환 (아마 1.1ver일것임!)
  delay(500);
}
