String rcvString = "";
float weight = 0.0;
float height = 0.0;
float BMI = 0.0;
int i = 0;

void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("\nPlease enter your weight in kilograms: ");
}

void loop() {
  if (Serial.available()) {
     rcvString = Serial.readString();
     if (i == 0) {
      weight = rcvString.toFloat();
      Serial.print(weight);
      Serial.println("\nPlease enter your height in metres: ");
      i = 1;
     }
     else {
      height = rcvString.toFloat();
      Serial.print(height);
      i = 0;
     }
      BMI = (weight/(pow(height, 2.0)));
        if (BMI < 16) {
            Serial.println("\nYou are severly underweight.");
         }
        else if (BMI > 16 and BMI <= 18.4) {
            Serial.println("\nYou are underweight.");
         }
        else if (BMI > 18.4 and BMI <= 24.9){
            Serial.println("\nYour weight is normal.");
         }
        else if (BMI > 24.9 and BMI <= 29.9){
            Serial.println("\nYou are overweight.");
         }
        else if (BMI > 29.9 and BMI <= 34.9){
            Serial.println("\nYou are moderately obese.");
         }
        else if (BMI > 34.9 and BMI <= 39.9){
            Serial.println("\nYou are severely obese.");
         }
        else {
            Serial.println("\nYou are morbidly obese.");
         }

        Serial.println("\n\nPlease enter your weight in kilograms: ");
       }
     }
}
