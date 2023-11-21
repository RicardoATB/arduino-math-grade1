#if 0
Highlights:
- Use of ternary operation
- Static
- Representing loop in a flowchart
#endif

//------------------------------------

#define MAX_NUM_CHAR 32
#define MIN 1
#define MAX_1 11
#define MAX_2 21

byte LEDpins[] = {13, 12, 8, 7, 3, 2}; // GREEN: 13-/12+, YELLOW: 8-/7+, RED: 3-/2+
char receivedChars[MAX_NUM_CHAR];
int guessNum;
int a,b,op;
int correctAnswer;
boolean convertStrToInt = false;

void setup() {
    randomSeed(analogRead(0));
    Serial.begin(9600);
    Serial.println("Let's do some Math!");

    for (byte i = 0; i < (sizeof(LEDpins)); i++) {
        pinMode(LEDpins[i], OUTPUT);
        digitalWrite(LEDpins[i], LOW); // all LEDs start turned off
    }
}

void generateQuestion() {
    randNumber(MAX_1, MAX_2);
    op = random(0,2); // 1 (+), 0 (-)
    (op) ? correctAnswer = a + b : correctAnswer = a - b;
    guessNum = -1;
}

void displayQuestion() {
    Serial.print(a);
    (op) ? Serial.print(" + ") : Serial.print(" - ");
    Serial.print((String) b + " = ");
}

void randNumber(int m1, int m2) {
    do {
        a = random(MIN, m1);
        b = random(MIN, m2);
    } while (b > a);
}

void LEDgyr(byte gLED, byte yLED, byte rLED) {
    (gLED) ? digitalWrite(12,HIGH) : digitalWrite(12, LOW);
    (yLED) ? digitalWrite(7,HIGH) : digitalWrite(7, LOW);
    (rLED) ? digitalWrite(2,HIGH) : digitalWrite(2, LOW);
}

void turnLEDon() {
    if (guessNum == correctAnswer)
        LEDgyr(1,0,0) ;
    else if (guessNum == (correctAnswer - 1) || guessNum == (correctAnswer + 1))
        LEDgyr(0,1,0);
    else
        LEDgyr(0,0,1);
}

void getUserInput() {
    char c = NULL;
    char CR = '\r'; // Carriage Return (when ENTER is pressed)
    static byte idx = 0;	// "byte" type [0-255] >> MAX_NUM_CHAR
    // "static" to remember position in the string after each function call
    // "Variables declared as static will only be created and initialized the first time a function is called."

    while (c != CR) {
        while (Serial.available() == 0) {} // stay here waiting for user input
        c = Serial.read(); // read char typed

        if (c != CR)
            receivedChars[idx++] = c; // equivalent to: receivedChars[idx] = c; idx++;
        else {
            receivedChars[idx] = '\0'; // terminate the string with null character
            idx = 0; // resetting char array index to the next reading
            //convertStrToInt = true; // now we can convert the string to int
        }
    }

    guessNum = ((String)receivedChars).toInt(); // convert String to Int
    Serial.println();
    c = NULL;
}

void loop() {
    generateQuestion();
    do {
        displayQuestion();
        getUserInput();
        turnLEDon();
        delay(1000);
        LEDgyr(0,0,0); // turn LEDs off
    } while (guessNum != correctAnswer);
}
