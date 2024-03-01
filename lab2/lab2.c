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
#define USER_LAST_ROW 23

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
  for (col = 0 ; col < last_col ; col++) {
    fbputchar('*', SERVER_FIRST_ROW - 1, col);
    fbputchar('*', 23, col);
    fbputchar('-', SERVER_LAST_ROW  + 1, col);
  }

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
  int user_col = 0;

  int cursor = 0;
  int message_length = 0;
  char user_input[BUFFER_SIZE];

  // memset(user_input, ' ', BUFFER_SIZE);
  // user_input[BUFFER_SIZE - 1] = '\0';

  for (;;) {

    fbputs(user_input, USER_FIRST_ROW, 0);
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

      if (packet.keycode[0] == ENTER) {
        // send message to server
        write(sockfd, user_input, strlen(user_input));
        memset(user_input, ' ', BUFFER_SIZE);
        user_input[BUFFER_SIZE - 1] = '\0';
        cursor = 0;
        message_length = 0;
        user_row = USER_FIRST_ROW;
        user_col = 0;
      }
      else if (packet.keycode[0] == BACKSPACE) {
        user_input[cursor] = ' ';
        if (cursor > 0) {
          cursor--;
          if (user_col == 0) {
            user_col = last_col;
            user_row--;
          } else user_col--;
          
          message_length--;
          user_input[cursor] = ' ';
        }
      }
      else if (packet.keycode[0] == SPACE) {
        if (message_length < BUFFER_SIZE - 1) {
          if (user_col == last_col) {
            user_col = 0;
            user_row++;
          } else user_col++;

          user_input[cursor++] = ' ';
          message_length++;
        }
      }

      else if (packet.keycode[0] != EMPTY)
      {
        // VERY basic way to convert single keycode to char
        sprintf(temp_keystate, "%c", usb_to_ascii[packet.keycode[0]] + (IS_SHIFTED(packet.modifiers) ? 'A' - 'a': 0));
        //printf("%c\n", temp_keystate);

        if (message_length < BUFFER_SIZE - 1) {

          if (user_col == last_col) {
            user_col = 0;
            user_row++;
          } else user_col++;

          user_input[cursor++] = temp_keystate[0];
          message_length++;

          printf("user_input[%d]: %d\n", cursor-1, user_input[cursor-1]);
        }
      }

      // if (packet.keycode[0] == LEFT_ARROW) {
      //   if (cursor > 0) {
      //     cursor--;
      //     user_col--;
      //   }
      // }
      // if (packet.keycode[0] == RIGHT_ARROW) {
      //   if (cursor < message_length) {
      //     cursor++;
      //     user_col++;
      //   }
      // }
      // if (packet.keycode[0] == UP_ARROW) {
      //   if (user_row > USER_FIRST_ROW) {
      //     user_row--;
      //     cursor = cursor - (last_col  + 1); // move the cursor back the length of the row
      //   }
      // }
      // if (packet.keycode[0] == DOWN_ARROW) {
      //   if (user_row < USER_LAST_ROW && message_length > (last_col  + 1)) { // can only go down if there is a row to go down to
      //     user_row++;
      //     cursor = cursor + (last_col  + 1); // move the cursor forward the length of the row
      //   }
      // }

      if (packet.keycode[0] == 0x29) { /* ESC pressed? */
	      break;
      }

      /* RET pressed?
      if (packet.keycode[0] == 0x28) {
        // implement sending message here
      }
      */
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
  int last_row = getLastRow();
  int last_col = getLastCol();

  int row = SERVER_FIRST_ROW; // row in which messages begin on
  /* Receive data */
  while ( (n = read(sockfd, &recvBuf, BUFFER_SIZE - 1)) > 0 ) {
    recvBuf[n] = '\0';
    printf("%s\n", recvBuf);
    row = fbputs(recvBuf, row, 0)  + 1;
    // row = row + 1 + (strlen(recvBuf) / last_col);

    // when too many messages come through, clear all messages
    if(row == 20){
        // this has gotta be terribly inefficient, is there a better way?
        for (row = SERVER_FIRST_ROW; row <= SERVER_LAST_ROW; row++) {
          for (int col = 0 ; col < last_col ; col++) {
            fbputchar(' ', row, col);
          }
        }
      row = SERVER_FIRST_ROW;
    }
  }

  return NULL;
}

