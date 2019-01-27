/*  ELEC 490 Solar Power Optimizer
//isobel sucks
*/
// ANALOG PINS
#define VinPin A1

// DIGITAL PINS
#define DutyPin 3

// CONSTANTS
#define INPUT_POWER 250


int analogSense = 0;
float voltage = 0;
float power = 0;
int resistor = 10000;
int duty = 127;  //digital integer sent to PWM digital pin (50%)

void setup() {
  /*Used to make switching frequency 100kHz
  
  Adjust Duty cycle in main loop 
  HOW DO I TEST THIS
  */
  frequencySetup();
  
  pinMode(DutyPin, OUTPUT);
  TCCR2A = 0x23;
  TCCR2B = 0x09;
  OCR2A = 160-1;
  OCR2B = 0;
  TCNT2 = 0;
  Serial.begin(9600);
}

void loop() {
  //input_power();
  //Serial.println(power);
  //analogSense = analogRead(VinPin);
  for(int i = 27; i <= 122; i++){
    OCR2B = i;
    delay(100);
    Serial.println(i);
  }
  //period is 10us 
  //Serial.println(analogSense);
}

//Function to detect input power --> use Hall Effect sensor for current
void mppt(){
    analogSense = analogRead(VinPin);
    voltage = analogSense * (5.0/1023.0);
    power = (voltage*voltage)/resistor;
    if(power <= INPUT_POWER){
      //Increase duty cycle to increase voltage delivered
    }
    if(power > INPUT_POWER){
      //Decrease duty cycle to decrease output
    }
}

boolean efficiency(){
  //sense output voltage and compute based on output resistance
  return true;
}

/* Maximum Power from solar panel is 250W
  Vin ranges from (20-60)
  Measure the input current and calculate power
  If less, must buck voltage to achieve 225W (90%)
*/

// Create similar function for detecting output power




