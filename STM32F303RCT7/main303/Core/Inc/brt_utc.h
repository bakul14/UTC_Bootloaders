#ifndef __MAIN_H
#include "main.h"
#define __MAIN_H
#endif

//----------- USER SETTINGS-----------
#define DEVICE_ID 0x10
#define USED_CANBus hcan
//----------- USER SETTINGS-----------



/*--------Table of identifiers--------
Precharge				0x10
Discharge				0x11
VCU							0x12
BCU							0x13
AMS LV					0x14
Brake Light			0x15
CMUX Front			0x16
CMUX Rear				0x17
AMI							0x18
ASSI						0x19
FAN Regulator		0x1A
AMS Master			0x1B
EBS							0x1C
Steering Wheel	0x1D
VCDU						0x1E
----------Table of identifiers------*/


//-----------DEVELOPER SETTINGS (don't change)-----------
#define APP_START_ADDRESS 0x08002400
#define NUMBER_OF_PAGES 64
#define FLAG_ADDRESS 0x0800FC00
#define FLAG_VALUE 0x12345678
//-----------DEVELOPER SETTINGS (don't change)-----------


extern uint8_t FeedbackBuf[1];
extern uint8_t RX_data[8];

extern const uint32_t flag_address;
extern const uint32_t flag_value;
extern uint32_t PageError;

extern uint32_t program_data_1;
extern uint32_t program_data_2;
extern uint64_t address_data;

extern uint8_t FLAG_ERASE_OVER;
extern uint8_t FLAG_DOWNLOAD_OVER;

extern CAN_HandleTypeDef USED_CANBus;

extern CAN_TxHeaderTypeDef pTxHeader;
extern CAN_RxHeaderTypeDef pRxHeader;
extern uint32_t TxMailbox;
extern CAN_FilterTypeDef sFilterConfig;

extern FLASH_EraseInitTypeDef pEraseInit;

extern void GoTo(void);
extern void CheckUpdate(void);
extern void BootloaderProcessing(void);
extern void CANBusStart(void);
extern void ChangeVectorTable(void);
extern inline void CANProcessing(void);
extern inline void StartUpdate(void);
