#include "main.h"

#include <stdio.h>

#include <tonc.h>
#include <string.h>

#include "gameBoard.h"
#include "gameBoard.c"

#include "lv1.h"
#include "lv1.c"

#include "cursor.h"
#include "cursor.c"

#include "player.h"
#include "player.c"

#include "ball.h"
#include "ball.c"


OBJ_ATTR obj_buffer[128];
OBJ_ATTR *cursor;

OBJ_ATTR *player;

OBJ_ATTR *ball;



unsigned int NewMap[512];
unsigned int NewCover[512];

unsigned int flag[512];
unsigned int uncovered[512];



int tiles[72];


int mineNum = 20;




int cursorPosition;
int cursorX = 0;
int cursorY = 0;



int damangeAmount;
int baseDamage;

int playerHealth = 10;
int enemyHealth  = 10;



int animTimer;
int animRunning;
int tid;


int PlayerAnimframe;
int blinkAnim[] =
{
    4,
    20,
    36,
    20,
    4
};


int main()
{
    InitScreen();

    // Init BG 0 for text on screen entries.
    tte_init_se_default(0, BG_CBB(3)|BG_SBB(27));
    tte_init_con();
    
    pal_bg_bank[15][1]= CLR_WHITE;
    
    tte_write("#{P:88,88}");
    tte_write("New Game");
    tte_write("#{P:80,96}");
    tte_write("Press START");
    
    InitPlayer();
    InitEnemy();
    
    while(1)
    {
        key_poll();
        vid_vsync();

        
        if(animTimer == 15)
        {
            animTimer = 0;
            
            if(animRunning == 0)
            {
                int blink = qran_range(0, 10);
                if(blink == 4)
                {
                    animRunning = 1;
                }
            }

            if(animRunning == 1)
                AnimPlayer(blinkAnim);
            
        }
        else
            animTimer++;
        
        moveCursor();
        
        if(key_hit(KEY_START))
        {
            for (int i = 0; i < 512; i ++)
            {
                flag[i] = 0;
                uncovered[i] = 0;
            }
            
            
            playerHealth = 99;
            enemyHealth = 99;
            
            tte_write("#{er:0,0,240,160}");
            
            UpdateText();
            
            InitCursor();
            
            
            
            
            GenerateMinefield();
            MakeCover();
            MakeMap();
            
            InitCover();
            
            REG_DISPCNT= DCNT_MODE0 | DCNT_BG0 | DCNT_BG1 | /*DCNT_BG2 |*/ DCNT_BG3 | DCNT_OBJ | DCNT_OBJ_1D;
            
        }
        
        if(key_hit(KEY_A))
        {
            if(flag[CursorToTile()] == 1)
            {
                
            }
            if(uncovered[CursorToTile()] == 1)
            {
                
            }
            else
            {
                uncovered[CursorToTile()] = 1;
                
                RemoveCover(CursorToTile());
                
                if(smineCalc(cursorX,cursorY) < 99)
                {
                    enemyHealth -= smineCalc(cursorX,cursorY);
                }
                
                
                UpdateText();
            }
            
            
        }
        
        if(key_hit(KEY_B))
        {
            
            if(flag[CursorToTile()] == 1)
            {
                
            }
            if(uncovered[CursorToTile()] == 1)
            {
                
            }
            else
            {
                flag[CursorToTile()] = 1;
                
                FlagCover(CursorToTile());
                
                if(smineCalc(cursorX,cursorY) == 99)
                {
                    enemyHealth -= 10;
                }
                
                UpdateText();
            }
            
            
        }
        
        
        if(enemyHealth <= 0)
        {
            GameWin();
        }
        else if(playerHealth <= 0)
        {
            GameOver();
        }
    }
}






  
void moveCursor()
{
    
    if(key_hit(KEY_LEFT))
        cursorX --;
    else if(key_hit(KEY_RIGHT))
        cursorX ++;
    
    if(key_hit(KEY_UP))
        cursorY --;
    else if(key_hit(KEY_DOWN))
        cursorY ++;
    
    if(cursorX < 0)
        cursorX = 11;
    if(cursorY < 0)
        cursorY = 5;

    if(cursorX > 11)
        cursorX = 0;
    if(cursorY > 5)
        cursorY = 0;
    
    obj_set_pos(&obj_buffer[0], 24 + (cursorX * 16), 56 + (cursorY * 16));
    
    oam_copy(oam_mem, obj_buffer, 1);   // Update OAM
}













void UpdateText()
{
    tte_write("#{er:0,0,240,0}");
    
    char pHealth[2];
    char eHealth[2];
    sprintf(pHealth, "%d", playerHealth);
    
    
    if(enemyHealth >= 0)
    {
        sprintf(eHealth, "%d", enemyHealth);
    }
    else
    {
        sprintf(eHealth, "%d", 0);
    }
    
    
    
    tte_write("#{P:80,0}");
    tte_write(pHealth);
    
    
    tte_write("#{P:128,0}");
    tte_write(eHealth);
    
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  GAMEFUNCS

void GameWin()
{
    obj_set_attr(
        &obj_buffer[0],
        ATTR0_SQUARE,
        ATTR1_SIZE_32,
        ATTR2_PALBANK(1) | 128 | ATTR2_PRIO(0)
                 );
    oam_copy(oam_mem, obj_buffer, 1);   // Update OAM
    REG_DISPCNT= DCNT_MODE0 | DCNT_BG0 | DCNT_BG3 | DCNT_OBJ | DCNT_OBJ_1D;
    
    tte_write("#{P:88,88}");
    tte_write("You Win!");
    tte_write("#{P:80,96}");
    tte_write("Press START");
}

void GameOver()
{
    obj_set_attr(
        &obj_buffer[0],
        ATTR0_SQUARE,
        ATTR1_SIZE_32,
        ATTR2_PALBANK(1) | 128 | ATTR2_PRIO(0)
                 );
    oam_copy(oam_mem, obj_buffer, 1);   // Update OAM
    REG_DISPCNT= DCNT_MODE0 | DCNT_BG0 | DCNT_BG3 | DCNT_OBJ | DCNT_OBJ_1D;
    
    tte_write("#{P:80,64}");
    tte_write("Game Over");
    tte_write("#{P:80,96}");
    tte_write("Press START");
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  MINEMAP


void MakeMap()
{
    REG_BG1HOFS = -24;
    REG_BG1VOFS = -56;

    for (int y = 0; y < 6; y++)
    {
        for (int x = 0; x < 12; x++)
        {
            int mineLoc = x + (12 * y);
            int GBLoc   = x + (32 * y);
            
            if(tiles[mineLoc] == 99)
            {
                NewMap[GBLoc]      = gameBoardMap[484 + 10];
                NewMap[GBLoc + 16] = gameBoardMap[500 + 10];
            }
            else
            {
                NewMap[GBLoc]      = gameBoardMap[484 + tiles[GBLoc]];
                NewMap[GBLoc + 16] = gameBoardMap[500 + tiles[GBLoc]];
            }
            
            
        }
    }
    
    memcpy32(&se_mem[30][0], NewMap, 512);                                      // Load new map into SBB 31
    REG_BG1CNT= BG_PRIO(2) | BG_CBB(0) | BG_SBB(30) | BG_REG_32x32;             // Set BG1 to use CBB 0 and SBB 31 (new map) with a 32x32 size
}


void GenerateMinefield()
{
    for (int i = 0; i < 72; i++)
        tiles[i] = 0;
    for (int i = 0; i < mineNum; i++)
    {
        int temp = qran_range(0, 73);
        
        if(tiles[temp] == 0)
            tiles[temp] = 99;
        else
            i --;
    }
    
    
    for (int ty = 0; ty < 6; ty++)
    {
        for (int tx = 0; tx < 12; tx++)
        {
            tiles[tx + (ty*32)] = smineCalc(tx,ty);
        }
    }
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  COVERMAP

void InitCover()
{
    REG_BG2HOFS = -24;
    REG_BG2VOFS = -56;

    for (int y = 0; y < 6; y++)
    {
        for (int x = 0; x < 12; x++)
        {
            int GBLoc   = x + (32 * y);
            

            NewCover[GBLoc] = 0x00010001;
            NewCover[GBLoc + 16] = 0x00020002;
        }
    }
    
    
    
    memcpy32(&se_mem[31][0], NewCover, 512);                                      // Load new map into SBB 31
    REG_BG2CNT= BG_PRIO(1) | BG_CBB(0) | BG_SBB(31) | BG_REG_32x32;             // Set BG1 to use CBB 0 and SBB 31 (new map) with a 32x32 size
}


void MakeCover()
{
    for (int y = 0; y < 6; y++)
    {
        for (int x = 0; x < 12; x++)
        {
            int GBLoc   = x + (32 * y);
            int mineLoc = x + (12 * y);
            
            if(NewCover[GBLoc] == 1)
            {
                NewCover[GBLoc]      = NULL;
                NewCover[GBLoc + 16] = NULL;
            }

            
            
            
            if(NewCover[GBLoc] == 2)
            {
                NewCover[GBLoc]      = gameBoardMap[484 + 11];
                NewCover[GBLoc + 16] = gameBoardMap[500 + 11];
            }
        }
    }
    
    memcpy32(&se_mem[31][0], NewCover, 512);                                      // Load new map into SBB 32
    REG_BG2CNT= BG_PRIO(1) | BG_CBB(0) | BG_SBB(31) | BG_REG_32x32;             // Set BG1 to use CBB 0 and SBB 32 (new map) with a 32x32 size
}


void FlagCover(int tile)
{
    NewCover[tile] = 2;
    MakeCover();
}

void RemCov(int tile)
{
    NewCover[tile] = 1;
    MakeCover();
}



void openNeighbors(int tile)
{
//    for (int y = 0; y < 6; y++)
//    {
//        for (int x = 0; x < 12; x++)
//        {
//            if(tiles[x + (y*32)] == 0)
//            {
//                RemCov(x + (y*32));
//            }
//        }
//    }
}



void RemoveCover(int tile)
{
    
    if(smineCalc(cursorX,cursorY) == 0)
    {
        RemCov(tile);
    }
    
    
    openNeighbors(tile);
    
    
    if(smineCalc(cursorX,cursorY) == 99)
    {
        GameOver();
    }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  BG


void InitScreen()
{
    oam_init(obj_buffer, 128);                                                                     // initialize oam with obj_buffer (size 128)

    memcpy32(pal_bg_bank[0], gameBoardPal, gameBoardPalLen/4);                      // Load BG palette
    memcpy32(&tile_mem[0][0], gameBoardTiles, gameBoardTilesLen/4);             // Load tiles into CBB 0
    memcpy32(&se_mem[29][0], gameBoardMap, gameBoardMapLen/4);                  // Load map into SBB 30
    
    
    
    memcpy32(pal_bg_bank[1], lv1Pal, lv1PalLen/4);                      // Load BG palette
    memcpy32(&tile_mem[1][0], lv1Tiles, lv1TilesLen/4);             // Load tiles into CBB 0
    memcpy32(&se_mem[28][0], lv1Map, lv1MapLen/4);                  // Load map into SBB 30

    
    for(int i = 0; i < 1024; i ++)
    {
        se_mem[28][i] += SE_PALBANK(1);
        se_mem[31][i] += SE_PALBANK(3);
    }



    REG_BG1CNT= BG_PRIO(2) | BG_CBB(0) | BG_SBB(30) | BG_REG_32x32;             // Set BG1 to use CBB 0 and SBB 31 with a 32x32 size - tiles
    REG_BG2CNT= BG_PRIO(1) | BG_CBB(0) | BG_SBB(31) | BG_REG_32x32;             // Set BG2 to use CBB 0 and SBB 32 with a 32x32 size - cover
    
    REG_BG3CNT= BG_PRIO(0) | BG_CBB(1) | BG_SBB(28) | BG_REG_32x32 | BG_4BPP;             // Set BG1 to use CBB 0 and SBB 31 with a 32x32 size - level


    REG_DISPCNT= DCNT_MODE0 | DCNT_BG0 | DCNT_BG1 | DCNT_BG2 | DCNT_BG3 | DCNT_OBJ | DCNT_OBJ_1D;                   // Set up screen in MODE0, with BG0, BG1 and BG2 turned on and OBJ enabled
    
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  TILE FUNCTIONS


int CursorToTile()
{
    int pos = cursorX + (cursorY * 32);
    
    return pos;
}

int smineCalc(int x, int y)
{
    int mineLoc = x + (12 * y);
    int smine = 0;
    
    if(tiles[mineLoc] == 99)
    {
        smine = 99;
    }
    else
    {
        if(x < 11)
        {
            if(tiles[mineLoc + 1] == 99)
                smine++;
        }
        if(x > 0)
        {
            if(tiles[mineLoc - 1] == 99)
                smine++;
        }
        
        if(y < 6)
        {
            if(tiles[mineLoc + 12] == 99)
                smine++;
        }
        if(y > 0)
        {
            if(tiles[mineLoc - 12] == 99)
                smine++;
        }
        
        if(y < 6 && x < 11)
        {
            if(tiles[mineLoc + 12 + 1] == 99)
                smine++;
        }
        if(y < 6 && x > 0)
        {
            if(tiles[mineLoc + 12 - 1] == 99)
                smine++;
        }
        if(y > 0 && x < 11)
        {
            if(tiles[mineLoc - 12 + 1] == 99)
                smine++;
        }
        if(y > 0 && x > 0)
        {
            if(tiles[mineLoc - 12 - 1] == 99)
                smine++;
        }
    }

    return smine;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  SPRITES

void InitCursor()
{
    cursorX = 0;
    cursorY = 0;
    
    memcpy32(pal_obj_mem, cursorPal, cursorPalLen/4);                                              // copy sprite palette into palette memory
    
    memcpy32(&tile_mem[4][0], cursorTiles, cursorTilesLen/4);                                      // copy sprite information (4bpp boxed sprite) into low memory (cbb == 4)
    
    OBJ_ATTR *cursor= &obj_buffer[0];
    obj_set_attr(
        cursor,
        ATTR0_SQUARE,
        ATTR1_SIZE_16,
        ATTR2_PALBANK(0) | 0 | ATTR2_PRIO(1)
                 );
    oam_copy(oam_mem, obj_buffer, 1);   // Update OAM
}

void InitPlayer()
{
    memcpy32(pal_obj_mem + 16, playerPal, playerPalLen/4);                                              // copy sprite palette into palette memory
    
    memcpy32(&tile_mem[4][4], playerTiles, playerTilesLen/4);                                      // copy sprite information (4bpp boxed sprite) into low memory (cbb == 4)
    
    OBJ_ATTR *player= &obj_buffer[1];
    obj_set_attr(
        player,
        ATTR0_SQUARE,
        ATTR1_SIZE_32,
        ATTR2_PALBANK(1) | 4 | ATTR2_PRIO(0)
                 );
    
    obj_set_pos(&obj_buffer[1], 8,16);
    oam_copy(oam_mem, obj_buffer, 2);   // Update OAM
}

void InitEnemy()
{
    memcpy32(pal_obj_mem + 32, ballPal, ballPalLen/4);                                              // copy sprite palette into palette memory
    
    memcpy32(&tile_mem[4][52], ballTiles, ballTilesLen/4);                                      // copy sprite information (4bpp boxed sprite) into low memory (cbb == 4)
    
    OBJ_ATTR *ball= &obj_buffer[2];
    obj_set_attr(
        ball,
        ATTR0_SQUARE,
        ATTR1_SIZE_32,
        ATTR2_PALBANK(2) | 52 | ATTR2_PRIO(0)
                 );
    
    obj_set_pos(&obj_buffer[2], 200,16);
    oam_copy(oam_mem, obj_buffer, 3);   // Update OAM
}



void AnimPlayer(int anim[])
{
    if(PlayerAnimframe >= sizeof(anim))
    {
        PlayerAnimframe = 0;
        animRunning = 0;
    }
    tid = anim[PlayerAnimframe];
    PlayerAnimframe ++;
    
    obj_set_attr(
        &obj_buffer[1],
        ATTR0_SQUARE,
        ATTR1_SIZE_32,
        ATTR2_PALBANK(1) | tid | ATTR2_PRIO(0)
                 );
    obj_set_pos(&obj_buffer[1], 8,16);
    oam_copy(oam_mem, obj_buffer, 2);   // Update OAM
}
