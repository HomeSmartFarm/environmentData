#include <MsTimer2.h>
#include <SPI.h>        // SPI 라이브러리 추가
#include <SD.h>         // SD 라이브러리 추가
#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);
File myFile;

int cnt=0;      //카운트값
const int cs=4; //sd카드리더기핀
const int bt=10;   //버튼핀

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  dht.begin();
  
  pinMode(bt, INPUT_PULLUP);

  //MsTimer2::set(5000, writeData);   //test
  MsTimer2::set(3600000, writeData);  //1시간
  MsTimer2::start();
  
  if (!SD.begin(cs)) {                           // SD 카드 초기화 및 연결 
    Serial.println("초기화 실패!!!");            // SD 카드 연결 실패시 
    return;
  }
  Serial.println("초기화 성공!!!");

}
void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(bt)==LOW){
    writeHarvest();
    delay(300);
  }
  
}

void writeHarvest(){
  Serial.println("수확적기 실행");
  myFile = SD.open("data1.txt", FILE_WRITE);      // 파일 test.txt 열기(파일 없으면 생성) 및 쓰기 설정 

  if (myFile) {                                  // 파일이 성공적으로 열렸는지 검사
    Serial.print("파일 작성중");     
    myFile.println("수확");                         // 파일 닫기
    myFile.close();
    Serial.println("파일 작성완료");
  }else {
    Serial.println("error opening test.txt");    // 파일 오픈에 실패하면 출력
  }
}
void writeData(){
  Serial.println("타이머인터럽트실행");
  myFile = SD.open("data1.txt", FILE_WRITE);      // 파일 test.txt 열기(파일 없으면 생성) 및 쓰기 설정 
  float t = dht.readTemperature();
  float h = dht.readHumidity();


  
  if (myFile) {                                  // 파일이 성공적으로 열렸는지 검사
    Serial.println("파일 작성중"); 
    if(!isnan(t) && !isnan(h)){
      Serial.print("Humidity: ");
      Serial.print(h);
      Serial.print("%  Temperatuer: ");
      Serial.print(t);
      Serial.println(" *C");
      myFile.print("H: ");
      myFile.print(h);
      myFile.print("%   T: ");
      myFile.print(t);
      myFile.print("*C\n");
      myFile.close();                              // 파일 닫기
      Serial.println("파일 작성완료");
      
    
    }else {
      Serial.println("온습도읽기실패");
    }
    
    
  } else {
    Serial.println("error opening test.txt");    // 파일 오픈에 실패하면 출력
  }


}
