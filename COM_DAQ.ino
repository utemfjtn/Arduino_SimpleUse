void(* resetFunc) (void) = 0;
String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
double readVal0 = 0;
double readVal1 = 0;
double readVal2 = 0;
double readVal3 = 0;
double readVal4 = 0;
double readVal5 = 0;
double volConvert = 0.1282; //used for analog divider board. currently connect 680k and 100k resistor.
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
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
    } else if (inputString.indexOf("UPDATECONVERT") != -1){
      updateVolConvert();
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
  Serial.println("Reset,reset All values and strings");
  Serial.println("Stop,Stop Analog read");
  Serial.println("Start,Start Analog read with delay 250ms");
  Serial.println("UpdateConvert,eg:UpdateConvert:0.1234, update the analog divider(defalut:0.1282, 680k + 100k resistor) factor");
  Serial.println("HELP,Show all commands and samples.");
  inputString = "";
  stringComplete = false;
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
  volConvert = 0.1282; //used for analog divider board. currently connect 680k and 100k resistor.
  // clear the string:
  inputString = "";
  stringComplete = false;
  delay(5);
  Serial.print("Reseted All!volConvert:");
  Serial.println(volConvert);
  delay(5);
  resetFunc();
}
void getAllVoltage(){
    double val0 = 0;
    double val1 = 0;
    double val2 = 0;
    double val3 = 0;
    double val4 = 0;
    double val5 = 0;
    int acqloops = 10;
    int intdelay = 2;
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
      stringComplete = true;
    }
    delay(10);
  }
}
