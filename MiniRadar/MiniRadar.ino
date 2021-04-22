//淘宝『有名称的店铺』https://somebodys.taobao.com/
//更新日期 2020/06/04
//MiniRadar 超声波雷达 程序
//本程序对应商品 https://item.taobao.com/item.htm?id=620943696051

//web版程序连接：  https://create.arduino.cc/editor/wjd76/9c74df70-f519-4db8-8d0f-b1bfce84870f/preview
//Github版链接：  https://github.com/shihaipeng03/MiniRadar

#include <Servo.h> 
#include <SPI.h>
#include "Ucglib.h"           
//显示屏的lib 如果没有该lib请按Ctrl+Shift+I 从 库管理器中搜索 ucglib，并安装
//上面报错，请观看视频教程 2分30秒起 https://www.bilibili.com/video/BV1ff4y1975o


#define  trigPin   6          //超声波模块的Trig口 6#
#define  echoPin   5          //超声波模块的echo口 5#
#define  ServoPin  3          //底座舵机端口 3#
int Ymax = 128;               //屏幕的竖向像素数
int Xmax = 160;               //屏幕的横向像素数
int Xcent = Xmax / 2;         //x中位
int base = 118;               //基线高度
int scanline = 105;           //雷达扫描线长度

Servo baseServo; 
Ucglib_ST7735_18x128x160_HWSPI ucg(/*cd=*/ 9, /*cs=*/ 10, /*reset=*/ 8);

void setup(void)
{
 
      ucg.begin(UCG_FONT_MODE_SOLID); //初始化屏幕
      ucg.setRotate90();              //设置成横屏  如果屏幕显示方向是反的，可以修改函数 setRotate90 或 setRotate270
      
      pinMode(trigPin, OUTPUT);       //设置trigPin端口模式
      pinMode(echoPin, INPUT);        //设置echoPin端口模式
      Serial.begin(115200);             //设置串口传输率
      baseServo.attach(ServoPin);     //初始化舵机
    
      //欢迎屏幕
      ucg.setFontMode(UCG_FONT_MODE_TRANSPARENT);
      ucg.setColor(0, 0, 100, 0);
      ucg.setColor(1, 0, 100, 0);
      ucg.setColor(2, 20, 20,20);
      ucg.setColor(3, 20, 20, 20);
      ucg.drawGradientBox(0, 0, 160, 128);
      ucg.setPrintDir(0);
      ucg.setColor(0, 5, 0);
      ucg.setPrintPos(27,42);
      ucg.setFont(ucg_font_logisoso18_tf);  
      ucg.print("Mini Radar");
      ucg.setColor(0, 255, 0);
      ucg.setPrintPos(25,40);
      ucg.print("Mini Radar");
      ucg.setFont(ucg_font_helvB08_tf);
      ucg.setColor(20, 255, 20);
      ucg.setPrintPos(40,100);
      ucg.print("Testing...");
      baseServo.write(90);
    
      //测试底座的运行情况，注意检测底座位置和转动姿态，是否有卡住（或者导线缠绕）的情况。
      for(int x=0;x<180;x+=5)
          { baseServo.write(x);
            delay(50);
           }
      ucg.print("OK!");
      delay(500);
      ucg.setColor(0,0, 0, 0);
      ucg.setColor(1,0, 0, 0);
      ucg.setColor(2,0, 0, 0);
      ucg.setColor(3,0, 0, 0);
      ucg.drawGradientBox(0, 0, 160, 128);
      delay(400);
    
      //清屏
      //ucg.clearScreen();
      cls();
      ucg.setFontMode(UCG_FONT_MODE_SOLID);
      ucg.setFont(ucg_font_orgv01_hr);
  
}


void cls()
{
  //清屏
  ucg.setColor(0, 0, 0, 0);
  for(int s=0;s<128;s++)
  {
    ucg.drawHLine(0,s,160);
    delay(4);
  }
    
  //ucg.drawBox(0, 0, 160, 60);

}


int calculateDistance()
{ 
      long duration;
      //trigPin断电 并 等待2微妙
      digitalWrite(trigPin, LOW); 
      delayMicroseconds(2);
      //trigPin加电 延时 10微妙 再断电
      digitalWrite(trigPin, HIGH); 
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);
      //读取echoPin返回声波的传播时间（微妙）
      duration = pulseIn(echoPin, HIGH);
      //将回声时间转换成距离数值
      return duration*0.034/2;
}

void fix_font() 
{
      ucg.setColor(0, 180, 0);
      ucg.setPrintPos(70,14);
      ucg.print("1.00");
      ucg.setPrintPos(70,52);
      ucg.print("0.50");
      ucg.setPrintPos(70,90);
      ucg.print("0.25");
}

void fix()
{

      ucg.setColor(0, 40, 0);
      //画基线圆盘
      ucg.drawDisc(Xcent, base+1, 3, UCG_DRAW_ALL); 
      ucg.drawCircle(Xcent, base+1, 115, UCG_DRAW_UPPER_LEFT);
      ucg.drawCircle(Xcent, base+1, 115, UCG_DRAW_UPPER_RIGHT);
      ucg.drawCircle(Xcent, base+1, 78, UCG_DRAW_UPPER_LEFT);
      ucg.drawCircle(Xcent, base+1, 78, UCG_DRAW_UPPER_RIGHT);
      ucg.drawCircle(Xcent, base+1, 40, UCG_DRAW_UPPER_LEFT);
      ucg.drawCircle(Xcent, base+1, 40, UCG_DRAW_UPPER_RIGHT);
      ucg.drawLine(0, base+1, Xmax,base+1);
     
      ucg.setColor(0, 120, 0);
      //画刻度表
       for(int i= 40;i < 140; i+=2)
       {

        if (i % 10 == 0) 
          ucg.drawLine(105*cos(radians(i))+Xcent,base - 105*sin(radians(i)) , 113*cos(radians(i))+Xcent,base - 113*sin(radians(i)));
        else
        
         ucg.drawLine(110*cos(radians(i))+Xcent,base - 110*sin(radians(i)) , 113*cos(radians(i))+Xcent,base - 113*sin(radians(i)));
       }
          
       //画一些装饰性图案 
       ucg.setColor(0,200,0);
       ucg.drawLine(0,0,0,18);
       for(int i= 0;i < 5; i++)
       {
          ucg.setColor(0,random(200)+50,0);
          ucg.drawBox(2,i*4,random(14)+2,3);
       }

       ucg.setColor(0,180,0);
       ucg.drawFrame(146,0,14,14);
       ucg.setColor(0,60,0);
       ucg.drawHLine(148,0,10);
       ucg.drawVLine(146,2,10);
       ucg.drawHLine(148,13,10);
       ucg.drawVLine(159,2,10);
        
       ucg.setColor(0,220,0);
       ucg.drawBox(148,2,4,4);
       ucg.drawBox(148,8,4,4);
       ucg.drawBox(154,8,4,4);
       ucg.setColor(0,100,0);
       ucg.drawBox(154,2,4,4);

       ucg.setColor(0,90,0);
       ucg.drawTetragon(62,123,58,127,98,127,102,123);
       ucg.setColor(0,160,0);
       ucg.drawTetragon(67,123,63,127,93,127,97,123);
       ucg.setColor(0,210,0);
       ucg.drawTetragon(72,123,68,127,88,127,92,123);
}



void loop(void)
{
  
  int distance;
  
  fix(); 
  fix_font();  //重绘屏幕背景元素

  for (int x=180; x > 4; x-=2){       //底座舵机从180~0度循环
     
      baseServo.write(x);             //调整舵机角度
      
      //绘制雷达扫描线
      int f = x - 4; 
      ucg.setColor(0, 255, 0);
      ucg.drawLine(Xcent, base, scanline*cos(radians(f))+Xcent,base - scanline*sin(radians(f)));
      f+=2;
      ucg.setColor(0, 128, 0);
      ucg.drawLine(Xcent, base, scanline*cos(radians(f))+Xcent,base - scanline*sin(radians(f)));
      f+=2;
      ucg.setColor(0, 0, 0);
      ucg.drawLine(Xcent, base, scanline*cos(radians(f))+Xcent,base - scanline*sin(radians(f)));
      ucg.setColor(0,200, 0);
      //测距
      distance = calculateDistance();
     
      //根据测得距离在对应位置画点
      if (distance < 100)
      {
        ucg.setColor(255,0,0);
        ucg.drawDisc(distance*cos(radians(x))+Xcent,-distance*sin(radians(x))+base, 1, UCG_DRAW_ALL);
      }
      else
      { //超过1米以上的，用黄色画在边缘区域示意
        ucg.setColor(255,255,0);
        ucg.drawDisc(116*cos(radians(x))+Xcent,-116*sin(radians(x))+base, 1, UCG_DRAW_ALL);
      }
    
           
      //调试代码，输出角度和测距值  
      Serial.print(x); 
      Serial.print("    ,   ");
      Serial.println(distance); 
     

      if (x > 70 and x < 110)  fix_font();  //扫描线和数字重合时，重绘数字
 
      ucg.setColor(0,155,  0);
      ucg.setPrintPos(0,126);
      ucg.print("DEG: "); 
      ucg.setPrintPos(24,126);
      ucg.print(x);
      ucg.print("  ");
      ucg.setPrintPos(125,126);
      ucg.print("  ");
      ucg.print(distance);
      ucg.print("cm  "); 
      
  }
  //ucg.clearScreen();  //清屏 如果arduino供电不足，可能会引起白屏（显示信号中断）可以用 cls();函数代替 ucg.clearScreen(); 
  delay(50);
  cls();   //如有频繁白屏情况，可以使用该函数 。或者增加外部供电
 
  fix(); 
  fix_font();          //重绘屏幕背景元素
  
  for (int  x=1; x < 176; x+=2){     
      baseServo.write(x);             //调整舵机角度
      
      //绘制雷达扫描线
      int f = x + 4;
      ucg.setColor(0, 255, 0);
      ucg.drawLine(Xcent, base, scanline*cos(radians(f))+Xcent,base - scanline*sin(radians(f)));
      f-=2;
      ucg.setColor(0, 128, 0);
      ucg.drawLine(Xcent, base, scanline*cos(radians(f))+Xcent,base - scanline*sin(radians(f)));
      f-=2;
      ucg.setColor(0, 0, 0);
      ucg.drawLine(Xcent, base, scanline*cos(radians(f))+Xcent,base - scanline*sin(radians(f)));
      ucg.setColor(0, 200, 0);
      //测距
      distance = calculateDistance();

      //根据测得距离在对应位置画点
      if (distance < 100)
      {
        ucg.setColor(255,0,0);
        ucg.drawDisc(distance*cos(radians(x))+Xcent,-distance*sin(radians(x))+base, 1, UCG_DRAW_ALL);
      }
      else
      { //超过1米以上的，用黄色画在边缘区域示意
        ucg.setColor(255,255,0);
        ucg.drawDisc(116*cos(radians(x))+Xcent,-116*sin(radians(x))+base, 1, UCG_DRAW_ALL);
      }
           
      //调试代码，输出角度和测距值  
      Serial.print(x); 
      Serial.print("    ,   ");
      Serial.println(distance); 
     
      if (x > 70 and x < 110)  fix_font();  //扫描线和数字重合时，重绘数字
      
      ucg.setColor(0,155,  0);
      ucg.setPrintPos(0,126);
      ucg.print("DEG: "); 
      ucg.setPrintPos(24,126);
      ucg.print(x);
      ucg.print("   ");
      ucg.setPrintPos(125,126);
      ucg.print("   ");
      ucg.print(distance);
      ucg.print("cm   "); 
  
  }
 //ucg.clearScreen(); //
 delay(50);
 cls();


}
