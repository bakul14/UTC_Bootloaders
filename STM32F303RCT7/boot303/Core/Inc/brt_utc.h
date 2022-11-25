#ifndef __MAIN_H
#include "main.h"
#define __MAIN_H
#endif

//----------- USER SETTINGS-----------
#define DEVICE_ID 0x12
#define USED_CANBus hcan
//----------- USER SETTINGS-----------



/*--------Table of identifiers--------
0x10	Precharge
0x11	Discharge
0x12	VCU
0x13	BCU
0x14	AMS LV
0x15	Brake Light
0x16	CMUX Front
0x17	CMUX Rear
0x18	AMI
0x19	ASSI
0x1A	FAN Regulator
0x1B	AMS Master
0x1C	EBS
0x1D	Steering Wheel
0x1E	VCDU
----------Table of identifiers------*/


//-----------DEVELOPER SETTINGS (don't change)-----------
#define APP_START_ADDRESS 0x08002800
#define NUMBER_OF_PAGES 128
#define FLAG_ADDRESS 0x0803F800
#define FLAG_VALUE 0x12345678
//-----------DEVELOPER SETTINGS (don't change)-----------


extern uint8_t FeedbackBuf[1];
extern uint8_t RX_data[8];

extern const uint32_t flag_address;
extern const uint32_t flag_value;
extern uint32_t PageError;

extern volatile uint32_t program_data_1;
extern volatile uint32_t program_data_2;
extern uint64_t address_data;

extern volatile uint8_t FLAG_ERASE_OVER;
extern volatile uint8_t FLAG_DOWNLOAD_OVER;

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
