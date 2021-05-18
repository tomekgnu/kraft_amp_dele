extern volatile bool mainsDetected;
extern volatile bool generatorDetected;
extern const char *messages[];

void checkMains(){
    volatile int cnt = 0;    
    while(cnt++ < 16000);
    mainsDetected = !digitalRead(MAINS_DETECT_PIN);
}

void checkGenerator(){
    volatile int cnt = 0;
    while(cnt++ < 16000);
    generatorDetected = !digitalRead(GENERATOR_DETECT_PIN);
}

void detectPower(){
    mainsDetected = !digitalRead(MAINS_DETECT_PIN);
    generatorDetected = !digitalRead(GENERATOR_DETECT_PIN);
    digitalWrite(MAINS_LED_PIN,mainsDetected);
    digitalWrite(GENERATOR_LED_PIN,generatorDetected);
}

void printMessage(int first,int second){
    static int upper = -1,lower = -1;
    if(first == upper && second == lower)
        return;
    if(first < 0 || first >= ENDMSG)
        return;
    if(second < 0 || second >= ENDMSG)
        return;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(messages[first]);
    lcd.setCursor(0,1);
    lcd.print(messages[second]);
    upper = first;
    lower = second;
   
}
