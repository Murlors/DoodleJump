#include "DoodleJump.h"
Player::Player(string path, string theme) {
	path += theme + "/";
	im_left = new IMAGE((path + "Doodle NormalLeft.png").c_str());
	im_right = new IMAGE((path + "Doodle NormalRight.png").c_str());
	im_fly_left1 = new IMAGE((path + "Doodle FlyLeft1.png").c_str());
	im_fly_left2 = new IMAGE((path + "Doodle FlyLeft2.png").c_str());
	im_fly_right1 = new IMAGE((path + "Doodle FlyRight1.png").c_str());
	im_fly_right2 = new IMAGE((path + "Doodle FlyRight2.png").c_str());
	im_rocket_left = new IMAGE((path + "Doodle RocketLeft.png").c_str());
	im_rocket_right = new IMAGE((path + "Doodle RocketRight.png").c_str());
	width = im_left->getwidth();
	height = im_left->getheight();
	retry_clean();
}
void Player::retry_clean() {
	playerStatus = p_right;
	im_show = im_right;
	x_middle = 0;
	y_bottom = _Doodle_Jump_Height_ / 2;
	vx = 4;
	vy = 0;
	rebound_vy = 8;
	isPlayer_yMax = false;
	isPlayer_died = false;
}
void Player::change_theme(string theme, string path) {
	path += theme + "/";
	im_left = new IMAGE((path + "Doodle NormalLeft.png").c_str());
	im_right = new IMAGE((path + "Doodle NormalRight.png").c_str());
	im_fly_left1 = new IMAGE((path + "Doodle FlyLeft1.png").c_str());
	im_fly_left2 = new IMAGE((path + "Doodle FlyLeft2.png").c_str());
	im_fly_right1 = new IMAGE((path + "Doodle FlyRight1.png").c_str());
	im_fly_right2 = new IMAGE((path + "Doodle FlyRight2.png").c_str());
	im_rocket_left = new IMAGE((path + "Doodle RocketLeft.png").c_str());
	im_rocket_right = new IMAGE((path + "Doodle RocketRight.png").c_str());
	width = im_left->getwidth();
	height = im_left->getheight();
	if (playerStatus == p_left) {
		im_show = im_left;
	} else if (playerStatus == p_right) {
		im_show = im_right;
	} else if (playerStatus == fly_left1) {
		im_show = im_fly_left1;
	} else if (playerStatus == fly_left2) {
		im_show = im_fly_left2;
	} else if (playerStatus == fly_right1) {
		im_show = im_fly_right1;
	} else if (playerStatus == fly_right2) {
		im_show = im_fly_right2;
	} else if (playerStatus == rocket_left) {
		im_show = im_rocket_left;
	} else if (playerStatus == rocket_right) {
		im_show = im_rocket_right;
	}
}
void Player::draw() {
	moveto(x_middle, -(y_bottom - height / 2 - _Doodle_Jump_Height_ / 2));
	showimage(im_show);
}
void Player::moveLeft() {
	x_middle -= vx;
	if (playerStatus == p_right) {
		playerStatus = p_left;
		im_show = im_left;
	} else if (playerStatus == fly_right1) {
		playerStatus = fly_left2;
		im_show = im_fly_left2;
	} else if (playerStatus == fly_right2) {
		playerStatus = fly_left1;
		im_show = im_fly_left1;
	} else if (playerStatus == rocket_right) {
		playerStatus = rocket_left;
		im_show = im_rocket_left;
	}
	if (x_middle <= -_Doodle_Jump_Width_ / 2) {
		x_middle = _Doodle_Jump_Width_ / 2;
	}
}
void Player::moveRight() {
	x_middle += vx;
	if (playerStatus == p_left) {
		playerStatus = p_right;
		im_show = im_right;
	} else if (playerStatus == fly_left1) {
		playerStatus = fly_right2;
		im_show = im_fly_right2;
	} else if (playerStatus == fly_left2) {
		playerStatus = fly_right1;
		im_show = im_fly_right1;
	} else if (playerStatus == rocket_left) {
		playerStatus = rocket_right;
		im_show = im_rocket_right;
	}
	if (x_middle >= _Doodle_Jump_Width_ / 2) {
		x_middle = -_Doodle_Jump_Width_ / 2;
	}
}
void Player::autoJump() {
	if (playerStatus == p_left) {
		im_show = im_left;
	} else if (playerStatus == p_right) {
		im_show = im_right;
	}
}
void Player::autoJump_fly() {
	if (playerStatus == p_left) {
		playerStatus = fly_left1;
		im_show = im_fly_left1;
	} else if (playerStatus == p_right) {
		playerStatus = fly_right1;
		im_show = im_fly_right1;
	} else if (playerStatus == fly_left1) {
		playerStatus = fly_left2;
		im_show = im_fly_left2;
	} else if (playerStatus == fly_left2) {
		playerStatus = fly_left1;
		im_show = im_fly_left1;
	} else if (playerStatus == fly_right1) {
		playerStatus = fly_right2;
		im_show = im_fly_right2;
	} else if (playerStatus == fly_right2) {
		playerStatus = fly_right1;
		im_show = im_fly_right1;
	}
}
void Player::autoJump_rocket() {
	if (playerStatus == p_left) {
		playerStatus = rocket_left;
		im_show = im_rocket_left;
	} else if (playerStatus == p_right) {
		playerStatus = rocket_right;
		im_show = im_rocket_right;
	}
}
void Player::JudgeisPlayer_yMax() {
	if ((y_bottom < _Doodle_Jump_Height_ / 2) && (vy != 0 || land.land_vy > 0) && !isPlayer_yMax) {
		y_bottom = _Doodle_Jump_Height_ / 2;
		isPlayer_yMax = true;
		land.land_vy = -vy;
		vy = 0;
	}
	if ((land.land_vy <= 0) && (vy == 0)) {
		land.land_vy = 0;
		isPlayer_yMax = false;
	}
}
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
void Player::updateYcoordinate() {
	vy += _Gravity_;
	y_bottom += vy;
}
void Player::JudgeisPlayer_died() {
	if (y_bottom >= _Doodle_Jump_Height_) {
		if (!isPlayer_died) {
			if (welcome.sound_effect)scene.PlayMusic("sounds/falldown.wav");
			isPlayer_died = true;
		}
		if (y_bottom <= _Doodle_Jump_Height_ * 2) {
			y_bottom += vy;
			isPlayer_yMax = true;
			land.land_vy = -vy;
			moveto(0, 0);
			showimage(welcome.im_you_died);
		} else {
			welcome.interfacetype = I_gameover;
		}
	}
}
