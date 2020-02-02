#include<SPIMemory.h>
#include <SPI.h>
#include <SD.h>

uint32_t strAddr;

#define BAUD_RATE 115200
#define FLASH 10
#define SDPIN  4

SPIFlash flash;
File myFile;

bool readSerialStr(String &inputStr);

void setup() {
  Serial.begin(BAUD_RATE);
  flash.begin(FLASH);
  SD.begin(SDPIN);
}

void loop() {
  randomSeed(analogRead(FLASH));
  strAddr = random(0, flash.getCapacity());
  String inputString = "";
  while (!Serial) {}
  while(!Serial.available()){}
  readSerialStr(inputString);
  flash.writeStr(strAddr, inputString);
  Serial.print(F("Written string: "));
  Serial.println(inputString);
  Serial.print(F("To address: "));
  Serial.println(strAddr);
  String outputString = "";
  if (flash.readStr(strAddr, outputString)) {
    Serial.print(F("Read string: "));
    Serial.println(outputString);
    Serial.print(F("From address: "));
    Serial.println(strAddr);
  }
  while (!flash.eraseSector(strAddr));
  Serial.print("Initializing SD card...");
  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");
  Serial.println("Creating FlashData.txt...");
  myFile = SD.open("FlashData.txt", FILE_WRITE);
  myFile.println(outputString);
  myFile.close();
  Serial.println("writing done");
  Serial.println("Reading the file");
  if (SD.exists("FlashData.txt")) {
    Serial.println("FlashData.txt exists.");
    myFile = SD.open("FlashData.txt");
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
  } else {
    Serial.println("FlashData.txt doesn't exist.");
  }
}

bool readSerialStr(String &inputStr) {
  if (!Serial)
    Serial.begin(115200);
  while (Serial.available()) {
    inputStr = Serial.readStringUntil('\n');
    Serial.println(inputStr);
    return true;
  }
  return false;
}
