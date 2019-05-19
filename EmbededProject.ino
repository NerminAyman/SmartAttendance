#include <SPI.h>
#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <LiquidCrystal.h>

#define SS_PIN 53
#define RST_PIN 5

int Con=20;
long startSession = 0;
long endSession = 0;
long durationOfSession=0;
unsigned long time0;
String attendanceNames[3] = {"Haidy","Nermin","Nada"};
float attendance[3][6]={{-9069,0,0,0,0,0},{-21781,0,0,0,0,0},{0,0,0,0,0,0}};
int times = 0;
int countOfPeople = 0;
bool sessionEnded = false;
bool studentIN = false;
int redLed = 12;
int greenLed = 11;
int buzzer = 10;
int smokeA0 = A5;

// Your threshold value
int sensorThres = 200;

LiquidCrystal lcd(7, 8, 13, 4, 3, 2);
MFRC522 rfid(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  
  analogWrite(6,Con);
  lcd.begin(16,2);
  
  pinMode(10, OUTPUT);
  analogWrite(10,128);
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(smokeA0, INPUT);
  Serial.begin(9600);
}

void loop() {
  int analogSensor = analogRead(smokeA0);
  
  // Checks if it has reached the threshold value of the smoke sensor
  if (analogSensor > sensorThres)
  {
    digitalWrite(redLed, HIGH);
    digitalWrite(greenLed, LOW);
    digitalWrite(buzzer,HIGH);
  }else{
    digitalWrite(buzzer,LOW);
    lcd.setCursor(0,0);
    lcd.print("Number of Students = ");
    lcd.print(countOfPeople);
   // Serial.print("Number of Students = ");
   // Serial.print(countOfPeople);
    lcd.scrollDisplayLeft();
    lcd.setCursor(0,1);
    digitalWrite(redLed, LOW);
    digitalWrite(greenLed, HIGH);
  }
  delay(100);

  //Readin from RFID
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
    return;
 
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);

  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K){
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }


  String strID = "";
  for (byte i = 0; i < 4; i++) {
    strID +=
    (rfid.uid.uidByte[i] < 0x10 ? "0" : "") +
    String(rfid.uid.uidByte[i], DEC);
  }

  digitalWrite(greenLed, LOW);
  digitalWrite(redLed, HIGH);
  delay(1000);
  digitalWrite(redLed, LOW);
  digitalWrite(greenLed, HIGH);
  
  int studentID = strID.toInt();
  strID.toUpperCase();
  time0 = millis();
  delay(1000);
  //Serial.print("Tap card key: ");
  //Serial.println(strID);
//  Serial.println(strID);
  
  //condition check instructor's entrance and exit
  if(strID.equals("1920155326") && times == 0){
    startSession = millis();
  //  Serial.print("Session starts at: ");
   // Serial.println(startSession);
    times = 1;
  }else{
    if(strID.equals("1920155326")){
      times = 0;
      endSession = millis();
      durationOfSession=endSession-startSession;
     // Serial.print("Session ends at: ");
     // Serial.println(endSession);
     // Serial.print("duration: ");
     // Serial.println(durationOfSession);
      sessionEnded=true;
    }
  }

  //Condition check the student's entrance and exit
  if(!(strID.equals("1920155326"))){
    for(int i = 0; i<3; i++){
     // Serial.println(studentID == attendance[i][0]);
     // Serial.println(times == 1 );
     // Serial.println(endSession == 0);
      if(studentID == attendance[i][0] && times == 1 && endSession == 0){
         if(attendance[i][5] == 0){
            attendance[i][1] = millis();
       //     Serial.println("Start Student: ");
       //     Serial.println(attendance[i][1]);
            countOfPeople = countOfPeople + 1;
            attendance[i][5] = 1;
          }else{
              attendance[i][2] = millis();
         //     Serial.println("End Student: ");
         //     Serial.println(attendance[i][2]);
              attendance[i][3]= attendance[i][3] + (attendance[i][2]-attendance[i][1]);
         //    Serial.print("duration of student");
         //     Serial.println(attendance[i][3]);
              countOfPeople = countOfPeople - 1;
              attendance[i][5] = 0;
          }
      }
    }
  }
  //Serial.print("Tap card key: ");
  //Serial.println(studentID);
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();

  //Condition checks if session ends
  if (sessionEnded){
    for(int i = 0; i<3; i++){
      if(attendance[i][5] == 1){
        attendance[i][2] = endSession;
        attendance[i][3]= attendance[i][3] + (attendance[i][2]-attendance[i][1]);
        //Serial.print("duration of student");
        //Serial.println(attendance[i][3]);
        attendance[i][5] = 0;

      }
      if((attendance[i][3]/durationOfSession)>0.75){
       attendance[i][4]=1;
       Serial.print(attendanceNames[i]);
       Serial.print(" ");
       Serial.print(attendance[i][0]);
       Serial.println(" Attended");
      }else{
       Serial.print(attendanceNames[i]);
       Serial.print(" ");
       Serial.print(attendance[i][0]);
       Serial.println(" Absent");
      }
    }
    countOfPeople = 0;
    sessionEnded = false;
  }    
}
