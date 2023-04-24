#include "Common.h"

void SaveData::game_save() {
    ofstream savedata;
    size_t StringLength;
    savedata.open("savedata.dat", ios::out | ios::binary);
    //Welcome部分
    savedata.write((char *) &welcome.themetype, sizeof(DJTheme));
    savedata.write((char *) &welcome.bgm, sizeof(bool));
    savedata.write((char *) &welcome.sound_effect, sizeof(bool));
    StringLength = welcome.name.size();
    savedata.write((char *) &StringLength, sizeof(size_t));
    savedata.write(welcome.name.c_str(), welcome.name.size());
    //Land部分
    savedata.write((char *) &land.land_width, sizeof(float));
    savedata.write((char *) &land.land_height, sizeof(float));
    savedata.write((char *) &land.land_vy, sizeof(float));
    savedata.write((char *) &land.score, sizeof(int));
    savedata.write((char *) &land.broken_y, sizeof(int));
    for (auto &i: land.land)
        savedata.write((char *) &i, sizeof(Land::LandState));
    //Player部分
    savedata.write((char *) &player.playerStatus, sizeof(PlayerStatus));
    savedata.write((char *) &player.x_middle, sizeof(float));
    savedata.write((char *) &player.y_bottom, sizeof(float));
    savedata.write((char *) &player.vx, sizeof(float));
    savedata.write((char *) &player.vy, sizeof(float));
    savedata.write((char *) &player.width, sizeof(float));
    savedata.write((char *) &player.height, sizeof(float));
    savedata.write((char *) &player.rebound_vy, sizeof(float));
    savedata.write((char *) &player.isPlayer_yMax, sizeof(bool));
    savedata.write((char *) &player.isPlayer_died, sizeof(bool));
    savedata.close();
}

bool SaveData::game_load() {
    ifstream savedata;
    size_t StringLength;
    savedata.open("savedata.dat", ios::in | ios::binary);
    if (savedata.peek() == EOF) {
        savedata.close();
        return false;
    }
    //Welcome部分
    savedata.read((char *) &welcome.themetype, sizeof(DJTheme));
    savedata.read((char *) &welcome.bgm, sizeof(bool));
    savedata.read((char *) &welcome.sound_effect, sizeof(bool));
    savedata.read((char *) &StringLength, sizeof(size_t));
    char *buffer = new char[StringLength + 1];
    savedata.read(buffer, StringLength);
    buffer[StringLength] = '\0';
    welcome.name = buffer;
    delete[]buffer;
    //Land部分
    savedata.read((char *) &land.land_width, sizeof(float));
    savedata.read((char *) &land.land_height, sizeof(float));
    savedata.read((char *) &land.land_vy, sizeof(float));
    savedata.read((char *) &land.score, sizeof(int));
    savedata.read((char *) &land.broken_y, sizeof(int));
    for (auto &i: land.land) {
        savedata.read((char *) &i, sizeof(Land::LandState));
        if (i.landType == landrocket) {
            i.im_land = land.im_landrocket;
        } else if (i.landType == landfly) {
            i.im_land = land.im_landfly;
        } else if (i.landType == mvspring1) {
            i.im_land = land.im_mvspring1;
        } else if (i.landType == mvland) {
            i.im_land = land.im_move;
        } else if (i.landType == fragile) {
            i.im_land = land.im_break;
        } else if (i.landType == landspring1) {
            i.im_land = land.im_landspring1;
        } else if (i.landType == normal) {
            i.im_land = land.im_normal;
        }
    }
    //Player部分
    savedata.read((char *) &player.playerStatus, sizeof(PlayerStatus));
    savedata.read((char *) &player.x_middle, sizeof(float));
    savedata.read((char *) &player.y_bottom, sizeof(float));
    savedata.read((char *) &player.vx, sizeof(float));
    savedata.read((char *) &player.vy, sizeof(float));
    savedata.read((char *) &player.width, sizeof(float));
    savedata.read((char *) &player.height, sizeof(float));
    savedata.read((char *) &player.rebound_vy, sizeof(float));
    savedata.read((char *) &player.isPlayer_yMax, sizeof(bool));
    savedata.read((char *) &player.isPlayer_died, sizeof(bool));
    savedata.close();
    welcome.change_theme();
    return true;
}
