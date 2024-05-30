
///*******************************************************************************
//* Header Files
//*******************************************************************************/
#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"

///******************************************************************************
//* Macros
//*******************************************************************************/
//Macros for CO2 sensor UART communication

#define TX_CO2 P12_1
#define RX_CO2 P12_0
//
#define DATA_BITS_8     8
#define STOP_BITS_1     1
#define BAUD_RATE       9600
#define UART_DELAY      10u
#define RX_BUF_SIZE     9
#define TX_BUF_SIZE     9

//Macros for co2 sensor

#define READ_CO2_CONCENTRATION 0x86
#define SPAN_POINT_CALIBRATION 0x88
#define ZERO_POINT_CALIBRATION 0x87

int main(void)
{
    cy_rslt_t result;
    cyhal_uart_t mh_co2;
    uint32_t     actualbaud;
    uint8_t      tx_buf[TX_BUF_SIZE] = {0xFF,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x79};
    uint8_t      rx_buf[RX_BUF_SIZE];
    size_t       tx_length = TX_BUF_SIZE;
    size_t       rx_length = RX_BUF_SIZE;

    /* Initialize the device and board peripherals */
    result = cybsp_init();

    /* Board init failed. Stop program execution */
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    const cyhal_uart_cfg_t cnf_co2 =
        {
            .data_bits      = DATA_BITS_8,
            .stop_bits      = STOP_BITS_1,
            .parity         = CYHAL_UART_PARITY_NONE,
            .rx_buffer      = rx_buf,
            .rx_buffer_size = RX_BUF_SIZE
        };


    result = cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX, 9600);
    cyhal_uart_init(&mh_co2, TX_CO2, RX_CO2, NC, NC, NULL, &cnf_co2);
    cyhal_uart_set_baud(&mh_co2, BAUD_RATE, &actualbaud);


    /* If retarget-io init failed stop program execution */
    CY_ASSERT(result == CY_RSLT_SUCCESS);

    /* Enable global interrupts */
    __enable_irq();

    tx_buf[2] = ZERO_POINT_CALIBRATION;

    // Begin Tx Transfer
    cyhal_uart_write(&mh_co2, (void*)tx_buf, &tx_length);
    cyhal_system_delay_ms(UART_DELAY);


    tx_buf[2] = READ_CO2_CONCENTRATION;
    // Begin Tx Transfer
    cyhal_uart_write(&mh_co2, (void*)tx_buf, &tx_length);
    cyhal_system_delay_ms(UART_DELAY);


    // Begin Rx Transfer
    cyhal_uart_read(&mh_co2, rx_buf, &rx_length);
    cyhal_system_delay_ms(UART_DELAY);



    for (;;)
    {
//    	printf("\rCO2  =   %.2f\r\n", fco2data);

    	CyDelay(1000);

    	int ihigh_co2 = rx_buf[3];
    	int ilow_co2 = rx_buf[4];

        int concentration = ihigh_co2 * 256 + ilow_co2;

    	    	 //printf("%d\n"concentration);





    }
}

