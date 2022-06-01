#include "DoodleJump.h"
Land::Land(string path) {
	im_normal = new IMAGE((path + "land.png").c_str());
	im_break = new IMAGE((path + "break.png").c_str());
	im_move = new IMAGE((path + "mvland.png").c_str());
	im_broken = new IMAGE((path + "broken.png").c_str());
	im_landfly = new IMAGE((path + "landfly.png").c_str());
	im_landrocket = new IMAGE((path + "landrocket.png").c_str());
	im_landspring1 = new IMAGE((path + "landspring1.png").c_str());
	im_landspring2 = new IMAGE((path + "landspring2.png").c_str());
	im_mvspring1 = new IMAGE((path + "mvspring1.png").c_str());
	im_mvspring2 = new IMAGE((path + "mvspring2.png").c_str());
	land_width = im_normal->getwidth();
	land_height = im_normal->getheight();
	retry_clean();
}
void Land::retry_clean() {
	score = 0;
	for (int i = 0; i < _land_num_; i++)land[i].vx = 1.0;
	for (int i = 0; i < _land_num_; i++)Land_type(i, 54, 72, 84, 90, 95, 98, 100);
	for (int i = 0; i < _land_num_; i++)land[i].top_y = (i + 1) * (_Doodle_Jump_Height_ / _land_num_);
}
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
void Land::draw() {
	for (int i = 0; i < _land_num_; i++) {
		if (land[i].landType == broken) {
			moveto(land[i].middle_x, -((broken_y += 5) - _Doodle_Jump_Height_ / 2));//破碎地板下落
			showimage(im_broken);
		} else if (land[i].landType == mvland || land[i].landType == mvspring1) {//移动的方块不超过边界
			if (land[i].middle_x - land_width / 2 <= -_Doodle_Jump_Width_ / 2 || land[i].middle_x + land_width / 2 >= _Doodle_Jump_Width_ / 2) {
				if (land[i].middle_x - land_width / 2 <= -_Doodle_Jump_Width_ / 2)
					land[i].middle_x = -_Doodle_Jump_Width_ / 2 + land_width / 2;
				if (land[i].middle_x + land_width / 2 >= _Doodle_Jump_Width_ / 2)
					land[i].middle_x = _Doodle_Jump_Width_ / 2 - land_width / 2;
				land[i].vx = -land[i].vx;
			}
			moveto(land[i].middle_x += land[i].vx, -(land[i].top_y - _Doodle_Jump_Height_ / 2));
			showimage(land[i].im_land);
		} else if (land[i].landType == normal || land[i].landType == landspring1 || land[i].landType == fragile) {
			moveto(land[i].middle_x, -(land[i].top_y - _Doodle_Jump_Height_ / 2));
			showimage(land[i].im_land);
		} else if (land[i].landType == landfly) {
			moveto(land[i].middle_x, -(land[i].top_y - 25 - _Doodle_Jump_Height_ / 2));
			showimage(im_landfly);
		} else if (land[i].landType == landrocket) {
			moveto(land[i].middle_x, -(land[i].top_y - 25 - _Doodle_Jump_Height_ / 2));
			showimage(im_landrocket);
		} else if (land[i].landType == landspring2) {
			moveto(land[i].middle_x, -(land[i].top_y - 25 - _Doodle_Jump_Height_ / 2));
			showimage(im_landspring2);
		} else if (land[i].landType == mvspring2) {
			moveto(land[i].middle_x, -(land[i].top_y - 25 - _Doodle_Jump_Height_ / 2));
			showimage(im_mvspring2);
		}
	}
	show_topbar();
}
void Land::show_topbar() {
	moveto(0, 370);
	showimage(welcome.im_topbar);
	moveto(-175, 375);
	textcolor(32, 32, 32);
	textsize(24);
	showtext(score);
	moveto(200, 375);
	showimage(welcome.button_pause);
}
void Land::updateLandY() {//销毁和生成
	land_vy -= _Gravity_;
	for (int i = 0; i < _land_num_; i++) {
		land[i].top_y += land_vy;
		if (land[i].landType == broken) {
			broken_y += land_vy;
			if (broken_y > _Doodle_Jump_Height_)
				land[i].landType = broken_over;
		}
		if (land_vy > 0)
			score += land_vy / 6.0;
	}
	for (int i = 0; i < _land_num_; i++)
		if (land[i].top_y >= _Doodle_Jump_Height_) {
			if (score < 1e4)Land_type(i, 54, 72, 84, 90, 95, 98, 100);
			else if (score < 5e4)Land_type(i, 36, 64, 80, 86, 90, 97, 100);
			else if (score < 1e5)Land_type(i, 25, 60, 80, 86, 90, 96, 100);
			else Land_type(i, 10, 60, 85, 88, 92, 97, 100);
			land[i].middle_x = (rand() % (_Doodle_Jump_Width_ - (int)land_width) + land_width / 2) - _Doodle_Jump_Width_ / 2;
			land[i].top_y = land[i].top_y - _Doodle_Jump_Height_;
		}
}
