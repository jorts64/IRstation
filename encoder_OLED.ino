#include <Wire.h>  // Include Wire if you're using I2C
#include <SFE_MicroOLED.h>  // Include the SFE_MicroOLED library
#define PIN_RESET 255  // Connect RST to pin 9
#define DC_JUMPER 0
MicroOLED oled(PIN_RESET, DC_JUMPER);    // I2C declaration 

int PosMax = 3;
String noms[] = {"A1","B2","C3","D4"};

 
 int pinA = D5;  // Connected to CLK on KY-040
 int pinB = D6;  // Connected to DT on KY-040
 int pinS = D7;  // polsador
 int encoderPosCount = 0; 
 int pinALast;  
 int aVal;
 boolean pols;
 boolean bCW;

 void setup() { 
   pinMode (pinA,INPUT);
   pinMode (pinB,INPUT);
   pinMode (pinS,INPUT);
   /* Read Pin A
   Whatever state it's in will reflect the last position   
   */
   pinALast = digitalRead(pinA);   

  oled.begin();    // Initialize the OLED
  oled.clear(ALL); // Clear the display's internal memory
  oled.display();  // Display what's in the buffer (splashscreen)
  delay(1000);     // Delay 1000 ms
  oled.clear(PAGE); // Clear the buffer.
    printTitle(noms[encoderPosCount], 1);

   Serial.begin (9600);
 } 

 void loop() {
 
   aVal = digitalRead(pinA);
   if (aVal != pinALast){ // Means the knob is rotating
     // if the knob is rotating, we need to determine direction
     // We do that by reading pin B.
     if (digitalRead(pinB) == aVal) {  // Means pin A Changed first - We're Rotating Clockwise
       encoderPosCount ++;
       if (encoderPosCount > PosMax)
        encoderPosCount = 0;
       bCW = true;
     } else {// Otherwise B changed first and we're moving CCW
       bCW = false;
       encoderPosCount--;
       if (encoderPosCount < 0)
        encoderPosCount = PosMax;
     }
     Serial.print ("Rotated: ");
     if (bCW){
       Serial.println ("clockwise");
     }else{
       Serial.println("counterclockwise");
     }
     Serial.print("Encoder Position: ");
     Serial.println(encoderPosCount);
     printTitle(noms[encoderPosCount], 1);
     aVal = digitalRead(pinA);   
     
   } 
   pinALast = aVal;
   pols = digitalRead(pinS);
   if (pols==LOW) {
    oled.invert(true);
    delay(1000);
    oled.invert(false);
    Serial.println("premut");
   }
   
 } 

void printTitle(String title, int font)
{
  int middleX = oled.getLCDWidth() / 2;
  int middleY = oled.getLCDHeight() / 2;
  
  oled.clear(PAGE);
  oled.setFontType(font);
  // Try to set the cursor in the middle of the screen
  oled.setCursor(middleX - (oled.getFontWidth() * (title.length()/2)),
                 middleY - (oled.getFontWidth() / 2));
  // Print the title:
  oled.print(title);
  oled.display();
}
 
