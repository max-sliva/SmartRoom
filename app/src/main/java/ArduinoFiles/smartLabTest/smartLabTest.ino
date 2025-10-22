int vent1 = 3;
int vent2 = 4;
int motor1 = 5;
int motor2 = 6;
int led = 7;
int windowOpenedVal = 30, windowClosedVal = 900, windowCurVal;
bool windowIsOpened = false;
//bool testMode = true;
//#define DEBUG_MODE

#ifdef DEBUG_MODE
  #define curSerial Serial
  #define mode "TestMode"
#else
  #define curSerial Serial1
  #define mode "WorkMode"
#endif

void windowWork(String key, String val){
  //сделать if для двух окон
  if (val=="open" && !windowIsOpened){
    
    if(windowCurVal > windowOpenedVal){
      
    }
  }
}

void doCommand(String key, String val){
  if (key=="vent"){
    digitalWrite(vent1,(val=="on")? HIGH : LOW);
  } else if (key=="windowLeft"){
    windowWork(key, val);
  } else if (key=="windowRight"){
    windowWork(key, val);
  } else if (key=="door"){
    
  } else if (key=="led"){
    analogWrite(led,val.toInt());
  }  
}
    
void setup(){
  pinMode(led, OUTPUT);
  pinMode(motor1, OUTPUT);
  pinMode(motor2, OUTPUT);
  pinMode(vent1, OUTPUT);
  pinMode(vent2, OUTPUT);
  digitalWrite(vent2, LOW);
  digitalWrite(vent1, LOW);
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial.println(mode);
  windowCurVal = analogRead(A0);
  Serial.print("windowCurVal");
  Serial.print(" : ");
  Serial.println(windowCurVal);
}
void loop(){
  if (curSerial.available()){
//    String str = curSerial.readString();
    String key = curSerial.readStringUntil(':');
    String val = curSerial.readStringUntil('\n');
    
    Serial.print(key);
    Serial.print(" : ");
    Serial.println(val);
    Serial.println();
    doCommand(key, val);
  }
  windowCurVal = analogRead(A0);
  if (windowCurVal < windowClosedVal) windowIsOpened = true;
  else  windowIsOpened = fals;
}
