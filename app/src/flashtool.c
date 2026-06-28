#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include "Flashtool.h"

LOG_MODULE_REGISTER(flashtool, CONFIG_EC_LOG_LEVEL);

#define SYSTEM_BASE 0x40020000UL
#define GPIO_BASE   0x40090000UL
#define SPIC_BASE   0x40010200UL
#define WDT_BASE    0x4000C000UL
#define MCM_BASE    0x40091000UL
#define SRAM2_BASE  0x2003FF00UL
#define SRAM2_USAGE 0x40UL

#define EC_OK 0x0
#define EC_FAIL 0x1
#define EC_DEVICE_BUSY 0x2

#define IO_Write32(_addr_, _wdata_) \
  (*((volatile unsigned int *)(_addr_)) = (_wdata_))
 
#define IO_Write8(_addr_, _wdata_) \
  (*((volatile unsigned char *)(_addr_)) = (_wdata_))

#define IO_Read32(_addr_) \
  (*((volatile unsigned int *)(_addr_)))

#define IO_Read8(_addr_) \
  (*((volatile unsigned char *)(_addr_)))

void EnterFlashMode(void);
void SelectInternalFlash(void);
void SelectExternalFlash(void);
void SPIC_UserMode(void);
uint8_t SPIC_Erase(uint32_t Address);
void SPIC_AutoMode(void);
void ExitFlashMode(void);
void WDTReset(void);

void flashtool_thread(void *p1, void *p2, void *p3)
{
    uint32_t period = *(uint32_t *)p1;
    uint8_t ret = 0;
    uint8_t erase_times;
    uint32_t Address;
    uint8_t AddrHH, AddrH, AddrM, AddrL;
    uint8_t spic_freq;

    LOG_INF("init flashtool thread");

    for(uint8_t i=0; i<SRAM2_USAGE; i+=4){
        IO_Write32((SRAM2_BASE + i), 0x00000000);
    }

    while(1){
        if(IO_Read8(SRAM2_BASE + 0x00) == 0x00){
            k_msleep(period);
            continue;
        }

        if(IO_Read8(SRAM2_BASE + 0x04) == 0x01){
            EnterFlashMode();
            IO_Write32((SRAM2_BASE + 0x04), 0xA5A5A5A5);
        }

        if(IO_Read8(SRAM2_BASE + 0x08) == 0x01){
            ExitFlashMode();
            for(uint8_t i=4; i<SRAM2_USAGE; i+=4){
                IO_Write32((SRAM2_BASE + i), 0x00000000);
            }
            IO_Write32((SRAM2_BASE + 0x08), 0xA5A5A5A5);
        }

        if(IO_Read8(SRAM2_BASE + 0x0C) == 0x01){
            SelectInternalFlash();
            SPIC_AutoMode();
            IO_Write32((SRAM2_BASE + 0x0C), 0xA5A5A5A5);
        }

        if(IO_Read8(SRAM2_BASE + 0x0C) == 0x02){
            LOG_DBG("Select External Flash");
            SelectExternalFlash();
            SPIC_AutoMode();
            IO_Write32((SRAM2_BASE + 0x0C), 0xA5A5A5A5);
        }
        
        if(IO_Read8(SRAM2_BASE + 0x20) == 0x01){
            spic_freq = IO_Read8(SRAM2_BASE + 0x30);
            erase_times = IO_Read8(SRAM2_BASE + 0x2C);
            AddrHH = IO_Read8(SRAM2_BASE + 0x10);
            AddrH = IO_Read8(SRAM2_BASE + 0x14);
            AddrM = IO_Read8(SRAM2_BASE + 0x18);
            AddrL = IO_Read8(SRAM2_BASE + 0x1C);
            Address = ((AddrHH << 24) | (AddrH << 16) | (AddrM << 8) | (AddrL << 0));
            /* SPIC->BAUDR_b.SCKDV = spic_freq; */
            IO_Write32(SPIC_BASE + 0x14, spic_freq);
            for(uint8_t i=0; i<erase_times; i++){
                ret = SPIC_Erase(Address);
                Address += 0x10000;
                if(ret){
                    IO_Write32((SRAM2_BASE + 0x20), 0x00000000);
                    IO_Write32((SRAM2_BASE + 0x28), 0x02020202); // Fail
                    break;
                }
            }
            if(!ret){
                IO_Write32((SRAM2_BASE + 0x20), 0x00000000);
                IO_Write32((SRAM2_BASE + 0x28), 0x01010101); // Success
            }
        }

        if(IO_Read8(SRAM2_BASE + 0x24) == 0x01){
            LOG_DBG("WDT Reset\n");
            WDTReset();
        }

        if(IO_Read8(SRAM2_BASE + 0x34) == 0x01){
            LOG_DBG("SPIC User Mode\n");
            SPIC_UserMode();
            IO_Write32((SRAM2_BASE + 0x34), 0xA5A5A5A5);
        }

        if(IO_Read8(SRAM2_BASE + 0x34) == 0x02){
            LOG_DBG("SPIC Auto Mode\n");
            SPIC_AutoMode();
            IO_Write32((SRAM2_BASE + 0x34), 0xA5A5A5A5);
        }

        k_msleep(period);
    }

}

void EnterFlashMode()
{
    IO_Write32(SPIC_BASE + 0x08, 0x00000000); // SPIC->SSIENR = 0;
    IO_Write32(SPIC_BASE + 0x10, 0x00000001); // SPIC->SER_b.SEL = 1;
    IO_Write32(SPIC_BASE + 0x14, 0x00000002); // SPIC->BAUDR_b.SCKDV = 2;
}

void SelectInternalFlash()
{
    /*
    GPIO_BASE->GCR_b[106].INDETEN = 1;
    GPIO_BASE->GCR_b[106].MFCTRL = 0;
    */
    //IO_Write32(GPIO_BASE + 0x1A8, 0x00008002);
    *(uint32_t *)(GPIO_BASE + 0x1A8) |= (0x1 << 1);
    *(uint32_t *)(GPIO_BASE + 0x1A8) &= ~(0x7 << 8);

    /*
    GPIO_BASE->GCR_b[107].INDETEN = 1;
    GPIO_BASE->GCR_b[107].MFCTRL = 0;
    */
    //IO_Write32(GPIO_BASE + 0x1AC, 0x00008002);
    *(uint32_t *)(GPIO_BASE + 0x1AC) |= (0x1 << 1);
    *(uint32_t *)(GPIO_BASE + 0x1AC) &= ~(0x7 << 8);

    /*
    GPIO_BASE->GCR_b[108].INDETEN = 1;
    GPIO_BASE->GCR_b[108].MFCTRL = 0;
    */
    //IO_Write32(GPIO_BASE + 0x1B0, 0x00008002);
    *(uint32_t *)(GPIO_BASE + 0x1B0) |= (0x1 << 1);
    *(uint32_t *)(GPIO_BASE + 0x1B0) &= ~(0x7 << 8);

    /*
    GPIO_BASE->GCR_b[109].INDETEN = 1;
    GPIO_BASE->GCR_b[109].MFCTRL = 0;
    */
    //IO_Write32(GPIO_BASE + 0x1B4, 0x00008002);
    *(uint32_t *)(GPIO_BASE + 0x1B4) |= (0x1 << 1);
    *(uint32_t *)(GPIO_BASE + 0x1B4) &= ~(0x7 << 8);

    /*
    GPIO_BASE->GCR_b[111].INDETEN = 1;
    GPIO_BASE->GCR_b[111].MFCTRL = 0;
    */
    //IO_Write32(GPIO_BASE + 0x1BC, 0x00008002);
    *(uint32_t *)(GPIO_BASE + 0x1BC) |= (0x1 << 1);
    *(uint32_t *)(GPIO_BASE + 0x1BC) &= ~(0x7 << 8);

    /*
    GPIO_BASE->GCR_b[122].INDETEN = 1;
    GPIO_BASE->GCR_b[122].MFCTRL = 0;
    */
    //IO_Write32(GPIO_BASE + 0x1E8, 0x00008002);
    *(uint32_t *)(GPIO_BASE + 0x1E8) |= (0x1 << 1);
    *(uint32_t *)(GPIO_BASE + 0x1E8) &= ~(0x7 << 8);

    /*
    GPIO_BASE->GCR_b[124].INDETEN = 1;
    GPIO_BASE->GCR_b[124].MFCTRL = 0;
    */
    //IO_Write32(GPIO_BASE + 0x1F0, 0x00008002);
    *(uint32_t *)(GPIO_BASE + 0x1F0) |= (0x1 << 1);
    *(uint32_t *)(GPIO_BASE + 0x1F0) &= ~(0x7 << 8);

    IO_Write32(MCM_BASE, 0x00008001);
    IO_Write32(MCM_BASE + 0x04, 0x00000001);
    IO_Write32(MCM_BASE + 0x08, 0x00000001);
    IO_Write32(MCM_BASE + 0x0C, 0x00000001);  
    IO_Write32(MCM_BASE + 0x10, 0x00000001);   
    IO_Write32(MCM_BASE + 0x14, 0x00000001);
}

void SelectExternalFlash()
{
    /*
    GPIO_BASE->GCR_b[106].INDETEN = 1;
    GPIO_BASE->GCR_b[106].MFCTRL = 0;
    */
    // IO_Write32(GPIO_BASE + 0x1A8, 0x00008002);
    *(uint32_t *)(GPIO_BASE + 0x1A8) |= (0x1 << 1);
    *(uint32_t *)(GPIO_BASE + 0x1A8) &= ~(0x7 << 8);
    /*
    GPIO_BASE->GCR_b[107].INDETEN = 1;
    GPIO_BASE->GCR_b[107].MFCTRL = 1;
    */
    // IO_Write32(GPIO_BASE + 0x1AC, 0x00008102);

    /*
    GPIO_BASE->GCR_b[108].INDETEN = 1;
    GPIO_BASE->GCR_b[108].MFCTRL = 1;
    */
    // IO_Write32(GPIO_BASE + 0x1B0, 0x00008102);
    *(uint32_t *)(GPIO_BASE + 0x1B0) |= (0x1 << 1);
    *(uint32_t *)(GPIO_BASE + 0x1B0) &= ~(0x7 << 8);
    *(uint32_t *)(GPIO_BASE + 0x1B0) |= (0x1 << 8);
    /*
    GPIO_BASE->GCR_b[109].INDETEN = 1;
    GPIO_BASE->GCR_b[109].MFCTRL = 1;
    */
    // IO_Write32(GPIO_BASE + 0x1B4, 0x00008102);
    *(uint32_t *)(GPIO_BASE + 0x1B4) |= (0x1 << 1);
    *(uint32_t *)(GPIO_BASE + 0x1B4) &= ~(0x7 << 8);
    *(uint32_t *)(GPIO_BASE + 0x1B4) |= (0x1 << 8);
    /*
    GPIO_BASE->GCR_b[111].INDETEN = 1;
    GPIO_BASE->GCR_b[111].MFCTRL = 1;
    */
    // IO_Write32(GPIO_BASE + 0x1BC, 0x00008102);
    *(uint32_t *)(GPIO_BASE + 0x1BC) |= (0x1 << 1);
    *(uint32_t *)(GPIO_BASE + 0x1BC) &= ~(0x7 << 8);
    *(uint32_t *)(GPIO_BASE + 0x1BC) |= (0x1 << 8);
    /*
    GPIO_BASE->GCR_b[122].INDETEN = 1;
    GPIO_BASE->GCR_b[122].MFCTRL = 0;
    */
    // IO_Write32(GPIO_BASE + 0x1E8, 0x00008002);
    *(uint32_t *)(GPIO_BASE + 0x1E8) |= (0x1 << 1);
    *(uint32_t *)(GPIO_BASE + 0x1E8) &= ~(0x7 << 8);
    *(uint32_t *)(GPIO_BASE + 0x1E8) |= (0x1 << 8);
    /*
    GPIO_BASE->GCR_b[124].INDETEN = 1;
    GPIO_BASE->GCR_b[124].MFCTRL = 0;
    */
    // IO_Write32(GPIO_BASE + 0x1F0, 0x00008002);
    *(uint32_t *)(GPIO_BASE + 0x1F0) |= (0x1 << 1);
    *(uint32_t *)(GPIO_BASE + 0x1F0) &= ~(0x7 << 8);
    *(uint32_t *)(GPIO_BASE + 0x1F0) |= (0x1 << 8);
}

void SPIC_UserMode(void)
{
    IO_Write32(SPIC_BASE, 0x8100031F);
}

uint8_t SPIC_Erase(uint32_t Address)
{
    uint8_t getSPICBUSY;
    uint8_t getSPICEN;
    uint8_t getStatus;
    uint32_t retry_cnt;

/* SPIC_Exit4bytes */
    
    /*
    if(SPIC->SR_b.BUSY) {
		return EC_DEVICE_BUSY;
	}
    */
    getSPICBUSY = IO_Read8(SPIC_BASE + 0x28);
    if((getSPICBUSY & 0x1) == 1){
        return EC_DEVICE_BUSY;
    }

    /* SPIC_BASE->SSIENR = 0x00000000; */
    IO_Write32(SPIC_BASE + 0x08, 0x00000000);

    /*
    SPIC_BASE->CTRL0_b.USERMODE = 1; // User Mode 
	SPIC_BASE->CTRL0_b.TMOD = 0; // Trans mode
	SPIC_BASE->CTRL0_b.DATACH = 0; // Single Channel for Data channel
	SPIC_BASE->CTRL0_b.ADDRCH = 0; // Single Channel for Address channel
	SPIC_BASE->CTRL0_b.CMDCH = 0; //Single Channel for Command channel
    */
    IO_Write32(SPIC_BASE, 0x8100001F);

    /*
	SPIC_BASE->USERLENGTH_b.CMDLEN = 1; // Set Command Length
	SPIC_BASE->USERLENGTH_b.ADDRLEN = 0; // Set Address Length(Default 3)
    SPIC_BASE->USERLENGTH_b.RDDUMMYLEN = 0;
    */
    IO_Write32(SPIC_BASE + 0x118, 0x00001000);

    /* SPIC_BASE->TXNDF = 0x00000000; // Set Write data length */
    IO_Write32(SPIC_BASE + 0x130, 0x00000000);

    /* SPIC_BASE->RXNDF = 0x00000000; // Set Read data length */
    IO_Write32(SPIC_BASE + 0x04, 0x00000000);
    
	/* SPIC_BASE->DR_BYTE = 0xE9; */
    IO_Write8(SPIC_BASE + 0x60, 0xE9);

    /* SPIC_BASE->SSIENR_b.SPICEN = 1; // Start trans */
    IO_Write8(SPIC_BASE + 0x08, 0x01);

    /*
    uint32_t retry_cnt = 800000;
	while((SPIC->SSIENR_b.SPICEN == 1) && (retry_cnt--));
	return retry_cnt == 0;
    */
    retry_cnt = 800000;
    do{
        getSPICEN = IO_Read8(SPIC_BASE + 0x08);
        retry_cnt--;
    }while(((getSPICEN & 0x1) != 0) && (retry_cnt));

    if(retry_cnt == 0){
        /* SPIC->FLUSH = 1 */
        IO_Write8(SPIC_BASE + 0x128, 0x01);
        /* SPIC_BASE->SSIENR_b.SPICEN = 0; */
        IO_Write8(SPIC_BASE + 0x08, 0x00);
        return EC_FAIL;
    }

    /* SPIC_BASE->SSIENR_b.SPICEN = 0; */
    IO_Write32(SPIC_BASE + 0x08, 0x00);

/* SPIC_WriteEn */

    /*
    if(SPIC->SR_b.BUSY) {
		return EC_DEVICE_BUSY;
	}
    */
    getSPICBUSY = IO_Read8(SPIC_BASE + 0x28);
    if((getSPICBUSY & 0x1) == 1){
        return EC_DEVICE_BUSY;
    }

    /* SPIC_BASE->SSIENR = 0x00000000; */
    IO_Write32(SPIC_BASE + 0x08, 0x00000000);

    /*
    SPIC_BASE->CTRL0_b.USERMODE = 1; // User Mode 
	SPIC_BASE->CTRL0_b.TMOD = 0; // Trans mode
	SPIC_BASE->CTRL0_b.DATACH = 0; // Single Channel for Data channel
	SPIC_BASE->CTRL0_b.ADDRCH = 0; // Single Channel for Address channel
	SPIC_BASE->CTRL0_b.CMDCH = 0; //Single Channel for Command channel
    */
    IO_Write32(SPIC_BASE, 0x8100001F);

    /*
	SPIC_BASE->USERLENGTH_b.CMDLEN = 1; // Set Command Length
	SPIC_BASE->USERLENGTH_b.ADDRLEN = 0; // Set Address Length(Default 3)
    SPIC_BASE->USERLENGTH_b.RDDUMMYLEN = 0;
    */
    IO_Write32(SPIC_BASE + 0x118, 0x00001000);

    /* SPIC_BASE->TXNDF = 0x00000000; // Set Write data length */
    IO_Write32(SPIC_BASE + 0x130, 0x00000000);

    /* SPIC_BASE->RXNDF = 0x00000000; // Set Read data length */
    IO_Write32(SPIC_BASE + 0x04, 0x00000000);
    
	/* SPIC_BASE->DR_BYTE = 0x06; // Write Enable Command */
    IO_Write8(SPIC_BASE + 0x60, 0x06);
    
    /* SPIC_BASE->SSIENR_b.SPICEN = 1; // Start trans */
    IO_Write8(SPIC_BASE + 0x08, 0x01);

    /*
    uint32_t retry_cnt = 800000;
	while((SPIC->SSIENR_b.SPICEN == 1) && (retry_cnt--));
	return retry_cnt == 0;
    */
    retry_cnt = 800000;
    do{
        getSPICEN = IO_Read8(SPIC_BASE + 0x08);
        retry_cnt--;
    }while(((getSPICEN & 0x1) != 0) && (retry_cnt));

    if(retry_cnt == 0){
        /* SPIC->FLUSH = 1 */
        IO_Write8(SPIC_BASE + 0x128, 0x01);
        /* SPIC_BASE->SSIENR_b.SPICEN = 0; */
        IO_Write8(SPIC_BASE + 0x08, 0x00);
        return EC_FAIL;
    }

    /* SPIC_BASE->SSIENR_b.SPICEN = 0; */
    IO_Write8(SPIC_BASE + 0x08, 0x00);

	while(1) {
/* SPIC_FlashStatus */

        /*
        if(SPIC->SR_b.BUSY) {
            return EC_DEVICE_BUSY;
        }
        */
        getSPICBUSY = IO_Read8(SPIC_BASE + 0x28);
        if((getSPICBUSY & 0x1) == 1){
            return EC_DEVICE_BUSY;
        }

        /* SPIC_BASE->SSIENR = 0x00000000; */
        IO_Write32(SPIC_BASE + 0x08, 0x00000000);

        /*
        SPIC_BASE->CTRL0_b.USERMODE = 1; // User Mode 
        SPIC_BASE->CTRL0_b.TMOD = 3; // Trans mode
        SPIC_BASE->CTRL0_b.DATACH = 0; // Single Channel for Data channel
        SPIC_BASE->CTRL0_b.ADDRCH = 0; // Single Channel for Address channel
        SPIC_BASE->CTRL0_b.CMDCH = 0; //Single Channel for Command channel
        */
        IO_Write32(SPIC_BASE, 0x8100031F);

        /*
        SPIC_BASE->USERLENGTH_b.CMDLEN = 1; // Set Command Length
        SPIC_BASE->USERLENGTH_b.ADDRLEN = 0; // Set Address Length(Default 3)
        SPIC_BASE->USERLENGTH_b.RDDUMMYLEN = 0;
        */
        IO_Write32(SPIC_BASE + 0x118, 0x00001000);

        /* SPIC_BASE->TXNDF = 0x00000000; // Set Write data length */
        IO_Write32(SPIC_BASE + 0x130, 0x00000000);

        /* SPIC_BASE->RXNDF = 0x00000001; // Set Read data length */
        IO_Write32(SPIC_BASE + 0x04, 0x00000001);
        
        /* SPIC_BASE->DR_BYTE = 0x05; // Read Status-1 Command */
        IO_Write8(SPIC_BASE + 0x60, 0x05);

        /* SPIC_BASE->SSIENR_b.SPICEN = 1; // Start trans */
        IO_Write8(SPIC_BASE + 0x08, 0x01);

        /*
        uint32_t retry_cnt = 800000;
        while((SPIC->SSIENR_b.SPICEN == 1) && (retry_cnt--));
        return retry_cnt == 0;
        */
        retry_cnt = 800000;
        do{
            getSPICEN = IO_Read8(SPIC_BASE + 0x08);
            retry_cnt--;
        }while(((getSPICEN & 0x1) != 0) && (retry_cnt));

        if(retry_cnt == 0){
            /* SPIC->FLUSH = 1 */
            IO_Write8(SPIC_BASE + 0x128, 0x01);
            /*
            SPIC_BASE->SSIENR_b.SPICEN = 0;
            SPIC_BASE->CTRL0_b.TMOD = 0; // Trans mode
            */
            IO_Write8(SPIC_BASE + 0x08, 0x00);
            IO_Write32(SPIC_BASE, 0x8100001F);
            return EC_FAIL;
        }

        /*
        SPIC_BASE->SSIENR_b.SPICEN = 0;
        */
        IO_Write8(SPIC_BASE + 0x08, 0x00);
        
        /*
        SPIC_BASE->CTRL0_b.TMOD = 0; // Trans mode
        */
        IO_Write32(SPIC_BASE, 0x8100001F);

        /*
        if((SPIC_BASE->DR_BYTE & 0x03) == 0x02){
            break;
        }
        */
        getStatus = IO_Read8(SPIC_BASE + 0x60);
        if((getStatus & 0x03) == 0x02){
            break;
        }
	}

/* SPIC_Erase */
	uint8_t AddrHH, AddrH, AddrM, AddrL;
	AddrHH = (uint8_t)((Address & 0xFF000000) >> 24);
    AddrH = (uint8_t)((Address & 0x00FF0000) >> 16);
	AddrM = (uint8_t)((Address & 0x0000FF00) >> 8);
	AddrL = (uint8_t)(Address & 0x000000FF);
    
    /*
    if(SPIC->SR_b.BUSY) {
		return EC_DEVICE_BUSY;
	}
    */
    getSPICBUSY = IO_Read8(SPIC_BASE + 0x28);
    if((getSPICBUSY & 0x1) == 1){
        return EC_DEVICE_BUSY;
    }

    /*
    SPIC_BASE->SSIENR = 0x00000000;
    */
    IO_Write32(SPIC_BASE + 0x08, 0x00000000);

    /*
    SPIC_BASE->CTRL0_b.USERMODE = 1; // User Mode 
	SPIC_BASE->CTRL0_b.TMOD = 0; // Trans mode
	SPIC_BASE->CTRL0_b.DATACH = 0; // Single Channel for Data channel
	SPIC_BASE->CTRL0_b.ADDRCH = 0; // Single Channel for Address channel
	SPIC_BASE->CTRL0_b.CMDCH = 0; //Single Channel for Command channel
    */
    IO_Write32(SPIC_BASE, 0x8100001F);

    /*
	SPIC_BASE->USERLENGTH_b.CMDLEN = 1; // Set Command Length
	SPIC_BASE->USERLENGTH_b.ADDRLEN = 3; // Set Address Length(Default 3)
    SPIC_BASE->USERLENGTH_b.RDDUMMYLEN = 0;
    */
    IO_Write32(SPIC_BASE + 0x118, 0x00031000);
    
    /* SPIC_BASE->TXNDF = 0x00000000; // Set Write data length */
    IO_Write32(SPIC_BASE + 0x130, 0x00000000);

    /* SPIC_BASE->RXNDF = 0x00000000; // Set Read data length */
    IO_Write32(SPIC_BASE + 0x04, 0x00000000);

    /* SPIC_BASE->DR_BYTE = 0xD8; // Block erase (64K) */
    IO_Write8(SPIC_BASE + 0x60, 0xD8);

    /* SPIC_BASE->DR_BYTE = AddrH; */
    IO_Write8(SPIC_BASE + 0x60, AddrH);

    /* SPIC_BASE->DR_BYTE = AddrM; */
    IO_Write8(SPIC_BASE + 0x60, AddrM);

    /* SPIC_BASE->DR_BYTE = AddrL; */
    IO_Write8(SPIC_BASE + 0x60, AddrL);

    /* SPIC_BASE->SSIENR_b.SPICEN = 1; // Start trans */
    IO_Write8(SPIC_BASE + 0x08, 0x01);

    /*
    uint32_t retry_cnt = 800000;
    while((SPIC->SSIENR_b.SPICEN == 1) && (retry_cnt--));
    return retry_cnt == 0;
    */
    retry_cnt = 800000;
    do{
        getSPICEN = IO_Read8(SPIC_BASE + 0x08);
        retry_cnt--;
    }while(((getSPICEN & 0x1) != 0) && (retry_cnt));

    if(retry_cnt == 0){
        /* SPIC->FLUSH = 1 */
        IO_Write8(SPIC_BASE + 0x128, 0x01);
        /* SPIC_BASE->SSIENR_b.SPICEN = 0; */
        IO_Write8(SPIC_BASE + 0x08, 0x00);
        return EC_FAIL;
    }

    /* SPIC_BASE->SSIENR_b.SPICEN = 0; */
    IO_Write8(SPIC_BASE + 0x08, 0x00);

    while(1) {
/* SPIC_FlashStatus */

        /*
        if(SPIC->SR_b.BUSY) {
            return EC_DEVICE_BUSY;
        }
        */
        getSPICBUSY = IO_Read8(SPIC_BASE + 0x28);
        if((getSPICBUSY & 0x1) == 1){
            return EC_DEVICE_BUSY;
        }
    
        /* SPIC_BASE->SSIENR = 0x00000000; */
        IO_Write32(SPIC_BASE + 0x08, 0x00000000);

        /*
        SPIC_BASE->CTRL0_b.USERMODE = 1; // User Mode 
        SPIC_BASE->CTRL0_b.TMOD = 3; // Trans mode
        SPIC_BASE->CTRL0_b.DATACH = 0; // Single Channel for Data channel
        SPIC_BASE->CTRL0_b.ADDRCH = 0; // Single Channel for Address channel
        SPIC_BASE->CTRL0_b.CMDCH = 0; //Single Channel for Command channel
        */
        IO_Write32(SPIC_BASE, 0x8100001F);

        /*
        SPIC_BASE->USERLENGTH_b.CMDLEN = 1; // Set Command Length
        SPIC_BASE->USERLENGTH_b.ADDRLEN = 0; // Set Address Length(Default 3)
        SPIC_BASE->USERLENGTH_b.RDDUMMYLEN = 0;
        */
        IO_Write32(SPIC_BASE + 0x118, 0x00001000);

        /* SPIC_BASE->TXNDF = 0x00000000; // Set Write data length */
        IO_Write32(SPIC_BASE + 0x130, 0x00000000);

        /* SPIC_BASE->RXNDF = 0x00000001; // Set Read data length */
        IO_Write32(SPIC_BASE + 0x04, 0x00000001);
        
        /* SPIC_BASE->DR_BYTE = 0x05; // Read Status-1 Command */
        IO_Write8(SPIC_BASE + 0x60, 0x05);

        /* SPIC_BASE->SSIENR_b.SPICEN = 1; // Start trans */
        IO_Write8(SPIC_BASE + 0x08, 0x01);

        /*
        uint32_t retry_cnt = 800000;
        while((SPIC->SSIENR_b.SPICEN == 1) && (retry_cnt--));
        return retry_cnt == 0;
        */
        retry_cnt = 800000;
        do{
            getSPICEN = IO_Read8(SPIC_BASE + 0x08);
            retry_cnt--;
        }while(((getSPICEN & 0x1) != 0) && (retry_cnt));

        if(retry_cnt == 0){
            /* SPIC->FLUSH = 1 */
            IO_Write8(SPIC_BASE + 0x128, 0x01);
            /*
            SPIC_BASE->SSIENR_b.SPICEN = 0;
            SPIC_BASE->CTRL0_b.TMOD = 0; // Trans mode
            */
            IO_Write8(SPIC_BASE + 0x08, 0x00);
            IO_Write32(SPIC_BASE, 0x8100001F);
            return EC_FAIL;
        }

        /* SPIC_BASE->SSIENR_b.SPICEN = 0; */
        IO_Write8(SPIC_BASE + 0x08, 0x00);
        
        /* SPIC_BASE->CTRL0_b.TMOD = 0; // Trans mode */
        IO_Write32(SPIC_BASE, 0x8100001F);

        /*
        if((SPIC_BASE->DR_BYTE & 0x03) == 0x00){
            break;
        }
        */
        getStatus = IO_Read8(SPIC_BASE + 0x60);
        if((getStatus & 0x01) == 0x00){
            break;
        }

	}

    /* SPIC_BASE->CTRL0_b.USERMODE = 0; */
    IO_Write32(SPIC_BASE, 0x0100001F);
    
    return EC_OK;
}

void SPIC_AutoMode(void)
{
    /* SPIC_BASE->CTRL0_b.USERMODE = 0; */
    IO_Write32(SPIC_BASE, 0x0100031F);
}

void ExitFlashMode()
{
    /* GPIO_BASE->GCR_b[106].MFCTRL = 0; */
    IO_Write8(GPIO_BASE + 0x1A8 + 0x01, 0x80);
    /* GPIO_BASE->GCR_b[107].MFCTRL = 0; */
    IO_Write8(GPIO_BASE + 0x1AC + 0x01, 0x80);
    /* GPIO_BASE->GCR_b[108].MFCTRL = 0; */
    IO_Write8(GPIO_BASE + 0x1B0 + 0x01, 0x80);
    /* GPIO_BASE->GCR_b[109].MFCTRL = 0; */
    IO_Write8(GPIO_BASE + 0x1B4 + 0x01, 0x80);
    /* GPIO_BASE->GCR_b[111].MFCTRL = 0; */
    IO_Write8(GPIO_BASE + 0x1BC + 0x01, 0x80);
    /* GPIO_BASE->GCR_b[122].MFCTRL = 0; */
    IO_Write8(GPIO_BASE + 0x1E8 + 0x01, 0x80);
    /* GPIO_BASE->GCR_b[124].MFCTRL = 0; */
    IO_Write8(GPIO_BASE + 0x1F0 + 0x01, 0x80);
}

void WDTReset()
{
    /* SYSTEM->PERICLKPWR2_b.WDTCLKPWR = 1; */
    IO_Write8(SYSTEM_BASE + 0xE8, 0x07);
    /* WDT_BASE->CTRL_b.RSTEN = 1; */
    IO_Write8(WDT_BASE, 0x02);
    /* WDT_BASE->DIV = 1; */
    IO_Write8(WDT_BASE + 0x0C, 0x01);
    /* WDT_BASE->CNT = 1; */
    IO_Write8(WDT_BASE + 0x08, 0x01);
    /* WDT_BASE->INTEN_b.WDTINTEN = 1; */
    IO_Write8(WDT_BASE + 0x10, 0x01);
	/* WDT_BASE->CTRL_b.EN = 1; */
    IO_Write8(WDT_BASE, 0x03);
}
