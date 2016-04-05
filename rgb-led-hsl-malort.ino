// HSV fade/bounce for Arduino - scruss.com - 2010/09/12

// brished up by HPD 20160401

// define for color wheel UP or DOWN
// wait around 2sec if solid color reached
// math HSV->RGB corrected
// debug to serial added


// PWM: 3, 5, 6, 9, 10, and 11. Provide 8-bit PWM output with the analogWrite() function. 
#define PWM3   3
#define PWM5   5
#define PWM6   6
#define PWM9   9
#define PWM10 10
#define PWM11 11

//------------------------------------------------------------------------------
// USER SETTING

// where are the wires?
// pollin
//#define RED      PWM9  // pin for red LED
//#define GREEN    PWM10 // pin for green LED
//#define BLUE     PWM11 // pin for blue LED

// velleman
#define RED      PWM3  // pin for red LED
#define GREEN    PWM5  // pin for green LED
#define BLUE     PWM6  // pin for blue LED

int DEBUG = 0;      // DEBUG flag; if set to 1, will write values back via serial port, 2=more verbose

// 1/1000 sec
#define DELAY    100
#define DELAY_SECTOR_CHANGE  (DELAY * 100)


#define HUE_TURN_UP
// #define HUE_TURN_DOWN

#define CONST_SATUR    1.0
#define CONST_VALUE    1.0

//------------------------------------------------------------------------------
// END USER SETTING

long RGB_pin_mapping[3] = { RED, GREEN, BLUE };

// map Hue 0° to 360° to 0 to 6 sectors
#define HUE_MIN  0.0
#define HUE_MAX  6.0
#define HUE_DELTA 0.01

#define LED_DIG13 13

typedef struct RgbColor
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
} RgbColor;

long RGB_out[3];
long rgbval;

// for reasons unknown, if value !=0, the LED doesn't light. Hmm ...
// and saturation seems to be inverted
// pre-given start value RED:
float hue=0.0, saturation=CONST_SATUR, value=CONST_VALUE;

#ifdef HUE_TURN_UP  
int i_last_hi=0;
#else
int i_last_hi=5;
#endif

/*
chosen LED SparkFun sku: COM-09264
 has Max Luminosity (RGB): (2800, 6500, 1200)mcd
 so we normalize them all to 1200 mcd -
 R  250/600  =  107/256
 G  250/950  =   67/256
 B  250/250  =  256/256
 */
//long bright_normalize[3] = { 107, 67, 256};
long bright_normalize[3] = { 256, 256, 256};

int last_blink=LOW;

void setup () {
  int pinNo, k;
  
  randomSeed(analogRead(4));
  for (k=0; k<3; k++) {
    pinNo=RGB_pin_mapping[k];
    pinMode(pinNo, OUTPUT);
    RGB_out[k]=0;
    analogWrite(pinNo, RGB_out[k] * bright_normalize[k]/256);
  }
  
  // initialize digital pin 13 as an output.
  pinMode(LED_DIG13, OUTPUT);

  if (DEBUG) {           // If we want to see values for debugging...
    Serial.begin(19200);  // ...set up the serial ouput 
    Serial.println("Malort Michael!");
        Serial.print("sat: ");
        Serial.print(saturation);
        Serial.print(" | value: ");
        Serial.println(value);
        delay(1500);
  }

}

void loop() {

  int k, pinNo;
  
#ifdef HUE_TURN_UP  
  hue += HUE_DELTA;
  if (hue >= HUE_MAX) {
    hue=HUE_MIN;
  }
#else
  hue -= HUE_DELTA;
  if (hue < HUE_MIN) {
    hue=HUE_MAX - HUE_DELTA;
  }
#endif


      digitalWrite(LED_DIG13, last_blink);   // turn the LED on (HIGH is the voltage level)
      if ( last_blink == LOW ) { last_blink = HIGH; } else { last_blink=LOW; }

      
  if (DEBUG) { 
        Serial.print(" HSV ");
        Serial.print( floor( hue*60) );
        Serial.print(" -> ");
  }
   
  rgbval=HSV_to_RGB(hue, saturation, value);
  RGB_out[0] = (rgbval & 0x00FF0000) >> 16; // there must be better ways
  RGB_out[1] = (rgbval & 0x0000FF00) >> 8;
  RGB_out[2] = rgbval & 0x000000FF;
  for (k=0; k<3; k++) { // for all three colours
    pinNo=RGB_pin_mapping[k];
    analogWrite(pinNo, RGB_out[k] * bright_normalize[k]/256);
  if (DEBUG) { 
        Serial.print(RGB_out[k] * bright_normalize[k]/256);
        Serial.print(" / ");  
  }
  }
  if (DEBUG) { 
        Serial.println("");  
     }
        
  delay(DELAY);
}

long HSV_to_RGB( float h, float s, float v ) {
  /* modified from Alvy Ray Smith's site: http://www.alvyray.com/Papers/hsv2rgb.htm */
  // H is given on [0, 6]. S and V are given on [0, 1].
  // RGB is returned as a 24-bit long #rrggbb
  int hi;
  float m, n, f;

  // out of range: return black
  if ((s<0.0) || (s>1.0) || (v<0.0) || (v>1.0)) {
    return 0L;
  }

  if ((h < 0.0) || (h > 6.0) || (s == 0.0) ) {
    return long( v * 255 ) + long( v * 255 ) * 256 + long( v * 255 ) * 65536;
  }
  
  hi = floor(h);
  f = h - hi;
  if ( !( hi & 1) ) {
    f = 1 - f; // if hi is even
  }
  m = v * (1 - s);
  n = v * (1 - s * f); 

  if (DEBUG==2) { 
     Serial.print(" hi: ");
     Serial.println(hi);
  }

if ( i_last_hi != hi) {
  if (DEBUG==2) { 
     Serial.print(" hi sector change! wait sec " );
     Serial.println( DELAY_SECTOR_CHANGE );
  }
  i_last_hi = hi;
  delay(DELAY_SECTOR_CHANGE);
}
  switch (hi) {
  case 6:
  case 0: 
    return long(v * 255 ) * 65536 + long( n * 255 ) * 256 + long( m * 255);
  case 1: 
    return long(n * 255 ) * 65536 + long( v * 255 ) * 256 + long( m * 255);
  case 2: 
    return long(m * 255 ) * 65536 + long( v * 255 ) * 256 + long( n * 255);
  case 3: 
    return long(m * 255 ) * 65536 + long( n * 255 ) * 256 + long( v * 255);
  case 4: 
    return long(n * 255 ) * 65536 + long( m * 255 ) * 256 + long( v * 255);
  case 5: 
    return long(v * 255 ) * 65536 + long( m * 255 ) * 256 + long( n * 255);
  }


} 

