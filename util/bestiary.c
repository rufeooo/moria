#define main _main
#include "../build.c"
#undef main

#define AL(x) (sizeof(x) / sizeof(x[0]))
int
main()
{
  init_scorefile();
  init_curses();
  init_signals();
  fileD = fopen("bestiary.txt", "wb");

  py.misc.lev = 1;
  wizard = 1;
  for (int it = 0; it < AL(c_list); ++it) {
    clear_screen();
    roff_recall(it);
  }
  printf("%ju creatures written to bestiary.txt\n", AL(c_list));

  fclose(fileD);
  exit_game();
  return 0;
}
