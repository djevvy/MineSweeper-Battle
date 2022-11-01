
//{{BLOCK(lv1)

//======================================================================
//
//	lv1, 256x256@4, 
//	+ palette 16 entries, not compressed
//	+ 286 tiles (t|f reduced) not compressed
//	+ regular map (flat), not compressed, 32x32 
//	Total size: 32 + 9152 + 2048 = 11232
//
//	Time-stamp: 2022-09-20, 16:46:02
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.16
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_LV1_H
#define GRIT_LV1_H

#define lv1TilesLen 9152
extern const unsigned int lv1Tiles[2288];

#define lv1MapLen 2048
extern const unsigned int lv1Map[512];

#define lv1PalLen 32
extern const unsigned short lv1Pal[16];

#endif // GRIT_LV1_H

//}}BLOCK(lv1)
