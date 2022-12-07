
#define main stubmain
#include "../moria.c"
#undef main

#define CMP(field)                                \
  int cmp_##field(const void* lh, const void* rh) \
  {                                               \
    const treasure_type* lt = lh;                 \
    const treasure_type* rt = rh;                 \
    return lt->field - rt->field;                 \
  }
CMP(weight);

int
main()
{
  qsort(object_list, MAX_OBJECTS, sizeof(treasure_type), cmp_weight);
  for (int it = 0; it < MAX_OBJECTS; ++it) {
    treasure_type* t = &object_list[it];

    uint8_t tval = t->tval;
    if (tval == TV_HAFTED || tval == TV_POLEARM || tval == TV_SWORD) {
      printf("%3d  %-31s     (%c) (%d.%d lbs) [%dd%d]\n", it, t->name, t->tchar,
             t->weight / 10, t->weight % 10, t->damage[0], t->damage[1]);
    }
    // int32u flags;    /* Special flags	*/
    // int8u tval;      /* Category number	*/
    // int16 p1;        /* Misc. use variable	*/
    // int32 cost;      /* Cost of item		*/
    // int8u subval;    /* Sub-category number	*/
    // int8u number;    /* Number of items	*/
    // int16u weight;   /* Weight		*/
    // int16 tohit;     /* Plusses to hit	*/
    // int16 todam;     /* Plusses to damage	*/
    // int16 ac;        /* Normal AC		*/
    // int16 toac;      /* Plusses to AC	*/
    // int8u damage[2]; /* Damage when hits	*/
    // int8u level;     /* Level item first found */
  }
  return 0;
}
