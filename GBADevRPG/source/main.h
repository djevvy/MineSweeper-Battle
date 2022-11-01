#ifndef MAIN_H
#define MAIN_H

int smineCalc(int x, int y);


void GenerateMinefield();
void MakeMap();
void MakeCover();
void InitScreen();
void InitCursor();
void InitCover();

int SmineToTileTop();
int SmineToTileBottom();

void AnimPlayer(int anim[]);


void GameOver();

void InitPlayer();
void InitEnemy();


void updateText();

#endif
