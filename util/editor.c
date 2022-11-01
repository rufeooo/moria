// Character Save File Editor
#define main stubmain
#include "../moria.c"
#undef main

int
main()
{
  init_scorefile();
  init_curses();
  panic_save = 1;
  char* p = getenv("HOME");
  sprintf(savefile, "%s/%s", p, MORIA_SAV);
  int generate;
  get_char(&generate);

  if (inventory[INVEN_HANDS].name2) {
    puts(special_names[inventory[INVEN_HANDS].name2]);
    inventory[INVEN_HANDS].tohit = 2;
    inventory[INVEN_HANDS].todam = 3;
    save_char();
  } else {
    msg_print("guess not");
  }
  inkey();

  restore_term();

  return 0;
}
