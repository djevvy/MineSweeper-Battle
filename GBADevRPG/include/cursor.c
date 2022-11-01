
//{{BLOCK(cursor)

//======================================================================
//
//	cursor, 16x16@4, 
//	+ palette 16 entries, not compressed
//	+ 4 tiles not compressed
//	Total size: 32 + 128 = 160
//
//	Time-stamp: 2022-09-07, 11:34:13
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.16
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

const unsigned int cursorTiles[32] __attribute__((aligned(4))) __attribute__((visibility("hidden")))=
{
	0x11111100,0x00000000,0x00000001,0x00000001,0x00000001,0x00000001,0x10000001,0x01000001,
	0x00111111,0x00000000,0x10000000,0x10000000,0x10000000,0x10000000,0x10000001,0x10000010,
	0x01000001,0x10000001,0x00000001,0x00000001,0x00000001,0x00000001,0x00000000,0x11111100,
	0x10000010,0x10000001,0x10000000,0x10000000,0x10000000,0x10000000,0x00000000,0x00111111,
};

const unsigned short cursorPal[16] __attribute__((aligned(4))) __attribute__((visibility("hidden")))=
{
	0x0000,0x7FFF,0x44C8,0x2D42,0x48F1,0x268A,0x18F1,0x3B37,
	0x0169,0x010A,0x416B,0x2463,0x49EF,0x3906,0x1000,0x5A73,
};

//}}BLOCK(cursor)
