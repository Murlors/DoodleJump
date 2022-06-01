
# 第1章  项目介绍
## 1.1  项目背景
最开始我选择制作的游戏是俄罗斯方块，不过感觉俄罗斯方块所涉及到的方面还是太少了，并且难以添加一些新的功能与玩法，于是便决定换个游戏制作。

在试玩了网上的一些游戏后，我决定制作一款游戏——涂鸦跳跃（Doodle Jump），决定尝试面向对象编程，将游戏中的不同元素都分为各个对象进行编写。代码行数接近一千行，代码的可读性仍有待提高。

## 1.2  技术支持

所使用的IDE是由royqh1979所编译的Dev C++  
编译器版本是GCC 10.3.0

图形库使用的是虞老师的LogoC  
程序代码部分还使用了
```cpp
iostream C++标准输入输出库（仅在调试中使用）
fstream C++文件处理库（用于游戏存档和得分记录）
map C++ STL利用红黑树存储键值对（得分记录）
thread C++ STL多线程支持（主要用于获取键盘以及鼠标操作）
编译时加入了-lwinmm指令以实现播放声音的支持（调用mciSendString函数）
```

## 1.3  项目结构 

+ image：包含游戏的图片
    - Land：地板的图片
    - System：各个界面的图片以及按钮的图片
    - Theme：包含四个主题不同的背景和小人
    - Welcome：开始界面相关图片
+ sounds：包含游戏中的声音
+ DoodleJump.h:头文件包含函数的声明
+ DoodleJump.cpp:对子文件的调用
+ Land.cpp:包含Land类，与地块有关
+ Player.cpp:包含Player类，与小人有关
+ SaveData.cpp:包含SaveData类，存取存档
+ Scene.cpp:包含Scene类，场景有关
+ Welcome.cpp:包含Welcome类，欢迎界面

# 第2章  系统设计
## 2.1  四个宏定义
```cpp
_Doodle_Jump_Width_:游戏窗口宽度
_Doodle_Jump_Height_:游戏窗口高度
_land_num_:游戏中所包含的最大地块数量
_Gravity_:重力加速度
```

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

## 2.4  SaveData类
```cpp
void game_save();//游戏存档
bool game_load();//游戏继续
```

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

# 第3章  关键技术介绍

## 3.1  LogoC图片的存储
由于LogoC中IMAGE类的构造函数仅包含按照路径构造，为了方便修改，以及避免在class声明时就将IMAGE类初始化导致异常(IMAGE类必须在好像在调用setup函数之后才能够使用)，我都采用了IMAGE * 指针类型来储存IMAGE对象的地址。  
路径的写法我采用了C++的string类型，方便修改，在最后使用c_str方法转化为C语言风格的字符数组字符串当作路径。

## 3.2  声音的播放与终止
声音的播放我采用的是MCI(Media Control Interface，媒体控制接口)发送命令的方式实现的。  
在GCC等非MSVC编译器中要加入-lwinmm指令使得编译器识别该函数。

## 3.3  键盘操作的获取
由于键盘的长按有判定时间，大概在几百毫秒左右，但是显示在操作中就会导致小人先动一下然后再持续移动。  
为了解决这个问题，我们可以把小人的左移右移设为一个状态，按下左移或者右移时记录下这个状态，然后在松开这些键的时候把状态归零。然后根据当前所处的状态进行操作的更新。

## 3.4  实现简陋的输入框功能
通过接收输入输出存入字符串，再用showtext函数显示出来，虽然简陋，也不支持中文的输入，不过也算是实现了这个功能。

## 3.5  游戏的存档与读档
游戏存档为了防止被修改导致游戏出现异常，使用了二进制来存取数据，对于固定大小的类型直接按所占字节进行存取即可，对于非定长类型例如字符串，可以先存储字符串的长度，再存入字符数据即可，这样读取时就知道需要读取多少字节的数据，实现字符串的二进制读写。

## 3.6  地板的随机生成 
先使用srand((unsigned)time(NULL));按时间初始化随机数种子，再通过rand()函数生成随机数，按照不同概率生成地板类型。可以将概率写入函数参数表中，这样可以更加方便地按照游戏的进行更改不同的概率，提高游戏难度。

## 3.7  小人与地板的碰撞判断
碰撞部分较为愚蠢的采用了手写碰撞距离的判断，不过该游戏所需碰撞的分类并不多，手写距离只是限制了地板以及小人高度的变化。通过多分支结构来判断不同地板的情况。

# 第4章  软件说明书

## 4.1  开始界面

开始：开始游戏并清空存档  
继续：读取存档以开始游戏，若无存档会报错。  
规则：按规则按钮会显示规则。按下一页及返回按钮可以退出规则界面。  
排行榜：显示排行榜，按返回按键或者点击界面的其他地方可以退出排行榜界面。  
设置：点击音乐音效旁的开关可以开关音乐音效；点击主题旁边的四个小人可以更改主题；点击“点我改名”可以改名。点设置界面的其他地方可以退出该界面  

## 4.2  游戏界面

游戏界面：按A/D/←/→来控制小人的运动  
在碰到这三个地板上的时物件时会高速运动一段路程  
碰到该地板时，地板会破碎并反弹一次  
该地板是会左右移动的  
游戏目标就是尽可能得到最高的分数  
掉出游戏界面即角色死亡  
暂停：暂停按键在右上角，也可以通过ESC按键来呼出暂停界面  
设置按键可以调出设置；菜单按键可以回到开始界面，同时也会把当前游戏存档；重玩按键可以重开；按下返回按键退出暂停状态。  
死亡后可以按重玩按钮重新游戏；按排行榜看排行榜；点击菜单回到开始界面  

# 第5章  总结
## 5.1  遇到的问题及解决方案

### 5.1.1  IMAGE对象的创建
出于对性能以及代码可读性的考虑，我决定使用IMAGE对象来存储图片。但是我在创建IMAGE对象时经常遇到错误。  
最后经过单独把IMAGE类抽出来排查发现IMAGE类的创建必须在运行setup()函数之后。  
初步估计setup()函数中应该包含了LogoC图形库初始化相关代码。

### 5.1.2  输入框功能如何实现
由于LogoC中没有输入框相关控件的支持，我只能通过getkey()函数来实现键盘的输入和输出，再使用showtext()函数显示出来。

### 5.1.3  小人运动卡顿问题
游戏中在操作小人运动时会出现小人先动一下然后再持续移动。  
这是因为由于键盘的长按有判定时间，大概在几百毫秒左右。为了解决这个问题，我们可以把小人的左移右移设为一个状态，按下左移或者右移时记录下这个状态，然后在松开这些键的时候把状态归零。然后根据当前所处的状态进行操作的更新，就能实现无延迟的小人移动。

### 5.1.4  鼠标运动卡顿问题
在游戏运行过程中，经常会出现鼠标点击却要过一段时间才有反应的情况，是因为等待接受的mouse信号过多导致迟滞。解决方案就是通过多线程将获取鼠标的代码独立在一个线程中运行。

## 5.2  尚未解决的问题

### 5.2.1  输入框功能仍旧不够完善
所实现的输入框功能过于简陋，仅能实现英文单词和数字的输入，无法实现例如中文的输入，导致玩家名称仅能是英文名，暂无良好的解决方法。

### 5.2.2  MCI对wav格式音乐支持问题
在背景音乐播放完一遍时，我需要重新播放一遍背景音乐，一般来说，我们可以在MCI命令中加一段"repeat"即可，但是在实际操作中，倘若播放的音乐格式是wav格式，不仅不能实现重复，音乐也会播放失败，我只能将需要循环播放的背景音乐转为mp3格式，经测试能够正常循环播放，但是还是对于MCI对wav格式的支持感到不解。

### 5.2.3  字体在不同电脑上显示不同
LogoC中的textfont()函数支持的字体选择仅能通过字体在计算机中的序号来选择，导致了在不同电脑中的字体显示不一样。解决方法也是有的，因为我所需要的仅仅只是数字和字母，所以我可以通过把这些字体以位图的方式呈现，再在显示时截取相应片段即可。  
但是工作量有亿点点大，最终决定还是放弃了。

### 5.2.4  代码行数过多，可读性较差
项目所有文件的代码总行数超过了1000行(10+171+466+90+86+111+232=1166)，在UI设计中的行数过于庞大(超过半数代码)，在实现基础的游戏功能时仅用了200多行的代码，在项目逐渐庞大之后，不经常写注释的弊端便暴露出来，导致后半段写的代码可能包含一些重复的代码，降低了可读性，也使得代码看上去没有那么美观。

### 5.2.5  CPU占用过高
写出来的程序所占用的资源过多，在我自己的电脑（2015款的MacBook Air）会有极为严重的卡顿现象，即使使用多线程也无法改善。而且在学校机房的电脑上，运行游戏时也会有接近20%的CPU占用，暂无良好的解决方案。

# 参考资料
> 1. 《LogoC 使用参考》 虞铭财
> 2. 菜鸟教程
