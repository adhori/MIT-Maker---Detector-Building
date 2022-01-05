//These constants will not change during operation of device

const int tempPin = A0;       //pin the thermistor is attached to     THIS NEEDS TO BE AN ANALOG PIN!!!!!!!1!!!
const int rledPin = 2;        //pin the red led is attached to        digital pins
const int gledPin = 4;        //pin the green led is attached to
const int bledPin = 7;        //pin the blue led is attached to
float voltIn = 1023.0;    //voltage provided to thermistor 5=1023.0 
const int numReadings = 20;       //number of readings to take the average of
const int delayTotal = 100;   //delay in milliseconds

// LED temperature ranges

const int rledMin = -15;       //min r temp value in celsius
const int rledMax = 28.3;       //max r temp value in celsius
const int gledMin = 28.4;       //min g temp value in celsius
const int gledMax = 39.7;       //max g temp value in celsius
const int bledMin = 39.7;       //min b temp value in celsius
const int bledMax = 200;       //max b temp value in celsius

// variables; these are changed to calculate the voltage and temperature



int Vo;                       //voltage between thermistor and known resistor
float R1 = 10000;             //Known resistor value 
float lnR2, R2, T, tC;       //defines variables; sets them to be floating point values
                              //R2 = calculated resistance of thermistor
                              //T  = calculated temperature in kelvin
                              //tC = calculated temperature in celsius
float A = 1.009249522e-03;    //Steinhart-Hart coefficients; vary based on the type and model of thermistor and the temp range of interest
float B = 2.378405444e-4; 
float C = 2.019202697e-07; 

//array variables and shit

int readings[numReadings];        //array of readings from the thermistor
int readIndex = 0;
int total = 0;
float average = 0;
float vAverage;

void setup() {
  
  pinMode(rledPin,  OUTPUT);  //set rled pin to output mode
  pinMode(gledPin,  OUTPUT);  //set gled pin to output mode
  pinMode(bledPin,  OUTPUT);  //set bled pin to output mode
  
  Serial.begin(9600);         //begins output to serial monitor

  for (int thisReading = 0; thisReading < numReadings; thisReading++) {readings[thisReading] = 0;

  }
  
}

void loop() {

   total = total - readings[readIndex];       //subtract the last reading

   readings[readIndex] = analogRead(tempPin); //take readings from the sensor

   total = total + readings[readIndex];       //add the reading to the total

   readIndex = readIndex + 1;                 //advance to the next position in the array

   if (readIndex >= numReadings) {            //if at end of array

      readIndex = 0;                          //wrap around to the beginning
    
   }

    average = total / numReadings;      //calculate the average

    
               
      if (readIndex % 10 == 0)  {         //if readIndex is a multiple of 10, calculate the temperature
        
        
              R2 = ((voltIn * R1 / (float)average) - R1);              //calculates resistance of thermistor using voltage divider equation
                
              lnR2 = log(R2);                                     //designates log(R2) as a variable
                
              T = (1.0 / (A + B*lnR2 + C*lnR2*lnR2*lnR2));        //calculates the temp in kelvin using the Steinhart-Hart equation
                
              tC = T - 270.35;                                    //changes the temp in kelvin to celsius


                Serial.print(readIndex);
                Serial.print("\n");
                Serial.print("Average");
                Serial.print(average);
                Serial.print("\n");
                Serial.print("Temperature: ");                    //displays the temperature in the serial output
                Serial.print(tC);
                Serial.println(" C"); 

                  
                       if (  (rledMin <= tC) &&  (tC <= rledMax)  ) {                           
                    digitalWrite(rledPin, HIGH);      //turns on rled pin
                }
                
                else {                                //turns rled off if temp is not in correct range
                   digitalWrite(rledPin, LOW);        
                }
                
                if (  (gledMin <= tC) &&  (tC <= gledMax)  ) {   
                    digitalWrite(gledPin, HIGH);      //turns on gled pin
                }
              
                else {                                //turns gled off if temp is not in correct range
                    digitalWrite(gledPin, LOW);
                }
                
                if (  (bledMin <= tC) &&  (tC <= bledMax)  ) { 
                    digitalWrite(bledPin, HIGH);      //turns on bled pin
                }
              
                else {                                //turns bled off if temp is not in correct range
                    digitalWrite(bledPin, LOW);
                }
     
         
      }
      
  
  
  delay(delayTotal);   //how fast the temp updates in milliseconds
  
}
