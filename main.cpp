/******************************************************************************
 * Tank Battle, Main form
 * Author:  Martin Foster
 * Date: December 2017
 * Revision history:
 * Date      Author          Description
 * --------  --------------- -------------------------------------------------
 *
 * TODO: Game menu
 * TODO: Show Score
 * TODO: Top Ten list. Show standing. Highlight current score.
 * TODO: Game help
 * TODO: Game controller.
******************************************************************************/
#ifdef __cplusplus
    #include <cstdlib>
    #include <ctime>
#else
    #include <stdlib.h>
    #include <time.h>
#endif

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include "DrawText.h"
#include "gameMessageBox.h"
#include "ItemList.h"

const int BlueTank = 1;
const int RedTank = 2;
const int DeadTank = 3;
const int DIR_COUNT = 8;
const int EXP_COUNT = 3;
const int DEAD_COUNT = 2;
const int GoodGuyIdx = 0;
const int FPS = 15;
const int FRAME_DELAY = 1000/FPS;

int height = 600;
int width = 800;

SDL_Surface *blueTanks;
SDL_Surface *redTanks;
SDL_Surface *deadTanks;
SDL_Surface *explosions;
int ShotStartX[DIR_COUNT];
int ShotStartY[DIR_COUNT];

const int tankWidth = 32;
const int tankHeight = 32;
const int explosionWidth = 24;
const int explosionHeight = 24;

int  MaxX, MaxY;
SDL_Surface *BlockImage;
SDL_Surface *TreeImage;
int blueCount, redCount;
int leftCnt=0;
int rightCnt=0;
ItemList *bulletList;
ItemList *tanksList;
ItemList *explosionList;
ItemList *blocksList;
ItemList *treeList;
int curScrn;
char sUserName[40]; // Plenty for user
SDL_Surface* Background = NULL;
SDL_Surface* SpriteImage = NULL;
SDL_Surface* Backbuffer = NULL;

enum GameStates {ePlaying, eDrawMenu, eDoMenu,  eQuit} gameState;

DrawText *drawText;
double score = 0;
bool done = false;

SDL_Surface* LoadImage(char* fileName);
void InitGame();
void ClearScreen();
void FreeResources();
bool ProgramIsRunning();
void InitImages();
void InitLists();
void InitShotOffset();
void InitScrn1();
void InitScrn2();
void InitScrn3();
void InitScrn4();
void CheckKeyPress();
char GetKeyboardChar();
void UpdateGame();
void MoveTank(int tankIdx, int cnt);
int MoveTopLeft(int pos, int cnt);
int MoveBtmRight(int pos, int cnt, int max_val);
void NewScreenCheck(int tankIdx);
int CheckGameOver();
void ChkCollisions();
bool chkBump(int x, int y);
bool TankCollision(int x,int y, int *idx);
bool WallCollision(int x, int y, int *idx);
bool Collision(int x, int y, int x1, int y1, int x2, int y2);
bool AimingAtTarget(int idx);
void badGuyRoutine(int tankIdx);
void DrawImage(SDL_Surface* image, SDL_Surface* destSurface, int x, int y);
void DrawImageFrame(SDL_Surface* image, SDL_Surface* destSurface, int x, int y, int width, int height, int frame);
void PaintGame();
void ShowScore();

int main(int argc, char* args[])
{
    static int result=0;
    char *sWin[] = {(char *)"You have defeated the invaders!",
                    nullptr, (char *)"Play again? (Y/N)", nullptr};
    char *sLost[] = {(char *)"Your tank has been destroyed.",
                     (char *)"Play again? (Y/N)", nullptr};
    char msg[40];
    gameMessageBox *msgBox = new gameMessageBox();

    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("Failed to initialize SDL!\n");
        return 0;
    }

    Backbuffer = SDL_SetVideoMode(800, 600, 32, SDL_SWSURFACE);

    SDL_WM_SetCaption("Tank Commander", NULL);

    drawText = new DrawText();
    InitGame();

    gameState = ePlaying;
    while(ProgramIsRunning() && !done)
    {
        long int oldTime = SDL_GetTicks();  //We will use this later to see how long it took to update the frame
        switch (gameState) {
            case ePlaying: {
                UpdateGame();
                CheckKeyPress();
                PaintGame();
                ShowScore();
                result = CheckGameOver();
                if(result > 0)
                    gameState = eDrawMenu;
                break;
            }
            case eDrawMenu:{
                if(result == 1) {
                    sprintf(msg,"Your score: %4.1f", score);
                    sWin[1] = msg;
                    msgBox->ShowMessageBox(Backbuffer, sWin, width / 4, height / 4, width / 2, height / 3);
                }
                else
                    msgBox->ShowMessageBox(Backbuffer,sLost ,width/4, height/4, width/2, height/3);
                gameState = eDoMenu;
                break;
            }
            case eDoMenu: {
                char c = GetKeyboardChar();
                if (c == 'Y') {
                    InitLists();
                    curScrn = 0;
                    gameState = ePlaying;
                } else if (c == 'N') {
                    gameState = eQuit;
                }

                   break;
            }
            case eQuit:
                done = true;
                break;
        }
        int frameTime = SDL_GetTicks() - oldTime;

        if (frameTime < FRAME_DELAY)            //Dont delay if we dont need to
            SDL_Delay(FRAME_DELAY - frameTime);     //Delay
        SDL_Flip(Backbuffer);
    }

    FreeResources();
    delete drawText;
    delete msgBox;
    SDL_Quit();

    return 0;
}

/****************************************************************************
*
****************************************************************************/
bool ProgramIsRunning()
{
    bool running = true;
    SDL_Event event;


    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT)
            running = false;
    }

    return running;
}

/*******************************************************************************
*
*******************************************************************************/
void ClearScreen()
{
    SDL_FillRect(Backbuffer, NULL, SDL_MapRGB(Backbuffer->format, 220, 250, 200));
}

/*******************************************************************************
*
*******************************************************************************/
SDL_Surface* LoadImage(char* fileName)
{
    SDL_Surface* imageLoaded = NULL;
    SDL_Surface* processedImage = NULL;

    imageLoaded = SDL_LoadBMP(fileName);

    if(imageLoaded != NULL)
    {
        processedImage = SDL_DisplayFormat(imageLoaded);
        SDL_FreeSurface(imageLoaded);

        if(processedImage != NULL)
        {
            Uint32 colorKey = SDL_MapRGB(processedImage->format, 0xFF, 0xFF, 0xFF);
            SDL_SetColorKey(processedImage, SDL_SRCCOLORKEY, colorKey);
        }

    }

    return processedImage;
}

/*******************************************************************************
*
*******************************************************************************/
void FreeResources()
{
    SDL_FreeSurface(Background);
    SDL_FreeSurface(SpriteImage);
    explosionList->deleteList();
    delete explosionList;
    bulletList->deleteList();
    delete bulletList;
    tanksList->deleteList();
    delete tanksList;
    blocksList->deleteList();
    delete blocksList;
    treeList->deleteList();
    delete treeList;
} // FreeResources

/****************************************************************************
*
****************************************************************************/
void InitGame()
{
    bulletList = new ItemList();
    tanksList  = new ItemList();
    explosionList  = new ItemList();
    blocksList   = new ItemList();
    treeList  = new ItemList();
    InitImages();
    InitLists();
    InitShotOffset();
    MaxX = width - tankWidth;
    MaxY = height - tankHeight;
    /* initialize random seed: */
    srand (time(NULL));

    curScrn = 0;
    sUserName[0] = 0; // clear the name
} // InitGame

/*****************************************************************************
* Shot should start after canon barrel. This routin sets the offset relative
* to the tank image based on the direction.
*****************************************************************************/
void InitShotOffset()
{
    // Up
    ShotStartX[0] = tankWidth / 2;
    ShotStartY[0] = 0;
    // Up/Right
    ShotStartX[1] = tankWidth;
    ShotStartY[1] = 0;
    // Right
    ShotStartX[2] = tankWidth;
    ShotStartY[2] = tankHeight / 2;
    // Rignt/Down
    ShotStartX[3] = tankWidth;
    ShotStartY[3] = tankHeight;
    // Down
    ShotStartX[4] = tankWidth / 2;
    ShotStartY[4] = tankHeight;
    // Down/Left
    ShotStartX[5] = 0;
    ShotStartY[5] = tankHeight;
    // Left
    ShotStartX[6] = 0;
    ShotStartY[6] = tankHeight / 2;
    // Left/Up
    ShotStartX[7] = 0;
    ShotStartY[7] = 0;
}

/******************************************************************************
* Read the image files.
******************************************************************************/
void InitImages()
{
    // Init Tank Images
    blueTanks = LoadImage((char *)"images/TankSpriteBlue.bmp");
    redTanks = LoadImage((char *)"images/TankSpriteRed.bmp");
    deadTanks = LoadImage((char *)"images/deadTankSprite.bmp");
    explosions = LoadImage((char *)"images/ExplosionSprite.bmp");

    BlockImage = LoadImage((char *)"images/Bricks.bmp");
    TreeImage = LoadImage((char *)"images/Tree1.bmp");
}

/******************************************************************************
* Clear all lists and counters. Reset the screens
******************************************************************************/
void InitLists()
{
    explosionList->deleteList();
    bulletList->deleteList();
    tanksList->deleteList();
    blocksList->deleteList();
    treeList->deleteList();
    blueCount = 1;
    redCount  = 8;
    score = 1000.0;
    InitScrn1();
    InitScrn2();
    InitScrn3();
    InitScrn4();
}

/******************************************************************************
* Procedure: InitScrn1;
* Parameters: None
* This function initializes screen #1, the top left game screen.
*     ||
* [0] ||  1
* ==========
*  2  ||  3
*     ||
******************************************************************************/
void InitScrn1()
{
    const int boxTop = 110;
    const int boxLeft = 90;
    const int boxH = 20; // blocks high
    const int boxW = 20; // blocks wide
    const int scrNo = 0;

    ItemList::TItemRec *TnkPtr;
    ItemList::TItemRec *ptRec;

    int w = BlockImage->w;
    int h = BlockImage->h;
    int gate_y1 = (height / 4) + (BlockImage->h / 2);
    int gate_y2 = gate_y1 * 2;
    int gate_x1 = (width / 4) + (BlockImage->w / 2);
    int gate_x2 = gate_x1 * 2;

    int i = w;

    if(blocksList == NULL)
        blocksList = new ItemList;
    else
        blocksList->deleteList();

    do{
        ptRec = new ItemList::TItemRec;
        ptRec->x = i;
        ptRec->y = 0;
        ptRec->scrn = scrNo;
        blocksList->addItem(ptRec);
        if((i < gate_x1) ||(i >= gate_x2))
        {
            ptRec = new ItemList::TItemRec;
            ptRec->x = i;
            ptRec->y = height - h;
            ptRec->scrn = scrNo;
            blocksList->addItem(ptRec);
        }
        i = i + w;
    } while( i < (width - w));

    i = 0;
    do{
        ptRec = new ItemList::TItemRec;
        ptRec->x = 0;
        ptRec->y = i;
        ptRec->scrn = scrNo;
        blocksList->addItem(ptRec);
        if((i < gate_y1) || (i >= gate_y2))
        {
            ptRec = new ItemList::TItemRec;
            ptRec->x = width - w;
            ptRec->y = i;
            ptRec->scrn = scrNo;
            blocksList->addItem(ptRec);
        }
        i = i + h;
    } while( i < (height - h));

    int x = boxLeft;
    int y = boxTop;
    for(i = 0; i < boxW; i++)
    {
        //top
        ptRec = new ItemList::TItemRec;
        ptRec->x = x;
        ptRec->y = y;
        ptRec->scrn = scrNo;
        blocksList->addItem(ptRec);
        // bottom
        ptRec = new ItemList::TItemRec;
        ptRec->x = x;
        ptRec->y = boxTop + (boxH * h);
        ptRec->scrn = scrNo;
        blocksList->addItem(ptRec);
        x = x + w;
    } // next i

    TnkPtr = new ItemList::TItemRec;
    TnkPtr->directionIdx = 4;
    TnkPtr->color = BlueTank; // Good guy
    TnkPtr->x = 20;
    TnkPtr->y = 20;
    TnkPtr->scrn = scrNo;
    tanksList->addItem(TnkPtr);

    TnkPtr = new ItemList::TItemRec;
    TnkPtr->directionIdx = 4;
    TnkPtr->color = RedTank; // Bad guy
    TnkPtr->x = 150;
    TnkPtr->y = 30;
    TnkPtr->scrn = scrNo;
    tanksList->addItem(TnkPtr);

    TnkPtr = new ItemList::TItemRec;
    TnkPtr->directionIdx = 2;
    TnkPtr->color = RedTank; // Bad guy
    TnkPtr->x = 160;
    TnkPtr->y = 200;
    TnkPtr->scrn = scrNo;
    tanksList->addItem(TnkPtr);

    ptRec = new ItemList::TItemRec;
    ptRec->x = (width / 2) - 40;
    ptRec->y = height / 2;
    ptRec->scrn = scrNo;
    treeList->addItem(ptRec);

    ptRec = new ItemList::TItemRec;
    ptRec->x = (width / 2) - 60;
    ptRec->y = (height / 2) + 10;
    ptRec->scrn = scrNo;
    treeList->addItem(ptRec);

    ptRec = new ItemList::TItemRec;
    ptRec->x = (width / 2) - 50;
    ptRec->y = (height / 2) + 15;
    ptRec->scrn = scrNo;
    treeList->addItem(ptRec);
    //*********************************************
} // InitScrn1

/******************************************************************************
* Procedure: InitScrn2;
* Parameters: None
* This function initializes screen #2, the top right game screen.
*     ||
*  0  || [1]
* ==========
*  2  ||  3
*     ||
******************************************************************************/
void InitScrn2()
{
    const int boxTop = 110;
    const int boxLeft = 90;
    const int boxH = 20; // blocks high
    const int boxW = 20; // blocks wide
    const int scrNo = 1;

    ItemList::TItemRec *TnkPtr;
    ItemList::TItemRec *ptRec;

    int w = BlockImage->w;
    int h = BlockImage->h;
    int gate_y1 = (height / 4) + (BlockImage->h / 2);
    int gate_y2 = gate_y1 * 2;
    int gate_x1 = (width / 4) + (BlockImage->w / 2);
    int gate_x2 = gate_x1 * 2;

    int i = w;
    do{
        ptRec = new ItemList::TItemRec;
        ptRec->x = i;
        ptRec->y = 0;
        ptRec->scrn = scrNo;
        blocksList->addItem(ptRec);
        if((i < gate_x1) ||(i >= gate_x2))
        {
            ptRec = new ItemList::TItemRec;
            ptRec->x = i;
            ptRec->y = height - h;
            ptRec->scrn = scrNo;
            blocksList->addItem(ptRec);
        }
        i = i + w;
    } while( i < (width - w));

    i = 0;
    do{
        ptRec = new ItemList::TItemRec;
        ptRec->x = width - w;
        ptRec->y = i;
        ptRec->scrn = scrNo;
        blocksList->addItem(ptRec);
        if((i < gate_y1) || (i >= gate_y2))
        {
            ptRec = new ItemList::TItemRec;
            ptRec->x = 0;
            ptRec->y = i;
            ptRec->scrn = scrNo;
            blocksList->addItem(ptRec);
        }
        i = i + h;
    } while( i < (height - h));

    int x = boxLeft;
    int y = boxTop;
    for(i = 0; i < boxW; i++)
    {
        //top
        ptRec = new ItemList::TItemRec;
        ptRec->x = x;
        ptRec->y = y;
        ptRec->scrn = scrNo;
        blocksList->addItem(ptRec);
        // bottom
        ptRec = new ItemList::TItemRec;
        ptRec->x = x + (boxW * w);
        ptRec->y = y;
        ptRec->scrn = scrNo;
        blocksList->addItem(ptRec);
        y = y + h;
    } // next i

    TnkPtr = new ItemList::TItemRec;
    TnkPtr->directionIdx = 4;
    TnkPtr->color = RedTank; // Bad guy
    TnkPtr->x = width - 40;
    TnkPtr->y = 30;
    TnkPtr->scrn = scrNo;
    tanksList->addItem(TnkPtr);

    TnkPtr = new ItemList::TItemRec;
    TnkPtr->directionIdx = 2;
    TnkPtr->color = RedTank; // Bad guy
    TnkPtr->x = 160;
    TnkPtr->y = 200;
    TnkPtr->scrn = scrNo;
    tanksList->addItem(TnkPtr);

    ptRec = new ItemList::TItemRec;
    ptRec->x = (width / 2) - 40;
    ptRec->y = height / 2;
    ptRec->scrn = scrNo;
    treeList->addItem(ptRec);

    ptRec = new ItemList::TItemRec;
    ptRec->x = (width / 2) - 60;
    ptRec->y = (height / 2) + 10;
    ptRec->scrn = scrNo;
    treeList->addItem(ptRec);

    ptRec = new ItemList::TItemRec;
    ptRec->x = (width / 2) - 50;
    ptRec->y = (height / 2) + 15;
    ptRec->scrn = scrNo;
    treeList->addItem(ptRec);

    ptRec = new ItemList::TItemRec;
    ptRec->x = (width / 4) - 20;
    ptRec->y = (height / 4) + 20;
    ptRec->scrn = scrNo;
    treeList->addItem(ptRec);

    ptRec = new ItemList::TItemRec;
    ptRec->x = (width / 3) - 60;
    ptRec->y = (height / 4) + 10;
    ptRec->scrn = scrNo;
    treeList->addItem(ptRec);


    ptRec = new ItemList::TItemRec;
    ptRec->x = (width / 3) - 50;
    ptRec->y = (height / 4) + 15;
    ptRec->scrn = scrNo;
    treeList->addItem(ptRec);
    //***********************************************}
} // void InitScrn2()

/******************************************************************************
* Procedure: InitScrn3;
* Parameters: None
* This function initializes screen #2, the top right game screen.
*
*     ||
*  0  ||  1
* ==========
* [2] ||  3
*     ||
******************************************************************************/
void InitScrn3()
{
    const int boxTop = 115;
    const int boxLeft = 90;
    const int boxH = 18; // blocks high
    const int boxW = 22; // blocks wide
    const int scrNo = 2;

    ItemList::TItemRec *TnkPtr;
    ItemList::TItemRec *ptRec;

    int w = BlockImage->w;
    int h = BlockImage->h;
    int gate_y1 = (height / 4) + (BlockImage->h / 2);
    int gate_y2 = gate_y1 * 2;
    int gate_x1 = (width / 4) + (BlockImage->w / 2);
    int gate_x2 = gate_x1 * 2;

    int i = w;
    do{
        ptRec = new ItemList::TItemRec;
        ptRec->x = i;
        ptRec->y = height - h;
        ptRec->scrn = scrNo;
        blocksList->addItem(ptRec);
        if((i < gate_x1) ||(i >= gate_x2))
        {
            ptRec = new ItemList::TItemRec;
            ptRec->x = i;
            ptRec->y = 0;
            ptRec->scrn = scrNo;
            blocksList->addItem(ptRec);
        }
        i = i + w;
    } while( i < (width - w));

    i = 0;
    do{
        ptRec = new ItemList::TItemRec;
        ptRec->x = 0;
        ptRec->y = i;
        ptRec->scrn = scrNo;
        blocksList->addItem(ptRec);
        if((i < gate_y1) || (i >= gate_y2))
        {
            ptRec = new ItemList::TItemRec;
            ptRec->x = width - w;
            ptRec->y = i;
            ptRec->scrn = scrNo;
            blocksList->addItem(ptRec);
        }
        i = i + h;
    } while( i < (height - h));

    int x = boxLeft;
    int y = boxTop;
    for(i = 0; i < boxW; i++)
    {
        //top
        ptRec = new ItemList::TItemRec;
        ptRec->x = x;
        ptRec->y = y;
        ptRec->scrn = scrNo;
        blocksList->addItem(ptRec);
        // bottom
        ptRec = new ItemList::TItemRec;
        ptRec->x = x + (boxW * w);
        ptRec->y = y;
        ptRec->scrn = scrNo;
        blocksList->addItem(ptRec);
        y = y + h;
    } // next i

    TnkPtr = new ItemList::TItemRec;
    TnkPtr->directionIdx = 4;
    TnkPtr->color = RedTank; // Bad guy
    TnkPtr->x = width - 40;
    TnkPtr->y = 30;
    TnkPtr->scrn = scrNo;
    tanksList->addItem(TnkPtr);

    TnkPtr = new ItemList::TItemRec;
    TnkPtr->directionIdx = 2;
    TnkPtr->color = RedTank; // Bad guy
    TnkPtr->x = 160;
    TnkPtr->y = height - 60;
    TnkPtr->scrn = scrNo;
    tanksList->addItem(TnkPtr);

    ptRec = new ItemList::TItemRec;
    ptRec->x = (width / 2) - 45;
    ptRec->y = (height / 2) - 10;
    ptRec->scrn = scrNo;
    treeList->addItem(ptRec);

    ptRec = new ItemList::TItemRec;
    ptRec->x = (width / 2) - 60;
    ptRec->y = (height / 2) + 10;
    ptRec->scrn = scrNo;
    treeList->addItem(ptRec);

    ptRec = new ItemList::TItemRec;
    ptRec->x = (width / 2) - 50;
    ptRec->y = (height / 2) + 15;
    ptRec->scrn = scrNo;
    treeList->addItem(ptRec);

    ptRec = new ItemList::TItemRec;
    ptRec->x = (width / 4) - 20;
    ptRec->y = (height / 4) + 20;
    ptRec->scrn = scrNo;
    treeList->addItem(ptRec);

    ptRec = new ItemList::TItemRec;
    ptRec->x = (width / 3) - 60;
    ptRec->y = (height / 4) + 10;
    ptRec->scrn = scrNo;
    treeList->addItem(ptRec);

    ptRec = new ItemList::TItemRec;
    ptRec->x = (width / 3) - 50;
    ptRec->y = (height / 4) + 15;
    ptRec->scrn = scrNo;
    treeList->addItem(ptRec);
    //***********************************************}
} // InitScrn3()

/******************************************************************************
* Procedure: InitScrn3;
* Parameters: None
* This function initializes screen #2, the top right game screen.
*
*     ||
*  0  ||  1
* ==========
*  2  || [3]
*     ||
******************************************************************************/
void InitScrn4()
{
    const int boxTop = 110;
    const int boxLeft = 110;
    const int boxH = 18; // blocks high
    const int boxW = 18; // blocks wide
    const int scrNo = 3;

    ItemList::TItemRec *TnkPtr;
    ItemList::TItemRec *ptRec;

    int w = BlockImage->w;
    int h = BlockImage->h;
    int gate_y1 = (height / 4) + (BlockImage->h / 2);
    int gate_y2 = gate_y1 * 2;
    int gate_x1 = (width / 4) + (BlockImage->w / 2);
    int gate_x2 = gate_x1 * 2;

    int i = w;
    do{
        ptRec = new ItemList::TItemRec;
        ptRec->x = i;
        ptRec->y = height - h;
        ptRec->scrn = scrNo;
        blocksList->addItem(ptRec);
        if((i < gate_x1) ||(i >= gate_x2))
        {
            ptRec = new ItemList::TItemRec;
            ptRec->x = i;
            ptRec->y = 0;
            ptRec->scrn = scrNo;
            blocksList->addItem(ptRec);
        }
        i = i + w;
    } while( i < (width - w));

    i = 0;
    do{
        ptRec = new ItemList::TItemRec;
        ptRec->x = width - w;
        ptRec->y = i;
        ptRec->scrn = scrNo;
        blocksList->addItem(ptRec);
        if((i < gate_y1) || (i >= gate_y2))
        {
            ptRec = new ItemList::TItemRec;
            ptRec->x = 0;
            ptRec->y = i;
            ptRec->scrn = scrNo;
            blocksList->addItem(ptRec);
        }
        i = i + h;
    } while( i < (height - h));

    int x = boxLeft;
    int y = boxTop;
    for(i = 0; i < boxW; i++)
    {
        //top
        ptRec = new ItemList::TItemRec;
        ptRec->x = x;
        ptRec->y = y;
        ptRec->scrn = scrNo;
        blocksList->addItem(ptRec);
        // bottom
        ptRec = new ItemList::TItemRec;
        ptRec->x = x;
        ptRec->y = boxTop + (boxH * h);;
        ptRec->scrn = scrNo;
        blocksList->addItem(ptRec);
        x = x + w;
    } // next i

    x = boxLeft;
    for(i = 0; i < boxH; i++)
    {
        //top
        ptRec = new ItemList::TItemRec;
        ptRec->x = x;
        ptRec->y = y;
        ptRec->scrn = scrNo;
        blocksList->addItem(ptRec);
        y = y + h;
    } // next i

    TnkPtr = new ItemList::TItemRec;
    TnkPtr->directionIdx = 4;
    TnkPtr->color = RedTank; // Bad guy
    TnkPtr->x = width - 40;
    TnkPtr->y = 30;
    TnkPtr->scrn = scrNo;
    tanksList->addItem(TnkPtr);

    TnkPtr = new ItemList::TItemRec;
    TnkPtr->directionIdx = 2;
    TnkPtr->color = RedTank; // Bad guy
    TnkPtr->x = 160;
    TnkPtr->y = height - 60;
    TnkPtr->scrn = scrNo;
    tanksList->addItem(TnkPtr);

    ptRec = new ItemList::TItemRec;
    ptRec->x = (width / 2) - 45;
    ptRec->y = (height / 2) - 10;
    ptRec->scrn = scrNo;
    treeList->addItem(ptRec);

    ptRec = new ItemList::TItemRec;
    ptRec->x = (width / 2) - 60;
    ptRec->y = (height / 2) + 10;
    ptRec->scrn = scrNo;
    treeList->addItem(ptRec);

    ptRec = new ItemList::TItemRec;
    ptRec->x = (width / 2) - 50;
    ptRec->y = (height / 2) + 15;
    ptRec->scrn = scrNo;
    treeList->addItem(ptRec);

    ptRec = new ItemList::TItemRec;
    ptRec->x = (width / 4) - 20;
    ptRec->y = (height / 4) + 20;
    ptRec->scrn = scrNo;
    treeList->addItem(ptRec);

    ptRec = new ItemList::TItemRec;
    ptRec->x = (width / 3) - 60;
    ptRec->y = (height / 4) + 10;
    ptRec->scrn = scrNo;
    treeList->addItem(ptRec);

    ptRec = new ItemList::TItemRec;
    ptRec->x = (width / 3) - 50;
    ptRec->y = (height / 4) + 15;
    ptRec->scrn = scrNo;
    treeList->addItem(ptRec);
    //***********************************************}
} // InitScrn4()

/******************************************************************************
* Key down event handler.
******************************************************************************/
void CheckKeyPress() {
    ItemList::TItemRec *TnkPtr = tanksList->getItem(GoodGuyIdx);
    Uint8 *key = SDL_GetKeyState(NULL); // SDL_GetKeyboardState(NULL);
    bool pnt = false;

    if (key[SDLK_RIGHT]) {
        if (rightCnt++ > 1) {
            TnkPtr->directionIdx = TnkPtr->directionIdx + 1;
            if (TnkPtr->directionIdx >= DIR_COUNT)
                TnkPtr->directionIdx = 0;
            pnt = true;
        }
    } else if (key[SDLK_LEFT]) {
        if (leftCnt++ > 1) {
            TnkPtr->directionIdx = TnkPtr->directionIdx - 1;
            if (TnkPtr->directionIdx < 0)
                TnkPtr->directionIdx = DIR_COUNT - 1;
            pnt = true;
        }
    } else if (key[SDLK_UP]) {
        MoveTank(GoodGuyIdx, 3);
        pnt = true;
    } else if (key[SDLK_SPACE]) {
        ItemList::TItemRec *brPtr = new ItemList::TItemRec;
        brPtr->directionIdx = TnkPtr->directionIdx;
        brPtr->dist = 0;
        brPtr->x = TnkPtr->x + ShotStartX[TnkPtr->directionIdx];
        brPtr->y = TnkPtr->y + ShotStartY[TnkPtr->directionIdx];
        brPtr->scrn = curScrn;
        bulletList->addItem(brPtr);
        pnt = true;
    } else if (key[SDLK_ESCAPE]) {
        // Close the application
        rightCnt = 0;
        leftCnt = 0;
        pnt = true;
    }
    if (pnt) {
        rightCnt = 0;
        leftCnt = 0;
    }
} // CheckKeyPress()

char GetKeyboardChar()
{
    char c = ' ';
    Uint8 *key = SDL_GetKeyState(NULL); // SDL_GetKeyboardState(NULL);
    if(key[SDLK_y])
        c = 'Y';
    else if(key[SDLK_n])
        c= 'N';
    return c;
} // GetKeyboardChar

/****************************************************************************
* Summary: Update all game objects.
****************************************************************************/
void UpdateGame()
{
    const int moveCnt = 6;
    char s[20];
    //bool done;

    // Move bullets
    for(int i = bulletList->Count()-1; i>=0 ; i--)
    {
        ItemList::TItemRec *brPtr = bulletList->getItem(i);
        // track the distance the bullet moved
        brPtr->dist = brPtr->dist + moveCnt;
        bool bullet_done =false;
        switch(brPtr->directionIdx)
        {
            case 0:   // Up
                if((brPtr->y - moveCnt) > 1)
                    brPtr->y = brPtr->y - moveCnt;
                else
                    bullet_done = true;
                break;
            case 1:  // Up / right
                if((brPtr->y - moveCnt) > 1)
                    brPtr->y = brPtr->y - moveCnt;
                else
                    bullet_done = true;
                if((brPtr->x + moveCnt) < width)
                    brPtr->x = brPtr->x + moveCnt;
                else
                    bullet_done = true;
                break;
            case 2:  // right
                if((brPtr->x + moveCnt) < width)
                    brPtr->x = brPtr->x + moveCnt;
                else
                    bullet_done = true;
                break;
            case 3:  // down / right
                if((brPtr->y + moveCnt) < height)
                    brPtr->y = brPtr->y + moveCnt;
                else
                    bullet_done = true;
                if((brPtr->x + moveCnt) < width)
                    brPtr->x = brPtr->x + moveCnt;
                else
                    bullet_done = true;
                break;
            case 4:  // Down
                if((brPtr->y + moveCnt) < height)
                    brPtr->y = brPtr->y + moveCnt;
                else
                    bullet_done = true;
                break;
            case 5:  // Down / left
                if((brPtr->y + moveCnt) < height)
                    brPtr->y = brPtr->y + moveCnt;
                else
                    bullet_done = true;
                if((brPtr->x - moveCnt) > 1)
                    brPtr->x = brPtr->x - moveCnt;
                else
                    bullet_done = true;
                break;
            case 6:  // Left
                if((brPtr->x - moveCnt) > 1)
                    brPtr->x = brPtr->x - moveCnt;
                else
                    bullet_done = true;
                break;
            case 7:  // Up / Left
                if((brPtr->y - moveCnt) > 1)
                    brPtr->y = brPtr->y - moveCnt;
                else
                    bullet_done = true;
                if((brPtr->x - moveCnt) > 1)
                    brPtr->x = brPtr->x - moveCnt;
                else
                    bullet_done = true;
                break;
        } // end switch
        if(bullet_done)
        {
             bulletList->deleteItem(i);
        }
    } // next i
    // Animate explosions
    for(int i = explosionList->Count()-1; i>=0; i--)
    {
        ItemList::TItemRec *ExpRec = explosionList->getItem(i);
        ExpRec->directionIdx++;
        if(ExpRec->directionIdx >= EXP_COUNT)
        {
            explosionList->deleteItem(i);
        }
    }

    blueCount = 0;
    redCount = 0;
    for(int i = tanksList->Count()-1; i>=0; i--)
    {
        ItemList::TItemRec *TnkPtr = tanksList->getItem(i);

        if (TnkPtr->color == DeadTank)
        {
             TnkPtr->directionIdx++;
             if(TnkPtr->directionIdx >= DEAD_COUNT)
                TnkPtr->directionIdx = 0;
        }
        else if(TnkPtr->color == BlueTank)
            blueCount++;
        else if(TnkPtr->color == RedTank)
        {
            redCount++;
            badGuyRoutine(i);
        }
    } // next i
    if(score > 0)
        score -= 0.1;

     ChkCollisions();
    //Image1.Repaint;
    CheckGameOver();
}

/*****************************************************************************
* Summary:
* Parameters:
*   tankIdx, Tank to be moved.
*   cnt, Number of pixelsto move
*****************************************************************************/
void MoveTank(int tankIdx, int cnt)
{
    ItemList::TItemRec *TnkPtr  = tanksList->getItem(tankIdx);
    int x = TnkPtr->x;
    int y = TnkPtr->y;
    switch(TnkPtr->directionIdx)
    {
        case 0:
            y = MoveTopLeft(TnkPtr->y, cnt);
            break;
        case 1:
            y = MoveTopLeft(TnkPtr->y, cnt);
            x = MoveBtmRight(TnkPtr->x, cnt, MaxX );
            break;
        case 2:
            x = MoveBtmRight(TnkPtr->x, cnt, MaxX );
            break;
        case 3:
            x = MoveBtmRight(TnkPtr->x, cnt, MaxX );
            y = MoveBtmRight(TnkPtr->y, cnt, MaxY);
            break;
        case 4:
            y = MoveBtmRight(TnkPtr->y, cnt, MaxY);
            break;
        case 5:
            y = MoveBtmRight(TnkPtr->y, cnt, MaxY);
            x = MoveTopLeft(TnkPtr->x, cnt);
            break;
        case 6:
            x = MoveTopLeft(TnkPtr->x, cnt);
            break;
        case 7:
            y = MoveTopLeft(TnkPtr->y, cnt);
            x = MoveTopLeft(TnkPtr->x, cnt);
            break;
    } // end switch
    int xt = TnkPtr->x + ShotStartX[TnkPtr->directionIdx];
    int yt = TnkPtr->y + ShotStartY[TnkPtr->directionIdx];

    if(!chkBump( xt,yt))
    {
        TnkPtr->x = x;
        TnkPtr->y = y;
    }
    NewScreenCheck(tankIdx);
} // MoveTank

/*****************************************************************************
* Summary:
* Parameters:
*****************************************************************************/
int MoveTopLeft(int pos, int cnt)
{
    pos = pos - cnt;
    if(pos < 1)
        pos = 0;
    return pos;
}

/*****************************************************************************
* Summary:
* Parameters:
*****************************************************************************/
int MoveBtmRight(int pos, int cnt, int max_val)
{
    pos = pos + cnt;
    if(pos >= max_val)
        pos = max_val - 1;
    return pos;
}

/*****************************************************************************
* Summary: When moving thru a door, mov tank to the new screen.
*     ||
*  0  ||  1
* ==========
*  2  ||  3
*     ||
* Parameters:
*****************************************************************************/
void NewScreenCheck(int tankIdx)
{
    ItemList::TItemRec *TnkPtr  = tanksList->getItem(tankIdx);
    bool thruDoor = false;
    if(TnkPtr->scrn == 0)
    {
        if(TnkPtr->x >= (MaxX-4))
        {
            TnkPtr->scrn = 1;
            TnkPtr->x = 5;
            thruDoor = true;
        }
        else if(TnkPtr->y >= (MaxY-4))
        {
            // Screen 3?
            TnkPtr->scrn = 2;
            TnkPtr->y = 5;
            thruDoor = true;
        }
    }
    else if(TnkPtr->scrn == 1)
    {
        if(TnkPtr->x < 4)
        {
            TnkPtr->scrn = 0;
            TnkPtr->x = MaxX - (tankWidth + 1);
            thruDoor = true;
        }
        else if(TnkPtr->y >= (MaxY-4))
        {
            TnkPtr->scrn = 3;
            TnkPtr->y = 5;
            thruDoor = true;
        }
    }
    else if(TnkPtr->scrn == 2)
    {
        if(TnkPtr->y < 4)
        {
            TnkPtr->scrn = 0;
            TnkPtr->y = MaxY - (tankHeight + 1);
            thruDoor = true;
        }
        else if(TnkPtr->x >= (MaxX-4))
        {
            TnkPtr->scrn = 3;
            TnkPtr->x = 5;
            thruDoor = true;
        }
    }
    else if(TnkPtr->scrn == 3)
    {
        if(TnkPtr->x < 4)
        {
            TnkPtr->scrn = 2;
            TnkPtr->x = MaxX - (tankWidth + 1);
            thruDoor = true;
        }
        else if(TnkPtr->y < 4)
        {
            TnkPtr->scrn = 1;
            TnkPtr->y = MaxY - (tankHeight + 1);
            thruDoor = true;
        }
    }
    if (thruDoor && (TnkPtr->color == BlueTank)) // Goog guy?
        curScrn = TnkPtr->scrn;
}

/*****************************************************************************
*
*****************************************************************************/
int CheckGameOver()
{
    int rval = 0;
    if(redCount < 1)
    {
        rval = 1;
    }
    else if(blueCount < 1) // You loose
    {
        rval = 2;
    }
    return rval;
}

/*****************************************************************************
* Summary:
* Parameters: None
*****************************************************************************/
void ChkCollisions()
{
    int j=0;
    for(int i = bulletList->Count()-1; i >= 0;i--)
    {
        ItemList::TItemRec *brPtr =  bulletList->getItem(i);
        if(TankCollision(brPtr->x, brPtr->y, &j))
        {
            if(j >= 0 && j < tanksList->Count())
            {
               ItemList::TItemRec *TnkPtr = tanksList->getItem(j);
               ItemList::TItemRec *ExpRec = new ItemList::TItemRec();
               ExpRec->x = brPtr->x - (explosionWidth / 2);
               ExpRec->y = brPtr->y - (explosionHeight / 2);
               ExpRec->directionIdx = 0;
               ExpRec->scrn = curScrn;
               explosionList->addItem(ExpRec);
               TnkPtr->color = DeadTank;
               TnkPtr->directionIdx = 0;
               bulletList->deleteItem(i);
            }
        }
        else if(WallCollision(brPtr->x, brPtr->y, &j))
        {
            if(j >= 0 && j <  blocksList->Count())
            {
               ItemList::TItemRec *ExpRec = new ItemList::TItemRec();
               ExpRec->x = brPtr->x - (explosionWidth / 2);
               ExpRec->y = brPtr->y - (explosionHeight / 2);
               ExpRec->directionIdx = 0;
               ExpRec->scrn = curScrn;
               explosionList->addItem(ExpRec);
               bulletList->deleteItem(i);
            }
        }
    } // next i
}

/*****************************************************************************
* Summary:
* Parameters:
*****************************************************************************/
bool chkBump(int x, int y)
{
    bool retVal = false;
    int idx = 0;

    return TankCollision(x,y,&idx) || WallCollision(x,y, &idx);
}

/*****************************************************************************
* Summary: When moving thru a door, mov tank to the new screen.
* Parameters:
*****************************************************************************/
bool TankCollision(int x,int y, int *idx)
{
    ItemList::TItemRec *tnkPtr;
    bool retval = false;

    *idx = -1;
    int i = tanksList->Count() - 1;
    while(i >=0)
    {
        tnkPtr = tanksList->getItem(i);
        if(tnkPtr->scrn == curScrn)
        {
            int x1 = tnkPtr->x;
            int y1 = tnkPtr->y;
            int x2 = tnkPtr->x + tankWidth;
            int y2 = tnkPtr->y + tankHeight;
            if(Collision(x, y, x1, y1, x2, y2))
            {
                retval = true;
                *idx = i;
                i = 0;
                break;
            }
        }
        i = i-1;
    }

    return retval;
}

/*****************************************************************************
* Summary: When moving thru a door, mov tank to the new screen.
* Parameters: x,y  - Point to be tested
*             idx  - Index of the wall block to ne checked
* Returns: True if point is inside the wall block
* Test collision of point with a wall block.
*****************************************************************************/
bool WallCollision(int x, int y, int *idx)
{
    bool retval = false;
    ItemList::TItemRec *ptRec;

    int i = blocksList->Count() - 1;
    while(i >=0)
    {
        ptRec = blocksList->getItem(i);
        if(ptRec->scrn == curScrn)
        {
            int x1 = ptRec->x - 1;
            int y1 = ptRec->y - 1;
            int x2 = ptRec->x + BlockImage->w;
            int y2 = ptRec->y + BlockImage->h;
            if(Collision(x, y, x1, y1, x2, y2))
            {
                retval = true;
                *idx = i;
                i = 0;
                break;
            }
        }
        i = i - 1;
    }
    return retval;
}

/*****************************************************************************
* Test to see if point is inside rectangle.
* Parameters:
*   x, y            - Point
*   x1, y1, x2, y2  - Rectangle
* Returns:
*****************************************************************************/
bool Collision(int x, int y, int x1, int y1, int x2, int y2)
{
    bool retval = ((x >x1) && (x <x2) && (y > y1) && (y < y2));

    return retval;
} // Collision

/******************************************************************************
* Test to see if tank[idx] is aiming at a target.
******************************************************************************/
bool AimingAtTarget(int idx)
{
    bool retVal = false;
    ItemList::TItemRec *tnkPtr = tanksList->getItem(idx);
    int x2 = tnkPtr->x + (tankWidth / 2);
    int y2 = tnkPtr->y + (tankHeight / 2);
    int dir = tnkPtr->directionIdx;

    tnkPtr = tanksList->getItem(GoodGuyIdx);

    if(tnkPtr->color == BlueTank )
    {
        int x1 = tnkPtr->x + (tankWidth / 2);
        int y1 = tnkPtr->y + (tankHeight / 2);

        int deltaX  = (x1 - x2);
        int deltaY  = (y2 - y1);
        int t = 999;
        if(deltaY != 0)
            t = (deltaX * 100) / deltaY;
        switch(dir)
        {
            case 0:
                retVal = (deltaY > 1) && ( abs(t) < 20);
                break;
            case 1:
                retVal = (deltaX > 1) && ( t > 40) and (t < 300);
                break;
            case 2:
                retVal = (deltaX > 1) && ( abs(t) > 400);
                break;
            case 3:
                retVal = (deltaX > 1) && ( t < -40) and (t > -300);
                break;
            case 4:
                retVal = (deltaY < -1) && ( abs(t) < 20);
                break;
            case 5:
                retVal = (deltaX < -1) && ( t > 40) and (t < 300);
                break;
            case 6:
                retVal = (deltaX < -1) && ( abs(t) > 400);
                break;
            case 7:
                retVal = (deltaX < -1) && ( t < -40) and (t > -300);
                break;
        }
    }

    return retVal;
}// AimingAtTarget
/*****************************************************************************
 * Summary:
 * Parameters:
 *****************************************************************************/
void badGuyRoutine(int tankIdx) {
    ItemList::TItemRec *tnkPtr = tanksList->getItem(tankIdx);

    int i = rand() % 10 + 1;
    if (i == 2) {
        tnkPtr->directionIdx++;
        if (tnkPtr->directionIdx >= DIR_COUNT)
            tnkPtr->directionIdx = 0;
    } else if (i == 4) {
        tnkPtr->directionIdx--;
        if (tnkPtr->directionIdx < 1)
            tnkPtr->directionIdx = DIR_COUNT - 1;
    } else {
        MoveTank(tankIdx, 2);
        if (tnkPtr->y < 2) {
            if (tnkPtr->directionIdx < 2)
                tnkPtr->directionIdx++;
            else if (tnkPtr->directionIdx == (DIR_COUNT - 1))
                tnkPtr->directionIdx--;
        } else if (tnkPtr->y >= (MaxY - 2)) {
            if ((tnkPtr->directionIdx < 6) && (tnkPtr->directionIdx >= 3))
                    tnkPtr->directionIdx--;
            else if (tnkPtr->directionIdx == 6)
                tnkPtr->directionIdx++;
        } else if (tnkPtr->x < 2) {
            if (tnkPtr->directionIdx == 7)
                tnkPtr->directionIdx = 0;
            else if (tnkPtr->directionIdx >= 5)
                tnkPtr->directionIdx--;
        } else if (tnkPtr->x >= (MaxX - 2)) {
            if (tnkPtr->directionIdx == 1)
                tnkPtr->directionIdx--;
            else if ((tnkPtr->directionIdx == 2) || (tnkPtr->directionIdx == 3))
                tnkPtr->directionIdx--;
        }
    }
    if (AimingAtTarget(tankIdx) && (tnkPtr->scrn == curScrn)) {
        ItemList::TItemRec *brPtr = new ItemList::TItemRec();
        brPtr->directionIdx = tnkPtr->directionIdx;
        brPtr->dist = 0;
        brPtr->scrn = tnkPtr->scrn;
        brPtr->x = tnkPtr->x + ShotStartX[tnkPtr->directionIdx];
        brPtr->y = tnkPtr->y + ShotStartY[tnkPtr->directionIdx];
        bulletList->addItem(brPtr);
    }
} // badGuyRoutine

/****************************************************************************
*
****************************************************************************/
void DrawImage(SDL_Surface* image, SDL_Surface* destSurface, int x, int y)
{
    SDL_Rect destRect;
    destRect.x = x;
    destRect.y = y;

    SDL_BlitSurface(image, NULL, destSurface, &destRect);
}

/****************************************************************************
*
****************************************************************************/
void DrawImageFrame(SDL_Surface* image, SDL_Surface* destSurface,
             int x, int y, int width, int height, int frame)
{
    SDL_Rect destRect;
    destRect.x = x;
    destRect.y = y;

    int columns = image->w/width;

    SDL_Rect sourceRect;
    sourceRect.y = (frame/columns)*height;
    sourceRect.x = (frame%columns)*width;
    sourceRect.w = width;
    sourceRect.h = height;

    SDL_BlitSurface(image, &sourceRect, destSurface, &destRect);
}

/****************************************************************************
* Summary: Paint all of  the objects on the game screen.
****************************************************************************/
void PaintGame()
{
    ItemList::TItemRec *ptRec;

    ClearScreen();

    // Draw Wall
    for(int i = 0; i< blocksList->Count(); i++)
    {
        ptRec = blocksList->getItem(i);
        if(ptRec->scrn == curScrn)
            DrawImage(BlockImage, Backbuffer, ptRec->x, ptRec->y);
    } // next i

    // Draw tanks
    for(int i = 0; i < tanksList->Count(); i++)
    {
        ptRec = tanksList->getItem(i);
        if(ptRec->scrn == curScrn)
        {
            if (ptRec->color == BlueTank)
            {
                DrawImageFrame(blueTanks, Backbuffer, ptRec->x, ptRec->y,
                   tankWidth, tankHeight, ptRec->directionIdx);
            }
            else if (ptRec->color == RedTank)
            {
                DrawImageFrame(redTanks, Backbuffer, ptRec->x, ptRec->y,
                    tankWidth, tankHeight, ptRec->directionIdx);
            }
            else if (ptRec->color == DeadTank)
            {
                DrawImageFrame(deadTanks, Backbuffer, ptRec->x, ptRec->y,
                    tankWidth, tankHeight, ptRec->directionIdx);
            }
        }
    } // next i

    // Draw bullets
    for(int i = 0; i < bulletList->Count(); i++)
    {
        ptRec =  bulletList->getItem(i);
        if(ptRec->scrn == curScrn)
        {
            SDL_Rect* rect = new SDL_Rect;
            rect->x = ptRec->x-1;
            rect->y = ptRec->y-1;
            rect->h = 3;
            rect->w = 3;
            SDL_FillRect(Backbuffer, rect, 0);
        }
    }

    // Draw explosions
    for(int i = 0; i < explosionList->Count(); i++)
    {
        ptRec = explosionList->getItem(i);
        if(ptRec->scrn == curScrn)
        {
            DrawImageFrame(explosions, Backbuffer, ptRec->x, ptRec->y,
                    explosionWidth, explosionHeight, ptRec->directionIdx);
        }
    }

    // Draw Trees
    for(int i = 0; i < treeList->Count(); i++)
    {
        ptRec = treeList->getItem(i);
        if(ptRec->scrn == curScrn)
        {
            DrawImage(TreeImage, Backbuffer, ptRec->x, ptRec->y);
        }
    }

} // PaintGame()


void ShowScore()
{
    char s[20];
    sprintf(s, "Score: %4.1f", score);
    int x= width - 150;
    int y= height- 80;

    drawText->printText(Backbuffer, s, x, y);
}

