//toevoegen afstandmeter
  #include "Ultrasonic.h"
  Ultrasonic ultrasonic(7); //D7

//toevoegen teller
  #include <elapsedMillis.h>
  elapsedMillis timeElapsed; //maakt teller aan

//toevoegen servo
  #include <Servo.h>
  Servo myservo; //object 'myservo' creeeren
  int pos = 0;   //variable om servo position in op te slaan

//toevoegen temperatuurmeter
  #include "DHT.h"
  #define DHTpin 4 //D4
  DHT dht(DHTpin, DHT11);

//toevoegen led strip
  #include <Adafruit_NeoPixel.h>
  #define PIN 6
  #define NUMPIXELS 15
  Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600); //aansluiten serial monitor
  pixels.begin(); //initialiseren NeoPixel strip object
  dht.begin(); //initialiseren temperatuur meter
  myservo.attach(8);  //aansluiten servo op D8
}

void loop() {
//Stap 1: afstandmeter meet of iemand onder de douche staat
  long RangeInCentimeters; //variabele voor het opslaan van de afstand
  RangeInCentimeters = ultrasonic.MeasureInCentimeters();
  delay(50); //korte tijd tussen metingen
   
  //als er iemand onder de douche staat, voer stap 2 t/m 6 uit
  if (RangeInCentimeters < 1000){

//Stap 2: led verandert elke minuut van kleur
  pixels.clear(); //start met de led uit   
  if (timeElapsed < 60000) {for(int i=0; i<NUMPIXELS; i++) {pixels.setPixelColor(i, pixels.Color(0, 255, 0)); pixels.show();}} //onder 1 minuut is de led aan in felgroen
  else if (timeElapsed < 120000) {for(int i=0; i<NUMPIXELS; i++) {pixels.setPixelColor(i, pixels.Color(0, 255, 0)); pixels.show();}} //tussen 1 en 2 minuten is de led aan in felgroen
  else if (timeElapsed < 180000) {for(int i=0; i<NUMPIXELS; i++) {pixels.setPixelColor(i, pixels.Color(150, 255, 0)); pixels.show();}} //tussen 2 en 3 minuten is de led aan in lichtgroen
  else if (timeElapsed < 240000) {for(int i=0; i<NUMPIXELS; i++) {pixels.setPixelColor(i, pixels.Color(255, 50, 0)); pixels.show();}} //tussen 3 en 4 minuten is de led aan in oranje
  else if (timeElapsed < 300000) {for(int i=0; i<NUMPIXELS; i++) {pixels.setPixelColor(i, pixels.Color(240, 30, 30)); pixels.show();}} //tussen 4 en 5 minuten is de led aan in lichtrood
  
//Stap 3: led gaat knipperen na 5 minuten
  else if (timeElapsed < 360000) {for(int i=0; i<NUMPIXELS; i++) {pixels.setPixelColor(i, pixels.Color(255, 0, 0)); pixels.show();} delay(500); 
  for(int i=0; i<NUMPIXELS; i++) {pixels.setPixelColor(i, pixels.Color(0, 0, 0)); pixels.show();} delay(1000);} //tussen 5 en 6 minuten is de led aan in felrood en knippert
  else if (timeElapsed > 360000) {for(int i=0; i<NUMPIXELS; i++) {pixels.setPixelColor(i, pixels.Color(255, 0, 0)); pixels.show();} delay(500); 
  for(int i=0; i<NUMPIXELS; i++) {pixels.setPixelColor(i, pixels.Color(0, 0, 0)); pixels.show();} delay(500);} //na 6 minuten is de led aan in felrood en knippert 2x sneller 

      
//Stap 4: servo draait warme kraan uit na 5 minuten
  if (timeElapsed > 300000 && timeElapsed < 301000) for (pos = 0; pos <= 90; pos += 1) { //gaat na 5 minuten van 0 naar 90 graden in stappen van 1 graden
    myservo.write(pos);              //servo gaat naar positie toegewezen aan variabele 'pos', hier 90 
    delay(15);                       //het duurt 15ms voor de servo om de positie te bereiken
    }

//Stap 5: temperatuur meten en eventueel aanpassen
  float t = dht.readTemperature(); //leest de waarden uit de sensor
  Serial.print("temperatuur "); //geeft de waarden weer in de serial monitor
  Serial.println(t, 1); //,1 om de afgeronde temperatuur weer te geven
  delay(10000); //meet elke 10 seconden
  
  //als de temperatuur boven de 30 graden celcius is draai de kraan opnieuw naar 90 graden
  if (t > 30 && timeElapsed > 300000 && timeElapsed < 301000) for (pos = 0; pos <= 90; pos += 1) { //gaat na 5 minuten van 0 naar 90 graden
   myservo.write(pos);              //servo gaat naar positie toegewezen aan variabele 'pos', hier 90
   delay(15);                       //het duurt 15ms voor de servo om de positie te bereiken
   }

//Stap 6: afstandmeter meet of iemand na 6 minuten NOG STEEDS onder de koude douche staat
   if (RangeInCentimeters < 1000 && timeElapsed > 360000 && timeElapsed < 361000) for (pos = 0; pos <= 90; pos += 1) { //gaat na 6 minuten van 0 naar 90 graden
   myservo.write(pos);              //servo gaat naar positie toegewezen aan variabele 'pos', hier 90
   delay(15);                       //het duurt 15ms voor de servo om de positie te bereiken
   } else {Serial.println(timeElapsed);} //als er niemand onder de douche staat, print de totale douche tijd 
  ;} 
;}
