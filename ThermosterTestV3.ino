//Last modified by: tek.chhetri
//The goal of this code is to record the temp of the resister in the precharger circuit and measure the iterations until which it blows up
//To use this code, replace the ROOM_TEMP with the current reading and the code will output the temp from the thermistor & number of times the code ran in the serial monitor.
//NOTE: connect the power to 3.3V and make a voltage divider circuit with a 10kΩ register. 3.3 Supply voltage is used to minimize noise
//NOTE: Type anything and hit enter in the serial moniter to start executing the loop.

//These values are in the datasheet
#define Rt 10000   // Ω
#define B 3950      // K  
//--------------------------------------

#define R 10000  //R=10KΩ
#define BITS 1023 //bits used for ADC, using 16bits for higher precision
#define ROOM_TEMP 20.1
#define VCC 3.3

//variables
float VR, RT, ln, TX, T0, Rntc,logRntc, Vntc, T, kelvins, celcius, farenheit; //Rntc = resistance of NTC thermistor
//float c1 = 1.440548920932420e-03, c2 = 2.690725842060890e-04, c3 = 1.661922621891600e-07; //use this for the extended formula

int counter = 0;
int prechargeRelay = 7;
int PosContactor = 2;
int bleedPrechargeRelay = 4;
int NegContactor = 36;

void setup() {
  Serial.begin(9600); 
  analogReference(EXTERNAL);
  pinMode(prechargeRelay, OUTPUT);
  pinMode(PosContactor, OUTPUT);
  pinMode(bleedPrechargeRelay, OUTPUT);
  pinMode(NegContactor, OUTPUT);
}

//float calculations(float readings) {
// Vntc = readings;              //Acquisition analog value of Voltage accross the thermistor
// Vntc = BITS/Vntc - 1;             //Converted to digital value
// Vntc = R/Vntc;
// 
// Rntc = Vntc/Rt; 
// logRntc = log(Rntc);
//  
// logRntc /= B;
// logRntc += 1.0 / (ROOM_TEMP+273.15);
// kelvins = 1.0/logRntc;
// celcius = kelvins - 273.15;
//  //farenheit = (celcius * 9.0)/ 5.0 + 32.0; 
//  return celcius;
//}

void loop() { 
  if (Serial.available() > 0) {
//    int startCommand;
//    startCommand = Serial.read();
    
//  celcius = calculations(analogRead(A0));
//  Serial.print(",");
//  Serial.print(celcius);
//  Serial.print(",");
  
  digitalWrite(NegContactor, HIGH);
  digitalWrite(bleedPrechargeRelay, LOW);
  delay(500);
  digitalWrite(prechargeRelay, HIGH);
  delay(999);
  //charging complete: Vcapacitor = Vsource
  digitalWrite(PosContactor, HIGH);
  digitalWrite(prechargeRelay, LOW);
  
  delay(1000);
  digitalWrite(NegContactor, LOW);
  digitalWrite(PosContactor, LOW);
  digitalWrite(bleedPrechargeRelay, HIGH);
  
//  celcius = calculations(analogRead(A0));
//  Serial.print(celcius);
//  Serial.print(",");
  
  delay(1000); //discharging complete //to discharge 63% of Capacitor's voltage time taken = Capacitance*resistance
  counter++;
  
  Serial.print("Test Number: ");
  Serial.print(counter);
  Serial.print('\n');
 }
}
