 
#include "../ins/main.h"   
 
#if 1
 
typedef struct
{
    uint16_t cnt;
    uint16_t led_cnt; 
} sLvM_TmCntType;
   
    
//==============================
 
extern void LvM_InitCbk(void);
extern void LvM_CountCbk(void);
extern void LvM_MainCbk(void);

#endif