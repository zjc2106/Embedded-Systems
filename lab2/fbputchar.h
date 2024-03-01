#ifndef _FBPUTCHAR_H
#  define _FBPUTCHAR_H

#define FBOPEN_DEV -1          /* Couldn't open the device */
#define FBOPEN_FSCREENINFO -2  /* Couldn't read the fixed info */
#define FBOPEN_VSCREENINFO -3  /* Couldn't read the variable info */
#define FBOPEN_MMAP -4         /* Couldn't mmap the framebuffer memory */
#define FBOPEN_BPP -5          /* Unexpected bits-per-pixel */

extern int fbopen(void);
extern void fbputchar(char, int, int);
extern int fbputs(const char *, int, int);
// zac functions
extern void fbclear(void);
extern int getLastCol(void);
extern int getLastRow(void);

extern void cursor_fbputchar(char c, int row, int col);
extern void fbclearrows(int start, int end);

#endif
