#include <Adafruit_INA260.h>

Adafruit_INA260 ina260 = Adafruit_INA260();

bool debug = false;
float maxVoltageThreshold = 12520;
float minCurrentThreshold = 50;
int maxIntervalCheck = 300; //in seconds
int numIntervals=maxIntervalCheck+1;

 enum : byte {CHARGING,CHARGED};
auto state = CHARGED;

void setup() {
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(A2, OUTPUT);

  // Wait until serial port is opened max 500 ms
  int startupDelay = 0;
  int startupDelayMax = 500;
  while (!Serial || startupDelay < startupDelayMax) { 
    delay(10); 
    startupDelay += 10;
  }

  
  
  Serial.println("Adafruit INA260 Test");

  if (!ina260.begin()) {
    Serial.println("Couldn't find INA260 chip");
    while (1);
  }
  Serial.println("Found INA260 chip");
}

void loop() {
  if(state==CHARGING){
    
    if( ina260.readBusVoltage() >= maxVoltageThreshold && ina260.readCurrent() < minCurrentThreshold){
      digitalWrite(A2, LOW);
      digitalWrite(LED_BUILTIN, LOW); 
      Serial.println("off");
      state = CHARGED;
    }

  }

  if(state == CHARGED){

    if(numIntervals>= maxIntervalCheck){
      numIntervals = 0;
      digitalWrite(A2, HIGH);
      delay(2000);
      Serial.println("timeout test on");
      if( ina260.readBusVoltage() >= maxVoltageThreshold*0.95 && ina260.readCurrent() < minCurrentThreshold){
        digitalWrite(A2, LOW);
        digitalWrite(LED_BUILTIN, LOW); 
        Serial.println("timeout test off");
      }
      else{
        Serial.println("timeout test on");
        state = CHARGING;
      }
      
    }
  }

  if(debug){
    Serial.print("Current: ");
    Serial.print(ina260.readCurrent());
    Serial.println(" mA");

    Serial.print("Bus Voltage: ");
    Serial.print(ina260.readBusVoltage());
    Serial.println(" mV");

    Serial.print("Power: ");
    Serial.print(ina260.readPower());
    Serial.println(" mW");

    Serial.println();
  }
  delay(1000);
  numIntervals++;
}
