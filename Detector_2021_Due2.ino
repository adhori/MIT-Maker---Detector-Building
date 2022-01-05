//import libraries

//These constants will not change during operation of the device
//  LEDs
const int rledPin = 2;
const int bledPin = 3;
const int gledPin = 4;
// LED salinity ranges

const int rledMin = 0;       //min r temp value in ppm
const int rledMax = 26;       //max r temp value in ppm
const int gledMin = 0;       //min g temp value in ppm
const int gledMax = 51;       //max g temp value in ppm
const int bledMin = 0;       //min b temp value in ppm
const int bledMax = 75;       //max b temp value in ppm
//  calc
int analogInput = 0;
float vout = 0.0;
float vin = 0.0;
float R1 = 4700.0; // resistance of R1 (100K) -see text!
float R2 = 4700.0; // resistance of R2 (10K) - see text!
int value = 0;


//conductivity shit
const int swtichPin = 6; // this sets a constant called swiconcentrationhPin to analog pin 6 on the Arduino (for optional LCD display)
int swiconcentrationhState = 0; //this sets the integer called swiconcentrationhState to 0 (or low)
int condVal; //this sets an integer called condVal which is used in calculations below

//void setup{} {
  //Serial.begin(9600); //establishes a connection to the Arduino at a baud rate of 9600
  //lcd.begin
//}

void setup() 
{
  pinMode(analogInput, INPUT);
  pinMode(rledPin,  OUTPUT);  //set rled pin to output mode
  pinMode(gledPin,  OUTPUT);  //set gled pin to output mode
  pinMode(bledPin,  OUTPUT);  //set bled pin to output mode
  //analogReadResolution(12);
  Serial.begin(9600);         //begins output to serial monitor
}

void loop() {
  condVal = analogRead(A0); //sets the integer condVal equal to value read from A0
  vout = (condVal * 3.30) / 1024.0; // see text
  vin = vout / (R2/(R1+R2));
  //As conductivity increases, voltage drop across probe also increases
  float concentration = vin*0.55; // converts EC to ppm of NaCl
  Serial.print("\n");
  Serial.print("Concentration: ");
  Serial.print(concentration, 4);
  Serial.print(" ppm NaCl      ");
  Serial.print("Voltage: ");
  Serial.print(condVal, 5);

  if (  (rledMin <= concentration) &&  (concentration <= rledMax)  ) {                           
      digitalWrite(rledPin, HIGH);      //turns on rled pin
    }
                
    else {                                //turns rled off if temp is not in correct range
      digitalWrite(rledPin, LOW);        
    }
                
    if (  (gledMin <= concentration) &&  (concentration <= gledMax)  ) {   
      digitalWrite(gledPin, HIGH);      //turns on gled pin
    }
              
    else {                                //turns gled off if temp is not in correct range
      digitalWrite(gledPin, LOW);
    }
                
    if (  (bledMin <= concentration) &&  (concentration <= bledMax)  ) { 
      digitalWrite(bledPin, HIGH);      //turns on bled pin
    }
              
    else {                                //turns bled off if temp is not in correct range
      digitalWrite(bledPin, LOW);
    }
}
