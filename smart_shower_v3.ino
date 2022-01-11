#include <elapsedMillis.h>
elapsedMillis timeElapsed;  
#include "Ultrasonic.h"
Ultrasonic ultrasonic(7);
#include <Adafruit_NeoPixel.h>
#define PIN 6
#define NUMPIXELS 15
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#include <Servo.h>
Servo myservo;
int pos = 0;

void setup() {
  Serial.begin(9600);
  pixels.begin();
}

void loop() {

//afstandmeter meet of iemand onder de douche staat
long RangeInCentimeters;
RangeInCentimeters = ultrasonic.MeasureInCentimeters();
delay(50);
Serial.println(RangeInCentimeters);
      
//Stap 1: led verandert elke minuut van kleur tot 5 minuten is bereikt
pixels.clear(); //start met de led uit   
if (timeElapsed < 60000) {for(int i=0; i<NUMPIXELS; i++) {pixels.setPixelColor(i, pixels.Color(0, 255, 0)); pixels.show();}} //onder 1 minuut is de led aan in felgroen
else if (timeElapsed < 120000) {for(int i=0; i<NUMPIXELS; i++) {pixels.setPixelColor(i, pixels.Color(0, 255, 0)); pixels.show();}} //tussen 1 en 2 minuten is de led aan in felgroen
else if (timeElapsed < 180000) {for(int i=0; i<NUMPIXELS; i++) {pixels.setPixelColor(i, pixels.Color(150, 255, 0)); pixels.show();}} //tussen 2 en 3 minuten is de led aan in lichtgroen
else if (timeElapsed < 240000) {for(int i=0; i<NUMPIXELS; i++) {pixels.setPixelColor(i, pixels.Color(255, 50, 0)); pixels.show();}} //tussen 3 en 4 minuten is de led aan in oranje
else if (timeElapsed < 300000) {for(int i=0; i<NUMPIXELS; i++) {pixels.setPixelColor(i, pixels.Color(240, 30, 30)); pixels.show();}} //tussen 4 en 5 minuten is de led aan in lichtrood
      
//Stap 2: na 5 minuten draait de servo de warme kraan uit (90deg)
else if (timeElapsed > 300000 && timeElapsed < 301000)
for (pos = 0; pos <= 90; pos += 1) {//gaat van 0 naar 90 graden in stappen van 1 graden
  myservo.attach(8); //aansluiten servo op D8, de servo hoeft nu pas actief te worden (dit voorkomt trillen in de eerste 5 minuten) 
  myservo.write(pos); //servo gaat naar positie toegewezen aan variabele 'pos', hier 90
  delay(15);  //tijd voor de servo om de nieuwe positie te bereiken
}
      
//Stap 3: tussen de 5 en 6 minuten wordt de led rood en gaat hij knipperen
else if (timeElapsed < 360000) {
  for(int i=0; i<NUMPIXELS; i++) {pixels.setPixelColor(i, pixels.Color(255, 0, 0)); pixels.show();} delay(500); 
  for(int i=0; i<NUMPIXELS; i++) {pixels.setPixelColor(i, pixels.Color(0, 0, 0)); pixels.show();} delay(1000);
}
    
//als er na 6 minuten nog iemand staat gaat de led 2x zo snel rood knipperen en draait de servo de kraan volledig dicht (180deg)
else if (timeElapsed > 360000) {
  if (RangeInCentimeters < 100){
  Serial.println("er staat iemand");
  for(int i=0; i<NUMPIXELS; i++) {pixels.setPixelColor(i, pixels.Color(255, 0, 0)); pixels.show();} delay(500);
  for(int i=0; i<NUMPIXELS; i++) {pixels.setPixelColor(i, pixels.Color(0, 0, 0)); pixels.show();} delay(500);
  for (pos = 90; pos <= 180; pos += 1) {myservo.write(pos); delay(15);} //en de servo draait de kraan dicht
  myservo.detach(); //zodat de servo niet blijft herhalen
  } 
    
  else {
  Serial.println("er staat niemand");
  Serial.println(timeElapsed); //als er niemand meer onder de douche staat, print de totale douche tijd
  pixels.clear(); //en doe het licht uit
  }
}
}
