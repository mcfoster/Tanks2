//
// Created by Chris Foster on 1/6/18.
//

#include "gameMessageBox.h"

gameMessageBox::gameMessageBox() {
    fColor = {0xFF, 0x7F, 0x7F};
    backColor = {0x00, 0x00, 0x1F};
}

gameMessageBox::~gameMessageBox() {

}

/****************************************************************************
 * Set the background color of the message box.
 * @param backColor
 ***************************************************************************/
void gameMessageBox::setBackColor(const SDL_Color &backColor) {
    gameMessageBox::backColor = backColor;
}

/****************************************************************************
 * Show message in a filled rectangle.
 * @param sImage buffer surfave where the message is to be painted
 * @param msg Array of message lines. The last line should be null.
 * @param x Box location
 * @param y
 * @param w Box size
 * @param h
 ***************************************************************************/
void gameMessageBox::ShowMessageBox(SDL_Surface *sImage, char *msg[], int x, int y, int w, int h){
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.h = h;
    rect.w = w;

    SDL_FillRect(sImage, &rect, SDL_MapRGB(sImage->format, backColor.r, backColor.g, backColor.b));

    int x1 = x+5;
    int y1 = y+5;
    for (int i = 0; msg[i] != nullptr; ++i) {
        printText(sImage, msg[i], x1, y1);
        y1 += fontRect.h + 2;
    }
}
