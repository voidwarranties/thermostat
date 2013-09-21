/*
  Void-Josto thermostat
*/

int B1L1 = 32;
int B1L2 = 24;
int B1push=30;

int time = 10; //(in seconden)
int count1 = 0; //loopt in seconden

int secondenteller = 0;

void setup() {                
  pinMode(B1L1, OUTPUT);
  pinMode(B1L2, OUTPUT);
  pinMode(B1push, INPUT);

}

void loop() {
  delay(100); //we loopen 10 keer / seconde
  
    //teller aan en uit zetten. Je kan minstens 1 seconde drukken voor je aan en uit gaat.
    if (digitalRead(B1push) == 1){
      if (count1 == 0){
       count1 = time;
       digitalWrite(B1L1, true);
      }else{
       if (count1 < (time-1)){
         count1 = 1;
         secondenteller = 0;
         digitalWrite(B1L1, false);
       }
      } 
    }

  secondenteller++; //onderstaand deel loopt 1 keer / seconde
  if (secondenteller == 10){
    secondenteller = 0;
    if (count1 > 0){
      count1--;
    }
    if (count1 > 0){
      digitalWrite(B1L1, true);
    }else{
      digitalWrite(B1L1, false);
    }
  }
  
}
