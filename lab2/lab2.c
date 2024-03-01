/*
 *
 * CSEE 4840 Lab 2 for 2019
 *
 * Name/UNI: Zachary Coeur (zjc2106)/Sanjay Rajasekharan (sr3765)
 */
#include "fbputchar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "usbkeyboard.h"
#include <pthread.h>
#include "keymappings.h"

/* Update SERVER_HOST to be the IP address of
 * the chat server you are connecting to
 */
/* arthur.cs.columbia.edu */
#define SERVER_HOST "128.59.19.114"
#define SERVER_PORT 42000

#define BUFFER_SIZE 128

#define SERVER_FIRST_ROW 1
#define SERVER_LAST_ROW 19

#define USER_FIRST_ROW 21
#define USER_LAST_ROW 22

#define FIRST_COL 0

int usb_to_ascii[] = {
    0,   // 0x00
    0,   // 0x01
    0,   // 0x02
    0,   // 0x03
    'a', // 0x04
    'b', // 0x05
    'c', // 0x06
    'd', // 0x07
    'e', // 0x08
    'f', // 0x09
    'g', // 0x0A
    'h', // 0x0B
    'i', // 0x0C
    'j', // 0x0D
    'k', // 0x0E
    'l', // 0x0F
    'm', // 0x10
    'n', // 0x11
    'o', // 0x12
    'p', // 0x13
    'q', // 0x14
    'r', // 0x15
    's', // 0x16
    't', // 0x17
    'u', // 0x18
    'v', // 0x19
    'w', // 0x1A
    'x', // 0x1B
    'y', // 0x1C
    'z'  // 0x1D
};

#define LEFT_SHIFT 0x02
#define RIGHT_SHIFT 0x20

#define IS_SHIFTED(x) (x == LEFT_SHIFT || x == RIGHT_SHIFT)

#define LEFT_CTRL 0x01
#define RIGHT_CTRL 0x10

#define IS_CTRL(x) (x == LEFT_CTRL || x == RIGHT_CTRL)

#define LEFT_ARROW 0x50
#define RIGHT_ARROW 0x4F
#define UP_ARROW 0x52
#define DOWN_ARROW 0x51

#define ENTER 0x28
#define EMPTY 0x00

#define BACKSPACE 0x2A
#define SPACE 0x2C

/*
 * References:
 *
 * https://web.archive.org/web/20130307100215/http://beej.us/guide/bgnet/output/html/singlepage/bgnet.html
 *
 * http://www.thegeekstuff.com/2011/12/c-socket-programming/
 * 
 */

int sockfd; /* Socket file descriptor */

struct libusb_device_handle *keyboard;
uint8_t endpoint_address;

pthread_t network_thread;
void *network_thread_f(void *);

void clear_user_input(char *user_input, int *cursor, int *message_length, int *user_row, int *user_col) {
  *user_row = USER_FIRST_ROW;
  *user_col = FIRST_COL;

  *cursor = 0;
  *message_length = 0;
  memset(user_input, '\0', BUFFER_SIZE);

  fbclearrows(USER_FIRST_ROW, USER_LAST_ROW);
}

int main()
{
  int err, col;

  struct sockaddr_in serv_addr;

  struct usb_keyboard_packet packet;
  int transferred;
  char keystate[12];

  // get last visible row and col of screen
  int last_row = 0;
  int last_col = 64;

  // testing var used for keyboard input
  char temp_keystate[1];


  if ((err = fbopen()) != 0) {
    fprintf(stderr, "Error: Could not open framebuffer: %d\n", err);
    exit(1);
  }


  fbclear();

  /* Draw rows of asterisks across the top and bottom of the screen */
  for (col = FIRST_COL ; col < last_col ; col++) {
    fbputchar('*', SERVER_FIRST_ROW - 1, col);
    fbputchar('*', 23, col);
    fbputchar('-', SERVER_LAST_ROW  + 1, col);
  }

  fbputchar('*', USER_LAST_ROW + 1, FIRST_COL);

  // fbputs("Hello CSEE 4840 World!", 4, 10);

  /* Open the keyboard */
  if ( (keyboard = openkeyboard(&endpoint_address)) == NULL ) {
    fprintf(stderr, "Did not find a keyboard\n");
    exit(1);
  }
    
  /* Create a TCP communications socket */
  if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
    fprintf(stderr, "Error: Could not create socket\n");
    exit(1);
  }

  /* Get the server address */
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(SERVER_PORT);
  if ( inet_pton(AF_INET, SERVER_HOST, &serv_addr.sin_addr) <= 0) {
    fprintf(stderr, "Error: Could not convert host IP \"%s\"\n", SERVER_HOST);
    exit(1);
  }

  /* Connect the socket to the server */
  if ( connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
    fprintf(stderr, "Error: connect() failed.  Is the server running?\n");
    exit(1);
  }

  /* Start the network thread */
  pthread_create(&network_thread, NULL, network_thread_f, NULL);

  /* Look for and handle keypresses */
  int user_row = USER_FIRST_ROW;
  int user_col = FIRST_COL;

  int cursor = 0;
  int message_length = 0;
  char user_input[BUFFER_SIZE];

  char clear_input[BUFFER_SIZE];
  memset(clear_input, ' ', BUFFER_SIZE);
  clear_input[BUFFER_SIZE - 1] = '\0';

  int prev_keycode = EMPTY;

  for (;;) {

    // fbputs(clear_input, USER_FIRST_ROW, FIRST_COL);
    fbclearrows(USER_FIRST_ROW, USER_LAST_ROW);
    fbputs(user_input, USER_FIRST_ROW, FIRST_COL);
    
    cursor_fbputchar((cursor >= message_length) ? ' ' : user_input[cursor], user_row, user_col);

    libusb_interrupt_transfer(keyboard, endpoint_address,
			      (unsigned char *) &packet, sizeof(packet),
			      &transferred, 0);
    if (transferred == sizeof(packet)) {
      printf("%s\n", user_input);
      sprintf(keystate, "%02x %02x %02x", packet.modifiers, packet.keycode[0],
	      packet.keycode[1]);
      printf("%s\n", keystate);
      // fbputs(keystate, 21, 0);

      if (packet.keycode[0] != prev_keycode){
        if (IS_CTRL(packet.modifiers)) {
          if (mapCharacter(packet.keycode[0], IS_SHIFTED(packet.modifiers)) == 'u') {
            clear_user_input(user_input, &cursor, &message_length, &user_row, &user_col);
          }
        }

        else if (packet.keycode[0] == LEFT_ARROW) {
          if (cursor > 0) {
            if (user_col == FIRST_COL) {
              user_col = last_col;
              user_row--;
            } else user_col--;
            cursor--;
          }
        }

        else if (packet.keycode[0] == RIGHT_ARROW) {
          if (cursor < message_length) {
            if (user_col == last_col) {
              user_col = FIRST_COL;
              user_row++;
            } else user_col++;

            cursor++;
          }
        }

        else if (packet.keycode[0] == ENTER) {
          // send message to server
          write(sockfd, user_input, strlen(user_input));
          clear_user_input(user_input, &cursor, &message_length, &user_row, &user_col);
        }

        else if (packet.keycode[0] == BACKSPACE) {
          fbputchar(' ', user_row, user_col); // clear currently displayed cursor 
          if (cursor > 0) {
            // shift everything from right
            for (int i = cursor; i < message_length; i++) {
              user_input[i] = user_input[i+1];
            }

            cursor--;
            message_length--;
            user_input[message_length] = '\0';

            if (user_col == FIRST_COL) {
              user_col = last_col;
              user_row--;
            } else user_col--;

            fbputchar(' ', user_row, user_col); // clear deleted character
            
          }
        }
        else if (packet.keycode[0] == SPACE) {
          if (message_length < BUFFER_SIZE - 1) {
            if (user_col >= last_col) {
              user_col = FIRST_COL;
              user_row++;
            } else user_col++;

            user_input[cursor++] = ' ';
            message_length++;
          }
        }

        else if (packet.keycode[0] != EMPTY)
        {
          // VERY basic way to convert single keycode to char
          sprintf(temp_keystate, "%c", mapCharacter(packet.keycode[0], IS_SHIFTED(packet.modifiers)));
          //printf("%c\n", temp_keystate);

          if (message_length < BUFFER_SIZE - 1 && temp_keystate[0] != 0) {

            if (user_col >= last_col - 1) {
              user_col = FIRST_COL;
              user_row++;
            } else user_col++;

            user_input[cursor++] = temp_keystate[0];
            message_length++;

            printf("user_input[%d]: %d\n", cursor-1, user_input[cursor-1]);
          }
        }



        if (packet.keycode[0] == 0x29) { /* ESC pressed? */
          break;
        }

      }
      prev_keycode = packet.keycode[0];


    }
  }

  /* Terminate the network thread */
  pthread_cancel(network_thread);

  /* Wait for the network thread to finish */
  pthread_join(network_thread, NULL);

  return 0;
}

void *network_thread_f(void *ignored)
{
  char recvBuf[BUFFER_SIZE];
  int n;

  // get last visible row and col of screen
  // int last_row = getLastRow();
  int last_col = getLastCol();

  int row = SERVER_FIRST_ROW; // row in which messages begin on
  /* Receive data */
  while ( (n = read(sockfd, &recvBuf, BUFFER_SIZE - 1)) > 0 ) {
    recvBuf[n] = '\0';
    printf("%s\n", recvBuf);
    if (strlen(recvBuf) / (last_col - FIRST_COL + 1) + row > SERVER_LAST_ROW) {
      fbclearrows(SERVER_FIRST_ROW, SERVER_LAST_ROW);
      row = SERVER_FIRST_ROW;
    }
    row = fbputs(recvBuf, row, FIRST_COL)  + 1;
    // row = row + 1 + (strlen(recvBuf) / last_col);

    // when too many messages come through, clear all messages
    if(row >= 20){
        // this has gotta be terribly inefficient, is there a better way?
        fbclearrows(SERVER_FIRST_ROW, SERVER_LAST_ROW);
      row = SERVER_FIRST_ROW;
    }
  }

  return NULL;
}

