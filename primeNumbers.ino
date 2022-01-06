/* Loops is useful when there is a need to generate a series of numbers in a sequence.
Generate the 1st 30 prime number in the number system.

What's the 26th Prime number in your series? Does your answer match up with everyone else?
*/

int i = 0;
int j = 0;
boolean prime;

void setup() {
  Serial.begin(115200);
  delay(1000)
  Serial.println("Here are the first 30 prime numbers in the number system: ")

  for (i = 2; i <=30; i++) {
    for (j = 3; j <= 30; j+=2) {
      if (i % j == 0) {
        prime = false;
        break;
      }
    }
  } else {
      prime = true;
      Serial.println(prime);

  
}

void loop() {
  
}
