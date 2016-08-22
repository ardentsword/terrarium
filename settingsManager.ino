#include <Arduino.h>

/* LoadAndSaveSettings
 * Joghurt 2010
 * Demonstrates how to load and save settings to the EEPROM
 */
// Contains EEPROM.read() and EEPROM.write()
#include <EEPROM.h>

// Tell it where to store your config data in EEPROM
#define CONFIG_START 32

bool loadSettings() {
  // To make sure there are settings, and they are YOURS!
  // If nothing is found it will use the default settings.
  if (EEPROM.read(CONFIG_START + 0) == CONFIG_VERSION[0] &&
      EEPROM.read(CONFIG_START + 1) == CONFIG_VERSION[1] &&
      EEPROM.read(CONFIG_START + 2) == CONFIG_VERSION[2])
  {
    for (unsigned int t=0; t<sizeof(mySettings); t++){
      *((char*)&mySettings + t) = EEPROM.read(CONFIG_START + t);
    }
    return true;
  }else{
    return false;
  }
}

void saveSettings() {
  for (unsigned int t=0; t<sizeof(mySettings); t++)
    EEPROM.write(CONFIG_START + t, *((char*)&mySettings + t));
}
