/* (A) Automated Shuttle bus service robot:

Turn your robot into an automated shuttle bus service. (Think MRT/MTR/LRT but without the rail track).
It will stop at each "Station" designated for the selected "Bus" services you pick. & it will wait for the "Passengers" to descend or embark onto the shuttle bus.

Hint: You can utilize the color sensor & line sensor to signify a "Bus Station".

Use red light to signify a bus stop, 
*/
 
#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"

#define I2C_SDA 19
#define I2C_SCL 18
// i2c Slave Co-processor - On the Robot MainBoard
#define I2CADDR_B 0x12

#define PEXP_I2CADDR 0x23
#define OLED_I2CAADR 0x3C

#define SR04_I2CADDR 0x57
#define OLED_I2CAADR 0x3C

#define I2C_SDA 19
#define I2C_SCL 18

#define NEO_PIXEL 5
#define LED_COUNT 3

#define LED 2
#define IR_RECV 4


// Control the Stepper on the
// Robotic MainBoard.
// Input:
//        motor - 0 (Motor A)
//                1 (Motor B)
//        direction - 0 -> Stop 
//                    1 -> Clockwise
//                    2 -> Counter-Clockwise
// Return:
//        0 - Sucess
//        1 - i2C write failure

#define STOP 0
#define CW 1
#define CCW 2
byte attinySlaveArrayBoard[3];

int setDirection(int motor, byte direction) {
    attinySlaveArrayBoard[0] = motor == 0 ? 0x13 : 0x23;  // Command 0x13 or 0x23
    attinySlaveArrayBoard[1] = (direction >= 0) && (direction <= 2) ? direction: 0;  
                                                          // Param1 - Stop/CW/CCW
    attinySlaveArrayBoard[2] = 0x00;  // Param2 - Dummy in this case
    delay(10);
    Wire.beginTransmission(I2CADDR_B);
    Wire.write(attinySlaveArrayBoard, 3); // Sends 3 bytes i2c to Co-processor.
    if (Wire.endTransmission () == 0) { // Receive 0 = success (ACK response) 
        Serial.println("i2c Write to 0x12 Sucessfull");
        return 0;
    }
    else {
        Serial.println("i2c Write Failed");
        return 1;
    }
}

int setMotorRunning(uint8_t motorState) {
    attinySlaveArrayBoard[0] = 0x01;  // Command 0x01
    attinySlaveArrayBoard[1] = motorState? 0x01:0x00;  // Param1 - Stop/Run
    attinySlaveArrayBoard[2] = 0x00;  // Param2 - Dummy in this case
    delay(10);
    Wire.beginTransmission(I2CADDR_B);
    Wire.write(attinySlaveArrayBoard, 3); // Sends 3 bytes i2c to Co-processor.
    if (Wire.endTransmission () == 0) { // Receive 0 = success (ACK response) 
        Serial.println("i2c Write to 0x12 Sucessfull");
        return 0;
    }
    else {
        Serial.println("i2c Write Failed");
        return 1;
    }
}

SSD1306AsciiWire oled;

uint8_t LEDState = LOW;

//Just some variables to hold temporary data
char text[10];
uint8_t  i, nec_state = 0; 
unsigned int command, address;

static unsigned long nec_code;
static boolean nec_ok = false;
static unsigned long timer_value_old;
static unsigned long timer_value;

void irISR() {
    
    timer_value = micros() - timer_value_old;       // Store elapse timer value in microS
    
    switch(nec_state){
        case 0:   //standby:                                      
            if (timer_value > 67500) {          // Greater than one frame...
                timer_value_old = micros();     // Reset microS Timer
                nec_state = 1;                  // Next state: end of 9ms pulse + LeadingSpace 4.5ms
                i = 0;
            }
        break;

        // Leading Mark = Leading pulse + leading space = 9000 + 4500 = 13500
        // max tolerance = 1000
        case 1:   //startPulse:
            if (timer_value >= (13500 - 1000) && timer_value <= (13500 + 1000)) { //its a Leading Mark
                i = 0;
                timer_value_old = micros();
                nec_state = 2;
            }
            else 
                nec_state = 0;
        break;

        //Bit 0 Mark length = 562.5µs pulse + 562.5µs space = 1125
        //Bit 1 Mark length = 562.5µs pulse + 3 * 562.5µs space = 2250
        //max tolerance = (2250 - 1125)/2 = 562.5 
        case 2:   //receiving:
            if (timer_value < (1125 - 562) || timer_value > (2250 + 562)) nec_state = 0; //error, not a bit mark
            else { // it's M0 or M1
                  nec_code = nec_code << 1; //push a 0 from Right to Left (will be left at 0 if it's M0)
                  if(timer_value >= (2250 - 562)) nec_code |= 0x01; //it's M1, change LSB to 1
                  i++;
                  
                  if (i==32) { //all bits received
                      nec_ok = true;
//                    detachInterrupt(IR_RECV);   // Optional: Disable external interrupt to prevent next incoming signal
                      nec_state = 0;
                      timer_value_old = micros();
                  }
                  else {
                      nec_state = 2; //continue receiving
                      timer_value_old = micros();
                  }
              }
        break;
      
        default:
            nec_state = 0;
        break;
    }
}

void setup() {
    Wire.begin(I2C_SDA, I2C_SCL);   
    oled.begin(&Adafruit128x64, OLED_I2CAADR); 
    Serial.begin(115200); //set up serial library baud rate to 115200
    
    pinMode(LED, OUTPUT);
    digitalWrite(LED, LEDState);
    pinMode(IR_RECV, INPUT);
    delay(2000);
    
    oled.setFont(Adafruit5x7);
    oled.clear();
    oled.println("NEC Decoder!");
    
    attachInterrupt(digitalPinToInterrupt(IR_RECV), irISR, FALLING);  //Setting IR detection on FALLING edge.
}

void loop() {
    if(nec_ok) {                                     // If a good NEC message is received
        nec_ok = false;                             // Reset decoding process
        
        LEDState = (LEDState == LOW) ? HIGH: LOW;   // Toggle LED to show new IR result
        digitalWrite(LED, LEDState);

        oled.clear();
        oled.println("NEC IR Received:");
        
        oled.print("Addr: ");
        address = nec_code >> 16;
        command = (nec_code & 0xFFFF) >> 8;         // Remove inverted Bits
        sprintf(text, "%04X", address);
        oled.println(text);                             // Display address in hex format

        oled.print("Cmd: ");
        sprintf(text, "%02X", command);
        oled.println(text);                             // Display command in hex format

        if(command == 0x18){
    oled.println("Move Forward");
    setDirection(0, CCW);
    setDirection(1, CW);
    setMotorRunning(HIGH);
        }

        else if(command == 0x4A){
    oled.println("Move Backward");
    setDirection(0, CW);
    setDirection(1, CCW);
    setMotorRunning(HIGH);
        }
        
        else if(command == 0x38){
    oled.println("Stop");
    setMotorRunning(LOW);
        }
        
        else if(command == 0x10){
    oled.println("Turn Left");
    setDirection(0, CCW);
    setDirection(1, CCW);
    setMotorRunning(HIGH);
        }
        else if(command == 0x5A){
    oled.println("Turn Right");
    setDirection(1, CW);
    setDirection(0, CW);
    setMotorRunning(HIGH);
        }
          
        }


//      attachInterrupt(digitalPinToInterrupt(IR_RECV), irISR, FALLING);  //Setting IR again on FALLING edge.
    }   

