# Doodle Jump
涂鸦跳跃

<!-- MarkdownTOC -->

- [第1章  项目介绍](#第1章-项目介绍)
    - [1.1  项目背景](#11-项目背景)
    - [1.2  技术支持](#12-技术支持)
    - [1.3  项目结构](#13-项目结构)
- [第2章  系统设计](#第2章-系统设计)
    - [2.1  四个宏定义](#21-四个宏定义)
    - [2.2  四种枚举类型](#22-四种枚举类型)
    - [2.3  Welcome类](#23-welcome类)
    - [2.4  SaveData类](#24-savedata类)
    - [2.5  Scene类](#25-scene类)
    - [2.6  Land类](#26-land类)
    - [2.7  Player类](#27-player类)
- [第3章  关键技术介绍](#第3章-关键技术介绍)
    - [3.1  LogoC图片的存储](#31-logoc图片的存储)
    - [3.2  声音的播放与终止](#32-声音的播放与终止)
    - [3.3  键盘操作的获取](#33-键盘操作的获取)
    - [3.4  实现简陋的输入框功能](#34-实现简陋的输入框功能)
    - [3.5  游戏的存档与读档](#35-游戏的存档与读档)
    - [3.6  地板的随机生成](#36-地板的随机生成)
    - [3.7  小人与地板的碰撞判断](#37-小人与地板的碰撞判断)
- [第4章  软件说明书](#第4章-软件说明书)
    - [4.1  开始界面](#41-开始界面)
    - [4.2  游戏界面](#42-游戏界面)
- [第5章  总结](#第5章-总结)
    - [5.1  遇到的问题及解决方案](#51-遇到的问题及解决方案)
    - [5.2  尚未解决的问题](#52-尚未解决的问题)
- [参考资料](#参考资料)

<!-- /MarkdownTOC -->
# 第1章  项目介绍

<a id="11-项目背景"></a>

## 1.1  项目背景

最开始我选择制作的游戏是俄罗斯方块，不过感觉俄罗斯方块所涉及到的方面还是太少了，并且难以添加一些新的功能与玩法，于是便决定换个游戏制作。
<!--more-->
![Tetris](https://s2.loli.net/2022/08/15/DVe1Q7j38nbIkrK.png)

在试玩了网上的一些游戏后，我决定制作一款游戏——涂鸦跳跃 `Doodle Jump`，决定尝试面向对象编程，将游戏中的不同元素都分为各个对象进行编写。代码行数接近一千行，代码的可读性仍有待提高。

<a id="12-技术支持"></a>

## 1.2  技术支持

所使用的IDE是由`royqh1979`所编译的`Dev C++`  
编译器版本是`GCC 10.3.0`

程序代码部分使用了

```cpp
#include "fstream" //C++文件处理库（用于游戏存档和得分记录）
#include "map" //C++ STL利用红黑树存储键值对（得分记录）
#include "thread" //C++ STL多线程支持（主要用于获取键盘以及鼠标操作）
#include "iostream" //C++标准输入输出库（仅在调试中使用）
#include "Logo.h" //图形库使用的是虞老师的LogoC
```

编译时加入了`-lwinmm`指令以实现播放声音的支持（调用`mciSendString`函数）

<a id="13-项目结构"></a>

## 1.3  项目结构

- `image`：包含游戏的图片
  - `Land`：地板的图片
  - `System`：各个界面的图片以及按钮的图片
  - `Theme`：包含四个主题不同的背景和小人
  - `Welcome`：开始界面相关图片
- `sounds`：包含游戏中的声音
- `DoodleJump.h`:头文件包含函数的声明
- `DoodleJump.cpp`:对子文件的调用
- `Land.cpp`:包含`Land`类，与地块有关
- `Player.cpp`:包含`Player`类，与小人有关
- `SaveData.cpp`:包含`SaveData`类，存取存档
- `Scene.cpp`:包含`Scene`类，场景有关
- `Welcome.cpp`:包含`Welcome`类，欢迎界面

<a id="第2章-系统设计"></a>

# 第2章  系统设计

<a id="21-四个宏定义"></a>

## 2.1  四个宏定义

```cpp
#define _Doodle_Jump_Width_ 500 //游戏窗口宽度
#define _Doodle_Jump_Height_ 800 //游戏窗口高度
#define _land_num_ 10 //游戏中所包含的最大地块数量
#define _Gravity_ 0.15 //重力加速度
```

<a id="22-四种枚举类型"></a>

## 2.2  四种枚举类型

程序使用了较多的枚举类型以提高代码可读性：

```cpp
enum InterfaceType { //用于描述程序当前所在的界面
    I_welcome, I_rule1, I_rule2, I_rank, I_option, I_change_name, I_gamerun, I_gamepause, I_gameretry, I_gameover
};
enum DJTheme { //游戏当前所用的主题名称
    Classic, Jungle, Underwater, Winter
};
enum PlayerStatus { //玩家所操控的小人所处的状态
    p_left, p_right, fly_left1, fly_right1, fly_left2, fly_right2, rocket_left, rocket_right
};
enum LandType { //LandType：地块的类型
    normal, fragile, broken, broken_over, mvland, landfly, landrocket, mvspring1, landspring1, mvspring2, landspring2
};
```

<a id="23-welcome类"></a>

## 2.3  Welcome类

```cpp
IMAGE * (*);//将图片存入该指针所指向的地址（按钮，各子界面图）
InterfaceType interfacetype;//当前界面类型 
DJtheme themetype;//当前主题类型
string path, theme, name;//路径；主题，玩家姓名
thread p_mouseget;//线程指针
bool bgm, sound_effect;//背景音乐，音效开关
multimap<int, string, sortcmp> scorerank_map;//记录排名的函数
Welcome();//构造函数，负责变量的初始化
void rank_read();//读取排名
void rank_save();//写入排名
void change_name();//改动玩家姓名
void welcome_mouseget();//捕获开始界面的操作，通常在多线程调用
void main_interface();//开始界面循环函数
void draw_welcome();//欢迎界面的绘制
void show_rule();//规则界面
void show_rank(InterfaceType pre);//排行榜界面
void show_option(InterfaceType pre);//设置界面
void show_pauselabel();//游戏暂停界面
void show_gameover();//游戏结束界面
void change_theme();//更改主题函数，调用各类变换主题子函数
void draw_jump();//开始界面跳动小人
void pauseget_mouse();//捕获暂停界面的操作，多线程中调用
void game_run();//游戏运行循环函数
```

<a id="24-savedata类"></a>

## 2.4  SaveData类

```cpp
void game_save();//游戏存档
bool game_load();//游戏继续
```

<a id="25-scene类"></a>

## 2.5  Scene类

```cpp
IMAGE * im_bk, im_basicline;//游戏场景背景图
int direct;//记录接收的按键
Scene();//构造函数，负责初始化
void change_theme(string theme, string path);//更改游戏场景背景图
void draw();//背景绘制
void PlayBGM(string MusicPath);//播放背景音乐
void PlayMusic(string MusicPath);//播放音效
void show();//调用三个draw函数
void updateWithoutInput();//与输入无关的更新
void updateWithKeyInput();//与键入有关的更新
```

<a id="26-land类"></a>

## 2.6  Land类

```cpp
IMAGE * (*);//将图片存入该指针所指向的地址（地面图片）
float land_width, land_height, land_vy;//地板宽高，地板速度
int score;//当前成绩
int broken_y;//记录破碎地板的y坐标

struct LandState {//单个地板的状况
    float middle_x;//地板中心x坐标
    float top_y;//地板上部y坐标（上部是为了判断碰撞方便）
    float vx;//移动地板移动速度
    LandType landType;//地板类型
    IMAGE *im_land;//地板图片，赋图片地址
} land[_land_num_];//共生成_land_num_个地板

Land(string path);//构造函数初始化
void retry_clean();//重新运行游戏时的初始化
void Land_type(int i);//随机生成地板
void draw();//绘制地板
void show_topbar();//绘制顶栏
void updateLandY();//更新地板
```

<a id="27-player类"></a>

## 2.7  Player类

```cpp
IMAGE * (*);//将图片存入该指针所指向的地址（玩家图片）
PlayerStatus playerStatus;//小人当前状态
float x_middle, y_bottom;//小人中心x坐标，底部y坐标（方便碰撞判断）
float vx, vy;//小人水平竖直方向速度
float width, height;//小人的宽高
float rebound_vy;//小人反弹的速度
bool isPlayer_yMax, isPlayer_died;//小人是否到达超过半高，超过半高则小人不动，地面动；判断小人是否死亡
Player(string path, string theme);//构造函数初始化
void retry_clean();//重新运行游戏时的初始化
void change_theme(string theme, string path);//更改主题(小人)
void draw();//绘制小人
void moveLeft();//小人左移显示切换
void moveRight();//小人右移显示切换
void autoJump();//回到初始小人
void autoJump_fly();//小人竹蜻蜓显示切换
void autoJump_rocket();//小人火箭显示切换
void JudgeisPlayer_yMax();//判断小人是否达到半高
void isOnLand();//小人与地面碰撞判断
void updateYcoordinate();//小人的运动更新Y轴坐标
void JudgeisPlayer_died();//判断玩家是否死亡
```

<a id="第3章-关键技术介绍"></a>

# 第3章  关键技术介绍

<a id="31-logoc图片的存储"></a>

## 3.1  LogoC图片的存储

```cpp
class IMAGE{
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
```

由于`LogoC`中`IMAGE`类的构造函数仅包含按照路径构造，为了方便修改，以及避免在`class`声明时就将`IMAGE`类初始化导致异常(`IMAGE`类必须在好像在调用`setup`函数之后才能够使用)，我都采用了`IMAGE * `指针类型来储存`IMAGE`对象的地址。  
路径的写法我采用了C++的`string`类型，方便修改，在最后使用`c_str`方法转化为C语言风格的字符数组字符串当作路径。

<a id="32-声音的播放与终止"></a>

## 3.2  声音的播放与终止

```cpp
void Scene::PlayBGM(string MusicPath) {
	string op = "open " + MusicPath + " alias BGM";
	mciSendString(op.c_str(), NULL, 0, NULL);
	mciSendString("play BGM repeat", NULL, 0, NULL);
}
void Scene::PlayMusic(string MusicPath) {
	string op = "open " + MusicPath + " alias soundeffect";
	mciSendString("close soundeffect", NULL, 0, NULL);
	mciSendString(op.c_str(), NULL, 0, NULL);
	mciSendString("play soundeffect", NULL, 0, NULL);
}
```

声音的播放我采用的是`MCI`(`Media Control Interface`，媒体控制接口)发送命令的方式实现的。  
在`GCC`等非`MSVC`编译器中要加入`-lwinmm`指令使得编译器识别该函数。

<a id="33-键盘操作的获取"></a>

## 3.3  键盘操作的获取
```cpp
void Scene::updateWithKeyInput() {
	if (keymsg()) {
		kmsg = getkey();
		if (kmsg.flag == KEY_DOWN) {
			if (kmsg.key == 'A' || kmsg.key == 'a' || kmsg.key == 37) {
				direct = -1;
			} else if (kmsg.key == 'D' || kmsg.key == 'd' || kmsg.key == 39) {
				direct = 1;
			} else if (kmsg.key == 27) {
				welcome.interfacetype = I_gamepause;
			}
		}
		if (kmsg.flag == KEY_UP) {
			if ((kmsg.key == 'A' || kmsg.key == 'a' || kmsg.key == 37) || (kmsg.key == 'D' || kmsg.key == 'd' || kmsg.key == 39)) {
				direct = 0;
			}
		}
	}
	if (direct == -1) {
		player.moveLeft();
	} else if (direct == 1) {
		player.moveRight();
	}
}
```
由于键盘的长按有判定时间，大概在几百毫秒左右，但是显示在操作中就会导致小人先动一下然后再持续移动。  
为了解决这个问题，我们可以把小人的左移右移设为一个状态，按下左移或者右移时记录下这个状态，然后在松开这些键的时候把状态归零。然后根据当前所处的状态进行操作的更新。

<a id="34-实现简陋的输入框功能"></a>

## 3.4  实现简陋的输入框功能

![](https://s2.loli.net/2022/08/15/BF45fyiV7tzvuEq.png)
```cpp
	while (interfacetype == I_change_name) {
		bool is_change = false;
		if (keymsg()) {
			kmsg = getkey();
			if (kmsg.flag == KEY_DOWN) {
				if (isalnum(kmsg.key)) {
					if (in.length() <= 10) {
						in += kmsg.key;
						is_change = true;
					}
				} else if (kmsg.key == 8) {
					if (in.length() > 0) {
						in.pop_back();
						is_change = true;
					}
				} else if (kmsg.key == 13 || kmsg.key == 27) {
					interfacetype = I_option;
				}
			}
		}
```

通过接收输入输出存入字符串，再用`showtext`函数显示出来，虽然简陋，也不支持中文的输入，不过也算是实现了这个功能。

<a id="35-游戏的存档与读档"></a>

## 3.5  游戏的存档与读档

游戏存档为了防止被修改导致游戏出现异常，使用了二进制来存取数据，对于固定大小的类型直接按所占字节进行存取即可，对于非定长类型例如字符串，可以先存储字符串的长度，再存入字符数据即可，这样读取时就知道需要读取多少字节的数据，实现字符串的二进制读写。

存档部分代码：
```cpp
void SaveData::game_save() {
	ofstream savedata;
	size_t StringLength;
	savedata.open("savedata.dat", ios::out | ios::binary);
	//Welcome部分
	savedata.write((char*)&welcome.themetype, sizeof(DJTheme));
	savedata.write((char*)&welcome.bgm, sizeof(bool));
	savedata.write((char*)&welcome.sound_effect, sizeof(bool));
	StringLength = welcome.name.size();
	savedata.write((char*)&StringLength, sizeof(size_t));
	savedata.write(welcome.name.c_str(), welcome.name.size());
	//Land部分
	savedata.write((char*)&land.land_width, sizeof(float));
	savedata.write((char*)&land.land_height, sizeof(float));
	savedata.write((char*)&land.land_vy, sizeof(float));
	savedata.write((char*)&land.score, sizeof(int));
	savedata.write((char*)&land.broken_y, sizeof(int));
	for (int i = 0; i < _land_num_; i++)
		savedata.write((char*)&land.land[i], sizeof(Land::LandState));
	//Player部分
	savedata.write((char*)&player.playerStatus, sizeof(PlayerStatus));
	savedata.write((char*)&player.x_middle, sizeof(float));
	savedata.write((char*)&player.y_bottom, sizeof(float));
	savedata.write((char*)&player.vx, sizeof(float));
	savedata.write((char*)&player.vy, sizeof(float));
	savedata.write((char*)&player.width, sizeof(float));
	savedata.write((char*)&player.height, sizeof(float));
	savedata.write((char*)&player.rebound_vy, sizeof(float));
	savedata.write((char*)&player.isPlayer_yMax, sizeof(bool));
	savedata.write((char*)&player.isPlayer_died, sizeof(bool));
	savedata.close();
}
```
读档部分代码：
```cpp
bool SaveData::game_load() {
	ifstream savedata;
	size_t StringLength;
	savedata.open("savedata.dat", ios::in | ios::binary);
	if (savedata.peek() == EOF) {
		savedata.close();
		return false;
	}
	//Welcome部分
	savedata.read((char*)&welcome.themetype, sizeof(DJTheme));
	savedata.read((char*)&welcome.bgm, sizeof(bool));
	savedata.read((char*)&welcome.sound_effect, sizeof(bool));
	savedata.read((char*)&StringLength, sizeof(size_t));
	char* buffer = new char[StringLength + 1];
	savedata.read(buffer, StringLength);
	buffer[StringLength] = '\0';
	welcome.name = buffer;
	delete []buffer;
	//Land部分
	savedata.read((char*)&land.land_width, sizeof(float));
	savedata.read((char*)&land.land_height, sizeof(float));
	savedata.read((char*)&land.land_vy, sizeof(float));
	savedata.read((char*)&land.score, sizeof(int));
	savedata.read((char*)&land.broken_y, sizeof(int));
	for (int i = 0; i < _land_num_; i++) {
		savedata.read((char*)&land.land[i], sizeof(Land::LandState));
		if (land.land[i].landType == landrocket) {
			land.land[i].im_land = land.im_landrocket;
		} else if (land.land[i].landType == landfly) {
			land.land[i].im_land = land.im_landfly;
		} else if (land.land[i].landType == mvspring1) {
			land.land[i].im_land = land.im_mvspring1;
		} else if (land.land[i].landType == mvland) {
			land.land[i].im_land = land.im_move;
		} else if (land.land[i].landType == fragile) {
			land.land[i].im_land = land.im_break;
		} else if (land.land[i].landType == landspring1) {
			land.land[i].im_land = land.im_landspring1;
		} else if (land.land[i].landType == normal) {
			land.land[i].im_land = land.im_normal;
		}
	}
	//Player部分
	savedata.read((char*)&player.playerStatus, sizeof(PlayerStatus));
	savedata.read((char*)&player.x_middle, sizeof(float));
	savedata.read((char*)&player.y_bottom, sizeof(float));
	savedata.read((char*)&player.vx, sizeof(float));
	savedata.read((char*)&player.vy, sizeof(float));
	savedata.read((char*)&player.width, sizeof(float));
	savedata.read((char*)&player.height, sizeof(float));
	savedata.read((char*)&player.rebound_vy, sizeof(float));
	savedata.read((char*)&player.isPlayer_yMax, sizeof(bool));
	savedata.read((char*)&player.isPlayer_died, sizeof(bool));
	savedata.close();
	welcome.change_theme();
	return true;
}
```
<a id="36-地板的随机生成"></a>

## 3.6  地板的随机生成
```cpp
void Land::Land_type(int i, int lnor, int lfr, int mv, int lspr, int lfly, int mvspr, int lroc) {
	int rd = rand() % 100;
	if (rd < lnor) {
		land[i].landType = normal;
		land[i].im_land = im_normal;
	} else if (rd < lfr) {
		land[i].landType = fragile;
		land[i].im_land = im_break;
	} else if (rd < mv) {
		land[i].landType = mvland;
		land[i].im_land = im_move;
	} else if (rd < lspr) {
		land[i].landType = landspring1;
		land[i].im_land = im_landspring1;
	} else if (rd < lfly) {
		land[i].landType = landfly;
		land[i].im_land = im_landfly;
	} else if (rd < mvspr) {
		land[i].landType = mvspring1;
		land[i].im_land = im_mvspring1;
	} else if (rd < lroc) {
		land[i].landType = landrocket;
		land[i].im_land = im_landrocket;
	}
	land[i].middle_x = rand() % (_Doodle_Jump_Width_ - (int)land_width - _Doodle_Jump_Width_ / 2);
}
```
先使用`srand((unsigned)time(NULL));`按时间初始化随机数种子，再通过`rand()`函数生成随机数，按照不同概率生成地板类型。可以将概率写入函数参数表中，这样可以更加方便地按照游戏的进行更改不同的概率，提高游戏难度。

<a id="37-小人与地板的碰撞判断"></a>

## 3.7  小人与地板的碰撞判断

碰撞部分较为愚蠢的采用了手写碰撞距离的判断，不过该游戏所需碰撞的分类并不多，手写距离只是限制了地板以及小人高度的变化。通过多分支结构来判断不同地板的情况。

```cpp
void Player::isOnLand() {
	for (int i = 0; i < _land_num_; i++) {
		if ((abs(y_bottom - land.land[i].top_y) <= vy) && (vy > 0)) {
			if (land.land[i].landType == mvspring1 || land.land[i].landType == landspring1) {
				if ((x_middle + 61 <= land.land[i].middle_x) || ((x_middle + 25) >= land.land[i].middle_x + land.land_width)) { // 大约估算图像中人物脚的距离,玩家没有落在地面上
					return;
				} else if ((x_middle + 61 >= land.land[i].middle_x + 30) && ((x_middle + 25) <= land.land[i].middle_x + 60)) { // 大约估算图像中人物脚的距离，玩家踩在弹簧上
					if (welcome.sound_effect)scene.PlayMusic("sounds/spring.wav");
					if (land.land[i].landType == mvspring1)
						land.land[i].landType = mvspring2;
					else {
						land.land[i].landType = landspring2;
					}
					y_bottom = land.land[i].top_y;
					vy = -(2.5) * rebound_vy;
				} else {
					if (welcome.sound_effect)scene.PlayMusic("sounds/jump.wav");
					vy =  -rebound_vy;
				}
				autoJump();
			} else if (land.land[i].landType == landfly) {
				if ((x_middle + 61 <= land.land[i].middle_x) || ((x_middle + 25) >= land.land[i].middle_x + land.land_width)) { // 大约估算图像中人物脚的距离,玩家没有落在地面上
					return;
				} else if ((x_middle + 61 >= land.land[i].middle_x + 30) && ((x_middle + 25) <= land.land[i].middle_x + 60)) { // 大约估算图像中人物脚的距离，玩家踩在竹蜻蜓上
					if (welcome.sound_effect)scene.PlayMusic("sounds/fly.wav");
					y_bottom = land.land[i].top_y;
					vy = -(4.2) * rebound_vy;
					autoJump_fly();
				} else {
					if (welcome.sound_effect)scene.PlayMusic("sounds/jump.wav");
					vy =  -rebound_vy;
					autoJump();
				}
			} else if (land.land[i].landType == landrocket) {
				if ((x_middle + 61 <= land.land[i].middle_x) || ((x_middle + 25) >= land.land[i].middle_x + land.land_width)) { // 大约估算图像中人物脚的距离,玩家没有落在地面上
					return;
				} else if ((x_middle + 61 >= land.land[i].middle_x + 30) && ((x_middle + 25) <= land.land[i].middle_x + 60)) { // 大约估算图像中人物脚的距离，玩家踩在竹蜻蜓上
					if (welcome.sound_effect)scene.PlayMusic("sounds/rocket.wav");
					y_bottom = land.land[i].top_y;
					vy = -(5.8) * rebound_vy;
					autoJump_rocket();
				} else {
					if (welcome.sound_effect)scene.PlayMusic("sounds/jump.wav");
					vy =  -rebound_vy;
					autoJump();
				}
			} else if (land.land[i].landType != broken) { //其他类型地面判断
				if ((x_middle + 61 >= land.land[i].middle_x) && ((x_middle + 25) <= land.land[i].middle_x + land.land_width && land.land[i].landType != broken_over)) { // 大约估算图像中人物脚的距离
					if (land.land[i].landType != fragile) {
						if (welcome.sound_effect)scene.PlayMusic("sounds/jump.wav");
					} else {
						if (welcome.sound_effect)scene.PlayMusic("sounds/break.wav");
						land.land[i].landType = broken;
						land.broken_y = land.land[i].top_y;
					}
					y_bottom = land.land[i].top_y;
					vy = - rebound_vy;
					autoJump();
				}
			}
		}
	}
	JudgeisPlayer_died();
}
```

<a id="第4章-软件说明书"></a>

# 第4章  软件说明书

<a id="41-开始界面"></a>

## 4.1  开始界面

![](https://s2.loli.net/2022/08/15/I9w8ZGLqOQn2Pmg.png)![](https://s2.loli.net/2022/08/15/7iqYJfT8OhINm2o.png)  
开始：开始游戏并清空存档  
继续：读取存档以开始游戏，若无存档会报错。  
![](https://s2.loli.net/2022/08/15/JgIC7cYAnrLO8kG.png)![](https://s2.loli.net/2022/08/15/iN2IbEFwpSmJ7Pe.png)  
规则：按规则按钮会显示规则。按下一页及返回按钮可以退出规则界面。  
![](https://s2.loli.net/2022/08/15/5f2KCYPNaI6dlui.png)  
排行榜：显示排行榜，按返回按键或者点击界面的其他地方可以退出排行榜界面。  
![](https://s2.loli.net/2022/08/15/NF3iyxKhSzstHZm.png)  
设置：点击音乐音效旁的开关可以开关音乐音效；点击主题旁边的四个小人可以更改主题；点击“点我改名”可以改名。点设置界面的其他地方可以退出该界面  
![](https://s2.loli.net/2022/08/15/BF45fyiV7tzvuEq.png)

<a id="42-游戏界面"></a>

## 4.2  游戏界面

![](https://s2.loli.net/2022/08/15/G3pKt4NfdbQOxYa.png)  
游戏界面：按`A/D/←/→`来控制小人的运动  
![](https://s2.loli.net/2022/08/15/9csZAj4F2bCqREU.png)![](https://s2.loli.net/2022/08/15/Qqt3zEakTfjWYbd.png)![](https://s2.loli.net/2022/08/15/RoBDq2K6aupbzXv.png)  
在碰到这三个地板上的时物件时会高速运动一段路程  
![](https://s2.loli.net/2022/08/15/9gRVDcsMfa1AmCI.png)碰到该地板时，地板会破碎并反弹一次  
![](https://s2.loli.net/2022/08/15/R3eoIql1sKzJGmr.png)该地板是会左右移动的  
游戏目标就是尽可能得到最高的分数  
掉出游戏界面即角色死亡  
![](https://s2.loli.net/2022/08/15/M5WyvBGA47mlNsT.png)  
暂停：暂停按键在右上角，也可以通过`ESC`按键来呼出暂停界面  
设置按键可以调出设置；菜单按键可以回到开始界面，同时也会把当前游戏存档；重玩按键可以重开；按下返回按键退出暂停状态。  
![](https://s2.loli.net/2022/08/15/Qk3aoTzsx4XLwbN.png)  
死亡后可以按重玩按钮重新游戏；按排行榜看排行榜；点击菜单回到开始界面  

<a id="第5章-总结"></a>

# 第5章  总结

<a id="51-遇到的问题及解决方案"></a>

## 5.1  遇到的问题及解决方案

### 5.1.1  IMAGE对象的创建

出于对性能以及代码可读性的考虑，我决定使用`IMAGE`对象来存储图片。但是我在创建`IMAGE`对象时经常遇到这种错误:  
![](https://s2.loli.net/2022/08/15/iWOy7caBtzSID6U.png)  
最后经过单独把`IMAGE`类抽出来排查发现`IMAGE`类的创建必须在运行`setup()`函数之后。  
初步估计`setup()`函数中应该包含了`LogoC`图形库初始化相关代码。

### 5.1.2  输入框功能如何实现

由于`LogoC`中没有输入框相关控件的支持，我只能通过`getkey()`函数来实现键盘的输入和输出，再使用`showtext()`函数显示出来。

### 5.1.3  小人运动卡顿问题

游戏中在操作小人运动时会出现小人先动一下然后再持续移动。  
这是因为由于键盘的长按有判定时间，大概在几百毫秒左右。为了解决这个问题，我们可以把小人的左移右移设为一个状态，按下左移或者右移时记录下这个状态，然后在松开这些键的时候把状态归零。然后根据当前所处的状态进行操作的更新，就能实现无延迟的小人移动。

### 5.1.4  鼠标运动卡顿问题

在游戏运行过程中，经常会出现鼠标点击却要过一段时间才有反应的情况，是因为等待接受的mouse信号过多导致迟滞。解决方案就是通过多线程将获取鼠标的代码独立在一个线程中运行。

<a id="52-尚未解决的问题"></a>

## 5.2  尚未解决的问题

### 5.2.1  输入框功能仍旧不够完善

所实现的输入框功能过于简陋，仅能实现英文单词和数字的输入，无法实现例如中文的输入，导致玩家名称仅能是英文名，暂无良好的解决方法。

### 5.2.2  MCI对wav格式音乐支持问题

在背景音乐播放完一遍时，我需要重新播放一遍背景音乐，一般来说，我们可以在`MCI`命令中加一段`"repeat"`即可，但是在实际操作中，倘若播放的音乐格式是`wav`格式，不仅不能实现重复，音乐也会播放失败，我只能将需要循环播放的背景音乐转为`mp3`格式，经测试能够正常循环播放，但是还是对于`MCI`对`wav`格式的支持感到不解。

### 5.2.3  字体在不同电脑上显示不同

`LogoC`中的`textfont()`函数支持的字体选择仅能通过字体在计算机中的序号来选择，导致了在不同电脑中的字体显示不一样。解决方法也是有的，因为我所需要的仅仅只是数字和字母，所以我可以通过把这些字体以位图的方式呈现，再在显示时截取相应片段即可。  
但是工作量有亿点点大，最终决定还是放弃了。

### 5.2.4  代码行数过多，可读性较差

项目所有文件的代码总行数超过了`1000`行(10+171+466+90+86+111+232=1166)，在UI设计中的行数过于庞大(超过半数代码)，在实现基础的游戏功能时仅用了`200`多行的代码，在项目逐渐庞大之后，不经常写注释的弊端便暴露出来，导致后半段写的代码可能包含一些重复的代码，降低了可读性，也使得代码看上去没有那么美观。

### 5.2.5  CPU占用过高

写出来的程序所占用的资源过多，在我自己的电脑（2015款的MacBook Air）会有极为严重的卡顿现象，即使使用多线程也无法改善。而且在学校机房的电脑上，运行游戏时也会有接近20%的CPU占用，暂无良好的解决方案。

<a id="参考资料"></a>

# 参考资料
>
> 1. 《LogoC 使用参考》 虞铭财
> 2. 菜鸟教程
