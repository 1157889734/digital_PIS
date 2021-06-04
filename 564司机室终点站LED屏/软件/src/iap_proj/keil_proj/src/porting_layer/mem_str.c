#include "includes.h"

/*
*********************************************************************************************************
*
* Description: clear ram
*
* Arguments  : pdest    is the start of the RAM to clear (i.e. write 0x00 to)
*                    size     is the number of bytes to clear.
*
* Returns    : none
*
* Notes      : 
*********************************************************************************************************
*/
void  mem_clr(uint8 *pdest, uint16 size)
{
    while (size > 0) {
        *pdest++ = (uint8)0;
        size--;
    }
}

void  mem_set(uint8 *pdest, uint8 ch, uint16 size)
{
    while (size > 0) {
        *pdest++ = ch;
        size--;
    }
}


/*
*********************************************************************************************************
*
* Description: copy ram
*
* Arguments  : pdest    is a pointer to the 'destination' memory block
*
*              psrc     is a pointer to the 'source'      memory block
*
*              size     is the number of bytes to copy.
*
* Returns    : none
*
* Notes      : 
*********************************************************************************************************
*/
void  mem_cpy(uint8 *pdest, uint8 *psrc, uint16 size)
{
    while (size > 0) {
        *pdest++ = *psrc++;
        size--;
    }
}

