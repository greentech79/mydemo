//example use of LCD4Bit_mod library

#include <LCD4Bit_mod.h> 
//create object to control an LCD.  
//number of lines in display=1
LCD4Bit_mod lcd = LCD4Bit_mod(2); 

//Key message
char msgs[5][15] = {"0", 
                    "1 ", 
                    "Down Key OK  ", 
                    "Left Key OK  ", 
                    "Select Key OK" };
                    
int  adc_key_val[5] ={30, 150, 360, 535, 760 };
int NUM_KEYS = 5;
char CHAR_NUMBERS[10][3] = {"0","1","2","3","4","5","6","7","8","9"};


int LEFT = 535;
int RIGHT = 30;
int UP = 150;
int DOWN = 360;
int SELECT = 760;

int key=-1;
int oldkey=-1;
bool KEY_LOCK=true;

int CURRENT_OPERATION =0;

void setup() { 
  pinMode(13, OUTPUT);  //we'll use the debug LED to output a heartbeat
 Serial.begin(57600);
  lcd.init();
  //optionally, now set up our application-specific display settings, overriding whatever the lcd did in lcd.init()
  //lcd.commandWrite(0x0F);//cursor on, display on, blink on.  (nasty!)
   lcd.clear();
  lcd.printIn("KEYPAD testing... pressing");
    
}

void loop() {
  int lastKeyPressed = 0;
  
  if(CURRENT_OPERATION == 0){
    
    Serial.println("Waiting for Select Key...");
     KEY_LOCK = true;
    while(adc_key_val[lastKeyPressed] != SELECT) {
      lastKeyPressed = read_A_key();
      delay(300);
    }
     CURRENT_OPERATION =1;
     Serial.println("Pressed:" + lastKeyPressed);
    // lastKeyPressed = -1;
  }

  if(CURRENT_OPERATION == 1) {
    Serial.println("Choose a number...");
     int numberValue = read_A_numberAT(0);
    CURRENT_OPERATION = 0;
     Serial.println(numberValue);
     lcd.clear();
     lcd.cursorTo(1,0);
    lcd.printIn("Number selected");
    lcd.cursorTo(2,0);
    lcd.printIn(CHAR_NUMBERS[numberValue]);
  }
   // delay(1000);
  
  
//  int numberValue = read_A_numberAT(0);
  
 // Serial.println("Number value:"+numberValue);
}

int read_A_numberAT(int cursor_position){
  int lastKeyPressed = 0;
  int number=0;
  int prevNumber = -1;
  bool keepReading = true;
   
  while(keepReading) {
    Serial.println(number);
    int aKeyPressed = -1;
    KEY_LOCK = true;
    while (aKeyPressed == -1) {
      aKeyPressed = read_A_key();
      delay(100);
    }
    KEY_LOCK = false;
    if(aKeyPressed != -1) {
      aKeyPressed = adc_key_val[aKeyPressed];
    }
    if(aKeyPressed == LEFT || aKeyPressed == RIGHT){
        keepReading = false;
        break;
    }
    else if(aKeyPressed == UP ){
      number= number+1;
    }
    else if(aKeyPressed == DOWN){
      number= number - 1;
    } 
    else if(aKeyPressed == SELECT){
        keepReading = false;
        break;
    }
     if(number < 0) number = 0;
     if(number > 9) number = 9;
    if(prevNumber != number) {
    prevNumber = number;
    lcd.cursorTo(2,cursor_position);
    lcd.printIn(CHAR_NUMBERS[number]);
    }
  }
  return number;
}

int read_A_key(){
  if(KEY_LOCK) {
  int adc_key_in = analogRead(0);    // read the value from the sensor  
  key = get_key(adc_key_in);            // convert into key press
  if (key != oldkey)            // if keypress is detected
  {
    delay(50);    // wait for debounce time
    adc_key_in = analogRead(0);    // read the value from the sensor  
    key = get_key(adc_key_in);            // convert into key press
    if (key != oldkey)        
    {     
      oldkey = key;
      if (key >=0){
        return key;
      }
    }
  }
  }
  return -1;
}

// Convert ADC value to key number
int get_key(unsigned int input)
{
	int k;
	for (k = 0; k < NUM_KEYS; k++) {
		if (input < adc_key_val[k]) {
    return k;
    }
	}
  if (k >= NUM_KEYS) {
    k = -1;     // No valid key pressed
  }
    return k;
}
