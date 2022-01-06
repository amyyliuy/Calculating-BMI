/* Loops is useful when there is a need to generate a series of numbers in a sequence.
Generate the 1st 30 prime number in the number system.

What's the 26th Prime number in your series? Does your answer match up with everyone else?
*/

int i = 1;
int j = 0;
int count = 1;

void setup() {
  Serial.begin(115200);
  delay(1500);
  
while (count <= 30) {
  for (j = 2; j < i; j++) {
    if (i % j == 0)
           break;
  }
  if (j == i) { 
    Serial.println(i);
    count++;
  }
  i++;
 }
}

void loop() {
  
}
