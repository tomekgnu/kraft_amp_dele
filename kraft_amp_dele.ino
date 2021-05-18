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