void(* resetFunc) (void) = 0;
String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
double readVal0 = 0;
double readVal1 = 0;
double readVal2 = 0;
double readVal3 = 0;
double readVal4 = 0;
double readVal5 = 0;
double volConvert = 0.135447; //used for analog divider board. currently(0.135447) connect 300k and 47k resistor.
double VolOffset = 0.5;
const int di1 = 2;
const int di2 = 3;
const int di3 = 4;
const int do1 = 5;
const int do2 =  6;
const int do3 =  7;
const int do4 =  8;
const int do5 =  9;
const int do6 =  10;
const int do7 =  11;   
const int do8 =  12;
int lastdi1;
int lastdi2;
int lastdi3;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(di1, INPUT_PULLUP);
  pinMode(di2, INPUT_PULLUP);
  pinMode(di3, INPUT_PULLUP);
  pinMode(do1, OUTPUT);
  pinMode(do2, OUTPUT);
  pinMode(do3, OUTPUT);
  pinMode(do4, OUTPUT);
  pinMode(do5, OUTPUT);
  pinMode(do6, OUTPUT);
  pinMode(do7, OUTPUT);
  pinMode(do8, OUTPUT);
  lastdi1 = 0;
  lastdi2 = 0;
  lastdi3 = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
  if (stringComplete) {
    //Serial.println(inputString);
    if (inputString.indexOf("RESET") != -1){
      resetAll();
    } else if (inputString.indexOf("STOP") != -1){
      // clear the string:
      inputString = "";
      stringComplete = false;
      Serial.println("Stop,Stop Analog read");
      delay(5);
    } else if (inputString.indexOf("HELP") != -1){
      showHelp();
    } else if (inputString.indexOf("DIEND") != -1){
      inputString = "";
      stringComplete = false;
      Serial.println("DIEnd, DILoop Stoped!");
    } else if (inputString.indexOf("DISINGLE") != -1){
      getDIs(true);
      inputString = "";
      stringComplete = false;
    } else if (inputString.indexOf("DILOOP") != -1){
      getDIs(false);
    } else if (inputString.indexOf("UPDATECONVERT") != -1){
      updateVolConvert();
    } else if (inputString.indexOf("DO") != -1){
      writeDO();
    } else if (inputString.indexOf("START") != -1){
      getAllVoltage();
      delay(250);
      stringComplete = true;
    } else {
      getAllVoltage();
      // clear the string:
      inputString = "";
      stringComplete = false;
      delay(5);
    }
  }
}
void showHelp(){
  //HELP
  Serial.println("Created by YAN.Qian at 20210619. Version 1.00");
  Serial.println("Reset,reset All values and strings");
  Serial.println("Stop,Stop Analog read");
  Serial.println("Start,Start Analog read with delay 250ms");
  Serial.println("UpdateConvert,eg:UpdateConvert:0.135447, used for analog divider board. currently(0.135447) connect 300k and 47k resistor.");
  Serial.println("DISingle,Get DI once,1 is true, 0 is false");
  Serial.println("DILoop,Get DI return every 10ms,1 is true, 0 is false");
  Serial.println("DIEnd,Stop Get DI, pin 2-4 means di 1-3.");
  Serial.println("DO,pin 5-12 means do 1-8.eg:DO:1,1; or DO:1,0; 1 is true, 0 is false");
  Serial.println("HELP,Show all commands and samples.");
  inputString = "";
  stringComplete = false;
}
void writeDO(){
  int pos = inputString.indexOf("DO");
  int ch = inputString.substring(pos+3,pos+4).toInt();
  bool val;
  if(inputString.substring(pos+5,pos+6)=="1"){
    val = true;
  } else{
    val = false;
  }
  switch (ch) {
    case 1:
        digitalWrite(do1, val);
      break;
    case 2:
        digitalWrite(do2, val);
      break;
    case 3:
        digitalWrite(do3, val);
      break;
    case 4:
        digitalWrite(do4, val);
      break;
    case 5:
        digitalWrite(do5, val);
      break;
    case 6:
        digitalWrite(do6, val);
      break;
    case 7:
        digitalWrite(do7, val);
      break;
    case 8:
        digitalWrite(do8, val);
      break;
    default:
      // statements
      break;
  }
  Serial.print("Do Write,Ch:");
  Serial.print(ch);
  Serial.print(",value:");
  Serial.println(val);
  // clear the string:
  inputString = "";
  stringComplete = false;
  delay(5);
}
void getDIs(bool single){
  int distatus1 = digitalRead(di1);
  int distatus2 = digitalRead(di2);
  int distatus3 = digitalRead(di3);
  if (single || lastdi1 != distatus1 || lastdi2 != distatus2 || lastdi3 != distatus3){
    lastdi1 = distatus1;
    lastdi2 = distatus2;
    lastdi3 = distatus3;
    Serial.print("DI:");
    Serial.print(distatus1);
    Serial.print(",");
    Serial.print(distatus2);
    Serial.print(",");
    Serial.print(distatus3);
    Serial.println(";");
  }
  delay(10);
}
void updateVolConvert(){
  volConvert = inputString.substring(inputString.indexOf("UpdateConvert")+14).toDouble();
  if (volConvert == 0.00 ){
    volConvert = 1.00;
  }
  Serial.print("Update Convert to ");
  Serial.println(volConvert);
  // clear the string:
  inputString = "";
  stringComplete = false;
  delay(5);
}
void resetAll(){
  readVal0 = 0;
  readVal1 = 0;
  readVal2 = 0;
  readVal3 = 0;
  readVal4 = 0;
  readVal5 = 0;
  // clear the string:
  inputString = "";
  stringComplete = false;
  delay(5);
  Serial.print("Reseted All!volConvert:");
  Serial.println(volConvert);
  delay(5);
  digitalWrite(do1, false);
  digitalWrite(do2, false);
  digitalWrite(do3, false);
  digitalWrite(do4, false);
  digitalWrite(do5, false);
  digitalWrite(do6, false);
  digitalWrite(do7, false);
  digitalWrite(do8, false);
  resetFunc();
}
void getAllVoltage(){
    delay(50);
    double val0 = 0;
    double val1 = 0;
    double val2 = 0;
    double val3 = 0;
    double val4 = 0;
    double val5 = 0;
    int acqloops = 50;
    int intdelay = 1;
    int k;    // counter variable
    for (k = 0; k < acqloops; k++)
    {
      val0 += analogRead(A0);    // read the input pin
      delay(intdelay);
      val1 += analogRead(A1);    // read the input pin
      delay(intdelay);
      val2 += analogRead(A2);    // read the input pin
      delay(intdelay);
      val3 += analogRead(A3);    // read the input pin
      delay(intdelay);
      val4 += analogRead(A4);    // read the input pin
      delay(intdelay);
      val5 += analogRead(A5);    // read the input pin
      delay(intdelay);
    }
    double adc = 0.0049;//fixed adc from 5v 10 bit.   
    readVal0 = (adc * val0 / acqloops) / volConvert; 
    readVal1 = (adc * val1 / acqloops) / volConvert; 
    readVal2 = (adc * val2 / acqloops) / volConvert; 
    readVal3 = (adc * val3 / acqloops) / volConvert; 
    readVal4 = (adc * val4 / acqloops) / volConvert;  
    readVal5 = (adc * val5 / acqloops) / volConvert; 
    if (readVal0 >= VolOffset + 0.5) readVal0 += VolOffset;
    if (readVal1 >= VolOffset + 0.5) readVal1 += VolOffset;
    if (readVal2 >= VolOffset + 0.5) readVal2 += VolOffset;
    if (readVal3 >= VolOffset + 0.5) readVal3 += VolOffset;
    if (readVal4 >= VolOffset + 0.5) readVal4 += VolOffset;
    if (readVal5 >= VolOffset + 0.5) readVal5 += VolOffset;
    Serial.print(readVal0);
    Serial.print(";");
    Serial.print(readVal1);
    Serial.print(";");
    Serial.print(readVal2);
    Serial.print(";");
    Serial.print(readVal3);
    Serial.print(";");
    Serial.print(readVal4);
    Serial.print(";");
    Serial.println(readVal5);
}


/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    inputString.toUpperCase();
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      Serial.print("inputString:" + inputString);
      stringComplete = true;
    }
    delay(5);
  }
}
