#include <Arduino.h>
#line 1 "c:\\Users\\Tomek\\Documents\\Arduino\\kraft_amp_dele\\kraft_amp_dele.ino"
#line 1 "c:\\Users\\Tomek\\Documents\\Arduino\\kraft_amp_dele\\kraft_amp_dele.ino"
#include <LiquidCrystal.h>

// C++ code
//

#define RELAY_PIN				8
#define MAINS_LED_PIN			9
#define GENERATOR_LED_PIN		10
#define MAINS_DETECT_PIN		2
#define GENERATOR_DETECT_PIN	3

volatile bool mainsDetected = false;
volatile bool generatorDetected = false;

enum {MAINS,NOPOWER,STARTING,STOPPING,STARTED,STOPPED,GENERATOR,DETECTED,ENDMSG};

const char *messages[] = {
  " Mains          ",
  " No power detc. ",
  " Start generat. ",
  " Stopping       ",
  " Started        ",
  " Stopped        ",
  " Generator      ",
  " Detected       "
};

const int rs = 12, en = 11, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#line 31 "c:\\Users\\Tomek\\Documents\\Arduino\\kraft_amp_dele\\kraft_amp_dele.ino"
void setup();
#line 56 "c:\\Users\\Tomek\\Documents\\Arduino\\kraft_amp_dele\\kraft_amp_dele.ino"
void loop();
#line 5 "c:\\Users\\Tomek\\Documents\\Arduino\\kraft_amp_dele\\functions.ino"
void checkMains();
#line 11 "c:\\Users\\Tomek\\Documents\\Arduino\\kraft_amp_dele\\functions.ino"
void checkGenerator();
#line 17 "c:\\Users\\Tomek\\Documents\\Arduino\\kraft_amp_dele\\functions.ino"
void detectPower();
#line 24 "c:\\Users\\Tomek\\Documents\\Arduino\\kraft_amp_dele\\functions.ino"
void printMessage(int first,int second);
#line 31 "c:\\Users\\Tomek\\Documents\\Arduino\\kraft_amp_dele\\kraft_amp_dele.ino"
void setup()
{
  //Serial.begin(115200);
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print("** Kraft&Dele **");
  lcd.setCursor(0,1);
  lcd.print("***  Switch  ***");
 
  pinMode(MAINS_DETECT_PIN, INPUT_PULLUP);
  pinMode(GENERATOR_DETECT_PIN, INPUT_PULLUP); 

  pinMode(RELAY_PIN, OUTPUT);
  pinMode(MAINS_LED_PIN, OUTPUT);
  pinMode(GENERATOR_LED_PIN, OUTPUT);

  digitalWrite(RELAY_PIN,HIGH); // relay open, generator cut-off

  attachInterrupt(digitalPinToInterrupt(MAINS_DETECT_PIN),checkMains,FALLING);
  attachInterrupt(digitalPinToInterrupt(GENERATOR_DETECT_PIN),checkGenerator,FALLING);

  interrupts();
     
}

void loop()
{
  int count = 0; 
  
  detectPower();

  if(mainsDetected == true){
    printMessage(MAINS,DETECTED);
    // disable relay
    digitalWrite(RELAY_PIN,HIGH);
    // stop_generator()
    while(generatorDetected == true){
      printMessage(STOPPING,GENERATOR);
      //delay(10);
      detectPower();
    }  
    
  }
  else if(generatorDetected){
    printMessage(GENERATOR,DETECTED);
    // enable relay
    digitalWrite(RELAY_PIN,LOW);
  }
  else{    
      printMessage(NOPOWER,STARTING);
      // start_generator()
      while(mainsDetected == false && generatorDetected == false){
        //delay(10);   
        detectPower();
      }
    }       
  
}
#line 1 "c:\\Users\\Tomek\\Documents\\Arduino\\kraft_amp_dele\\functions.ino"
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

