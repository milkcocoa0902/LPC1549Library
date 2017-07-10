#pragma once

#include <eeprom.hpp>
#include <error.hpp>
#include <iap.hpp>
#include <rom_adc_15xx.hpp>
#include <rom_can_15xx.hpp>
#include <rom_dma_15xx.hpp>
#include <rom_i2c_15xx.hpp>
#include <rom_pwr_15xx.hpp>
#include <rom_spi_15xx.hpp>
#include <rom_uart_15xx.hpp>


/** @defgroup ROMAPI_15XX CHIP: LPC15xx ROM API declarations and functions
 * @ingroup CHIP_15XX_Drivers
 * @{
 */

/**
 * @brief LPC15XX High level ROM API structure
 */
typedef struct {
	const uint32_t pUSBD;					/*!< USBD API function table base address */
	const uint32_t reserved0;				/*!< Reserved */
	const CAND_API_T *pCAND;				/*!< C_CAN API function table base address */
	const PWRD_API_T *pPWRD;				/*!< Power API function table base address */
	const uint32_t reserved1;				/*!< Reserved */
	const I2CD_API_T *pI2CD;				/*!< I2C driver API function table base address */
	const DMAD_API_T *pDMAD;				/*!< DMA driver API function table base address */
	const SPID_API_T *pSPID;				/*!< I2C driver API function table base address */
	const ADCD_API_T *pADCD;				/*!< ADC driver API function table base address */
	const UARTD_API_T *pUARTD;				/*!< UART driver API function table base address */
} LPC_ROM_API_T;

/* Pointer to ROM API function address */
#define LPC_ROM_API_BASE_LOC    0x03000200UL
#define LPC_ROM_API     (*(LPC_ROM_API_T * *) LPC_ROM_API_BASE_LOC)

/* Pointer to @ref CAND_API_T functions in ROM */
#define LPC_CAND_API    ((LPC_ROM_API)->pCAND)

/* Pointer to @ref PWRD_API_T functions in ROM */
#define LPC_PWRD_API    ((LPC_ROM_API)->pPWRD)

/* Pointer to @ref I2CD_API_T functions in ROM */
#define LPC_I2CD_API    ((LPC_ROM_API)->pI2CD)

/* Pointer to @ref DMAD_API_T functions in ROM for DMA */
#define LPC_DMAD_API    ((LPC_ROM_API)->pDMAD)

/* Pointer to @ref SPID_API_T functions in ROM for DMA */
#define LPC_SPID_API    ((LPC_ROM_API)->pSPID)

/* Pointer to @ref ADCD_API_T functions in ROM for pADCD */
#define LPC_ADCD_API    ((LPC_ROM_API)->pADCD)

/* Pointer to @ref UARTD_API_T functions in ROM for UARTs */
#define LPC_UARTD_API   ((LPC_ROM_API)->pUARTD)

/* Pointer to ROM IAP entry functions */
#define IAP_ENTRY_LOCATION        0x03000205UL

/**
 * @brief LPC15XX IAP_ENTRY API function type
 */
static inline void iap_entry(unsigned int cmd_param[5], unsigned int status_result[4])
{
	((IAP_ENTRY_T) IAP_ENTRY_LOCATION)(cmd_param, status_result);
}

/**
 * @}
 */

