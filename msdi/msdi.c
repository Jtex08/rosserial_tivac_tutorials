/*Texas A&M University
**Electronic Systems Engineering Technology
**ESET 420 Capstone II
**Author: Jonathan Noland
**File: msdi.c
**------------------------------------------------------------------------------
**This file contains the functions to use the TIC-10024-Q1 
*/

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/ssi.h"
#include "msdi.h"


/*
**Private Functions
*/

/*TIVA specific SPI transfer function
**Parameters:
**send_data: Read write bit set,the register, data, and parity bit to be sent.
**rcv_data: Pointer to data to be stored before it is parsed
**Returns status
 */
//static msdi_status_t MSDI_DATA_TRANSFER(
void MSDI_DATA_TRANSFER(
	const uint32_t send_data[NUM_SSI_DATA], uint32_t* const rcv_data[NUM_SSI_DATA]);

/*Function: MSDI_WRITE
**Prepares data to be sent
**Parameters:
**          Register name data is to be written to
**          Register setting 
*/
void MSDI_WRITE(uint32_t uiRegister,uint32_t uiSetting);

/*Function Name: MSDI_PARITY
**Description: Checks raw data for odd parity
**Parameters: raw data to be checked
**Returns: 0 if parity bit doesn't need to be set, 1 if parity bit needs to be set
*/
static msdi_parity_t MSDI_PARITY(uint32_t raw_val);
/*
**Private Functions Code
*/

/*Function Name: MSDI_DATA_TRANSFER
**Description: This function transfers the upper and lower 32 bits of data while simultaneously reading MISO bits
*/
void MSDI_DATA_TRANSFER(
	const uint32_t send_data[NUM_SSI_DATA], uint32_t* const rcv_data[NUM_SSI_DATA])
{
//msdi_status_t status = MSDI_STATUS_SUCCESS;

//CS low
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0);

//Send and recieve data at the same time
//for(ui32Index = 0; ui32Index < NUM_SSI_DATA; ui32Index++)
//  SSIDataPut(SSI0_BASE, ui32DataTx[ui32Index]);
    SSIDataPut(SSI0_BASE, send_data[0]);
    SSIDataGet(SSI0_BASE, &rcv_data[0]);
    while(SSIBusy(SSI0_BASE))
    {
    }

//}
    SSIDataPut(SSI0_BASE, send_data[1]);
    SSIDataGet(SSI0_BASE,&rcv_data[1]);
    while(SSIBusy(SSI0_BASE))
    {
    }
//}

 //cs high
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, GPIO_PIN_3);

 //   return MSDI_STATUS_SUCCESS;
}

/*Function Name: MSDI_WRITE
**Description: Prepares data to be sent to write to a register
*/
void MSDI_WRITE(uint32_t uiRegister,uint32_t uiSetting);
{
    uint32_t ui32DataTx[NUM_SSI_DATA];
    uint32_t pui32DataRx[NUM_SSI_DATA];

    uint32_t raw_val = (MSDI_REG_RW_W | (uiRegister<<1) | (uiSetting<<1));

    msdi_parity_t status = MSDI_PARITY(raw_val);

    if (status == SET_PARITY_BIT)
    {
        raw_val = raw_val | SET_PARITY_BIT_MASK;

    }

    ui32DataTx[0] = raw_val>>16;
    ui32DataTx[1] = raw_val & LOWER_TRANSFER_MASK;

    MSDI_DATA_TRANSFER(ui32DataTx,)//NEED TO FINISH need to pass an address from the top calling function to where the data will be stored






}
/*Function Name: MSDI_PARITY
**Description: Checks raw data to see if the parity bit needs to be set
*/
msdi_parity_t MSDI_PARITY(uint32_t raw_val)
{
    //unsigned int v = 0x11011000; // 32-bit word
    raw_val ^= raw_val >> 1;
    raw_val ^= raw_val >> 2;
    raw_val = (raw_val & 0x11111111U) * 0x11111111U;
    unsigned int x = (raw_val >> 28) & 1;
    
    if(x==1)
    {
        return LEAVE_PARITY_BIT;
    }
    else if (x==0)
    {
        return SET_PARITY_BIT;
    }   

}