#include <EEPROM.h>



void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

for (int i = 0; i < 2048; i++) {
 EEPROM.write(i, 0);
 }
  EEPROM.commit();
delay(500);

}
