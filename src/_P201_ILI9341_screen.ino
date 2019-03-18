#ifdef USES_P201

#include <TFT_eSPI.h>
#include <JPEGDecoder.h>

// Return the minimum of two values a and b
#define minimum(a,b)     (((a) < (b)) ? (a) : (b))

TFT_eSPI* Plugin_201_TFT = NULL;
TFT_eSPI_Button* Plugin_201_button_1 = NULL;
TFT_eSPI_Button* Plugin_201_button_2 = NULL;
TFT_eSPI_Button* Plugin_201_button_3 = NULL;

#define PLUGIN_201
#define PLUGIN_ID_201     201
#define PLUGIN_NAME_201   "ILI9341 touch buttons"
#define PLUGIN_VALUENAME1_201 "button1"
#define PLUGIN_VALUENAME2_201 "button2"
#define PLUGIN_VALUENAME3_201 "button3"

#define PLUGIN_VALUE_1 0
#define PLUGIN_VALUE_2 1
#define PLUGIN_VALUE_3 2

#define BUTTONS_MAX 3
#define PLUGIN_201_TYPE_SWITCH 0
#define PLUGIN_201_BUTTON_TYPE_NORMAL_SWITCH 0


#define KEY_FONT &tahoma9pt7b
#define FONT_7 &tahoma7pt7b
#define FONT_9 &tahoma9pt7b
#define FONT_12 &tahoma12pt7b
#define FONT_18 &tahoma18pt7b
#define FONT_24 &tahoma24pt7b
#define FONT_48 &tahoma48pt7b
#define FONT_96 &tahoma96pt7b


#define BLACK_SPOT
#define BACKGROUND TFT_BLACK

void Plugin_201_touch_calibrate()
{
  uint16_t calData[5] = { 185, 3488, 259, 3542, 1 };
  Plugin_201_TFT->setTouch(calData);
}

void Plugin_201_draw_time(String time) {
  Plugin_201_TFT->fillRect(130, 1, 69, 20, BACKGROUND);
  Plugin_201_TFT->setTextColor(TFT_WHITE, BACKGROUND);
  Plugin_201_TFT->setFreeFont(FONT_12);
  Plugin_201_TFT->drawString(time, 130, 1, 1);
}

void Plugin_201_draw_temperature(String temperature) {
  Plugin_201_TFT->fillRect(170, 20, 150, 98, BACKGROUND);
  Plugin_201_TFT->setTextColor(TFT_BLUE, BACKGROUND);
  Plugin_201_TFT->setFreeFont(FONT_48);
  Plugin_201_TFT->drawString(temperature, 170, 20, 1);
  Plugin_201_TFT->setFreeFont(FONT_9);
  Plugin_201_TFT->drawString("o", 273, 25, 1);
  Plugin_201_TFT->setFreeFont(FONT_24);
  Plugin_201_TFT->drawString("C", 284, 25, 1);
}

void Plugin_201_draw_humidity(String humidity) {
  Plugin_201_TFT->fillRect(10, 20, 150, 98, BACKGROUND);
  Plugin_201_TFT->setTextColor(TFT_BLUE, BACKGROUND);
  Plugin_201_TFT->setFreeFont(FONT_48);
  Plugin_201_TFT->drawString(humidity, 10, 20, 1);
  Plugin_201_TFT->setFreeFont(FONT_24);
  Plugin_201_TFT->drawString("%", 110, 25, 1);
}

void Plugin_201_draw_screen()
{
  Plugin_201_TFT->drawFastHLine(10, 0, 110, TFT_GREENYELLOW);
  Plugin_201_TFT->drawFastHLine(10, 18, 110, TFT_GREENYELLOW);
  Plugin_201_TFT->drawFastHLine(200, 0, 110, TFT_GREENYELLOW);
  Plugin_201_TFT->drawFastHLine(200, 18, 110, TFT_GREENYELLOW);
  Plugin_201_TFT->drawFastHLine(10, 120, 300, TFT_GREENYELLOW);

  Plugin_201_TFT->setTextColor(TFT_GREENYELLOW);
  Plugin_201_TFT->setFreeFont(FONT_7);
  Plugin_201_TFT->drawString("HUMIDITY", 40, 3);
  Plugin_201_TFT->drawString("TEMPERATURE", 210, 3);
  Plugin_201_draw_time(F("--:--"));
  Plugin_201_draw_button_1(0, true);
  Plugin_201_draw_button_2(0, true);
  Plugin_201_draw_button_3(0, true);
}

void Plugin_201_draw_button_1(int8_t state, boolean refreshImage)
{
  Plugin_201_draw_button("fan.jpg", 30, 130, state, refreshImage);
}

void Plugin_201_draw_button_2(int8_t state, boolean refreshImage)
{
  Plugin_201_draw_button("heat.jpg", 130, 130, state, refreshImage);
}

void Plugin_201_draw_button_3(int8_t state, boolean refreshImage)
{
  Plugin_201_draw_button("light.jpg", 230, 130, state, refreshImage);
}

void Plugin_201_draw_button(const char *btnImage, int x, int y, int8_t state, boolean refreshImage)
{
  if (refreshImage) {
    Plugin_201_draw_jpeg(btnImage, x, y+20);
  }
  Plugin_201_TFT->fillRect(x+5, y, 70, 15, BACKGROUND);
  Plugin_201_TFT->setFreeFont(KEY_FONT);
  Plugin_201_TFT->setTextColor(TFT_WHITE, BACKGROUND);
  if (state == 0 ) {
    Plugin_201_TFT->drawString("OFF", x+20, y);
  }
  if (state == 1 ) {
    Plugin_201_TFT->drawString("ON", x+25, y);
  }
  if (state == 2) {
    Plugin_201_TFT->drawString("AUTO", x+15, y);
  }
}

boolean Plugin_201(byte function, struct EventStruct *event, String& string)
{
  boolean success = false;
  static int8_t switchstate[3];
  static uint32_t touchDelay = 0;

  switch (function)
  {
    case PLUGIN_DEVICE_ADD:
      {
        Device[++deviceCount].Number = PLUGIN_ID_201;
        Device[deviceCount].Type = DEVICE_TYPE_TRIPLE;
        Device[deviceCount].VType = SENSOR_TYPE_TRIPLE;
        Device[deviceCount].Ports = 0;
        Device[deviceCount].PullUpOption = false;
        Device[deviceCount].InverseLogicOption = false;
        Device[deviceCount].FormulaOption = false;
        Device[deviceCount].ValueCount = 3;
        Device[deviceCount].SendDataOption = true;
        Device[deviceCount].TimerOption = true;
        Device[deviceCount].TimerOptional = false;
        Device[deviceCount].GlobalSyncOption = true;
        break;
      }

    case PLUGIN_GET_DEVICENAME:
      {
        string = F(PLUGIN_NAME_201);
        break;
      }

    case PLUGIN_GET_DEVICEVALUENAMES:
      {
        strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[0], PSTR(PLUGIN_VALUENAME1_201));
        strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[1], PSTR(PLUGIN_VALUENAME2_201));
        strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[2], PSTR(PLUGIN_VALUENAME3_201));
        break;
      }

    case PLUGIN_GET_DEVICEGPIONAMES:
      {
        event->String1 = formatGpioName_input(F("CS"));
        event->String2 = formatGpioName_input(F("D/C"));
        event->String3 = formatGpioName_input(F("SDI(MOSI) T_DIN"));
        break;
      }

    case PLUGIN_WEBFORM_LOAD:
      {
        addFormNote(F("1st=GPIO15, 2nd=GPIO02, 3rd=GPIO13"));
        addFormCheckBox(F("Send Boot state"),F("plugin_201_boot"),
        		Settings.TaskDevicePluginConfig[event->TaskIndex][0]);
        char timeTemplate[3][40];
        LoadCustomTaskSettings(event->TaskIndex, (byte*)&timeTemplate, sizeof(timeTemplate));
        addHtml(F("<TR><TD>Time format"));
        addHtml(F(":<TD><input type='text' size='40' maxlength='40' name='Plugin_201_template_0"));
        addHtml(F("' value='"));
        addHtml(timeTemplate[0]);
        addHtml(F("'>"));

        addHtml(F("<TR><TD>Temperature format"));
        addHtml(F(":<TD><input type='text' size='40' maxlength='40' name='Plugin_201_template_1"));
        addHtml(F("' value='"));
        addHtml(timeTemplate[1]);
        addHtml(F("'>"));

        addHtml(F("<TR><TD>Humidity format"));
        addHtml(F(":<TD><input type='text' size='40' maxlength='40' name='Plugin_201_template_2"));
        addHtml(F("' value='"));
        addHtml(timeTemplate[2]);
        addHtml(F("'>"));

        success = true;
        break;
      }

    case PLUGIN_WEBFORM_SAVE:
      {
        Settings.TaskDevicePluginConfig[event->TaskIndex][0] = isFormItemChecked(F("plugin_201_boot"));

        char timeTemplate[3][40];
        for (byte varNr = 0; varNr < 3; varNr++)
        {
          char argc[25];
          String arg = F("Plugin_201_template_");
          arg += varNr;
          arg.toCharArray(argc, 25);
          String tmpString = WebServer.arg(argc);
          strncpy(timeTemplate[varNr], tmpString.c_str(), sizeof(timeTemplate[varNr]));
        }
        SaveCustomTaskSettings(event->TaskIndex, (byte*)&timeTemplate, sizeof(timeTemplate));

        success = true;
        break;
      }
    case PLUGIN_INIT:
      {
        Serial.println ("TFT start!");
        if (!Plugin_201_TFT) {
              Serial.println ("TFT inin starting");
              Plugin_201_TFT = new TFT_eSPI;
              Plugin_201_TFT->init();
              Plugin_201_TFT->setRotation(3);
              Plugin_201_touch_calibrate();
              Plugin_201_button_1 = new TFT_eSPI_Button;
              Plugin_201_button_1->initButtonUL(Plugin_201_TFT, 230, 30, 72, 100, TFT_BLACK, TFT_DARKGREY, TFT_WHITE,"Auto", 1);
              Plugin_201_button_2 = new TFT_eSPI_Button;
              Plugin_201_button_2->initButtonUL(Plugin_201_TFT, 130, 30, 72, 100, TFT_BLACK, TFT_DARKGREY, TFT_WHITE,"Auto", 1);
              Plugin_201_button_3 = new TFT_eSPI_Button;
              Plugin_201_button_3->initButtonUL(Plugin_201_TFT, 30, 30, 72, 100, TFT_BLACK, TFT_DARKGREY, TFT_WHITE,"Auto", 1);
        }
        Serial.println ("TFT OK");
        Plugin_201_draw_temperature(F("--"));
        Plugin_201_TFT->fillScreen(BACKGROUND);
        Plugin_201_draw_screen();
        Serial.println ("TFT INIT OK");
        success = true;
        break;
      }
    case PLUGIN_REQUEST:
      {
      success = true;
        break;
      }
    case PLUGIN_UNCONDITIONAL_POLL:
      {
        success = true;
        break;
      }
    case PLUGIN_TEN_PER_SECOND:
      {
        if (Plugin_201_TFT && touchDelay == 0 ) {
            uint16_t x, y;
            if (Plugin_201_TFT->getTouch(&x, &y))
            {
                touchDelay = 1;
                if (Plugin_201_button_1->contains(x, y)) {
                  switchstate[PLUGIN_VALUE_1]++;
                  if (switchstate[PLUGIN_VALUE_1] > 2) {
                    switchstate[PLUGIN_VALUE_1] = 0;
                  }
                  Plugin_201_draw_button_1(switchstate[PLUGIN_VALUE_1], false);
                  UserVar[event->BaseVarIndex] = switchstate[PLUGIN_VALUE_1];
                  sendData(event);
                }
                if (Plugin_201_button_2->contains(x, y)) {
                  switchstate[PLUGIN_VALUE_2]++;
                  if (switchstate[PLUGIN_VALUE_2] > 2) {
                    switchstate[PLUGIN_VALUE_2] = 0;
                  }
                  Plugin_201_draw_button_2(switchstate[PLUGIN_VALUE_2], false);
                  UserVar[event->BaseVarIndex + 1] = switchstate[PLUGIN_VALUE_2];
                  sendData(event);
                }
                if (Plugin_201_button_3->contains(x, y)) {
                  switchstate[PLUGIN_VALUE_3]++;
                  if (switchstate[PLUGIN_VALUE_3] > 2) {
                    switchstate[PLUGIN_VALUE_3] = 0;
                  }
                  Plugin_201_draw_button_3(switchstate[PLUGIN_VALUE_3], false);
                  UserVar[event->BaseVarIndex + 2] = switchstate[PLUGIN_VALUE_3];
                  sendData(event);
                }
            }
        }
        success = true;
        break;
      }
    case PLUGIN_ONCE_A_SECOND:
      {
        if (touchDelay > 0) {
          touchDelay++;
          if (touchDelay == 4) {
            touchDelay = 0;
          }
        }
        success = true;
        break;
      }
    case PLUGIN_READ:
      {
        Serial.println ("TFT PLUGIN_READ");
        char deviceTemplate[3][40];
        LoadCustomTaskSettings(event->TaskIndex, (byte*)&deviceTemplate, sizeof(deviceTemplate));

        String tmpString = deviceTemplate[0];
        if (Plugin_201_TFT && tmpString.length())
        {
            String newString = parseTemplate(tmpString, 5);
            Plugin_201_draw_time(newString);
        }

        tmpString = deviceTemplate[1];
        if (Plugin_201_TFT && tmpString.length())
        {
            String newString = parseTemplate(tmpString, 2);
            Plugin_201_draw_temperature(newString.substring(0, 2));
        }

        tmpString = deviceTemplate[2];
        if (Plugin_201_TFT && tmpString.length())
        {
            String newString = parseTemplate(tmpString, 2);
            Plugin_201_draw_humidity(newString.substring(0, 2));
        }
        success = true;
        break;
      }

    case PLUGIN_WRITE:
      {
        String log = "";
        String command = parseString(string, 1);
        if (command == F("s201bst"))
        {
          String button = parseString(string, 2);
          String state = parseString(string, 3);
          int intButton = button.toInt();
          switchstate[intButton] = state.toInt();
          if (intButton == 0) {
            Plugin_201_draw_button_1(switchstate[PLUGIN_VALUE_1], false);
            UserVar[event->BaseVarIndex] = switchstate[PLUGIN_VALUE_3];
            sendData(event);
          }
          if (intButton == 1) {
            Plugin_201_draw_button_2(switchstate[PLUGIN_VALUE_2], false);
            UserVar[event->BaseVarIndex + 1] = switchstate[PLUGIN_VALUE_3];
            sendData(event);
          }
          if (intButton == 2) {
            Plugin_201_draw_button_3(switchstate[PLUGIN_VALUE_3], false);
            UserVar[event->BaseVarIndex + 2] = switchstate[PLUGIN_VALUE_3];
            sendData(event);
          }
          success = true;
        }
        if (command == F("l201temp"))
        {
          String temperature = parseString(string, 2);
          Plugin_201_draw_temperature(temperature);
          success = true;
        }
        if (command == F("l201hmd"))
        {
          String temperature = parseString(string, 2);
          Plugin_201_draw_humidity(temperature);
          success = true;
        }
        break;
      }

    case PLUGIN_TIMER_IN:
      {

        break;
      }
  }
  return success;
}


void Plugin_201_draw_jpeg(const char *filename, int xpos, int ypos) {
    // Open the named file (the Jpeg decoder library will close it after rendering image)
     fs::File jpegFile = SPIFFS.open( filename, "r");    // File handle reference for SPIFFS
    //  File jpegFile = SD.open( filename, FILE_READ);  // or, file handle reference for SD library

    if ( !jpegFile ) {
      Serial.print("ERROR: File \""); Serial.print(filename); Serial.println ("\" not found!");
      return;
    }

    // Use one of the three following methods to initialise the decoder:
    boolean decoded = JpegDec.decodeFsFile(jpegFile); // Pass a SPIFFS file handle to the decoder,
    //boolean decoded = JpegDec.decodeSdFile(jpegFile); // or pass the SD file handle to the decoder,
    //boolean decoded = JpegDec.decodeFsFile(filename);  // or pass the filename (leading / distinguishes SPIFFS files)
    // Note: the filename can be a String or character array type
    if (decoded) {
      // render the image onto the screen at given coordinates
      Plugin_201_jpeg_render(xpos, ypos);
    }
    else {
      Serial.println("Jpeg file format not supported!");
    }
}


//====================================================================================
//   Decode and render the Jpeg image onto the TFT screen
//====================================================================================
void Plugin_201_jpeg_render(int xpos, int ypos) {

  // retrieve infomration about the image
  uint16_t  *pImg;
  uint16_t mcu_w = JpegDec.MCUWidth;
  uint16_t mcu_h = JpegDec.MCUHeight;
  uint32_t max_x = JpegDec.width;
  uint32_t max_y = JpegDec.height;

  // Jpeg images are draw as a set of image block (tiles) called Minimum Coding Units (MCUs)
  // Typically these MCUs are 16x16 pixel blocks
  // Determine the width and height of the right and bottom edge image blocks
  uint32_t min_w = minimum(mcu_w, max_x % mcu_w);
  uint32_t min_h = minimum(mcu_h, max_y % mcu_h);

  // save the current image block size
  uint32_t win_w = mcu_w;
  uint32_t win_h = mcu_h;

  // record the current time so we can measure how long it takes to draw an image
  uint32_t drawTime = millis();

  // save the coordinate of the right and bottom edges to assist image cropping
  // to the screen size
  max_x += xpos;
  max_y += ypos;

  // read each MCU block until there are no more
  while ( JpegDec.readSwappedBytes()) { // Swap byte order so the SPI buffer can be used

    // save a pointer to the image block
    pImg = JpegDec.pImage;

    // calculate where the image block should be drawn on the screen
    int mcu_x = JpegDec.MCUx * mcu_w + xpos;  // Calculate coordinates of top left corner of current MCU
    int mcu_y = JpegDec.MCUy * mcu_h + ypos;

    // check if the image block size needs to be changed for the right edge
    if (mcu_x + mcu_w <= max_x) win_w = mcu_w;
    else win_w = min_w;

    // check if the image block size needs to be changed for the bottom edge
    if (mcu_y + mcu_h <= max_y) win_h = mcu_h;
    else win_h = min_h;

    // copy pixels into a contiguous block
    if (win_w != mcu_w)
    {
      uint16_t *cImg;
      int p = 0;
      cImg = pImg + win_w;
      for (int h = 1; h < win_h; h++)
      {
        p += mcu_w;
        for (int w = 0; w < win_w; w++)
        {
          *cImg = *(pImg + w + p);
          cImg++;
        }
      }
    }

    // draw image MCU block only if it will fit on the screen
    if ( ( mcu_x + win_w) <= Plugin_201_TFT->width() && ( mcu_y + win_h) <= Plugin_201_TFT->height())
    {
      Plugin_201_TFT->pushRect(mcu_x, mcu_y, win_w, win_h, pImg);
    }

    else if ( ( mcu_y + win_h) >= Plugin_201_TFT->height()) JpegDec.abort();

  }

  // calculate how long it took to draw the image
  drawTime = millis() - drawTime; // Calculate the time it took

}

#endif // USES_P201
