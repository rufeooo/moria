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

static struct termios save_termio;
static int curses_on = FALSE;

void
moriaterm()
{
#if !defined(MSDOS) && !defined(ATARI_ST) && !defined(VMS)
#ifndef AMIGA
#ifdef USG
#ifdef __linux__
  struct termios tbuf;
#else
  struct termio tbuf;
#endif
#else
  struct ltchars lbuf;
  struct tchars buf;
#endif
#endif
#endif

  curses_on = TRUE;
#ifndef BSD4_3
  crmode();
#else
#ifdef VMS
  vms_crmode();
#else
  cbreak();
#endif
#endif
  noecho();
  /* can not use nonl(), because some curses do not handle it correctly */
#ifdef MSDOS
  msdos_raw();
#else
#ifdef AMIGA
  init_color(0, 0, 0, 0);          /* pen 0 - black */
  init_color(1, 1000, 1000, 1000); /* pen 1 - white */
  init_color(2, 0, 300, 700);      /* pen 2 - blue */
  init_color(3, 1000, 500, 0);     /* pen 3 - orange */
#else
#if !defined(ATARI_ST) && !defined(VMS)
#ifdef USG
  (void)ioctl(0, TCGETA, (char*)&tbuf);
  /* disable all of the normal special control characters */
  tbuf.c_cc[VINTR] = (char)3; /* control-C */
  tbuf.c_cc[VQUIT] = (char)-1;
  tbuf.c_cc[VERASE] = (char)-1;
  tbuf.c_cc[VKILL] = (char)-1;
  tbuf.c_cc[VEOF] = (char)-1;

  /* don't know what these are for */
  tbuf.c_cc[VEOL] = (char)-1;
#ifdef VEOL2
  tbuf.c_cc[VEOL2] = (char)-1;
#endif

  /* stuff needed when !icanon, i.e. cbreak/raw mode */
  tbuf.c_cc[VMIN] = 1;  /* Input should wait for at least 1 char */
  tbuf.c_cc[VTIME] = 0; /* no matter how long that takes. */

  (void)ioctl(0, TCSETA, (char*)&tbuf);
#else
  /* disable all of the special characters except the suspend char, interrupt
     char, and the control flow start/stop characters */
  (void)ioctl(0, TIOCGLTC, (char*)&lbuf);
  lbuf.t_suspc = (char)26; /* control-Z */
  lbuf.t_dsuspc = (char)-1;
  lbuf.t_rprntc = (char)-1;
  lbuf.t_flushc = (char)-1;
  lbuf.t_werasc = (char)-1;
  lbuf.t_lnextc = (char)-1;
  (void)ioctl(0, TIOCSLTC, (char*)&lbuf);

  (void)ioctl(0, TIOCGETC, (char*)&buf);
  buf.t_intrc = (char)3; /* control-C */
  buf.t_quitc = (char)-1;
  buf.t_startc = (char)17; /* control-Q */
  buf.t_stopc = (char)19;  /* control-S */
  buf.t_eofc = (char)-1;
  buf.t_brkc = (char)-1;
  (void)ioctl(0, TIOCSETC, (char*)&buf);
#endif
#endif
#endif
#endif

#ifdef ATARIST_TC
  raw();
#endif
}

void
init_curses()
{
  int i, y, x;

#ifndef USG
  (void)ioctl(0, TIOCGLTC, (char*)&save_special_chars);
  (void)ioctl(0, TIOCGETP, (char*)&save_ttyb);
  (void)ioctl(0, TIOCGETC, (char*)&save_tchars);
#if !defined(atarist) && !defined(__GNUC__)
  (void)ioctl(0, TIOCLGET, (char*)&save_local_chars);
#endif
#else
#if !defined(VMS) && !defined(MSDOS) && !defined(ATARI_ST)
#ifndef AMIGA
  (void)ioctl(0, TCGETA, (char*)&save_termio);
#endif
#endif
#endif

  /* PC curses returns ERR */
#if defined(USG) && !defined(PC_CURSES) && !defined(AMIGA)
  if (initscr() == NULL)
#else
  if (initscr() == ERR)
#endif
  {
    (void)printf("Error allocating screen in curses package.\n");
    exit(1);
  }
  if (LINES < 24 || COLS < 80) /* Check we have enough screen. -CJS- */
  {
    (void)printf("Screen too small for moria.\n");
    exit(1);
  }
#ifdef SIGTSTP
#if defined(atarist) && defined(__GNUC__)
  (void)signal(SIGTSTP, (__Sigfunc)suspend);
#else
#ifdef __386BSD__
  (void)signal(SIGTSTP, (sig_t)suspend);
#else
  (void)signal(SIGTSTP, (void*)suspend);
#endif
#endif
#endif
  (void)clear();
  (void)refresh();
  moriaterm();
}

void
restore_term()
{
#ifdef AMIGA
  closetimer();
#endif

  if (!curses_on) return;
  (void)refresh();
#ifdef MSDOS
  (void)sleep(2); /* And let it be read. */
#endif
#ifdef VMS
  clear_screen();
  pause_line(15);
#endif
  /* this moves curses to bottom right corner */
  mvcur(stdscr->_cury, stdscr->_curx, LINES - 1, 0);
  endwin(); /* exit curses */
  (void)fflush(stdout);
#ifdef MSDOS
  msdos_noraw();
#endif
  /* restore the saved values of the special chars */
#ifdef USG
#if !defined(MSDOS) && !defined(ATARI_ST) && !defined(VMS)
#ifndef AMIGA
  (void)ioctl(0, TCSETA, (char*)&save_termio);
#endif
#endif
#else
  (void)ioctl(0, TIOCSLTC, (char*)&save_special_chars);
  (void)ioctl(0, TIOCSETP, (char*)&save_ttyb);
  (void)ioctl(0, TIOCSETC, (char*)&save_tchars);
#if !defined(atarist) && !defined(__GNUC__)
  (void)ioctl(0, TIOCLSET, (char*)&save_local_chars);
#endif
#endif
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
    (void)wrefresh(curscr);
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
  while (true) {
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
