/* I am pretty sure you have heard of encryption before. 
An early form of encryption named after Julius Caesar is the Caesar cipher. 
It is one of the simplest and most widely known encryption techniques.

To encrypt a line of text, each letter in the plaintext is replaced by a letter with some fixed number 
(communicated beforehand) of positions down the alphabet list.

Implement a software that could encrypt & decrypt a message with the CoreModule. 
You can verify your result with any Caesar cipher you can find online.


N <- input
WORD <- input
ALPHABET <- [“A”, “B”, “C”, “D” ...etc...]

for LETTER in WORD
for i <- 0 to 25
    if LETTER = ALPHABET [i]
      i <- i + N
      if i > 25
        i <- i - 26
      OUTPUT ALPHABET [i]
    else 
      i <- i + 1
    end if
end loop
end loop

*/

String rcvString = "";
int i = 0;
int j = 0;
int N = 0;
int x = 0;
char w0rd[] = "";
char letter;
String alphabet = "abcdefghijklmnopqrstuvwxyz";

void setup(){
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n Enter the position you want the encryption to be down the alphabetical list: ");
}

void loop(){
  if (Serial.available()) {
    rcvString = Serial.readString();
    if (x == 0) {
     N = rcvString.toInt();
     Serial.println(N);
     Serial.println("\n Enter your plain text: ");
     x = 1;
    }
    else {
     w0rd[] = rcvString.toChar();
     Serial.println(w0rd);
     x = 0;
    }
    for (i = 0; i < strlen(w0rd); i++) {
      for(j = 0; j <= 26; j++) {
        if (w0rd[i] == alphabet[j]) {
          Serial.println(alphabet[(j + N) % 26]);
        }
   
      }
    }
  
 }
}
