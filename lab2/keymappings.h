extern char mapCharacter(int keycode, int shift);

#define LEFT_SHIFT 0x02
#define RIGHT_SHIFT 0x20

#define IS_SHIFTED(x) (x == LEFT_SHIFT || x == RIGHT_SHIFT)

#define LEFT_CTRL 0x01
#define RIGHT_CTRL 0x10

#define IS_CTRL(x) (x == LEFT_CTRL || x == RIGHT_CTRL)

// non printable keystrokes
#define LEFT_ARROW 0x50
#define RIGHT_ARROW 0x4F
#define UP_ARROW 0x52
#define DOWN_ARROW 0x51

#define ENTER 0x28
#define RELEASE 0x00
#define ESCAPE 0x29

#define BACKSPACE 0x2A
