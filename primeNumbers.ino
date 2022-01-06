/* Loops is useful when there is a need to generate a series of numbers in a sequence.
Generate the 1st 30 prime number in the number system.

 Learn to seek for hint yourself (online)
What's the 26th Prime number in your series? Does your answer match up with everyone else?

*/

long i = 0;
long j = 0;
boolean prime;

void setup() {
  Serial.begin(115200);
  delay(1000)
  Serial.println("Here are the first 30 prime numbers in the number system: ")
  for ( i = 2; 
    


    prime = true;

    for (j = 3; j < Jmax; j+=2){
      if (i%j == 0){
        prime = false;
        break;
      }
    }

  
}

void loop() {
  
}
