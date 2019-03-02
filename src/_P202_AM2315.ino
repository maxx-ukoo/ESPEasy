#ifdef USES_P202
//#######################################################################################################
//#################### Plugin 202 Temperature and Humidity Sensor AM2315 ##############
//#######################################################################################################
//
// Temperature and Humidity Sensor AM2320
// written by https://github.com/maxx-ukoo
// based on this library: https://github.com/adafruit/Adafruit_AM2315
//

#include <AM2315.h>

#define PLUGIN_202
#define PLUGIN_ID_202        202
#define PLUGIN_NAME_202       "Environment - AM2315 [TESTING]"
#define PLUGIN_VALUENAME1_202 "Temperature"
#define PLUGIN_VALUENAME2_202 "Humidity"

AM2315* Plugin_202_AM2315 = NULL;

boolean Plugin_202(byte function, struct EventStruct *event, String& string)
{
  boolean success = false;

  switch (function)
  {
    case PLUGIN_DEVICE_ADD:
      {
        Device[++deviceCount].Number = PLUGIN_ID_202;
        Device[deviceCount].Type = DEVICE_TYPE_I2C;
        Device[deviceCount].VType = SENSOR_TYPE_TEMP_HUM;
        Device[deviceCount].Ports = 0;
        Device[deviceCount].PullUpOption = false;
        Device[deviceCount].InverseLogicOption = false;
        Device[deviceCount].FormulaOption = true;
        Device[deviceCount].ValueCount = 2;
        Device[deviceCount].SendDataOption = true;
        Device[deviceCount].TimerOption = true;
        Device[deviceCount].GlobalSyncOption = true;
        break;
      }
    case PLUGIN_INIT:
      {
        if (!Plugin_202_AM2315) {
              Plugin_202_AM2315 = new AM2315;
              Plugin_202_AM2315->begin();
              success = true;
        }
        break;
      }
    case PLUGIN_GET_DEVICENAME:
      {
        string = F(PLUGIN_NAME_202);
        break;
      }

    case PLUGIN_GET_DEVICEVALUENAMES:
      {
        strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[0], PSTR(PLUGIN_VALUENAME1_202));
        strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[1], PSTR(PLUGIN_VALUENAME2_202));
        break;
      }

    case PLUGIN_WEBFORM_LOAD:
      {
        success = true;
        break;
      }

    case PLUGIN_WEBFORM_SAVE:
      {
        success = true;
        break;
      }

    case PLUGIN_READ:
      {
        float humidity, temp;
        Plugin_202_AM2315->readTemperatureAndHumidity(temp, humidity);
        UserVar[event->BaseVarIndex] = temp;
        UserVar[event->BaseVarIndex + 1] = humidity;
        String log = F("AM2315: Temperature: ");
        log += UserVar[event->BaseVarIndex];
        addLog(LOG_LEVEL_INFO, log);
        log = F("AM2315: Humidity: ");
        log += UserVar[event->BaseVarIndex + 1];
        addLog(LOG_LEVEL_INFO, log);
        success = true;
      }
  }
  return success;
}


#endif // USES_P202
