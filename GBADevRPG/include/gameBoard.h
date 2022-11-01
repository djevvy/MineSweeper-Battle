
//{{BLOCK(gameBoard)

//======================================================================
//
//	gameBoard, 256x256@4, 
//	+ palette 16 entries, not compressed
//	+ 34 tiles (t|f reduced) not compressed
//	+ regular map (flat), not compressed, 32x32 
//	Total size: 32 + 1088 + 2048 = 3168
//
//	Time-stamp: 2022-09-26, 21:05:35
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.16
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_GAMEBOARD_H
#define GRIT_GAMEBOARD_H

#define gameBoardTilesLen 1088
extern const unsigned int gameBoardTiles[272];

#define gameBoardMapLen 2048
extern const unsigned int gameBoardMap[512];

#define gameBoardPalLen 32
extern const unsigned short gameBoardPal[16];

#endif // GRIT_GAMEBOARD_H

//}}BLOCK(gameBoard)
