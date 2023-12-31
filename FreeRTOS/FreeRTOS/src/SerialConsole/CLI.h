/**************************************************************************//**
* @file      CLI.h
* @brief     Files managing command line arguments
* @author    Adi
* @date      2024-1-2

******************************************************************************/
#ifndef CLI_H_
#define CLI_H_

/******************************************************************************
* Includes
******************************************************************************/
#include "dUART.h"
/******************************************************************************
* Defines
******************************************************************************/
#define COMMAND_LED		"led"
/******************************************************************************
* Variables
******************************************************************************/

/******************************************************************************
* Function Prototypes
******************************************************************************/
int32_t CLI_ExtractCmd(char * cmd, int32_t length);




#endif /* CLI_H_ */