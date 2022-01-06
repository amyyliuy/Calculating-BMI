/* 
 *  Ex_06_1 - Millis()
 *  Input & Output
 *  
 *  LED 
 *  -- IO2
 *  Switch
 *  -- IO9
 *  
 */

#define ONBOARD_LED 2    // LED IO2
#define ONBOARD_SW  9    // Switch IO9

int current_LED = LOW;

byte current_mode = 0;
long lastLED_Activity = 0;
byte current_ButtonState = HIGH;
byte previous_ButtonState = HIGH;

void setup() {
   Serial.begin(115200); //set up serial library baud rate to 115200
   pinMode(ONBOARD_LED, OUTPUT);
   pinMode(ONBOARD_SW, INPUT);

   digitalWrite(ONBOARD_LED, current_LED);    // turn the LED off by making the voltage LOW  
}

void loop() {
    // Example 1 - Print Millis()
    
    //Serial.print("Seconds(s): ");
    //Serial.println(millis()/1000);

    
    
    
    // Example 2 - Blink LED every second (1000 ms)
    
    // digitalWrite(ONBOARD_LED, ((millis() / 1000) % 2 == 0)? LOW: HIGH);
    
    

    
    // Example 3 - Multi Mode Selector - Blocking Problem
    
    // Button Reading portion
    current_ButtonState = digitalRead(ONBOARD_SW);
    
    // Transition from High to Low, Button press instance
    if ((current_ButtonState == LOW) && (previous_ButtonState == HIGH)){
        delay(5000);
        current_mode = (current_mode >= 3)? 0 : current_mode + 1;
        Serial.print("Button Mode ");
        Serial.println(current_mode);
    }
    previous_ButtonState = current_ButtonState;

    // Maybe do something with LED blinking here?
    // If you put a delay(5000) while doing LED blinking here, do you think it will affect the button read?

    
}
       
