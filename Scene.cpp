#include "DoodleJump.h"
Scene::Scene() {
	im_bk = new IMAGE("image/Theme/Classic/BackGround.png");
	im_basicline = new IMAGE("image/Theme/Classic/basicline.png");
	direct = 0;
}
void Scene::change_theme(string theme, string path) {
	im_bk = new IMAGE((path + theme + "/BackGround.png").c_str());
	im_basicline = new IMAGE((path + theme + "/basicline.png").c_str());
}
void Scene::draw() {
	moveto(0, 0);
	showimage(im_bk);
	showimage(im_basicline);
}
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
void Scene::show() {
	scene.draw();
	land.draw();
	player.draw();
}
void Scene::updateWithoutInput() {
	player.JudgeisPlayer_yMax();
	if (player.isPlayer_yMax == 0)
		player.updateYcoordinate();
	else land.updateLandY();
	if (player.playerStatus == fly_left1 || player.playerStatus == fly_left2 || player.playerStatus == fly_right1 || player.playerStatus == fly_right2) {
		player.autoJump_fly();
		if (player.vy == 0 && land.land_vy <= _Gravity_ && land.land_vy >= -_Gravity_) {
			if (player.playerStatus == fly_left1 || player.playerStatus == fly_left2) {
				player.playerStatus = p_left;
			}
			if (player.playerStatus == fly_right1 || player.playerStatus == fly_right2) {
				player.playerStatus = p_right;
			}
			player.autoJump();
		}
	} else if (player.playerStatus == rocket_left || player.playerStatus == rocket_right) {
		if (player.vy == 0 && land.land_vy <= _Gravity_ && land.land_vy >= -_Gravity_) {
			if (player.playerStatus == rocket_left) {
				player.playerStatus = p_left;
			}
			if (player.playerStatus == rocket_right) {
				player.playerStatus = p_right;
			}
			player.autoJump();
		}
	} else if (fabs(player.vy) < 4 && fabs(land.land_vy) < 4) {
		player.autoJump();
	}
	player.isOnLand();
}
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
