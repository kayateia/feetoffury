/*      
   Feet of Fury
        
   step.h

   Copyright (C)2002 Cryptic Allusion, LLC

   $Id: step.h,v 1.12 2003/03/02 07:11:55 bard Exp $
*/

#ifndef __STEP_H
#define __STEP_H

#define STEPFLAG_ADDED		0x0001	// Step was added due to a bomb
#define STEPFLAG_ERASED		0x0002	// Step was erased
#define STEPFLAG_ITEM_MASK	0xff00	// Mask for item flags
#define STEPFLAG_ITEM_BOMB	0x0100	// Arrow bomb
#define STEPFLAG_ITEM_DRUNKEN	0x0200	// Drunken arrow
#define STEPFLAG_ITEM_VORTEX	0x0300	// Vortex
#define STEPFLAG_ITEM_TURBO	0x0400	// Turbo
#define STEPFLAG_ITEM_PHASING	0x0500	// Phasing arrows
#define STEPFLAG_ITEM_FIREBALL	0x0600	// Fireball
#define STEPFLAG_ITEM_HEALTH	0x1000	// +Health
#define STEPFLAG_ITEM_ERASER	0x1100	// Arrow eraser
#define STEPFLAG_ITEM_DISPEL	0x1200	// Dispel
#define STEPFLAG_ITEM_REVERSE	0x1300	// Reverse

// Small struct with arrow specs; there's one of these per
// potential arrow position in the step data.
class Step {
public:
	Step() {
		blank();
	}
	int	pattern;	// Arrow pattern
	int	ch;		// Character for typing mode
	int	d;		// Dead-on time (in ms)
	int	flags;		// From the defines
	float	fade;		// Fade percent 0..1 for erased arrows

	enum {
		Up = 1 << 0,
		Left = 1 << 1,
		Right = 1 << 2,
		Down = 1 << 3
	};

	static uint32	toCont(int pat) {
		switch(pat) {
		case '0':
			return 0;
		case '8':
			return Up;
		case '4':
			return Left;
		case '6':
			return Right;
		case '2':
			return Down;
		case '7':
			return Up | Left;
		case '9':
			return Up | Right;
		case '1':
			return Down | Left;
		case '3':
			return Down | Right;
		case 'A':
			return Up | Down;
		case 'B':
			return Left | Right;
		case 'C':
			return Up | Down | Right;
		case 'D':
			return Up | Down | Left;
		case 'E':
			return Down | Left | Right;
		case 'F':
			return Up | Left | Right;
		case 'G':
			return Up | Down | Left | Right;
		default:
			Debug::printf("Invalid step data: %c / %d\n", pat, pat);
			// assert_msg( false, "Invalid step data" );
			return 0;
		}
	}

	static int toPattern(uint32 cont) {
		switch(cont) {
		case 0:
			return '0';
		case Up:
			return '8';
		case Left:
			return '4';
		case Right:
			return '6';
		case Down:
			return '2';
		case Up | Left:
			return '7';
		case Up | Right:
			return '9';
		case Down | Left:
			return '1';
		case Down | Right:
			return '3';
		case Up | Down:
			return 'A';
		case Left | Right:
			return 'B';
		case Up | Down | Right:
			return 'C';
		case Up | Down | Left:
			return 'D';
		case Down | Left | Right:
			return 'E';
		case Up | Left | Right:
			return 'F';
		case Up | Down | Left | Right:
			return 'G';
		default:
			Debug::printf("Invalid step data: %08lx\n", cont);
			// assert_msg( false, "Invalid step data" );
			return -1;
		}
	}

	void blank() {
		pattern = '0';
		ch = '\0';
		d = -1;
		flags = 0;
		fade = 1.0f;
		hit = NotYet;
	}

	enum Hit {
		NotYet = 0, Miss,
		Boo, Good, Great, Perfect,
		Added		// Overload this a bit
	}	hit;
};

#endif	/* __STEP_H */

