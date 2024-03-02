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
#include "displayconstants.h"

/* Update SERVER_HOST to be the IP address of
 * the chat server you are connecting to
 */
/* arthur.cs.columbia.edu */

/*
 * References:
 *
 * https://web.archive.org/web/20130307100215/http://beej.us/guide/bgnet/output/html/singlepage/bgnet.html
 *
 * http://www.thegeekstuff.com/2011/12/c-socket-programming/
 *
 */

#define SERVER_HOST "128.59.19.114"
#define SERVER_PORT 42000

#define BUFFER_SIZE 128
#define INPUT_LINES 2

int sockfd; /* Socket file descriptor */

int num_rows;
int num_cols;

struct libusb_device_handle *keyboard;
uint8_t endpoint_address;

pthread_t network_thread;
void *network_thread_f(void *);

void clear_user_input(char *user_input, int *cursor, int *message_length)
{
  *cursor = 0;
  *message_length = 0;
  memset(user_input, '\0', BUFFER_SIZE);
  fbclearrows(USER_FIRST_ROW, USER_LAST_ROW);
}

void render_user_input(char *user_input)
{
  fbclearrows(USER_FIRST_ROW, USER_LAST_ROW);
  fbputs(user_input, USER_FIRST_ROW, FIRST_COL);
}

int main()
{
  int err, col;

  struct sockaddr_in serv_addr;

  struct usb_keyboard_packet packet;
  int transferred;

  // testing var used for keyboard input
  char temp_keystate[1];

  if ((err = fbopen()) != 0)
  {
    fprintf(stderr, "Error: Could not open framebuffer: %d\n", err);
    exit(1);
  }

  num_rows = get_num_rows();
  num_cols = get_num_cols();

  int user_first_row = num_rows - (INPUT_LINES + 1);

  fbclear();

  /* Draw rows of asterisks across the top and bottom of the screen */
  // for (col = FIRST_COL; col < LAST_COL; col++)
  // {
  //   fbputchar('*', 0, col);
  //   fbputchar('*', num_rows - 1, col);
  //   fbputchar('-', user_first_row - 1, col);
  // }

  printf("%d\n", num_rows);
  printf("%d\n", num_cols);

  /* Open the keyboard */
  if ((keyboard = openkeyboard(&endpoint_address)) == NULL)
  {
    fprintf(stderr, "Did not find a keyboard\n");
    exit(1);
  }

  /* Create a TCP communications socket */
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    fprintf(stderr, "Error: Could not create socket\n");
    exit(1);
  }

  /* Get the server address */
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(SERVER_PORT);
  if (inet_pton(AF_INET, SERVER_HOST, &serv_addr.sin_addr) <= 0)
  {
    fprintf(stderr, "Error: Could not convert host IP \"%s\"\n", SERVER_HOST);
    exit(1);
  }

  /* Connect the socket to the server */
  if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
  {
    fprintf(stderr, "Error: connect() failed.  Is the server running?\n");
    exit(1);
  }

  /* Start the network thread */
  pthread_create(&network_thread, NULL, network_thread_f, NULL);

  /* Look for and handle keypresses */

  int cursor = 0;
  int message_length = 0;
  char user_input[BUFFER_SIZE];

  for (;;)
  {
    int cursor_row = (cursor / num_cols + user_first_row);
    int cursor_col = (cursor % num_cols);
    cursor_fbputchar((cursor >= message_length) ? ' ' : user_input[cursor], CURSOR_ROW(cursor), CURSOR_COL(cursor));

    libusb_interrupt_transfer(keyboard, endpoint_address,
                              (unsigned char *)&packet, sizeof(packet),
                              &transferred, 0);

    if (transferred == sizeof(packet))
    {
      // print incoming keyboard packet
      printf("%s\n", user_input);
      printf("%02x %02x %02x\n", packet.modifiers, packet.keycode[0], packet.keycode[1]);

      if (IS_CTRL(packet.modifiers))
      {
        if (mapCharacter(packet.keycode[0], IS_SHIFTED(packet.modifiers)) == 'u')
          clear_user_input(user_input, &cursor, &message_length);
          temp_keystate[0] = 0;
      }

      else if (packet.keycode[0] == LEFT_ARROW && cursor > 0)
      {
        cursor--;
        render_user_input(user_input);
        temp_keystate[0] = 0;
      }

      else if (packet.keycode[0] == RIGHT_ARROW && cursor < message_length)
      {
        cursor++;
        render_user_input(user_input);
          temp_keystate[0] = 0;
      }

      else if (packet.keycode[0] == ENTER)
      {
        write(sockfd, user_input, strlen(user_input)); // send message to server
        clear_user_input(user_input, &cursor, &message_length);
        temp_keystate[0] = 0;
      }

      else if (packet.keycode[0] == BACKSPACE && cursor > 0)
      {
        for (int i = cursor; i < message_length; i++)
          user_input[i - 1] = user_input[i];

        user_input[message_length--] = '\0';
        cursor--;

        render_user_input(user_input);
        temp_keystate[0] = 0;
      }

      else if (packet.keycode[0] == ESCAPE) {
        temp_keystate[0] = 0;
        break;
      }

      if (packet.keycode[0] == RELEASE)
      { // on RELEASE event, add temp_keystate to user_input
        if (message_length < BUFFER_SIZE - 1 && temp_keystate[0] != 0)
        {
          for (int i = message_length - 1; i >= cursor; i--)
            user_input[i + 1] = user_input[i];
          user_input[cursor++] = temp_keystate[0];
          user_input[++message_length] = '\0';

          render_user_input(user_input);
        }
        temp_keystate[0] = 0;
      }
      else                  // on PRESS event, update temp_keystate
          sprintf(temp_keystate, "%c", mapCharacter(packet.keycode[0], IS_SHIFTED(packet.modifiers)));
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

  int row = SERVER_FIRST_ROW;
  while ((n = read(sockfd, &recvBuf, BUFFER_SIZE - 1)) > 0)
  {
    recvBuf[n] = '\0';
    printf("%s\n", recvBuf);
    if ((strlen(recvBuf) / LINE_WIDTH) + row > SERVER_LAST_ROW)
    {
      fbclearrows(SERVER_FIRST_ROW, SERVER_LAST_ROW);
      row = SERVER_FIRST_ROW;
    }
    row = fbputs(recvBuf, row, FIRST_COL) + 1;
  }

  return NULL;
}
