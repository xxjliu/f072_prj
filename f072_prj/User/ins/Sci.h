  
#include "../ins/main.h"   
 
#if 1
 
typedef struct
{
    uint16_t cnt; 
} sSci_TmCntType;
#define SCI_SEND_DATA_LEN   128   
typedef struct
{
    uint8_t cnt; 
    uint8_t len; 
    uint8_t data[SCI_SEND_DATA_LEN]; 
} sSci_SendDataType;
    
//==============================
 
extern void Sci_SendByteCbk(void);
extern void Sci_InitCbk(void);
extern void Sci_CountCbk(void);
extern void Sci_MainCbk(void);

#endif