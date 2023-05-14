//
// Created by chris on 1/6/18.
//
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include "DrawText.h"

#ifndef TANKS2_GAMEMESSAGEBOX_H
#define TANKS2_GAMEMESSAGEBOX_H


class gameMessageBox : DrawText {
private:
    SDL_Color backColor = {0x0F, 0x0F, 0x0F};
public:
    void setBackColor(const SDL_Color &backColor);

public:
    gameMessageBox();
    virtual ~gameMessageBox();

    void ShowMessageBox(SDL_Surface *sImage, char *msg[], int x, int y, int w, int h);

};


#endif //TANKS2_GAMEMESSAGEBOX_H
