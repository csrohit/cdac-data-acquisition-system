#include <Arduino.h>
#include<U8g2lib.h>     //graphical diaplay library for multiple display drivers


//display_setup---------------------------------------------------------------------------------------------------------------------------------------------------
U8G2_SSD1306_128X64_NONAME_F_HW_I2C display(U8G2_R0,U8X8_PIN_NONE);
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
uint16_t circ_buff[SCREEN_WIDTH];
uint8_t curr_buff_index = 0;
uint8_t HEIGHT;
#define val_bar 1
//---------------------------------------------------------------------------------------------------------------------------------------------------


//pin_setup---------------------------------------------------------------------------------------------------------------------------------------------------
const uint8_t analog_in_pin = A0;
const uint16_t max_analog_val = 1024;
const uint16_t min_analog_val = 0;
//---------------------------------------------------------------------------------------------------------------------------------------------------


//function_declarations---------------------------------------------------------------------------------------------------------------------------------------------------
void graph_display(void);
uint16_t adc_read(void);
void line(uint8_t xPosition, uint16_t buff_val);
//---------------------------------------------------------------------------------------------------------------------------------------------------

void setup() {
//display_init---------------------------------------------------------------------------------------------------------------------------------------------------
  display.begin();
  display.clearBuffer();
  display.setFont(u8g2_font_bpixel_te);
  display.drawStr(0,7,"Graph View");
  display.sendBuffer();
  delay(1000);
  if(val_bar == 1){
    HEIGHT = SCREEN_HEIGHT - 8;
  }else{
    HEIGHT = SCREEN_HEIGHT;
  }
//---------------------------------------------------------------------------------------------------------------------------------------------------


//pin_init---------------------------------------------------------------------------------------------------------------------------------------------------
  pinMode(analog_in_pin,INPUT);
//---------------------------------------------------------------------------------------------------------------------------------------------------

}

void loop() {
//---------------------------------------------------------------------------------------------------------------------------------------------------
  graph_display();

//---------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------

}

uint16_t adc_read(void){
  uint16_t adc_value =0;  
  adc_value = analogRead(analog_in_pin);
  return adc_value;
}

void graph_display(void){
  display.clearBuffer();
  uint16_t buff_val = adc_read();
  display.setCursor(30,7);
  display.print(buff_val);
  circ_buff[curr_buff_index] = buff_val;
  curr_buff_index++;
  if(curr_buff_index >= display.getDisplayWidth()){
    curr_buff_index = 0;
  }
  uint8_t xPosition = 0;
  for(int i = curr_buff_index;i < display.getDisplayWidth();i++){
    buff_val = circ_buff[i];
    line(xPosition,buff_val);
    xPosition++;
  }
  for(int i = 0;i < curr_buff_index;i++){
    buff_val = circ_buff[i];
    line(xPosition,buff_val);
    xPosition++;
  }
  display.sendBuffer();
}

void line(uint8_t xPosition, uint16_t buff_val){
  uint8_t lineHeight = map(buff_val,min_analog_val,max_analog_val,0,HEIGHT);
  uint8_t yPosition = display.getDisplayHeight() - lineHeight;
  display.drawVLine(xPosition,yPosition,lineHeight);
}