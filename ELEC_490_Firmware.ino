/*  ELEC 490 Solar Power Optimizer
 *   Firmware code to run MPPT Tracking
*/
// ANALOG PINS
#define IinPin A0
#define VinPin A1
#define VoutPin A2

// DIGITAL PINS
#define DutyPin 3

// CONSTANTS
#define INPUT_POWER 250
#define RESISTANCE 5.60


int IinSense = 0; 
float VinSense = 0;
float VoutSense = 0;
float voltageIn = 0;
float voltageOut = 0;
int C = 2;
float Pb = 0;
float Pin = 0;
int d=60;
void setup() {
  // put your setup code here, to run once:
 frequencySetUp();
 Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  //OCR2B = 50;   //for debugging
  //Printing numbers required for testing of SPO
  IinSense_1();
  VinSense_1();
  VoutSense_1();
  
  mppt_DCSource();
}

void frequencySetUp(){
  //Increase the arduino frequency from 960Hz to 100kHz
  pinMode(DutyPin, OUTPUT);
  TCCR2A = 0x23;
  TCCR2B = 0x09;
  OCR2A = 160-1;
  OCR2B = 60;
  TCNT2 = 0;
  //in void loop write value from 0-160 for OCR2B to get Duty cycle
}

float IinSense_1(){
  float IIn;
  //IIR filter to get more appropriate values. Weights determined emperically.
  IinSense = 0.98*IinSense + 0.02*(float)(analogRead(IinPin));
  
  // Calculation of input voltage including resistive divider factor.
  IIn = IinSense*(-0.02895)+14.80; // Numbers calculated empirically to change analog values to Amperes 
  Serial.println(IIn);
  return IIn;
}

float VinSense_1(){
  //IIR filter to get more appropriate values. Weights determined emperically.
  VinSense = 0.98*VinSense + 0.02*analogRead(VinPin);

  // Calculation of output voltage including resistive divider factor.
  voltageIn = VinSense * (5.00/1023.0) *(1/0.091);
  Serial.println(voltageIn);
  return voltageIn;
}

float VoutSense_1(){
  //IIR filter to get more appropriate values. Weights determined emperically.
   VoutSense = 0.9*VoutSense + 0.1*analogRead(VoutPin);

   voltageOut = VoutSense*(5.0/1023.0)*(1/0.065);
   Serial.println(voltageOut);
   if(voltageOut > 71.0){ // Output capacitor checking
    OCR2B = 10;
   }
   return voltageOut;
}

//MPPT Algorithm
void mppt_DCSource(){
  char buffer[30];
   //d is the duty cycle value put into the OCR2B register
   //C is the change in duty cycle which is previously defined
   d = d + C;
   d=(d<0)?0:(d>255)?255:d;
   OCR2B=d;
   int k = 0;
   float Iin = IinSense_1();
   float Vin = VinSense_1();
   while(k<50){
    Iin = IinSense_1();
    Vin = VinSense_1();
    k++;
   }
   
   Pin = Iin*Vin;
   //If the current power is greater or equal to the previous power do not change the direction of the duty cycle
   if(Pin>=Pb){
    C = C;
   //If it is lower than the previous power value, change the sign of the change in duty cycle
   }else{
    C = -C;
   }

  sprintf(buffer,"Pin %d Pprev %d D: %d",(int)(Pin),(int)(Pb), (int)OCR2B*10/16);
  Serial.println(buffer);
  Pb = Pin;
  delay(1000);
  
}
