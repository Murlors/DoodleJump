#pragma once
#include "fstream"
#include "map"
#include "thread"
#include "iostream"
#include "Logo.h"
#define _Doodle_Jump_Width_ 500
#define _Doodle_Jump_Height_ 800
#define _land_num_ 10
#define _Gravity_ 0.15
using namespace std;
enum InterfaceType {
	I_welcome, I_rule1, I_rule2, I_rank, I_option, I_change_name, I_gamerun, I_gamepause, I_gameretry, I_gameover
};
enum DJTheme {
	Classic, Jungle, Underwater, Winter
};
enum PlayerStatus {
	p_left, p_right, fly_left1, fly_right1, fly_left2, fly_right2, rocket_left, rocket_right
};
enum LandType {
	normal, fragile, broken, broken_over, mvland, landfly, landrocket, mvspring1, landspring1, mvspring2, landspring2
};
MOUSEMSG msmsg;
KEYMSG kmsg;
struct sortcmp {
	bool operator ()(const int &a, const int &b) const {
		return a > b;
	}
};
class Welcome {
	public:
		IMAGE * im_bk;
		IMAGE * im_bk_bottombar;
		IMAGE * button_start;
		IMAGE * button_continue;
		IMAGE * button_rule;
		IMAGE * button_option;
		IMAGE * button_option_rect;
		IMAGE * button_menu;
		IMAGE * button_rank;
		IMAGE * button_rank_rect;
		IMAGE * button_next;
		IMAGE * button_return;
		IMAGE * button_retry;
		IMAGE * button_on;
		IMAGE * button_off;
		IMAGE * button_pause;
		IMAGE * im_circle;
		IMAGE * im_option;
		IMAGE * im_change_name;
		IMAGE * im_rank;
		IMAGE * im_crown1;
		IMAGE * im_crown2;
		IMAGE * im_crown3;
		IMAGE * im_rule1;
		IMAGE * im_rule2;
		IMAGE * im_topbar;
		IMAGE * im_pauselabel;
		IMAGE * im_you_died;
		IMAGE * im_your_name;
		IMAGE * im_your_score;
		IMAGE * im_highest_score;
		IMAGE * im_gameover;
		InterfaceType interfacetype;
		DJTheme themetype;
		string path, theme, name;
		thread p_mouseget;
		bool bgm, sound_effect;
		multimap<int, string, sortcmp> scorerank_map;
		Welcome();
		void rank_read();//读取排名
		void rank_save();//写入排名
		void change_name();
		void welcome_mouseget();
		void main_interface();//开始界面循环函数
		void draw_welcome();
		void show_rule();//规则
		void show_rank(InterfaceType pre);//排行榜
		void show_option(InterfaceType pre);//设置
		void show_pauselabel();
		void show_gameover();
		void change_theme();
		void draw_jump();//开始界面跳动小人
		void pauseget_mouse();
		void game_run();//游戏运行循环函数
};
class Scene {
	public:
		IMAGE * im_bk;
		IMAGE * im_basicline;
		int direct;
		Scene();
		void change_theme(string theme, string path = "image/Theme/");
		void draw();
		void PlayBGM(string MusicPath = "sounds/bgm.mp3");
		void PlayMusic(string MusicPath);
		void show();
		void updateWithoutInput();
		void updateWithKeyInput();
};
class Land {
	public:
		IMAGE *im_normal; //正常地面
		IMAGE *im_break; //易损地面
		IMAGE *im_move; //移动地面
		IMAGE *im_broken; //损坏地面
		IMAGE *im_landfly; //竹蜻蜓地板
		IMAGE *im_landrocket; //火箭地板
		IMAGE *im_landspring1; //带有弹簧的正常地面
		IMAGE *im_landspring2; //弹簧被踩到的正常地面
		IMAGE *im_mvspring1; //带有弹簧的移动地面
		IMAGE *im_mvspring2; //弹簧被踩到的移动地面
		float land_width, land_height, land_vy;
		int score;
		int broken_y; //记录破碎地板的y坐标
		struct LandState {
			float middle_x;
			float top_y;
			float vx;
			LandType landType;
			IMAGE *im_land;
		} land[_land_num_];

		Land(string path = "image/Land/");
		void retry_clean();
		void Land_type(int i, int lnor, int lfr, int mv, int lspr, int lfly, int mvspr, int lroc);//随机生成地板
		void draw();
		void show_topbar();
		void updateLandY();//更新地板
};
class Player {
	public:
		IMAGE *im_show;
		IMAGE *im_left;
		IMAGE *im_right;
		IMAGE *im_fly_left1;
		IMAGE *im_fly_left2;
		IMAGE *im_fly_right1;
		IMAGE *im_fly_right2;
		IMAGE *im_rocket_left;
		IMAGE *im_rocket_right;
		PlayerStatus playerStatus;
		float x_middle, y_bottom;
		float vx, vy;
		float width, height;
		float rebound_vy;
		bool isPlayer_yMax, isPlayer_died;
		Player(string path = "image/Theme/", string theme = "Classic");
		void retry_clean();
		void change_theme(string theme, string path = "image/Theme/");
		void draw();
		void moveLeft();
		void moveRight();
		void autoJump();
		void autoJump_fly();
		void autoJump_rocket();
		void JudgeisPlayer_yMax();
		void isOnLand();
		void updateYcoordinate();
		void JudgeisPlayer_died();
};
class SaveData {
	public:
		void game_save(); //游戏存档
		bool game_load(); //游戏继续
};
Welcome welcome;
Scene scene;
Land land;
Player player;
SaveData savedata;
