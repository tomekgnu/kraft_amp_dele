# 1 "c:\\Users\\Tomek\\Documents\\Arduino\\kraft_amp_dele\\kraft_amp_dele.ino"
# 1 "c:\\Users\\Tomek\\Documents\\Arduino\\kraft_amp_dele\\kraft_amp_dele.ino"
# 2 "c:\\Users\\Tomek\\Documents\\Arduino\\kraft_amp_dele\\kraft_amp_dele.ino" 2

// C++ code
//







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

  pinMode(2, 0x2);
  pinMode(3, 0x2);

  pinMode(8, 0x1);
  pinMode(9, 0x1);
  pinMode(10, 0x1);

  digitalWrite(8,0x1); // relay open, generator cut-off

  attachInterrupt(((2) == 2 ? 0 : ((2) == 3 ? 1 : -1)),checkMains,2);
  attachInterrupt(((3) == 2 ? 0 : ((3) == 3 ? 1 : -1)),checkGenerator,2);

  
# 52 "c:\\Users\\Tomek\\Documents\\Arduino\\kraft_amp_dele\\kraft_amp_dele.ino" 3
 __asm__ __volatile__ ("sei" ::: "memory")
# 52 "c:\\Users\\Tomek\\Documents\\Arduino\\kraft_amp_dele\\kraft_amp_dele.ino"
             ;

}

void loop()
{
  int count = 0;

  detectPower();

  if(mainsDetected == true){
    printMessage(MAINS,DETECTED);
    // disable relay
    digitalWrite(8,0x1);
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
    digitalWrite(8,0x0);
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
# 1 "c:\\Users\\Tomek\\Documents\\Arduino\\kraft_amp_dele\\functions.ino"
extern volatile bool mainsDetected;
extern volatile bool generatorDetected;
extern const char *messages[];

void checkMains(){
    volatile int cnt = 0;
    while(cnt++ < 16000);
    mainsDetected = !digitalRead(2);
}

void checkGenerator(){
    volatile int cnt = 0;
    while(cnt++ < 16000);
    generatorDetected = !digitalRead(3);
}

void detectPower(){
    mainsDetected = !digitalRead(2);
    generatorDetected = !digitalRead(3);
    digitalWrite(9,mainsDetected);
    digitalWrite(10,generatorDetected);
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
