#include "Common.h"

Player::Player(string path, const string &theme) {
    path += theme + "/";
    im_left = new IMAGE((path + "Doodle NormalLeft.png").c_str());
    im_right = new IMAGE((path + "Doodle NormalRight.png").c_str());
    im_fly_left1 = new IMAGE((path + "Doodle FlyLeft1.png").c_str());
    im_fly_left2 = new IMAGE((path + "Doodle FlyLeft2.png").c_str());
    im_fly_right1 = new IMAGE((path + "Doodle FlyRight1.png").c_str());
    im_fly_right2 = new IMAGE((path + "Doodle FlyRight2.png").c_str());
    im_rocket_left = new IMAGE((path + "Doodle RocketLeft.png").c_str());
    im_rocket_right = new IMAGE((path + "Doodle RocketRight.png").c_str());
    width = (float) im_left->getwidth();
    height = (float) im_left->getheight();
    retry_clean();
}

void Player::retry_clean() {
    playerStatus = p_right;
    im_show = im_right;
    x_middle = 0;
    y_bottom = DOODLE_JUMP_HEIGHT / 2;
    vx = 4;
    vy = 0;
    rebound_vy = 8;
    isPlayer_yMax = false;
    isPlayer_died = false;
}

void Player::change_theme(const string &theme, string path) {
    path += theme + "/";
    im_left = new IMAGE((path + "Doodle NormalLeft.png").c_str());
    im_right = new IMAGE((path + "Doodle NormalRight.png").c_str());
    im_fly_left1 = new IMAGE((path + "Doodle FlyLeft1.png").c_str());
    im_fly_left2 = new IMAGE((path + "Doodle FlyLeft2.png").c_str());
    im_fly_right1 = new IMAGE((path + "Doodle FlyRight1.png").c_str());
    im_fly_right2 = new IMAGE((path + "Doodle FlyRight2.png").c_str());
    im_rocket_left = new IMAGE((path + "Doodle RocketLeft.png").c_str());
    im_rocket_right = new IMAGE((path + "Doodle RocketRight.png").c_str());
    width = (float) im_left->getwidth();
    height = (float) im_left->getheight();
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
    moveto(x_middle, -(y_bottom - height / 2 - DOODLE_JUMP_HEIGHT / 2));
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
    if (x_middle <= -DOODLE_JUMP_WIDTH / 2) {
        x_middle = DOODLE_JUMP_WIDTH / 2;
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
    if (x_middle >= DOODLE_JUMP_WIDTH / 2) {
        x_middle = -DOODLE_JUMP_WIDTH / 2;
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
    if (playerStatus == p_left || playerStatus == fly_left2) {
        playerStatus = fly_left1;
        im_show = im_fly_left1;
    } else if (playerStatus == p_right || playerStatus == fly_right2) {
        playerStatus = fly_right1;
        im_show = im_fly_right1;
    } else if (playerStatus == fly_left1) {
        playerStatus = fly_left2;
        im_show = im_fly_left2;
    } else if (playerStatus == fly_right1) {
        playerStatus = fly_right2;
        im_show = im_fly_right2;
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
    if ((y_bottom < DOODLE_JUMP_HEIGHT / 2) && (vy != 0 || land.land_vy > 0) && !isPlayer_yMax) {
        y_bottom = DOODLE_JUMP_HEIGHT / 2;
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
    for (auto & i : land.land) {
        if ((abs(y_bottom - i.top_y) <= vy) && (vy > 0)) {
            if (i.landType == mvspring1 || i.landType == landspring1) {
                if ((x_middle + 61 <= i.middle_x) ||
                    ((x_middle + 25) >= i.middle_x + land.land_width)) { // 大约估算图像中人物脚的距离,玩家没有落在地面上
                    return;
                } else if ((x_middle + 61 >= i.middle_x + 30) &&
                           ((x_middle + 25) <= i.middle_x + 60)) { // 大约估算图像中人物脚的距离，玩家踩在弹簧上
                    if (welcome.sound_effect)scene.PlayMusic("sounds/spring.wav");
                    if (i.landType == mvspring1)
                        i.landType = mvspring2;
                    else {
                        i.landType = landspring2;
                    }
                    y_bottom = i.top_y;
                    vy = -(2.5) * rebound_vy;
                } else {
                    if (welcome.sound_effect)scene.PlayMusic("sounds/jump.wav");
                    vy = -rebound_vy;
                }
                autoJump();
            } else if (i.landType == landfly) {
                if ((x_middle + 61 <= i.middle_x) ||
                    ((x_middle + 25) >= i.middle_x + land.land_width)) { // 大约估算图像中人物脚的距离,玩家没有落在地面上
                    return;
                } else if ((x_middle + 61 >= i.middle_x + 30) &&
                           ((x_middle + 25) <= i.middle_x + 60)) { // 大约估算图像中人物脚的距离，玩家踩在竹蜻蜓上
                    if (welcome.sound_effect)scene.PlayMusic("sounds/fly.wav");
                    y_bottom = i.top_y;
                    vy = -(4.2) * rebound_vy;
                    autoJump_fly();
                } else {
                    if (welcome.sound_effect)scene.PlayMusic("sounds/jump.wav");
                    vy = -rebound_vy;
                    autoJump();
                }
            } else if (i.landType == landrocket) {
                if ((x_middle + 61 <= i.middle_x) ||
                    ((x_middle + 25) >= i.middle_x + land.land_width)) { // 大约估算图像中人物脚的距离,玩家没有落在地面上
                    return;
                } else if ((x_middle + 61 >= i.middle_x + 30) &&
                           ((x_middle + 25) <= i.middle_x + 60)) { // 大约估算图像中人物脚的距离，玩家踩在竹蜻蜓上
                    if (welcome.sound_effect)scene.PlayMusic("sounds/rocket.wav");
                    y_bottom = i.top_y;
                    vy = -(5.8) * rebound_vy;
                    autoJump_rocket();
                } else {
                    if (welcome.sound_effect)scene.PlayMusic("sounds/jump.wav");
                    vy = -rebound_vy;
                    autoJump();
                }
            } else if (i.landType != broken) { //其他类型地面判断
                if ((x_middle + 61 >= i.middle_x) &&
                    ((x_middle + 25) <= i.middle_x + land.land_width &&
                     i.landType != broken_over)) { // 大约估算图像中人物脚的距离
                    if (i.landType != fragile) {
                        if (welcome.sound_effect)scene.PlayMusic("sounds/jump.wav");
                    } else {
                        if (welcome.sound_effect)scene.PlayMusic("sounds/break.wav");
                        i.landType = broken;
                        land.broken_y = i.top_y;
                    }
                    y_bottom = i.top_y;
                    vy = -rebound_vy;
                    autoJump();
                }
            }
        }
    }
    JudgeisPlayer_died();
}

void Player::updateYcoordinate() {
    vy += GRAVITY;
    y_bottom += vy;
}

void Player::JudgeisPlayer_died() {
    if (y_bottom >= DOODLE_JUMP_HEIGHT) {
        if (!isPlayer_died) {
            if (welcome.sound_effect)scene.PlayMusic("sounds/falldown.wav");
            isPlayer_died = true;
        }
        if (y_bottom <= DOODLE_JUMP_HEIGHT * 2) {
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
