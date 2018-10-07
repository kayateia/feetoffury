#ifndef __MANUALFILE_H
#define __MANUALFILE_H

struct MnlHeader {
	char	magic[4];	// "FMNL"
	uint32	imgcnt;		// Number of image links
	uint32	tokencnt;	// Number of body tokens
	uint32	height;		// Total height in pixels of the doc
	char	padding[64 - 16];	// Zero
};

struct MnlImage {
	int32	id;			// -1 if invalid
	uint32	width, height;		// Effective width/height
	int	alpha;			// Non-zero if we want an alpha channel
	char	txrname[256 - 16];	// zero padded
};

struct MnlToken {
	uint32	tokenSize;	// Token total size in bytes
	int32	x, y;		// X and Y position
	uint32	color;		// Color of this token (if not an image)
	int32	size;		// Point size of this token (if not an image)
	int32	img;		// Image index (or -1 if not an image)
	uint32	textSize;	// Length of text (or zero for an image)
	char	text[0];	// Text
};

#endif
