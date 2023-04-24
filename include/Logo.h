/*
2019.3.11
1.修改penup后，调用用circle()程序无反应的bug
2.添加showtext(long long)
3.修改隐藏控制台功能
*/

#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_
#include <string>
#include <windows.h>
#include <gdiplus.h>
using namespace Gdiplus;
using namespace std;
enum
{
    BLACK=0,RED=1,BLUE=2,LIME=3,CYAN=4,YELLOW=5,BROWN=6,MEDIUMBLUE=7,
    GREY=8,PINK=9,GREEN=10,BLUEVIOLET=11,DARYCYAN=12,GOLD=13,CORAL=14,WHITE=15,
};
typedef enum MOUSE_BUTTON
{
	NOBUTTON=0,
	MOUSE_LEFT=1,
	MOUSE_MID=2,
	MOUSE_RIGHT=3,
}MOUSE_BUTTON;

typedef enum MOUSE_MSG_TYPE
{
	MOUSE_MOVE=0,
	BUTTON_DOWN=1,
	BUTTON_UP=2,
	BUTTON_DBLCLK=3,	
}MOUSE_MSG_TYPE;

typedef enum TEXT_ALIGN
{
	ALIGN_LEFT,
	ALIGN_CENTER,
	ALIGN_RIGHT
}TEXT_ALIGN;

typedef struct MOUSEMSG {
	int x;
	int y;
	MOUSE_BUTTON button;
	MOUSE_MSG_TYPE flag;
}MOUSEMSG;

typedef enum KEY_MSG_TYPE
{
	KEY_DOWN=0,
	KEY_UP=1,
	KEY_CHAR=2,	
}KEY_MSG_TYPE;

typedef struct KEYMSG {
	int key;
	KEY_MSG_TYPE flag;
}KEYMSG;

class IMAGE
{
	public:
		IMAGE(const char * s);
		IMAGE(const IMAGE &);
		~IMAGE();
		int getwidth();
		int getheight();
		Image * hImage; 
	private:		
		int width;
		int height;		
};
void setup(int w=800,int h=600,int x=0,int y=0,bool showconsole=true);
void close();

void forward(double);
void back(double);
void right(double);
void left(double);
void bk(double);
void right(double);
void fd(double);
void bk(double);
void rt(double);
void lt(double);

void penspeed(int);
void speed(int);

void penup();
void pendown();
void showpen();
void hidepen();
void showcursor();
void hidecursor();

void pencolor(int,int,int);
void pencolor(int);
void color(int,int,int);
void color(int);
void pc(int,int,int);
void pc(int);

void pensize(double);

void moveto(double,double);
void move(double,double);

void wait(double);
void pause();
void update();

void home();
void showXY(int mod=0,bool grid=false,int c=0);
void setX(int);
void setY(int);
void setAngle(int);
void setH(int);
int getX();
int getY();
int getAngle();
int getH();

void clearscreen(int,int,int);
void clearscreen(int);
void clearscreen();
void cls(int,int,int);
void cls(int);
void cls();

void setpixel(int,int,int);
void setpixel(int,int,int,int,int);
void HSLtoRGB(double ,double ,double, int &,int &,int &);
void HSLtoRGB(double ,double ,double, int *,int *,int *);
void line(double,double,double,double);

void rect(double,double);
void rect(double,double,int);
void rect(double,double,int,int,int);

void solidrect(double,double);
void solidrect(double,double,int);
void solidrect(double,double,int,int,int);

void ellipse(double,double);
void ellipse(double,double,int);
void ellipse(double,double,int,int,int);

void solidellipse(double,double);
void solidellipse(double,double,int);
void solidellipse(double,double,int,int,int);

void circle(double,int,int,int);
void circle(double,int);
void circle(double);

void solidcircle(double,int,int,int);
void solidcircle(double,int);
void solidcircle(double);

void arc(double,double,double,double);
void arc(double,double,double,double,int);
void arc(double,double,double,double,int,int,int);

void pie(double,double,double,double);
void pie(double,double,double,double,int);
void pie(double,double,double,double,int,int,int);

void  roundrect(double,double,double,int,int,int);
void  roundrect(double,double,double,int);
void  roundrect(double,double,double);

void  solidroundrect(double,double,double,int,int,int);
void  solidroundrect(double,double,double,int);
void  solidroundrect(double,double,double);

void title(const char *);
void consoletitle(const char *);

void textalign(TEXT_ALIGN text_align=ALIGN_CENTER);
void showtext(const char * s=NULL);
void showtext(char);
void showtext(int);
void showtext(double);
void showtext(float);
void showtext(string);
void showtext(long long);

void listfont();

void textcolor(int);
void textcolor(int,int,int);

void backcolor(int);
void backcolor(int,int,int);

void textsize(int);
void textfont(int);
void textfont(const char *s);

void picture(const char *,int w=0,int h=0,int x=0,int y=0);
void picture(const string,int w=0,int h=0,int x=0,int y=0);

void showimage(IMAGE *,int w=0,int h=0,int x=0,int y=0);
void showimage(IMAGE &,int w=0,int h=0,int x=0,int y=0);
int random(int,int);
int getclock();

bool mousemsg();
MOUSEMSG getmouse();

bool keymsg();
KEYMSG getkey();

int WINAPI msgbox(LPCTSTR ,LPCTSTR,UINT);
class PEN
{
    public:
        PEN();
        ~PEN();
        PEN & forward(double);
        PEN & back(double);
        PEN & right(double);
        PEN & left(double);
        PEN & fd(double);
        PEN & bk(double);
        PEN & setpixel(int,int,int);
        PEN & setpixel(int,int,int,int,int);
        PEN & line(double,double,double,double);
        PEN & rt(double);
        PEN & lt(double);
        PEN & move(int,int);
        PEN & moveto(int,int);
        PEN & color(int,int,int);
        PEN & c(int,int,int);
        PEN & color(int);
        PEN & c(int k);
        PEN & bkcolor(int,int,int);
        PEN & bkcolor(int);
        PEN & size(int);
        PEN & angle(int);
        PEN & speed(int);
        PEN & up();
        PEN & down();
        PEN & show();
        PEN & hide();

        PEN & cls();
        PEN & cls(int,int,int);
        PEN & cls(int);

        PEN & rect(double,double,int);
        PEN & rect(double,double);
        PEN & rect(double,double,int,int,int);

        PEN & r(double,double,int);
        PEN & r(double,double);
        PEN & r(double,double,int,int,int);

        PEN & solidrect(double,double);
        PEN & solidrect(double,double,int);
        PEN & solidrect(double,double,int,int,int);

        PEN & rr(double,double);
        PEN & rr(double,double,int);
        PEN & rr(double,double,int,int,int);

        PEN & ellipse(double,double);
        PEN & ellipse(double,double,int);
        PEN & ellipse(double,double,int,int,int);

        PEN & e(double,double);
        PEN & e(double,double,int);
        PEN & e(double,double,int,int,int);

        PEN & solidellipse(double,double);
        PEN & solidellipse(double,double,int);
        PEN & solidellipse(double,double,int,int,int);

        PEN & ee(double,double);
        PEN & ee(double,double,int);
        PEN & ee(double,double,int,int,int);

        PEN & circle(double);
        PEN & circle(double,int);
        PEN & circle(double,int,int,int);

        PEN & o(double);
        PEN & o(double,int);
        PEN & o(double,int,int,int);

        PEN & solidcircle(double);
        PEN & solidcircle(double,int);
        PEN & solidcircle(double,int,int,int);
        
    	PEN & arc(double,double,double,double);
		PEN & arc(double,double,double,double,int);
		PEN & arc(double,double,double,double,int,int,int);
		
		PEN & pie(double,double,double,double);
		PEN & pie(double,double,double,double,int);
		PEN & pie(double,double,double,double,int,int,int);
		
		PEN &  roundrect(double,double,double,int,int,int);
		PEN &  roundrect(double,double,double,int);
		PEN &  roundrect(double,double,double);
		
		PEN &  solidroundrect(double,double,double,int,int,int);
		PEN &  solidroundrect(double,double,double,int);
		PEN &  solidroundrect(double,double,double);

        PEN & oo(double);
        PEN & oo(double,int);
        PEN & oo(double,int,int,int);


        PEN & text(const char *);
        PEN & text(char);
        PEN & text(int);
        PEN & text(float);
        PEN & text(double);
        PEN & text(long long);
        PEN & text(string);

        PEN & operator << (const char *);
        PEN & operator << (char);
        PEN & operator << (int);
        PEN & operator << (float);
        PEN & operator << (double);
        PEN & operator << (string);
        PEN & operator << (long long);

        PEN & showimage(const IMAGE *,int,int,int,int);
        PEN & showimage(const IMAGE &,int,int,int,int);
        PEN & pic(const char *,int,int,int,int);
        PEN & pic(const string,int,int,int,int);

        PEN & setX(int);
        PEN & setY(int);
        PEN & setAngle(int);
        PEN & setH(int);
        int getX();
        int getY();
        int getAngle();
        int getH();
};

class TEXT
{
    public:
        TEXT & size(int);
        TEXT & face(int);
        TEXT & face(const char *);
        TEXT & color(int,int,int);
        TEXT & color(int c=0);
        TEXT & bkcolor(int,int,int);
        TEXT & bkcolor(int);
};
#endif
