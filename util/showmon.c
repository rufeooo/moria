
#define main stubmain
#include "../moria.c"
#undef main

#define CMP(field)                                \
  int cmp_##field(const void* lh, const void* rh) \
  {                                               \
    const creature_type* lt = lh;                 \
    const creature_type* rt = rh;                 \
    return lt->field - rt->field;                 \
  }
CMP(name);
CMP(cmove);
CMP(spells);
CMP(cdefense);
CMP(mexp);
CMP(sleep);
CMP(aaf);
CMP(ac);
CMP(speed);
CMP(cchar);
// CMP(hd[2]);
// CMP(damage[4]);
CMP(level);

#define INT_MAX 0x7fffffff
#define INT_MIN ~0x7fffffff
#define MIN(x, y) (x < y ? x : y)
#define MAX(x, y) (x > y ? x : y)
int
main()
{
  qsort(c_list, MAX_CREATURES, sizeof(creature_type), cmp_sleep);
  int min_sleep = INT_MAX;
  int max_sleep = INT_MIN;
  printf("sleep range [%d, %d]\n", min_sleep, max_sleep);
  printf("most wakeful (5)\n");
  for (int it = 0; it < 5; ++it) {
    creature_type* t = &c_list[it];

    printf("%s: %d\n", t->name, t->sleep);
    min_sleep = MIN(min_sleep, t->sleep);
    max_sleep = MAX(max_sleep, t->sleep);
  }
  printf("sleep range [%d, %d]\n", min_sleep, max_sleep);

  printf("median:\n");
  for (int it = MAX_CREATURES / 2 - 2; it < MAX_CREATURES / 2 + 2; ++it) {
    creature_type* t = &c_list[it];
    min_sleep = MIN(min_sleep, t->sleep);
    max_sleep = MAX(max_sleep, t->sleep);
    printf("%s: %d\n", t->name, t->sleep);
  }
  printf("sleep range [%d, %d]\n", min_sleep, max_sleep);

  printf("sleepiest (5):\n");
  for (int it = MAX_CREATURES - 6; it < MAX_CREATURES; ++it) {
    creature_type* t = &c_list[it];
    min_sleep = MIN(min_sleep, t->sleep);
    max_sleep = MAX(max_sleep, t->sleep);
    printf("%s: %d\n", t->name, t->sleep);
  }

  printf("sleep range [%d, %d]\n", min_sleep, max_sleep);
  return 0;
}
