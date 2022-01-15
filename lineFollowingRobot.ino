                    
/* 
 *  Ex_12 - PID Controller
 *  Line Following Robot using PID Controller
 *  
 *  PortExpander
 *  -- Bit 4 - 0 
 *  -- Line sensor Module
 *  -- Bit 7 - 5
 *  -- End Stop switch Module
 *  
 *  NeoPixel
 *  -- IO5
 */
 
#include <Wire.h>

#define I2C_SDA 19
#define I2C_SCL 18

#define PEXP_I2CADDR 0x23
#define OLED_I2CAADR 0x3C

#define NEO_PIXEL 5
#define LED_COUNT 3

uint8_t expanderData;

//Write a byte to the IO expander
void IOexpanderWrite(byte address, byte _data) {
    Wire.beginTransmission(address);
    Wire.write(_data);
    Wire.endTransmission(); 
}

//Read a byte from the IO expander
uint8_t IOexpanderRead(int address) {
    uint8_t _data;
    Wire.requestFrom(address, 1);
    if(Wire.available()) {
        _data = Wire.read();
    }
    return _data;
}



int8_t pidErrorMap[9][6] = 
{
    {1, 1, 1, 1, 0, 4},
    {1, 1, 1, 0, 0, 3},
    {1, 1, 1, 0, 1, 2},
    {1, 1, 0, 0, 1, 1},
    {1, 1, 0, 1, 1, 0},
    {1, 0, 0, 1, 1, -1},
    {1, 0, 1, 1, 1, -2},
    {0, 0, 1, 1, 1, -3},
    {0, 1, 1, 1, 1, -4},    
};

float Kp=4, Ki=0, Kd=0;
const float minSpeed = 0.5;
const float maxSpeed = 15;

float error=0, P=0, I=0, D=0, PID_value=0;
float previousError=0;
uint8_t sensor[5] = {0, 0, 0, 0, 0};

void readSensorValues()
{
    expanderData = IOexpanderRead(PEXP_I2CADDR);
    sensor[0] = bitRead(expanderData, 0);
    sensor[1] = bitRead(expanderData, 1);
    sensor[2] = bitRead(expanderData, 2);
    sensor[3] = bitRead(expanderData, 3);
    sensor[4] = bitRead(expanderData, 4);
    for (byte i = 0; i < 9; i++) {     
        if (sensor[0] == pidErrorMap[i][0] && sensor[1] == pidErrorMap[i][1] && 
            sensor[2] == pidErrorMap[i][2] && sensor[3] == pidErrorMap[i][3] && 
            sensor[4] == pidErrorMap[i][4]) {
            error = pidErrorMap[i][5];
        }
        if (sensor[0] + sensor[1] + sensor[2] + sensor[3] + sensor[4] == 5) {
            // No Line??
        } 
        else if (sensor[0] + sensor[1] + sensor[2] + sensor[3] + sensor[4] == 0) {
            // Full Line??
        } 
        else {
            // lastDefinedError = error;              
        }
    }
}

void calculatePID()
{
    P = error;
    I = I + error;
    D = error - previousError;
    PID_value = (Kp*P) + (Ki*I) + (Kd*D);
    previousError = error;
}

int setRPM(int motor, float RPM){
    attinySlaveArrayBoard[0] = motor == 0 ? 0x14 : 0x24;  // Command 0x14 or 0x24
    attinySlaveArrayBoard[1] = RPM_x_100 & 0xff;  // Param1 - Low
    attinySlaveArrayBoard[2] = (RPM_x_100 >> 8) & 0xff;  // Param2 - High
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

void setMotorSpeed(int left, int right)
{
    // Sorry kids... 
    // It is Left viewing from the front of the robot
    // And not the from the drivers seat. :(
                           
    Serial.print("Left = "); Serial.print(String(left));
    Serial.print(" Right = ");Serial.println(String(right));
    delay(100);
    
    setRPM(leftMotor, left);
    setRPM(rightMotor, right);  
}

void motorControl()
{
    float leftMotorSpeed = maxSpeed - PID_value;
    float rightMotorSpeed = maxSpeed + PID_value;
   
    leftMotorSpeed = constrain(leftMotorSpeed, minSpeed, maxSpeed);
    rightMotorSpeed = constrain(rightMotorSpeed, minSpeed, maxSpeed);
    
    setMotorSpeed(leftMotorSpeed, rightMotorSpeed);
}


void setup() {
    Wire.begin(I2C_SDA, I2C_SCL);   
    Serial.begin(115200); //set up serial library baud rate to 115200
    
    // Initialize PCF8574 configuration -> Pull High to Read
    IOexpanderWrite(PEXP_I2CADDR, 0xff);

    delay(2000);
}

void loop() {
    readSensorValues();
    calculatePID();
    motorControl();
}
