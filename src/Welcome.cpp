#include "Common.h"

Welcome::Welcome() {
    setup(DOODLE_JUMP_WIDTH, DOODLE_JUMP_HEIGHT, 0, 0, false);
    title("Í¿Ñ»ÌøÔ¾@ºúÁÕÌÎ");
    hidepen();
    penspeed(11);
    textsize(24);
    textfont(26);//Comic Sans MS//26//81
    bgm = true;
    sound_effect = true;
    if (sound_effect)scene.PlayBGM();
    path = "image/Welcome/";
    name = "PLAYER";
    theme = "Classic";
    rank_read();
    interfacetype = I_welcome;
    im_bk = new IMAGE((path + "background.png").c_str());
    im_bk_bottombar = new IMAGE((path + "Classic.png").c_str());
    im_rule1 = new IMAGE((path + "rule1.png").c_str());
    im_rule2 = new IMAGE((path + "rule2.png").c_str());
    path = "image/System/";
    im_topbar = new IMAGE((path + "topbar.png").c_str());
    im_pauselabel = new IMAGE((path + "pauselabel.png").c_str());
    im_option = new IMAGE((path + "option.png").c_str());
    im_change_name = new IMAGE((path + "change_name.png").c_str());
    im_rank = new IMAGE((path + "rank.png").c_str());
    im_your_name = new IMAGE((path + "your name.png").c_str());
    im_your_score = new IMAGE((path + "your score.png").c_str());
    im_highest_score = new IMAGE((path + "highest score.png").c_str());
    im_crown1 = new IMAGE((path + "crown1.png").c_str());
    im_crown2 = new IMAGE((path + "crown2.png").c_str());
    im_crown3 = new IMAGE((path + "crown3.png").c_str());
    im_you_died = new IMAGE((path + "died.png").c_str());
    im_gameover = new IMAGE((path + "gameover.png").c_str());
    path += "button/";
    button_start = new IMAGE((path + "start.png").c_str());
    button_continue = new IMAGE((path + "continue.png").c_str());
    button_rule = new IMAGE((path + "rule.png").c_str());
    button_option = new IMAGE((path + "option.png").c_str());
    button_option_rect = new IMAGE((path + "option_rect.png").c_str());
    button_menu = new IMAGE((path + "menu.png").c_str());
    button_rank = new IMAGE((path + "rank.png").c_str());
    button_rank_rect = new IMAGE((path + "rank_rect.png").c_str());
    button_next = new IMAGE((path + "next.png").c_str());
    button_return = new IMAGE((path + "return.png").c_str());
    button_retry = new IMAGE((path + "retry.png").c_str());
    button_on = new IMAGE((path + "on.png").c_str());
    button_off = new IMAGE((path + "off.png").c_str());
    button_pause = new IMAGE((path + "pause.png").c_str());
    im_circle = new IMAGE((path + "circle.png").c_str());
}

void Welcome::rank_read() {
    ifstream rank;
    string rub, rna;
    int rscore;
    rank.open("rank.txt", ios::in);
    rank >> rub >> name;
    while (rank >> rna >> rscore)
        scorerank_map.insert(make_pair(rscore, rna));
    rank.close();
}

void Welcome::rank_save() {
    ofstream rank;
    rank.open("rank.txt", ios::out);
    rank << "PlayerName " << name << "\n";
    for (auto & i : scorerank_map)
        rank << i.second << " " << i.first << "\n";
    rank.close();
}

void Welcome::change_name() {
    string in = name;
    moveto(0, 0);
    showimage(im_change_name);
    showtext(in.c_str());
    wait(0.0001);
    int st = getclock(), ed;
    while (ed = getclock(), ed - st < 10) {
        getkey();
        getmouse();
    }
    while (interfacetype == I_change_name) {
        bool is_change = false;
        if (keymsg()) {
            kmsg = getkey();
            if (kmsg.flag == KEY_DOWN) {
                if (isalnum(kmsg.key)) {
                    if (in.length() <= 10) {
                        in += (char) kmsg.key;
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
        if (mousemsg()) {
            msmsg = getmouse();
            if (msmsg.flag == BUTTON_UP) {
                if (!(msmsg.x >= -75 && msmsg.x <= 75 && msmsg.y >= -150 && msmsg.y <= 150)) {
                    interfacetype = I_option;
                }
            }
        }
        if (is_change) {
            draw_welcome();
            draw_jump();
            moveto(0, 0);
            showimage(im_option);
            moveto(30, 110);
            if (bgm)showimage(button_on);
            else showimage(button_off);
            move(0, -50);
            if (sound_effect)showimage(button_on);
            else showimage(button_off);
            if (themetype == Classic)moveto(0, 5);
            else if (themetype == Jungle)moveto(85, 5);
            else if (themetype == Underwater)moveto(0, -50);
            else if (themetype == Winter)moveto(85, -50);
            showimage(im_circle);
            moveto(0, 0);
            showimage(im_change_name);
            showtext(in.c_str());
            wait(0.0001);
        }
    }
    name = in;
    ofstream savedata;
    savedata.open("savedata.dat", ios::out | ios::binary);
    savedata.close();
    welcome.rank_save();
}

void Welcome::welcome_mouseget() {
    while (interfacetype == I_welcome) {
        if (mousemsg()) {
            msmsg = getmouse();
            if (msmsg.flag == BUTTON_UP) {
                if (msmsg.x >= -180 && msmsg.x <= -20 && msmsg.y >= 80 && msmsg.y <= 140) {
                    interfacetype = I_gamerun;
                } else if (msmsg.x >= -150 && msmsg.x <= 10 && msmsg.y >= -5 && msmsg.y <= 55) {
                    if (savedata.game_load())
                        interfacetype = I_gamerun;
                    else msgbox("ÎÞ´æµµ", "ÎÞ´æµµ", MB_OK | MB_ICONWARNING);
                } else if (msmsg.x >= 60 && msmsg.x <= 220 && msmsg.y >= -40 && msmsg.y <= 20) {
                    interfacetype = I_rule1;
                } else if (msmsg.x >= -130 && msmsg.x <= 30 && msmsg.y >= -305 && msmsg.y <= -245) {
                    interfacetype = I_rank;
                } else if (msmsg.x >= 50 && msmsg.x <= 210 && msmsg.y >= -305 && msmsg.y <= -245) {
                    interfacetype = I_option;
                }
            }
        }
    }
}

void Welcome::main_interface() {
    p_mouseget = thread(&Welcome::welcome_mouseget, this);
    p_mouseget.detach();
    while (interfacetype != I_gamerun) {
        draw_welcome();
        draw_jump();
        moveto(50, -180);
        showtext(name.c_str());
        wait(0.008);
        if (interfacetype == I_rule1 || interfacetype == I_rule2) {
            show_rule();
            p_mouseget = thread(&Welcome::welcome_mouseget, this);
            p_mouseget.detach();
        } else if (interfacetype == I_rank) {
            show_rank(I_welcome);
            p_mouseget = thread(&Welcome::welcome_mouseget, this);
            p_mouseget.detach();
        } else if (interfacetype == I_option) {
            show_option(I_welcome);
            p_mouseget = thread(&Welcome::welcome_mouseget, this);
            p_mouseget.detach();
        }
    }
    if (interfacetype == I_gamerun)game_run();
}

void Welcome::draw_welcome() {
    moveto(0, -350);
    showimage(im_bk_bottombar);
    moveto(0, 0);
    showimage(im_bk);
    moveto(-100, 110);
    showimage(button_start);
    moveto(-70, 25);
    showimage(button_continue);
    moveto(140, -10);
    showimage(button_rule);
    moveto(-50, -275);
    showimage(button_rank);
    move(180, 0);
    showimage(button_option);
}

void Welcome::show_rule() {
    moveto(0, 0);
    showimage(im_rule1);
    moveto(130, -80);
    showimage(button_next);
    wait(0.0001);
    while (interfacetype == I_rule1) {
        msmsg = getmouse();
        if (msmsg.flag == BUTTON_UP)
            if (msmsg.x >= 80 && msmsg.x <= 180 && msmsg.y >= -105 && msmsg.y <= -55)
                interfacetype = I_rule2;
    }
    moveto(0, 0);
    showimage(im_rule2);
    moveto(130, -80);
    showimage(button_return);
    wait(0.0001);
    while (interfacetype == I_rule2) {
        if (mousemsg()) {
            msmsg = getmouse();
            if (msmsg.flag == BUTTON_UP)
                if (msmsg.x >= 80 && msmsg.x <= 180 && msmsg.y >= -105 && msmsg.y <= -55)
                    interfacetype = I_welcome;
        }
    }
}

void Welcome::show_rank(InterfaceType pre) {
    moveto(0, 0);
    showimage(im_rank);
    moveto(125, -140);
    showimage(button_return);
    moveto(-110, 74);
    int k = 1;
    for (auto & i : scorerank_map) {
        if (k == 1)showimage(im_crown1);
        else if (k == 2)showimage(im_crown2);
        else if (k == 3)showimage(im_crown3);
        if ((k++) == 5)break;
        move(80, 0);
        showtext(i.second.c_str());
        move(150, 0);
        showtext(i.first);
        move(-230, -35);
    }
    wait(0.0001);
    while (interfacetype == I_rank) {
        msmsg = getmouse();
        if (msmsg.flag == BUTTON_UP)
            if (!(msmsg.x >= -210 && msmsg.x <= 210 && msmsg.y >= -190 && msmsg.y <= 190) ||
                (msmsg.x >= 75 && msmsg.x <= 175 && msmsg.y >= -165 && msmsg.y <= -115))
                interfacetype = pre;
    }
}

void Welcome::show_option(InterfaceType pre) {
    while (interfacetype == I_option) {
        moveto(0, 0);
        showimage(im_option);
        moveto(30, 110);
        if (bgm)showimage(button_on);
        else showimage(button_off);
        move(0, -50);
        if (sound_effect)showimage(button_on);
        else showimage(button_off);
        if (themetype == Classic)moveto(0, 5);
        else if (themetype == Jungle)moveto(85, 5);
        else if (themetype == Underwater)moveto(0, -50);
        else if (themetype == Winter)moveto(85, -50);
        showimage(im_circle);
        wait(0.0001);
        msmsg = getmouse();
        if (msmsg.flag == BUTTON_UP) {
            if (msmsg.x >= -20 && msmsg.x <= 80 && msmsg.y >= 85 && msmsg.y <= 135) {
                if (bgm)mciSendString("close BGM", nullptr, 0, nullptr);
                else scene.PlayBGM();
                bgm = !bgm;
            } else if (msmsg.x >= -20 && msmsg.x <= 80 && msmsg.y >= 35 && msmsg.y <= 85) {
                sound_effect = !sound_effect;
            } else if (msmsg.x >= -40 && msmsg.x <= 40 && msmsg.y >= -40 && msmsg.y <= 50) {
                themetype = Classic;
            } else if (msmsg.x >= 45 && msmsg.x <= 125 && msmsg.y >= -40 && msmsg.y <= 50) {
                themetype = Jungle;
            } else if (msmsg.x >= -40 && msmsg.x <= 40 && msmsg.y >= -90 && msmsg.y <= 0) {
                themetype = Underwater;
            } else if (msmsg.x >= 45 && msmsg.x <= 125 && msmsg.y >= -90 && msmsg.y <= 0) {
                themetype = Winter;
            } else if (msmsg.x >= -80 && msmsg.x <= 140 && msmsg.y >= -180 && msmsg.y <= -100) {
                if (pre != I_gamepause) {
                    interfacetype = I_change_name;
                    change_name();
                } else {
                    msgbox("´íÎóµÄ", "ÓÎÏ·ÖÐ²»ÄÜÐÞ¸ÄÃû×Ö\u8777", MB_OK | MB_ICONWARNING);
                }
            } else if (!(msmsg.x >= -160 && msmsg.x <= 160 && msmsg.y >= -210 && msmsg.y <= 210)) {
                interfacetype = pre;
            }
        }
    }
    change_theme();
}

void Welcome::show_pauselabel() {
    moveto(0, 0);
    showimage(im_pauselabel);
    moveto(0, -200);
    showimage(button_retry);
    moveto(120, -200);
    showimage(button_return);
    moveto(0, -140);
    showimage(button_option_rect);
    moveto(120, -140);
    showimage(button_menu);
    moveto(-75, 25);
    showimage(im_your_score);
    move(160, 0);
    showtext(land.score);
    moveto(-75, -25);
    showimage(im_highest_score);
    move(160, 0);
    showtext(scorerank_map.begin()->first);
    moveto(-75, -75);
    showimage(im_your_name);
    move(160, 0);
    showtext(name.c_str());
    wait(0.0001);
}

void Welcome::show_gameover() {
    moveto(0, 175);
    showimage(im_gameover);
    moveto(-100, 50);
    showimage(im_your_score);
    move(200, 0);
    showtext(land.score);
    moveto(-100, 0);
    showimage(im_highest_score);
    move(200, 0);
    showtext(scorerank_map.begin()->first);
    moveto(-100, -50);
    showimage(im_your_name);
    move(200, 0);
    showtext(name.c_str());
    moveto(30, -150);
    showimage(button_retry);
    moveto(-40, -225);
    showimage(button_rank_rect);
    moveto(100, -225);
    showimage(button_menu);
    wait(0.0001);
}

void Welcome::change_theme() {
    path = "image/Welcome/";
    if (themetype == Classic) {
        im_bk_bottombar = new IMAGE((path + "Classic.png").c_str());
        theme = "Classic";
    } else if (themetype == Jungle) {
        im_bk_bottombar = new IMAGE((path + "Jungle.png").c_str());
        theme = "Jungle";
    } else if (themetype == Underwater) {
        im_bk_bottombar = new IMAGE((path + "Underwater.png").c_str());
        theme = "Underwater";
    } else if (themetype == Winter) {
        im_bk_bottombar = new IMAGE((path + "Winter.png").c_str());
        theme = "Winter";
    }
    scene.change_theme(theme);
    player.change_theme(theme);
}

void Welcome::draw_jump() {
    static Land::LandState tmp_land = {-150, 625, 0, normal, land.im_normal};
    static double x_middle = -150, vy = 6, rebound_vy = 8, y_bottom = 0;
    if ((abs(y_bottom - tmp_land.top_y) <= vy) && (vy > 0))
        vy = -rebound_vy;
    vy += GRAVITY;
    y_bottom += vy;
    moveto(tmp_land.middle_x, -(tmp_land.top_y - DOODLE_JUMP_HEIGHT / 2));
    showimage(tmp_land.im_land);
    moveto(x_middle, -(y_bottom - player.height / 2 - DOODLE_JUMP_HEIGHT / 2));
    showimage(player.im_right);
}

void Welcome::pauseget_mouse() {
    while (interfacetype == I_gamerun) {
        if (mousemsg()) {
            msmsg = getmouse();
            if (msmsg.flag == BUTTON_UP) {
                if (msmsg.x >= 180 && msmsg.x <= 220 && msmsg.y >= 355 && msmsg.y <= 395) {
                    welcome.interfacetype = I_gamepause;
                }
            }
        }
    }
}

void Welcome::game_run() {
    p_mouseget = thread(&Welcome::pauseget_mouse, this);
    p_mouseget.detach();
    while (interfacetype != I_gameover && interfacetype != I_gameretry && interfacetype != I_welcome) {
        if (interfacetype == I_gamerun) {
            scene.show();
            scene.updateWithoutInput();
            scene.updateWithKeyInput();
            wait(0.008);
        } else if (interfacetype == I_gamepause) {
            show_pauselabel();
            while (interfacetype == I_gamepause) {
                if (mousemsg()) {
                    msmsg = getmouse();
                    if (msmsg.flag == BUTTON_UP) {
                        if (msmsg.x >= -50 && msmsg.x <= 50 && msmsg.y >= -225 && msmsg.y <= -175) {
                            welcome.interfacetype = I_gameretry;
                        } else if (msmsg.x >= 70 && msmsg.x <= 170 && msmsg.y >= -225 && msmsg.y <= -175) {
                            welcome.interfacetype = I_gamerun;
                        } else if (msmsg.x >= -50 && msmsg.x <= 50 && msmsg.y >= -165 && msmsg.y <= -115) {
                            welcome.interfacetype = I_option;
                            welcome.show_option(I_gamepause);
                            welcome.show_pauselabel();
                        } else if (msmsg.x >= 70 && msmsg.x <= 170 && msmsg.y >= -165 && msmsg.y <= -115) {
                            savedata.game_save();
                            welcome.interfacetype = I_welcome;
                        }
                    }
                }
            }
            if (interfacetype == I_gamerun) {
                p_mouseget = thread(&Welcome::pauseget_mouse, this);
                p_mouseget.detach();
            }
        }
    }
    if (interfacetype == I_gameover || interfacetype == I_gameretry) {
        ofstream savedata;
        savedata.open("savedata.dat", ios::out | ios::binary);
        savedata.close();
    }//Çå¿Õ´æµµ
    if (interfacetype == I_gameover) {
        show_gameover();
        if (land.score > 0) {
            scorerank_map.insert(make_pair(land.score, name));
            rank_save();
        }
        while (interfacetype == I_gameover) {
            if (mousemsg()) {
                msmsg = getmouse();
                if (msmsg.flag == BUTTON_UP) {
                    if (msmsg.x >= -20 && msmsg.x <= 80 && msmsg.y >= -175 && msmsg.y <= -125) {
                        interfacetype = I_gameretry;
                    } else if (msmsg.x >= -90 && msmsg.x <= 10 && msmsg.y >= -250 && msmsg.y <= -200) {
                        interfacetype = I_rank;
                        show_rank(I_gameover);
                        scene.draw();
                        land.show_topbar();
                        show_gameover();
                    } else if (msmsg.x >= 50 && msmsg.x <= 150 && msmsg.y >= -250 && msmsg.y <= -200) {
                        interfacetype = I_welcome;
                    }
                }
            }
        }
    }
    if (interfacetype == I_gameretry) {
        interfacetype = I_gamerun;
        land.retry_clean();
        player.retry_clean();
        game_run();
    }
    if (interfacetype == I_welcome) {
        land.retry_clean();
        player.retry_clean();
        main_interface();
    }
}
