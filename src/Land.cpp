#include "Common.h"

Land::Land(const string &path) {
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
    land_width = (float) im_normal->getwidth();
    land_height = (float) im_normal->getheight();
    retry_clean();
}

void Land::retry_clean() {
    score = 0;
    for (int i = 0; i < LAND_NUM; i++) {
        land[i].vx = 1.0;
        Land_type(i, 54, 72, 84, 90, 95, 98, 100);
        land[i].top_y = (i + 1) * (DOODLE_JUMP_HEIGHT / LAND_NUM);
    }
}

void Land::Land_type(int i, int lnor, int lfr, int mv, int lspr, int lfly, int mvspr, int lroc) {
    unsigned int rd = mt_rand() % 100;
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
    land[i].middle_x = mt_rand() % (DOODLE_JUMP_WIDTH - (int) land_width - DOODLE_JUMP_WIDTH / 2);
}

void Land::draw() {
    for (auto &i: land) {
        if (i.landType == broken) {
            moveto(i.middle_x, -((broken_y += 5) - DOODLE_JUMP_HEIGHT / 2));//破碎地板下落
            showimage(im_broken);
        } else if (i.landType == mvland || i.landType == mvspring1) {//移动的方块不超过边界
            if (i.middle_x - land_width / 2 <= -DOODLE_JUMP_WIDTH / 2 ||
                i.middle_x + land_width / 2 >= DOODLE_JUMP_WIDTH / 2) {
                if (i.middle_x - land_width / 2 <= -DOODLE_JUMP_WIDTH / 2)
                    i.middle_x = -DOODLE_JUMP_WIDTH / 2 + land_width / 2;
                if (i.middle_x + land_width / 2 >= DOODLE_JUMP_WIDTH / 2)
                    i.middle_x = DOODLE_JUMP_WIDTH / 2 - land_width / 2;
                i.vx = -i.vx;
            }
            moveto(i.middle_x += i.vx, -(i.top_y - DOODLE_JUMP_HEIGHT / 2));
            showimage(i.im_land);
        } else if (i.landType == normal || i.landType == landspring1 || i.landType == fragile) {
            moveto(i.middle_x, -(i.top_y - DOODLE_JUMP_HEIGHT / 2));
            showimage(i.im_land);
        } else if (i.landType == landfly) {
            moveto(i.middle_x, -(i.top_y - 25 - DOODLE_JUMP_HEIGHT / 2));
            showimage(im_landfly);
        } else if (i.landType == landrocket) {
            moveto(i.middle_x, -(i.top_y - 25 - DOODLE_JUMP_HEIGHT / 2));
            showimage(im_landrocket);
        } else if (i.landType == landspring2) {
            moveto(i.middle_x, -(i.top_y - 25 - DOODLE_JUMP_HEIGHT / 2));
            showimage(im_landspring2);
        } else if (i.landType == mvspring2) {
            moveto(i.middle_x, -(i.top_y - 25 - DOODLE_JUMP_HEIGHT / 2));
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
    land_vy -= GRAVITY;
    for (auto &i: land) {
        i.top_y += land_vy;
        if (i.landType == broken) {
            broken_y += land_vy;
            if (broken_y > DOODLE_JUMP_HEIGHT)
                i.landType = broken_over;
        }
        if (land_vy > 0)
            score += land_vy / 6.0;
    }
    for (int i = 0; i < LAND_NUM; i++)
        if (land[i].top_y >= DOODLE_JUMP_HEIGHT) {
            if (score < 1e4)Land_type(i, 54, 72, 84, 90, 95, 98, 100);
            else if (score < 5e4)Land_type(i, 36, 64, 80, 86, 90, 97, 100);
            else if (score < 1e5)Land_type(i, 25, 60, 80, 86, 90, 96, 100);
            else Land_type(i, 10, 60, 85, 88, 92, 97, 100);
            land[i].middle_x =
                    (mt_rand() % (DOODLE_JUMP_WIDTH - (int) land_width) + land_width / 2) - DOODLE_JUMP_WIDTH / 2;
            land[i].top_y = land[i].top_y - DOODLE_JUMP_HEIGHT;
        }
}
