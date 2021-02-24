#include <Keyboard.h>
#include<Mouse.h>
/****
 * 引脚定义
 * lx_line表示左摇杆URX引脚，ly_line表示左摇杆URY引脚，lz_line表示左摇杆sw引脚
 * rx_line表示右摇杆URX引脚，ry_line表示右摇杆URY引脚，rz_line表示右摇杆sw引脚
 * OnBtn表示开关引脚，接地
 * */
const int lx_line = 0;
const int ly_line = 1;
const int lz_line = 14;
const int OnBtn= 10;
const int rx_line = 2;
const int ry_line = 3;
const int rz_line = 9;
const int MouseMoveRange=40;//鼠标的一次移动距离
bool if_first_end=false;//判断是否是第一次结束
char keys[2][2]={//左摇杆上下左右所对应按下的按键
    {'w','s'},
    {'d','a'},
};
bool endkeys[2][2];
void setup() {

    pinMode(rx_line,INPUT_PULLUP);
    pinMode(ry_line,INPUT_PULLUP);
    pinMode(rz_line,INPUT_PULLUP);
    pinMode(lx_line,INPUT_PULLUP);
    pinMode(ly_line,INPUT_PULLUP);
    pinMode(lz_line,INPUT_PULLUP);
    pinMode(OnBtn,INPUT_PULLUP);
    Serial.begin(9600);
    for(int i=0;i<2;i++)
    {
        for(int j=0;j<2;j++)
        {
            endkeys[i][j]=false;
        }
    }
}
int judge(int num)
{
    //return num;
    if(num>400&&num<600)return 0;
    if(num<=400)return -1;
    return 1;
}
void judgelX(int num)
{
    if(num>400&&num<600)//原点
    {
        if(endkeys[0][0])
        {
            Keyboard.release(keys[0][0]);
            endkeys[0][0]=false;
        }
        if(endkeys[0][1])
        {
            Keyboard.release(keys[0][1]);
            endkeys[0][1]=false;
        }
        return ;
    }
    if(num<=400)//上
    {
        Keyboard.press(keys[0][1]);
        endkeys[0][1]=true;
        return ;
    }
    Keyboard.press(keys[0][0]);//下
    endkeys[0][0]=true;
    return ;
}
void judgelY(int num)
{
    if(num>400&&num<600)//原点
    {
        if(endkeys[1][0])
        {
            Keyboard.release(keys[1][0]);
            endkeys[1][0]=false;
        }
        if(endkeys[1][1])
        {
            Keyboard.release(keys[1][1]);
            endkeys[1][1]=false;
        }
        return ;
    }
    if(num<=400)//右
    {
        Keyboard.press(keys[1][1]);
        endkeys[1][1]=true;
        return ;
    }
    Keyboard.press(keys[1][0]);//左
    endkeys[1][0]=true;
    return ;
}
void judgelZ(int num)
{
    //return num;
    if(num==0)
    {
        Mouse.press(MOUSE_LEFT);
        return ;
    }
    Mouse.release(MOUSE_LEFT);
    return;
}
void judgerX(int num)
{
    //return num;
    if(num>400&&num<600)
    {
        Mouse.move(0, 0);
        return ;
    }
    if(num<=400)//上
    {
         Mouse.move(0, 40);
        return ;
    }
    Mouse.move(0, -40);//下
    return ;
}
void judgerY(int num)
{
    //return num;
    if(num>400&&num<600)
    {
        Mouse.move(0, 0);
        return ;
    }
    if(num<=400)//右
    {
       Mouse.move(-40, 0);
       return ;
    }
    Mouse.move(40, 0);//左
    return ;
}void judgerZ(int num)
{
    //return num;
    if(num==0)
    {
        Mouse.press(MOUSE_RIGHT);
        return ;
    }
    Mouse.release(MOUSE_RIGHT);
    return;
}
void loop() {
    int lx,ly,lz,rx,ry,rz;
    while(digitalRead(OnBtn)==LOW)//这里设计是引脚接地表示打开开关，即开启鼠标键盘模拟的功能
    {
        if(!if_first_end)
        {
            Keyboard.begin();
            Mouse.begin();
            delay(500);
        }
        if_first_end=true;
        lx=analogRead(lx_line);
        ly=analogRead(ly_line);
        lz=digitalRead(lz_line);
        rx=analogRead(rx_line);
        ry=analogRead(ry_line);
        rz=digitalRead(rz_line);
        Serial.print("lX:");//串口调试
        Serial.print(judge(lx));
        Serial.print(",lY:");
        Serial.print(judge(ly));
        Serial.print(",lZ:");
        Serial.print(lz);/**/
        Serial.print(" | rX:");
        Serial.print(judge(rx));
        Serial.print(",rY:");
        Serial.print(judge(ry));
        Serial.print(",rZ:");
        Serial.println(rz);
        judgelX(lx);
        judgelY(ly);
        judgelZ(lz);
        judgerX(rx);
        judgerY(ry);
        judgerZ(rz);
        delay(50);
    }
    if(if_first_end==true)
    {
        if_first_end=false;
        for(int i=0;i<2;i++)
        {
            for(int j=0;j<2;j++)
            {
                endkeys[i][j]=false;
            }
        }
        Keyboard.releaseAll();//释放所有按下的按键
        //Mouse.releaseAll();
        Mouse.release(MOUSE_ALL);
        Keyboard.end();
        Mouse.end();
    }
}
