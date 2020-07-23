#include <stdio.h>
#include <stdlib.h>
#include <sys/nearptr.h>
#include <pc.h>
#include <dos.h>
#include <string.h>
#include "font.h"


int width = 320;
int height = 200;

char *VGA = (char *)0xA0000;

void setMode(char mode) {
	union REGS regs;

	// Get BIOS video mode (320x200x256)
	regs.h.ah = 0x00;
	regs.h.al = mode;
	int86(0x10, &regs, &regs);
}

void pixel(int x, int y, char color) {
	VGA[y * width + x] = color;
}

// Print a char based on PrismOS font
int printChar(int x, int y, char c, char color) {
	int match = 0;
	for (int l = 0; l < 100; l++) {
		if (font[l].letter == c) {
			match = l;
			break;
		}
	}

	// Loop through 7 high 5 wide monochrome font
	int maxLength = 0;
	for (int py = 0; py < 7; py++) {
		for (int px = 0; px < 5; px++) {
			if (font[match].code[py][px] == '#') {
				pixel(x + px, y + py, color);

				// Dynamix width character spacing
				if (px > maxLength) {
					maxLength = px;
				}
			}
		}
	}

	return maxLength;
}

int printString(int x, int y, char *string, char color) {
	int cx = x;
	int cy = y;

	for (int c = 0; c < strlen(string); c++) {
		// Text wrap after space
		if (cx + 50 > width && string[c] == ' ') {
			cx = x;
			cy += 5 + 4;
			c++;
		}

		int length = printChar(cx, cy, string[c], color);

		if (string[c] == ' ') {
			length = 5;
		}

		cx += length + 3;
	}

	return cy;
}

void rect(int x, int y, int width, int height, char color) {
	// Top
	for (int rx = 0; rx < width; rx++) {
		pixel(x + rx, y, color);
	}

	// Bottom
	for (int rx = 0; rx < width; rx++) {
		pixel(x + rx, y + height, color);
	}

	// Left
	for (int ry = 0; ry < height; ry++) {
		pixel(x, y + ry, color);
	}

	// Bottom
	for (int ry = 0; ry < height; ry++) {
		pixel(x + width, y + ry, color);
	}
}
