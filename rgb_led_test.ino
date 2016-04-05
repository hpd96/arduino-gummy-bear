    // color swirl! connect an RGB LED to the PWM pins as indicated
    // in the #defines
    // public domain, enjoy!
     
    #define REDPIN   9
    #define GREENPIN 10
    #define BLUEPIN  11
     
    #define FADESPEED 750     // make this higher to slow down
     
    void setup() {
      pinMode(REDPIN, OUTPUT);
      pinMode(GREENPIN, OUTPUT);
      pinMode(BLUEPIN, OUTPUT);
        analogWrite(REDPIN, 0);
        analogWrite(GREENPIN, 0);
        analogWrite(BLUEPIN, 0);
    }
     
     
    void loop() {
      int r, g, b;
     
        analogWrite(REDPIN, 255);
        delay(FADESPEED);
        analogWrite(REDPIN, 0);

        analogWrite(GREENPIN, 255);
        delay(FADESPEED);
        analogWrite(GREENPIN, 0);

        analogWrite(BLUEPIN, 255);
        delay(FADESPEED);
        analogWrite(BLUEPIN, 0);

        analogWrite(REDPIN, 255);
        analogWrite(GREENPIN, 255);
        analogWrite(BLUEPIN, 255);
        delay(FADESPEED);
        
        analogWrite(REDPIN, 0);
        analogWrite(GREENPIN, 0);
        analogWrite(BLUEPIN, 0);
        delay(FADESPEED);
    }
    
