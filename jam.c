#include "config.h"
#include "constant.h"
#include "types.h"

#include "externs.h"

#include <ncurses.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h> /* prototype for execl */

#include <sys/socket.h>
#include <sys/types.h>

#ifndef FEATURE
enum { FEATURE = -1 };
#endif
enum {
  ENABLE_INPUT = (1 << 0),
};

static struct termios save_termio;
static int curses_on = FALSE;

void
moriaterm()
{
  struct termios tbuf;

  curses_on = TRUE;
  crmode();
  noecho();

  ioctl(0, TCGETA, (char*)&tbuf);
  /* disable all of the normal special control characters */
  tbuf.c_cc[VINTR] = (char)3; /* control-C */
  tbuf.c_cc[VQUIT] = (char)-1;
  tbuf.c_cc[VERASE] = (char)-1;
  tbuf.c_cc[VKILL] = (char)-1;
  tbuf.c_cc[VEOF] = (char)-1;

  /* don't know what these are for */
  tbuf.c_cc[VEOL] = (char)-1;

  /* stuff needed when !icanon, i.e. cbreak/raw mode */
  tbuf.c_cc[VMIN] = 1;  /* Input should wait for at least 1 char */
  tbuf.c_cc[VTIME] = 0; /* no matter how long that takes. */

  ioctl(0, TCSETA, (char*)&tbuf);
}

void
init_curses()
{
  int i, y, x;

  ioctl(0, TCGETA, (char*)&save_termio);

  /* PC curses returns ERR */
  if (initscr() == NULL)
  {
    printf("Error allocating screen in curses package.\n");
    exit(1);
  }
  if (LINES < 24 || COLS < 80) /* Check we have enough screen. -CJS- */
  {
    printf("Screen too small for moria.\n");
    exit(1);
  }
  clear();
  refresh();
  moriaterm();
}

void
restore_term()
{
  if (!curses_on) return;
  refresh();
  /* this moves curses to bottom right corner */
  mvcur(stdscr->_cury, stdscr->_curx, LINES - 1, 0);
  endwin(); /* exit curses */
  fflush(stdout);
  /* restore the saved values of the special chars */
  ioctl(0, TCSETA, (char*)&save_termio);
  curses_on = FALSE;
}

char
inkey()
{
  int i;

  refresh(); /* Dump IO buffer  	*/
  while (TRUE) {
    i = getch();

    if (i != CTRL('R')) return (char)i;
    wrefresh(curscr);
    moriaterm();
  }
}

int
main()
{
  int s1 = socket(AF_INET, SOCK_DGRAM, 0);
  printf("%d\n", s1);
  init_curses();

  int row = 0, col = 0;
  while (FEATURE & ENABLE_INPUT) {
    char c = inkey();
    if (c == CTRL('X')) break;
    mvaddch(row, col, c);
    clrtoeol();
  }

  int s2 = socket(AF_INET, SOCK_DGRAM, 0);
  printf("%d\n", s2);
  restore_term();
  return 0;
}
