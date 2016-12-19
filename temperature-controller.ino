#include <Adafruit_NeoPixel.h>
#include <Wire.h>//调用收发数据所使用的库函数
#include "I2Cdev.h"
 
#include <SHT2x.h>

#include "U8glib.h"
//-------字体设置，大、中、小
#define setFont_L u8g.setFont(u8g_font_7x13)
#define setFont_M u8g.setFont(u8g_font_fixed_v0r)
#define setFont_S u8g.setFont(u8g_font_fixed_v0r)
#define setFont_SS u8g.setFont(u8g_font_fub25n)
/*
font:
 u8g_font_7x13
 u8g_font_fixed_v0r
 u8g_font_chikitar
 u8g_font_osb21
 u8g_font_courB14r
 u8g_font_courB24n
 u8g_font_9x18Br
 */
//屏幕类型--------
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);

 
#define PIXEL_PIN    A0    // Digital IO pin connected to the NeoPixels.（灯）
#define PIXEL_COUNT  6
 
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);
//PIXEL_COUNT表示连的LED的数量；PIXEL_PIN表示LED灯所连的引脚； NEO_GRB + NEO_KHZ800表示显示颜色和变化闪烁频率

double analogVotage;  //模拟电压值
double temp;          //温度
unsigned int dutyCycle;  //占空比
unsigned int tempMin = 25;  //零速温度，设为串口观察到的环境温度
unsigned int tempMax = 35;  //满速温度，设为串口观察到的手握元件温度
void setup() {
  Serial.begin(115200);      //设置串口波特率
  analogReference(INTERNAL);  //调用板载1.1V基准源
  Wire.begin(); 
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}
void loop() {
  analogVotage = 1.1*(float)analogRead(A3)/1024;   //把ADC读取值换算成电压
  temp = 100*analogVotage;      //电压换算成温度
  u8g.firstPage();

 if (temp < tempMin){
      dutyCycle = 0;
      colorSet(strip.Color(0, 0,255));//蓝色
    //colorSet(int color)设置颜色的函数
    //strip.Color(R,G,B)修改RGB的值可以改变所显现的颜色，R：红色；G：绿色；B：蓝色；
      do {
      setFont_L;
      u8g.setPrintPos(4, 16);
      u8g.print("temperature:low");
      }while( u8g.nextPage() );
}
 
  else if (temp < tempMax){
      dutyCycle = (temp-tempMin)*255/(tempMax-tempMin);
      colorSet(strip.Color(255,255,0));//黄色
       do {
      setFont_L;
      u8g.setPrintPos(4, 16);
      u8g.print("temperature:nice");
      }while( u8g.nextPage() );
      }
  else {
      dutyCycle = 255;
      colorSet(strip.Color(255, 0, 0));//红色
      do {
      setFont_L;
      u8g.setPrintPos(4, 16);
      u8g.print("temperature:high");
      }while( u8g.nextPage() );}
  analogWrite(10,dutyCycle);  //产生PWM
  Serial.print("Temp: "); Serial.print(temp);
  Serial.print(" Degrees    Duty cycle: ");
  Serial.println(dutyCycle);
  delay(100);    //等待0.1秒，控制刷新速度
}

//unit32_t c表示定义灯的颜色
void colorSet(uint32_t c) {
  //for 函数循环语句
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);//i:表示第几个灯开始，从0开始算第一个灯；c 表示灯的颜色
    strip.show();//表示LED显示
    //delay(wait);
  }
}

