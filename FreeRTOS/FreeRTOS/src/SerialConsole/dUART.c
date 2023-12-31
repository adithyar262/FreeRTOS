/**************************************************************************//**
* @file      dUART.c
* @brief     UART Driver for serial communication
* @author    Adi
* @date      2023-12-31

******************************************************************************/

/******************************************************************************
* Includes
******************************************************************************/
#include <asf.h>
#include "dUART.h"
/******************************************************************************
* Defines
******************************************************************************/
#define RX_BUFFER_SIZE 512  ///< Size of character buffer for RX, in bytes
#define TX_BUFFER_SIZE 512  ///< Size of character buffers for TX, in bytes

/******************************************************************************
* Variables
******************************************************************************/
struct usart_module usart_instance;
char rxCharacterBuffer[RX_BUFFER_SIZE];                 ///< Buffer to store received characters
char txCharacterBuffer[TX_BUFFER_SIZE];                 ///< Buffer to store characters to be sent

cbuf_handle_t cbufRx;  ///< Circular buffer handler for receiving characters from the Serial Interface
cbuf_handle_t cbufTx;  ///< Circular buffer handler for transmitting characters from the Serial Interface

char latestRx;  ///< Holds the latest character that was received
char latestTx;  ///< Holds the latest character to be transmitted.
/******************************************************************************
* Forward Declarations
******************************************************************************/
static void dUART_Configure(void);
static void dUART_ConfigureCallbacks(void);

/******************************************************************************
* Callback Functions
******************************************************************************/
// Callback for when we finish writing characters to UART
void dUART_WriteCallback(struct usart_module *const usart_module);
// Callback for when we finis reading characters from UART
void dUART_ReadCallback(struct usart_module *const usart_module);

/******************************************************************************
* Static Functions
******************************************************************************/
/**************************************************************************//**
* @fn		static void dUART_Configure(void)
* @brief	Configure UART 
* @details 	Code to configure the SERCOM "EDBG_CDC_MODULE" to be a UART channel
			running at 115200 8N1				
* @param[in]	N/A
* @param[out]	N/A
* @return		N/A
* @note         
*****************************************************************************/

static void dUART_Configure(void)
{
    struct usart_config config_usart;
    usart_get_config_defaults(&config_usart);

    config_usart.baudrate = 115200;
    config_usart.mux_setting = EDBG_CDC_SERCOM_MUX_SETTING;
    config_usart.pinmux_pad0 = EDBG_CDC_SERCOM_PINMUX_PAD0;
    config_usart.pinmux_pad1 = EDBG_CDC_SERCOM_PINMUX_PAD1;
    config_usart.pinmux_pad2 = EDBG_CDC_SERCOM_PINMUX_PAD2;
    config_usart.pinmux_pad3 = EDBG_CDC_SERCOM_PINMUX_PAD3;
    while (usart_init(&usart_instance, EDBG_CDC_MODULE, &config_usart) != STATUS_OK) {
    }

    usart_enable(&usart_instance);
}

/**************************************************************************//**
* @fn		static void dUART_ConfigureCallbacks(void)
* @brief	Code to register callbacks				
* @param[in]	N/A
* @param[out]	N/A
* @return		N/A
* @note         
*****************************************************************************/
static void dUART_ConfigureCallbacks(void)
{
    usart_register_callback(&usart_instance, dUART_WriteCallback, USART_CALLBACK_BUFFER_TRANSMITTED);
    usart_register_callback(&usart_instance, dUART_ReadCallback, USART_CALLBACK_BUFFER_RECEIVED);
    usart_enable_callback(&usart_instance, USART_CALLBACK_BUFFER_TRANSMITTED);
    usart_enable_callback(&usart_instance, USART_CALLBACK_BUFFER_RECEIVED);
}

/******************************************************************************
* Global Functions
******************************************************************************/

/**************************************************************************//**
* @fn		void dUART_WriteString(const char * string)
* @brief		Writes a string to be written to the uart. Copies the
*				string to a ring buffer that is used to hold the
*				text send to the uart
* @details		Uses the ringbuffer 'cbufTx', which in turn uses the array
*				'txCharacterBuffer'. Modified to be thread safe.
* @param[in]	Pointer to string to be written
* @return		N/A
* @note			N/A
*****************************************************************************/
void dUART_WriteString(const char *string)
{
    if (string != NULL) {
        for (size_t iter = 0; iter < strlen(string); iter++) {
            circular_buf_put(cbufTx, string[iter]);
        }

        if (usart_get_job_status(&usart_instance, USART_TRANSCEIVER_TX) == STATUS_OK) {
            circular_buf_get(cbufTx, (uint8_t *)&latestTx);  // Perform only if the SERCOM TX is free (not busy)
            usart_write_buffer_job(&usart_instance, (uint8_t *)&latestTx, 1);
        }
    }
}

/**************************************************************************//**
* @fn		int dUART_ReadCharacter(uint8_t *rxChar)
* @brief		Reads a character from the RX ring buffer and stores it on
				the pointer given as an argument.
*				Also, returns -1 if there is no characters on the buffer
*				This buffer has values added to it when the UART receives
				ASCII characters from the terminal
* @details		Uses the ringbuffer 'cbufTx', which in turn uses the array
				'txCharacterBuffer'
* @param[in]	Pointer to a character. This function will return the
				character from the RX buffer into this pointer
* @return		Returns -1 if there are no characters in the buffer
* @note			Use to receive characters from the RX buffer (FIFO)
*****************************************************************************/
int dUART_ReadCharacter(uint8_t *rxChar)
{
    int a = circular_buf_get(cbufRx, (uint8_t *)rxChar);
    return a;
}

/**************************************************************************//**
* @fn		void dUART_Initialize(void)
* @brief	Initializes UART and buffers
* @details 	Initializes the UART - sets up the SERCOM to act as UART and registers the callbacks for
*			asynchronous reads and writes.				
* @param[in]	N/A
* @param[out]	N/A
* @return		N/A
* @note         
*****************************************************************************/
void dUART_Initialize(void)
{
    // Initialize circular buffers for RX and TX
    cbufRx = circular_buf_init((uint8_t *)rxCharacterBuffer, RX_BUFFER_SIZE);
    cbufTx = circular_buf_init((uint8_t *)txCharacterBuffer, RX_BUFFER_SIZE);

    // Configure USART and Callbacks
    dUART_Configure();
    dUART_ConfigureCallbacks();

    usart_read_buffer_job(&usart_instance, (uint8_t *)&latestRx, 1);  // Kicks off constant reading of characters
}

/**************************************************************************//**
* @fn		void dUART_Deinitialize(void)
* @brief	De initializes the UART 				
* @param[in]	N/A
* @param[out]	N/A
* @return		N/A
* @note         
*****************************************************************************/
void dUART_Deinitialize(void)
{
    usart_disable(&usart_instance);
}

/**************************************************************************//**
* @fn		void dUART_ReadCallback(struct usart_module *const usart_module)
* @brief	Callback called when the system finishes receives all the bytes 
*			requested from a UART read job
* @param[in]	N/A
* @param[out]	N/A
* @return		N/A
* @note
*****************************************************************************/
void dUART_ReadCallback(struct usart_module *const usart_module)
{
    // Add the latest read character into the RX circular Buffer
	circular_buf_put(cbufRx, (uint8_t)latestRx);                      
    // Order the MCU to keep reading
	usart_read_buffer_job(&usart_instance, (uint8_t *)&latestRx, 1);  
}

/**************************************************************************//**
* @fn		void dUART_WriteCallback(struct usart_module *const usart_module)
* @brief	Callback called when the system finishes sending all the bytes
*			requested from a UART read job
* @param[in]	N/A
* @param[out]	N/A
* @return		N/A
* @note
*****************************************************************************/
void dUART_WriteCallback(struct usart_module *const usart_module)
{
    // Only continue if there are more characters to send
	if (circular_buf_get(cbufTx, (uint8_t *)&latestTx) != -1)  
    {
        usart_write_buffer_job(&usart_instance, (uint8_t *)&latestTx, 1);
    }
}