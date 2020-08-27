/*형변환이 잘되는지 체크하는 코드*/
//그냥 >>우노<<보드에 슥삭 테스트만!

void setup() {
  Serial.begin(9600);
  
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
  
  Serial.println(send); //marsvest/25/45 가 출력되면 성공
}

void loop() {
}