#include <LiquidCrystal.h>


// Initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
 

int temperatures[4]={10,20,30,40};
int matrix[4][2]={{5,0},{10,0},{5,1},{10,1}};//matrix with menu coordinates
char* myMenuTable[]={"  Pump On   ", "  Pump Off  ", "  Heat On   ", "  Heat Off  "};
//"Menu 4", "Menu 5","Menu 6","^**************"};
enum{tempPumpOn, tempPumpOff, tempHeaterOn, tempHeaterOff};



class BCursor //blinking cursor without delay
{
  protected: static int NumCursors;//shoul check if any cursor is initialized
  
  public:
  long OnTime;//cursor On time
  long OffTime;//cursor Off time
  unsigned long previousMillis;    // stores last time cursor state was updated
  bool CState;//cursor state
  BCursor(long on, long off)//init cursor
  {
     OnTime=on; OffTime=off;
     CState=false;
     NumCursors++;  
  }
  int GecCCount(){//just mistype
    return this->NumCursors;
  }

  void Update(bool top)//here we updating cursor state
  {
    // check to see if it's time to change the state of the LED
    unsigned long currentMillis = millis();
    if (top){//if we are in top level of menu - blink!
      if((CState) && (currentMillis - previousMillis >= OnTime))
      {
        CState = !CState;  // Turn it off
        previousMillis = currentMillis;  // Remember the time
        lcd.noCursor();
      }
      else if ((!CState) && (currentMillis - previousMillis >= OffTime))
      {
        CState = !CState;  // turn it on
        lcd.cursor();
        previousMillis = currentMillis;   // Remember the time
      }
    }else{CState=true; lcd.cursor();}
  }
  
};

class HM//menu help 
{
  friend class MMenu;
  protected: static int NumHelpMenues;//shoul check if any cursor is initialized
  private: long OnTime, OffTime;//menu On time
  int currentMenuItem;
  public:
  int HelpCur;
  unsigned long previousMillis;    // stores last time cursor state was updated
  bool MState;//mstate is the state of menu help = updating state=true, not updating = false. We should not update menu help every loop
  HM()//init cursor
  {
  
     HelpCur=0;
     MState=false;
     OnTime=300;
     OffTime=400;
     NumHelpMenues++;  
  }
  int GecCCount(){
    return this->NumHelpMenues;
  }

  //Display Menu Option based on Index.
  void UpdateHelp(){
  //uint8_t a,b;
 // lcd.getCursor(a,b);


    unsigned long currentMillis = millis();
        //   Serial.println(currentMillis - previousMillis);
       //     Serial.print(MState );
    if((!MState)&&(currentMillis - previousMillis >= 300))
      {

        MState=!MState;
         previousMillis = currentMillis;   // Remember the time
         if (HelpCur>10) HelpCur=0;
         for(int x=0;x<4;x++){
              lcd.setCursor(matrix[x][0]-2,matrix[x][1]);
              lcd.print(myMenuTable[x][HelpCur]);
              lcd.setCursor(matrix[x][0]-1,matrix[x][1]);
              lcd.print(myMenuTable[x][HelpCur+1]);
              lcd.setCursor(matrix[x][0],matrix[x][1]); 
          }
          lcd.setCursor(matrix[currentMenuItem][0],matrix[currentMenuItem][1]);
        
      }
      else if ((MState)&&(currentMillis - previousMillis >= 400))
      {
        MState=!MState;
        previousMillis = currentMillis;  // Remember the time
        HelpCur+=1;//forward printed letters position

      }

  }

  
};


class MMenu{//main menu
  //friend class  HM;
  protected:static int Mcount;  static int ccc;
  private:     int currentMenuItem;
  public:
    int state;// = 0;
   
    int  stateFiltered, startMillis;
    int x;// = analogRead (0);
    int selected;//=1;
    bool top;  
    
    //States for the menu.
    HM m;
    int lastState,ll;
    int MenuItemsCounter; 
    MMenu(){
      
      selected = 1;  
     // state    = 0;
      top = true;
      //temperatures={10,20,30,40};
      //States for the menu.
      currentMenuItem = 0;
      lastState = 0;
      MenuItemsCounter=0; 
    }
    void Update(){
      m.UpdateHelp();//update help menu - running letters
      x  = analogRead (0);
      //Check analog values from LCD Keypad Shield - DFBot values only!
      if (x < 50) {
         //Right
     
          } else if (x < 150) {
         //Up
          state = 1;
          } else if (x < 320){
          //Down
          state = 2;
          // Serial.println("@@@@@@@@@@@@@@@@@@@@2");
          } else if (x < 500){
          //Left
          } else if (x < 800){
          //Select
           state = 3;
            top=!top;
          }else{
            Serial.println("really nul");
            state = 0;//no key pressed or key released
      }//end all ifs
            Serial.print(x);Serial.print(" ");Serial.println(state); 
           
    if (state != lastState) {//edge

      if (state!=0){
        //detect at first pos edge
          startMillis=millis();//remember millis on state changed from 0
          //Serial.println(startMillis);  
         // stateFiltered=state;
      }else if ((millis()-startMillis)>30){ 
         //here we should handle an event longer than 30ms

      //Serial.println(lastState);
         
         //  state=stateFiltered;
           //if we are in top menu
          if(top){
            if (state == 1) {
          
               currentMenuItem = currentMenuItem - 1;  
                if (currentMenuItem < 0) {
                    currentMenuItem = 3; 
                 }
                //displayMenu(currentMenuItem); 
                 } else if (state == 2) {
              //If Down
            
                currentMenuItem = currentMenuItem + 1;  
               if (currentMenuItem > 3) {
                    currentMenuItem = 0; 
                 }
          
           } else if (state == 3) {
               //If Selected - something else
             //  selectMenu(currentMenuItem); 
            }
           displayMenu(currentMenuItem);
            //Save the last State to compare.
      
           } 
         else{
        //here we should change our values in accordance with up/down and menuItem
          if (state == 1){
       
             temperatures[currentMenuItem]+=1;
       
          }else if (state == 2){
            temperatures[currentMenuItem]-=1;
           }
           //temperature limits
           if (temperatures[currentMenuItem]>80) temperatures[currentMenuItem]=80;
           if (temperatures[currentMenuItem]<2) temperatures[currentMenuItem]=2;
           displayMenu(currentMenuItem);
        }
        
      }//lastState = state;   
    }
   }
   void displayMenu(int currentMenuItem) {
     //just show all values and states here
     //lcd.clear();
      //ccc++;
      //Serial.println(ccc);
      //pump state showed here
      lcd.setCursor(0,0);
      lcd.print("P");
      //heater state showed here
      lcd.setCursor(0,1);
      lcd.print("H");
  
      //temp parameters matrix here
        for(int x=0;x<4;x++){
          lcd.setCursor(matrix[x][0],matrix[x][1]);
          lcd.print(temperatures[x]);
        }
        lcd.setCursor(matrix[currentMenuItem][0],matrix[currentMenuItem][1]);
        //clock to be showed here
      // delay(50);
    }
};

int HM::NumHelpMenues=0;
int MMenu::ccc=0;
int BCursor::NumCursors = 0;//init NumCursors at program start
//values should be loaded from SD card!
int HelpCur=0;
boolean top=true;//чё такое топ блять?? Запомнить - верхний уровень меню.Top menu selected - cursor should blink when it is true.
class MMenu;
BCursor cur(400,150);

MMenu Menu;

    int freeRam () 
    {
      extern int __heap_start, *__brkval; 
      int v; 
      return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
    }
void setup() {
   lcd.clear();
   //Set the characters and column numbers.
   lcd.begin(16, 2);

   
   Serial.begin(9600); 
   //int t= cur.GecCCount();
   
   //Print default title.
   
   Menu.displayMenu(tempPumpOn);
   
   
}
   unsigned long startMillis, endMillis; 
   int stateFiltered;
void loop() {

  //Call the main menu.
  //  lcd.setCursor(0,0);
  //lcd.print("P");
  Menu.Update();
  cur.Update(top);
// Serial.println(freeRam());


  //delay(500);
}




