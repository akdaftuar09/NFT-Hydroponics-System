#define SensorPin A3
#define Offset 0.00
#define samplingInterval 20
#define printInterval 800
#define ArrayLenth 40
#include <EEPROM.h>
#include "GravityTDS.h" 
#define TdsSensorPin A1
GravityTDS gravityTds;
float temperature = 25,tdsValue = 0,EC;
int pHArray[ArrayLenth]; 
int pHArrayIndex = 0;
int countTrueCommand;
int countTimeCommand;
boolean found = false;  

float phVal() {
  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  static float pHValue, voltage;
  if(millis()-samplingTime > samplingInterval) {
    pHArray[pHArrayIndex++] = analogRead(SensorPin);
    if(pHArrayIndex == ArrayLenth) {
      pHArrayIndex = 0;
    }
    voltage = avergearray(pHArray, ArrayLenth)*5.0/1024;
    pHValue = 3.5*voltage+Offset+3;
    samplingTime = millis();
  }
  if(millis()-printTime > printInterval) {
    Serial.print("Voltage:");
    Serial.print(voltage,2);
    Serial.print("    pH value: ");
    Serial.println(pHValue,2);
    printTime = millis();
  }
  return pHValue;
}

double avergearray(int* arr, int number) {
  int i;
  int max,min;
  double avg;
  long amount = 0;
  if(number <= 0) {
    Serial.println("Error number for the array to avraging!/n");
    return 0;
  }
  if(number < 5) {
    for(i = 0; i < number; i++) {
      amount += arr[i];
    }
    avg = amount/number;
    return avg;
  } 
  else {
    if(arr[0] < arr[1]) {
      min = arr[0];
      max = arr[1];
    }
    else {
      min = arr[1];
      max = arr[0];
    }
    for(i = 2; i < number; i++) {
      if(arr[i] < min) {
        amount += min;    
        min = arr[i];
      } 
      else {
        if(arr[i] > max) {
          amount += max; 
          max = arr[i];
        } 
        else {
          amount += arr[i];
        }
      }
    }
    avg = (double)amount/(number-2);
  }
  return avg;
}

void setup() {
  Serial.begin(9600);
   gravityTds.setPin(TdsSensorPin);
   gravityTds.setAref(5.0);  //reference voltage on ADC, default 5.0V on Arduino UNO
   gravityTds.setAdcRange(1024);  //1024 for 10bit ADC;4096 for 12bit ADC
   gravityTds.begin();  //initialization
}

void loop() {
  delay(1000);
  int valSensorPH = phVal();
  //temperature = readTemperature();  //add your temperature sensor and read it
    gravityTds.setTemperature(temperature);  // set the temperature and execute temperature compensation
    gravityTds.update();  //sample and calculate
    tdsValue = gravityTds.getTdsValue();  // then get the value
    EC=tdsValue*2/1000;
    Serial.print("EC Value is :");
    Serial.println(EC);
  }
