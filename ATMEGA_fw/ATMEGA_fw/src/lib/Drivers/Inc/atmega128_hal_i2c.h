#ifndef ATMEGA128_HAL_I2C_H_
#define ATMEGA128_HAL_I2C_H_

#include "atmega128_hal_def.h"

typedef struct
{
	uint32_t ClockSpeed;
	uint8_t OwnAddress;
	uint8_t GeneralCallMode;
	
} I2C_InitTypeDef;


typedef enum
{
  HAL_I2C_STATE_RESET             = 0x00U,   /*!< Peripheral is not yet Initialized         */
  HAL_I2C_STATE_READY             = 0x20U,   /*!< Peripheral Initialized and ready for use  */
  HAL_I2C_STATE_BUSY              = 0x24U,   /*!< An internal process is ongoing            */
  HAL_I2C_STATE_BUSY_TX           = 0x21U,   /*!< Data Transmission process is ongoing      */
  HAL_I2C_STATE_BUSY_RX           = 0x22U,   /*!< Data Reception process is ongoing         */
  HAL_I2C_STATE_LISTEN            = 0x28U,   /*!< Address Listen Mode is ongoing            */
  HAL_I2C_STATE_BUSY_TX_LISTEN    = 0x29U,   /*!< Address Listen Mode and Data Transmission
                                                 process is ongoing                         */
  HAL_I2C_STATE_BUSY_RX_LISTEN    = 0x2AU,   /*!< Address Listen Mode and Data Reception
                                                 process is ongoing                         */
  HAL_I2C_STATE_ABORT             = 0x60U,   /*!< Abort user request ongoing                */
  HAL_I2C_STATE_TIMEOUT           = 0xA0U,   /*!< Timeout state                             */
  HAL_I2C_STATE_ERROR             = 0xE0U    /*!< Error                                     */

} HAL_I2C_StateTypeDef;

typedef enum
{
	HAL_I2C_MODE_NONE		= 0x00U,
	HAL_I2C_MODE_MASTER		= 0x10U,
	HAL_I2C_MODE_SLAVE		= 0x20U	
} HAL_I2C_ModeTypeDef;

#define HAL_I2C_ERROR_NONE              0x00000000U    /*!< No error              */
#define HAL_I2C_ERROR_BERR              0x00000001U    /*!< BERR error            */
#define HAL_I2C_ERROR_ARLO              0x00000002U    /*!< ARLO error            */
#define HAL_I2C_ERROR_AF                0x00000004U    /*!< AF error              */
#define HAL_I2C_ERROR_OVR               0x00000008U    /*!< OVR error             */
#define HAL_I2C_ERROR_DMA               0x00000010U    /*!< DMA transfer error    */
#define HAL_I2C_ERROR_TIMEOUT           0x00000020U    /*!< Timeout Error         */
#define HAL_I2C_ERROR_SIZE              0x00000040U    /*!< Size Management error */
#define HAL_I2C_ERROR_DMA_PARAM         0x00000080U    /*!< DMA Parameter Error   */
#define HAL_I2C_WRONG_START             0x00000200U    /*!< Wrong start Error     */

typedef struct
{
	I2C_TypeDef *Instance;
	
	I2C_InitTypeDef Init;
	
	uint8_t *pBuffPtr;
	
	uint16_t XferSize;
	
	uint16_t XferCount;
	
	uint8_t XferOptions;
	
	uint8_t PreviousState;
	
	HAL_I2C_StateTypeDef State;
	
	HAL_I2C_ModeTypeDef Mode;
	
	uint8_t ErrorCode;
	
	uint8_t Devaddress;
	
	uint16_t Memaddress;
	
	uint8_t MemaddSize;
} I2C_HandleTypeDef;

#endif /* ATMEGA128_HAL_I2C_H_ */