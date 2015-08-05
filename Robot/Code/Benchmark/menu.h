#include <avr/EEPROM.h>
 
class MenuItem{
public:
  String    Name;
  int Value;
  uint16_t* EEPROMAddress;
  int Max;
  static uint16_t MenuItemCount;
  MenuItem(String name, int max){
    MenuItemCount++;
    EEPROMAddress = (uint16_t*)(2 * MenuItemCount);
    Name = name;
    Value = (int) eeprom_read_word(EEPROMAddress);
    Max = max;
  }
  void Save(){
    eeprom_write_word(EEPROMAddress, Value);
  }
};
 
uint16_t MenuItem::MenuItemCount = 0;
/* Add the menu items here */
MenuItem Speed = MenuItem("Speed", 255);
MenuItem PTape = MenuItem("P-Tape", 1023);
MenuItem DTape = MenuItem("D-Tape", 1023);
MenuItem Thresh = MenuItem("Thresh-Tape", 1023);
MenuItem PIR = MenuItem("P-IR", 1023);
MenuItem DIR = MenuItem("D-IR", 1023);
MenuItem PArm = MenuItem("PArm-Base", 1023);
MenuItem DArm = MenuItem("DArm-Base", 1023);
MenuItem IArm = MenuItem("IArm-Base", 1023);
MenuItem PArm1 = MenuItem("PArm-1", 1023);
MenuItem DArm1 = MenuItem("DArm-1", 1023);
MenuItem IArm1 = MenuItem("IArm-1", 1023);
MenuItem ITape = MenuItem("I-Tape", 1023);
MenuItem menuItems[] = {Speed, PTape, DTape, Thresh, PIR, DIR, PArm, DArm, IArm, PArm1, DArm1, IArm1, ITape};
 
void Menu(){
  LCD.clear(); LCD.home();
  LCD.print("Entering menu");
  delay(500);
 
  while (true){
    /* Show MenuItem value and knob value */
    int menuIndex = knob(6) * (MenuItem::MenuItemCount) / 1024;
    int val = map(knob(7), 0, 1024, 0, menuItems[menuIndex].Max + 1);
    LCD.clear(); LCD.home();
    LCD.print(menuItems[menuIndex].Name); LCD.print(" "); LCD.print(menuItems[menuIndex].Value);
    LCD.setCursor(0, 1);
    LCD.print("Set to "); LCD.print(val); LCD.print("?");
    delay(100);
 
    /* Press start button to save the new value */
    if (startbutton()){
      delay(50);
      if (startbutton()){
        menuItems[menuIndex].Value = val;
        menuItems[menuIndex].Save();
        delay(250);
      }
    }
 
    /* Press stop button to exit menu */
    if (stopbutton()){
      delay(50);
      if (stopbutton()){
        LCD.clear(); LCD.home();
        LCD.print("Leaving menu");
        delay(500);
        return;
      }
    }
  }
}
