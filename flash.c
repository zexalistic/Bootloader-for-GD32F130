#include "flash.h"
#include "wdg.h"

/*!
    \brief      gets the sector of a given address
    \param[in]  address: a given address
    \param[out] none
    \retval     the sector of a given address
*/
uint32_t fmc_sector_get(uint32_t address)
{
	uint32_t sector = 0;
	uint32_t addr_end;
	
	addr_end = FMC_ADDR_BASE + FMC_ALL_SIZE * FMC_PAGE_SIZE;
	if((address < addr_end) && (address >= FMC_ADDR_BASE))
	{
		sector = address & FMC_ADDR_MASK;
	}
	
	return sector;
}

/*!
    \brief      erases the sector of a given sector number
    \param[in]  fmc_sector: a given sector number
    \param[out] none
    \retval		1:succeed; 0:failed
*/
uint8_t fmc_erase_sector(uint32_t fmc_sector)
{
	/* unlock the flash program erase controller */
	FMC_Unlock();
	/* Clear All pending flags */
	FMC_ClearBitState(FMC_FLAG_EOP | FMC_FLAG_WERR | FMC_FLAG_PERR );
	/* wait the erase operation complete*/
	if(FMC_READY != FMC_ErasePage(fmc_sector))
	{
		return 0;
	}
	/* lock the flash program erase controller */
	FMC_Lock();
	/* feed the watchdog */
	//iwdg_feed();
	return 1;
}

/*!
	\brief      write 32 bit length data to a given address
	\param[in]  address: a given address
	\param[in]  length: data length
	\param[in]  data_32: data pointer
	\param[out] none
	\retval		1:succeed; 0:failed
*/
uint8_t fmc_write_word(uint32_t address, uint8_t *rxbuf, uint32_t rxsize)
{
	uint32_t i,data_32,tmp_addr;
	FMC_State temp_state;
	
  FMC_Unlock();
		
	tmp_addr=address;
	for(i=0; i<rxsize; i+=4)
	{
		data_32=rxbuf[i] | (rxbuf[i+1]<<8) | (rxbuf[i+2]<<16) | (rxbuf[i+3]<<24);
		
		temp_state = FMC_ProgramWord(tmp_addr, data_32);
		FMC_ClearBitState(FMC_FLAG_EOP | FMC_FLAG_WERR | FMC_FLAG_PERR );
		tmp_addr = tmp_addr + 4;
	}


	/* lock the flash program erase controller */
	FMC_Lock();
	
	return temp_state;
}

/*!
	\brief      read 32 bit length data from a given address
	\param[in]  address: a given address
	\param[in]  length: data length
	\param[in]  data_addr: data pointer
	\param[out] none
	\retval     none
*/
void fmc_read_word(uint32_t address, uint16_t length, uint32_t* data_addr)
{
	uint8_t i;
	for(i=0; i<length; i++)
	{
		data_addr[i] = *(__IO int32_t*)address;
		address=address + 4;
	}
}



uint8_t fmc_erase(uint32_t address, uint8_t erase_page_num){
	uint8_t EraseCounter,PageCounter,res=0;
	uint32_t tmp_addr;
	uint32_t *ptrd;
	
	/* unlock the flash program erase controller */
	FMC_Unlock();
	
    /* Clear All pending flags */
  FMC_ClearBitState(FMC_FLAG_EOP | FMC_FLAG_WERR | FMC_FLAG_PERR );
					
	/* Erase the FLASH pages */
	for(EraseCounter = 0; EraseCounter < erase_page_num; EraseCounter++)
	{
		tmp_addr=address+FMC_PAGE_SIZE*EraseCounter;
		FMC_ClearBitState(FMC_FLAG_EOP | FMC_FLAG_WERR | FMC_FLAG_PERR );
		FMC_ErasePage(tmp_addr);	
	}

	 FMC_Lock();
	
	ptrd = (uint32_t *)address;

	//check if the 26k pages are erased successfully
	for(EraseCounter = 0; EraseCounter < erase_page_num; EraseCounter++)
		{
			for(PageCounter = 0; PageCounter < 128; PageCounter++)  	//	1024/8=128 
			{
				if(*ptrd != 0xFFFFFFFF)
        { 
            res = ERASE_FAILED;
            return res;
        }
        else
        {
            ptrd++;
        }
			}
		}
		return res;
}

uint8_t fmc_write_single(uint32_t address, uint32_t *rxbuf)
{
	uint32_t data_32,tmp_addr;
	FMC_State temp_state;
	
		FMC_Unlock();
		
		tmp_addr=address;
		data_32= *(__IO uint32_t*)rxbuf ;
		
		temp_state = FMC_ProgramWord(tmp_addr, data_32);
		FMC_ClearBitState(FMC_FLAG_EOP | FMC_FLAG_WERR | FMC_FLAG_PERR );
		
	FMC_Lock();
	
		return temp_state;
    
	}



