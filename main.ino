/*
  Void-Josto thermostat
*/

const int L1[] = { 32, 33 };
const int L2[] = { 24, 25 };
const int push[] = { 30, 31 };

int time = 10;			//(in seconden)
int count[] = { 0, 0 };		//loopt in seconden

int secondenteller = 0;

void setup()
{
    pinMode(L1[0], OUTPUT);
    pinMode(L2[0], OUTPUT);
    pinMode(push[0], INPUT);
    pinMode(L1[1], OUTPUT);
    pinMode(L2[1], OUTPUT);
    pinMode(push[1], INPUT);
    Serial.begin(9600);
}

void loop()
{
    delay(100);			//we loopen 10 keer / seconde

    //teller aan en uit zetten. Je kan minstens 1 seconde drukken voor je aan en uit gaat.
    for (int i = 0; i <= 1; i++) {
	Serial.println(push[i], DEC);
	if (digitalRead(push[i]) == 1) {
	    if (count[i] == 0) {
		count[i] = time;
		digitalWrite(L1[i], true);
	    } else {
		if (count[i] < (time - 1)) {
		    count[i] = 1;
		    secondenteller = 0;
		    digitalWrite(L1[i], false);
		}
	    }
	}
    }

    secondenteller++;		//onderstaand deel loopt 1 keer / seconde
    if (secondenteller == 10) {
	secondenteller = 0;
	for (int i = 0; i <= 1; i++) {
	    if (count[i] > 0) {
		count[i]--;
	    }
	    if (count[i] > 0) {
		digitalWrite(L1[i], true);
	    } else {
		digitalWrite(L1[i], false);
	    }
	}
    }
}
