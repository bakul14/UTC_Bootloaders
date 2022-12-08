#include "brt_utc.h"

uint8_t FeedbackBuf[1] = {'F'};
uint8_t ReadyTX[] = "READY";
uint8_t RX_data[8];

const uint32_t flag_address = FLAG_ADDRESS;
const uint32_t flag_value = FLAG_VALUE;
uint32_t PageError = 0;

volatile uint32_t program_data_1 = 0;
volatile uint32_t program_data_2 = 0;
uint64_t address_data = APP_START_ADDRESS;

volatile uint8_t FLAG_ERASE_OVER = 0;
volatile uint8_t FLAG_DOWNLOAD_OVER = 0;

FLASH_EraseInitTypeDef pEraseInit;

CAN_TxHeaderTypeDef pTxHeader;
CAN_RxHeaderTypeDef pRxHeader;
uint32_t TxMailbox;
CAN_FilterTypeDef sFilterConfig;


void GoTo() //jump to the memory area specified in #define APP_START_ADDRESS
{
    uint32_t appJumpAdress;

    appJumpAdress = *((volatile uint32_t*)(APP_START_ADDRESS+4));

    HAL_RCC_DeInit();
    HAL_DeInit();

    void(*GoToApp)(void);
    GoToApp = (void (*)(void)) appJumpAdress;

    __disable_irq();
    __set_MSP(*((volatile uint32_t*)APP_START_ADDRESS));
    GoToApp();
}

void CheckUpdate(void)
{
    HAL_FLASH_Unlock();
    if (*(__IO  uint32_t*)flag_address == flag_value)
    {
        pEraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
        pEraseInit.PageAddress = APP_START_ADDRESS;
        pEraseInit.NbPages = NUMBER_OF_PAGES;
        HAL_FLASHEx_Erase(&pEraseInit, &PageError);

        FLAG_ERASE_OVER = 1;
    }
    else
    {
        HAL_FLASH_Lock();
        GoTo();
    }
}

void BootloaderProcessing(void)
{
    if (FLAG_DOWNLOAD_OVER == 1)
    {
        HAL_FLASH_Lock();
        NVIC_SystemReset();
    }

    if (FLAG_ERASE_OVER == 1)
    {
        pTxHeader.DLC = sizeof(ReadyTX);
        pTxHeader.StdId = DEVICE_ID;
        HAL_CAN_AddTxMessage(&USED_CANBus, &pTxHeader, ReadyTX, &TxMailbox);
        FLAG_ERASE_OVER = 0;
    }
}

void CANBusStart(void)
{
    pTxHeader.DLC = 8;
    pTxHeader.IDE = CAN_ID_STD;
    pTxHeader.RTR = CAN_RTR_DATA;
    pTxHeader.StdId = DEVICE_ID;
    pTxHeader.TransmitGlobalTime = DISABLE;

    sFilterConfig.FilterBank = 0;
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDLIST;
    sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
    sFilterConfig.SlaveStartFilterBank = 0;
    sFilterConfig.FilterIdHigh = DEVICE_ID << 5;
    sFilterConfig.FilterIdLow = DEVICE_ID << 5;
    sFilterConfig.FilterMaskIdHigh = DEVICE_ID << 5;
    sFilterConfig.FilterMaskIdLow = DEVICE_ID << 5;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_16BIT;
    sFilterConfig.FilterActivation = ENABLE;

    if (HAL_CAN_ConfigFilter(&USED_CANBus, &sFilterConfig)!= HAL_OK)
    {
        // Filter configuration Error
        Error_Handler();
    }

    if(HAL_CAN_Start(&USED_CANBus)!= HAL_OK)
    {
        //Start Error
        Error_Handler();
    }
    /* Activate CAN RX notification */
    if (HAL_CAN_ActivateNotification(&USED_CANBus, CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_TX_MAILBOX_EMPTY) != HAL_OK)
    {
        /* Notification Error */
        Error_Handler();
    }
}

void ChangeVectorTable(void)
{
    __disable_irq();
    SCB->VTOR = APP_START_ADDRESS;
    __enable_irq();
}

inline void CANProcessing()
{
    if (RX_data[0] == 'E' &&
            RX_data[1] == 'N' &&
            RX_data[2] == 'D')
    {
        HAL_FLASH_Lock();
        FLAG_DOWNLOAD_OVER = 1;
    }
	
    else if (RX_data[0] == 'S' &&
             RX_data[1] == 'T' &&
             RX_data[2] == 'A' &&
             RX_data[3] == 'R' &&
             RX_data[4] == 'T')
    {
        StartUpdate();
    }

    else
    {
        program_data_1 = (RX_data[0]) + (RX_data[1] << 8) + (RX_data[2] << 16) + (RX_data[3] << 24);
        program_data_2 = (RX_data[4]) + (RX_data[5] << 8) + (RX_data[6] << 16) + (RX_data[7] << 24);
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address_data, program_data_1);
        address_data += 4;
        program_data_1 = 0;
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address_data, program_data_2);
        address_data += 4;
        program_data_2 = 0;

        pTxHeader.DLC = 1;
        HAL_CAN_AddTxMessage(&USED_CANBus, &pTxHeader, FeedbackBuf, &TxMailbox);
    }
}

inline void StartUpdate(void)
{
    HAL_FLASH_Unlock();
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, flag_address, flag_value);
    HAL_FLASH_Lock();
    NVIC_SystemReset();
}
