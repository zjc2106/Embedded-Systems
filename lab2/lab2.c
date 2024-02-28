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
    fbputchar('*', 0, col);
    fbputchar('*', 23, col);
    fbputchar('-', 20, col);
  }

  fbputs("Hello CSEE 4840 World!", 4, 10);

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
  for (;;) {
    libusb_interrupt_transfer(keyboard, endpoint_address,
			      (unsigned char *) &packet, sizeof(packet),
			      &transferred, 0);
    if (transferred == sizeof(packet)) {
      sprintf(keystate, "%02x %02x %02x", packet.modifiers, packet.keycode[0],
	      packet.keycode[1]);
      printf("%s\n", keystate);
      fbputs(keystate, 21, 0);

      // VERY basic way to convert single keycode to char
      sprintf(temp_keystate, "%c", usb_to_ascii[packet.keycode[0]]);
      //printf("%c\n", temp_keystate);
      fbputs(temp_keystate, 22, 0);


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

  int row = 5; // row in which messages begin on
  /* Receive data */
  while ( (n = read(sockfd, &recvBuf, BUFFER_SIZE - 1)) > 0 ) {
    recvBuf[n] = '\0';
    printf("%s", recvBuf);
    fbputs(recvBuf, row, 0);
    row = row + 1;


    // when too many messages come through, clear all messages
    if(row == 20){
        // this has gotta be terribly inefficient, is there a better way?
        for (row = 5; row < 20; row++) {
          for (int col = 0 ; col < last_col ; col++) {
            fbputchar(' ', row, col);
          }
        }
      row = 5;
    }
  }

  return NULL;
}

