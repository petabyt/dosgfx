void setMode(char mode);
void pixel(int x, int y, char color);
int printChar(int x, int y, char c, char color);
int printString(int x, int y, char *string, char color);
void rect(int x, int y, int width, int height, char color);

extern char *VGA;
extern int height;
extern int width;
