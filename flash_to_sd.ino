#include<SPIMemory.h>
#include <SPI.h>
#include <SD.h>

uint32_t strAddr;

#define BAUD_RATE 9600
#define FLASH 10
#define SDPIN 9

SPIFlash flash;
File myFile;

bool readSerialStr(String &inputStr);

void setup() {
  pinMode(FLASH, OUTPUT);
  pinMode(SDPIN, INPUT);
  SPI.begin();
  Serial.begin(BAUD_RATE);
  flash.begin(FLASH);
  SD.begin(SDPIN);
  Serial.println("enter 1,2,3,4");
}

void loop() {
 String inputString = "";
 String outputString = "";
 String input;
 String Address;
 digitalWrite(FLASH, HIGH);
 digitalWrite(SDPIN, HIGH);
  while(!Serial.available()){}
  if(Serial.available()>0){
    readSerialStr(input);
    input.trim();
    //input = input -'0';
    Serial.println(input);
    int decide = input.toInt();
    Serial.println(decide);
    switch(decide){
      case 1: Serial.println("Writting mode on");
                digitalWrite(FLASH, LOW);
                randomSeed(analogRead(FLASH));
                Serial.print("enter the Address value from 0 to ");
                Serial.println(flash.getCapacity());
                while(!Serial.available()){}
                  readSerialStr(Address);
                  Address.trim();
                  strAddr = Address.toInt();
                //strAddr = random(0, flash.getCapacity());
                Serial.println("enter something");
                while (!Serial) {}
                while(!Serial.available()){}
                readSerialStr(inputString);
                flash.writeStr(strAddr, inputString);
                Serial.print(F("Written string: "));
                Serial.println(inputString);
                Serial.print(F("To address: "));
                Serial.println(strAddr);
                digitalWrite(FLASH, HIGH);
                break;
        case 2:   Serial.println("Reading Mode on");
                    digitalWrite(FLASH, LOW);
                    while(!Serial.available()){}
                    readSerialStr(Address);
                    Address.trim();
                    strAddr = Address.toInt();
                    if (flash.readStr(strAddr, outputString)) {
                      Serial.print(F("Read string: "));
                      Serial.println(outputString);
                      Serial.print(F("From address: "));
                      Serial.println(strAddr);
                    }
                   // while (!flash.eraseSector(strAddr));
                    digitalWrite(FLASH, HIGH);
                    break;
         case 3:  Serial.println("Writing mode in SD ON");   
                  digitalWrite(SDPIN, LOW);
                    Serial.println(outputString);
                    Serial.print("Initializing SD card...");
                    if (!SD.begin(SDPIN)) {
                      Serial.println("initialization failed!");
                      break;
                    }
                    else{
                    Serial.println("initialization done.");
                    Serial.println("Creating example.txt...");
                      myFile = SD.open("example.txt", FILE_WRITE);
                      myFile.println(outputString);
                      myFile.close();
                    Serial.println("writing done");
                    digitalWrite(SDPIN, HIGH);
                    break;
                    }
         case 4:  Serial.println("Reading mode in SD ON");
                  digitalWrite(SDPIN, LOW);
                  if (!SD.begin(SDPIN)) {
                      Serial.println("initialization failed!");
                      break;
                    }
                    else{
                       if (SD.exists("example.txt")) {
                          Serial.println("example.txt exists.");
                          myFile = SD.open("example.txt");
                          while (myFile.available()) {
                            Serial.write(myFile.read());
                          }
                        } else {
                          Serial.println("example.txt doesn't exist.");
                        }
                    }
                    digitalWrite(SDPIN, HIGH);
                    break;
                  
        default: Serial.println("Enter 1 for Writing in flash and 2 for reading from flash and 3 for writing in SD and 4 Reading from SD");
                  break;
    }
  }
}

bool readSerialStr(String &inputStr) {
  if (!Serial)
    Serial.begin(BAUD_RATE);
  while (Serial.available()) {
    inputStr = Serial.readStringUntil('\n');
    Serial.println(inputStr);
    return true;
  }
  return false;
}
