//import libraries

//These constants will not change during operation of device

int tempPin = A0;       //pin the thermistor is attached to     THIS NEEDS TO BE AN ANALOG PIN!!!!!!!1!!!
float sensor;
const int rledPin = 2;        //pin the red led is attached to        digital pins
const int gledPin = 4;        //pin the green led is attached to
const int bledPin = 7;        //pin the blue led is attached to
float voltIn = 1023.0;    //voltage provided to thermistor 5=1023.0 
const int numReadings = 1;       //number of readings to take the average of, this is the exact number of readings, not off by one
const int delayTotal = 100;   //delay in milliseconds


// LED temperature ranges

const int rledMin = 0;       //min r temp value in celsius
const int rledMax = 26;       //max r temp value in celsius
const int gledMin = 26;       //min g temp value in celsius
const int gledMax = 51;       //max g temp value in celsius
const int bledMin = 51;       //min b temp value in celsius
const int bledMax = 75;       //max b temp value in celsius


// variables; these are changed to calculate the voltage and temperature



//int Vo;                       //voltage between thermistor and known resistor
float rVolt;
//float R1 = 10000;             //Known resistor value 
float tC;       //defines variables; sets them to be floating point values
                          

//array variables, etc. 

int readIndex = 0;
float readings[numReadings];        //array of readings from the thermistor
float total = 0;
float average = 0;
float vAverage;

//calibration array 

float voltCalread[] = {0.69465, 0.84212, 1.19348, 1.06615};
float tempCalread[] = {20.5, 35.1, 70.6, 55.6};
float lrCoef[2]={0,0};
int sizeVolt = sizeof(voltCalread)/sizeof(*voltCalread);
int sizeTemp = sizeof(tempCalread)/sizeof(*tempCalread);
//Serial.print(sizeVolt);
//Serial.print(sizeTemp);

void lsrlLinReg(float* x, float*y, float* lrCoef, int n){
  // pass x and y arrays (pointers), lrCoef pointer, and n.  The lrCoef array is comprised of the slope=lrCoef[0] and intercept=lrCoef[1].  n is length of the x and y arrays.
 
 
  // initialize variables
  float sigmaX = 0;
  float sigmaY = 0;
  float sigmaXY = 0;
  float sigmaX2 = 0;
  float sigmaY2 = 0;

  //calculations
  for (int i=0; i<n; i++){
    sigmaX = sigmaX + x[i];
    sigmaY = sigmaY + y[i];
    sigmaXY = sigmaXY + (x[i]*y[i]);
    sigmaX2 = sigmaX2 + (x[i]*x[i]);
    sigmaY2 = sigmaY2 + (y[i]*y[i]); 
  }

  // least squares regression line algorithm
  lrCoef[0] = ((n * sigmaXY) - (sigmaX * sigmaY)) / ((n * sigmaX2) - (sigmaX * sigmaX));
  lrCoef[1] = (sigmaY - (lrCoef[0] * sigmaX)) / n;
  
}
/*
void simpLinReg(float* x, float* y, float* lrCoef, int n){
  // pass x and y arrays (pointers), lrCoef pointer, and n.  The lrCoef array is comprised of the slope=lrCoef[0] and intercept=lrCoef[1].  n is length of the x and y arrays.
  // http://en.wikipedia.org/wiki/Simple_linear_regression

  // initialize variables
  float xbar=0;
  float ybar=0;
  float xybar=0;
  float xsqbar=0;
  
  // calculations required for linear regression
  for (int i=0; i<=n; i++){
    xbar=xbar+x[i];
    ybar=ybar+y[i];
    xybar=xybar+x[i]*y[i];
    xsqbar=xsqbar+x[i]*x[i];
  }
  xbar=xbar/n;
  ybar=ybar/n;
  xybar=xybar/n;
  xsqbar=xsqbar/n;
  
  // simple linear regression algorithm
  lrCoef[0]=(xybar-xbar*ybar)/(xsqbar-xbar*xbar);
  lrCoef[1]=ybar-lrCoef[0]*xbar;
}
*/
void setup() 
{
  
  pinMode(rledPin,  OUTPUT);  //set rled pin to output mode
  pinMode(gledPin,  OUTPUT);  //set gled pin to output mode
  pinMode(bledPin,  OUTPUT);  //set bled pin to output mode
  analogReadResolution(12);
  Serial.begin(9600);         //begins output to serial monitor

  //linear regression calibration 
  lsrlLinReg(voltCalread, tempCalread, lrCoef, sizeVolt);
  Serial.print(lrCoef[0],8);
  Serial.print(" ");
  Serial.println(lrCoef[1],8);

  if (sizeVolt != sizeTemp) {                           
      Serial.print("WARNING!! ARRAY MISMATCH!!!");
      delay(5000);
  }
                
  else {                                //turns rled off if temp is not in correct range
      Serial.print("Array match complete!");
      delay(1000);      
  }



 /* for (int thisReading = 0; thisReading < numReadings; thisReading++) 
  {
  
    readings[thisReading] = 0;
    analogReadResolution(12);
  
  }

  */
}

void loop() 
{

  total = total - readings[readIndex];       //subtract the last reading

  readings[readIndex] = analogRead(tempPin); //take readings from the sensor

  total = total + readings[readIndex];       //add the reading to the total

  readIndex = readIndex + 1;                 //advance to the next position in the array

  if (readIndex >= numReadings) 
  {            //if at end of array

    readIndex = 0;                          //wrap around to the beginning
    
  }

  average = total / numReadings;      //calculate the average
   
  if (readIndex % 10 == 0)  
  {         //if readIndex is a multiple of 10, calculate the temperature

    rVolt = ( (float)average * (3.3/4095.0));
    tC = ((rVolt*lrCoef[0]) + lrCoef[1]);

    Serial.print("\n");
    Serial.print("Temperature: ");                    //displays the temperature in the serial output
    Serial.print(tC, 3);
    Serial.print(" C         "); 
    Serial.print("Voltage:  ");
    Serial.print(rVolt, 5);
    //Serial.print("\n");
    //Serial.print(sensor);

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
      
  
  
  delay(delayTotal);   //how fast the temp updates in milliseconds, edit actual delay up at the top in variables
  
}
