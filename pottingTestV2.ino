
//To use this code, replace the ROOM_TEMP with the current reading and the code will output the temp from the thermistor in the serial monitor.
//NOTE: connect the power to 3.3V and make a voltage divider circuit with a 10kΩ register. 3.3 Supply voltage is used to minimize noise

//These values are in the datasheet
#define Rt 10000   // Ω
#define B 3428      // K
//--------------------------------------

#define R 10000  //R=10KΩ
#define BITS 65535.0 //bits used for ADC, using 16bits for higher precision
#define ROOM_TEMP 22.1
#define VCC 5

//variables
float VRT, VR, RT, ln, TX, T0, celcius, celcius0, celcius1, celcius2, celcius3, celcius4, celcius5, celcius6, celcius7; //Rntc = resistance of NTC thermistor
//float c1 = 1.440548920932420e-03, c2 = 2.690725842060890e-04, c3 = 1.661922621891600e-07;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  T0 = ROOM_TEMP + 273.15;  
}

float calculations(float readings) {
VRT = analogRead(A0);              //Acquisition analog value of VRT
  VRT = (5.00 / 1023.00) * VRT;      //Conversion to voltage
  VR = VCC - VRT;
  RT = VRT / (VR / R);               //Resistance of RT

  ln = log(RT / Rt);
  TX = (1 / ((ln / B) + (1 / T0))); //Temperature from thermistor

  TX = TX - 273.15;                 //Conversion to Celsius

  return TX;
}

void loop() {
  // put your main code here, to run repeatedly:  

celcius0 = calculations(analogRead(A0));
//celcius1 = calculations(analogRead(A1));
//celcius2 = calculations(analogRead(A2));
//celcius3 = calculations(analogRead(A3));
//celcius4 = calculations(analogRead(A4));
//celcius5 = calculations(analogRead(A5));
//celcius6 = calculations(analogRead(A6));
//celcius7 = calculations(analogRead(A7));
 
  //output
 Serial.print(celcius0);
 Serial.print(",");
// Serial.print("\n");

  delay(1000);
}
