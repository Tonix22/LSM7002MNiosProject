#include <stdio.h>
#include "system.h"
#include "altera_avalon_pio_regs.h"
#include "sys/alt_irq.h"
#include <unistd.h>
#include "aip.h"
#include "ID00001001_dummy.h"

// **************************************************************
#include <LMS7002M/LMS7002M.h>
#include <LMS7002M/LMS7002M_logger.h>

#include <stdio.h>
#include <stdlib.h>

#include "platform.h"
//#include "broker.h"
//#include "xilinx_user_gpio.h"


#define SPI_DEVICE_ID				0
#define REF_FREQ (61.44e6/2)

#define EMIO_OFFSET 54
#define RESET_EMIO    (EMIO_OFFSET+0)
#define DIG_RST_EMIO  (EMIO_OFFSET+1)
#define RXEN_EMIO     (EMIO_OFFSET+2)
#define TXEN_EMIO     (EMIO_OFFSET+3)
#define DIO_DIR_CTRL1_EMIO   (EMIO_OFFSET+4)
#define DIO_DIR_CTRL2_EMIO   (EMIO_OFFSET+5)
#define IQSEL1_DIR_EMIO      (EMIO_OFFSET+6)
#define IQSEL2_DIR_EMIO      (EMIO_OFFSET+7)

#define SET_EMIO_OUT_LVL(emio, lvl) \
    gpio_init(emio); \
    gpio_direction(emio, 1); \
    gpio_set_value(emio, lvl);

#define CLEANUP_EMIO(emio) \
    gpio_direction(emio, 0); \

#define FPGA_REGS 0x43C00000

#define FPGA_REG_RD_SENTINEL 0 //readback a known value
#define FPGA_REG_RD_RX_CLKS 8 //sanity check clock counter
#define FPGA_REG_RD_TX_CLKS 12 //sanity check clock counter
#define FPGA_REG_RD_DATA_A 28 //RXA data for loopback test
#define FPGA_REG_RD_DATA_B 32 //RXB data for loopback test

#define FPGA_REG_WR_EXT_RST 12 //active high external reset
//#define FPGA_REG_WR_RX_STORE_OK 8 //can register RX samples (for test)
#define FPGA_REG_WR_DATA_A 28 //TXA data for loopback test
#define FPGA_REG_WR_DATA_B 32 //TXB data for loopback test
#define FPGA_REG_WR_TX_TEST 36

//***************************************************************************************


#define DUMMY_0 AIP_0_BASE
#define DUMMY_1 AIP_1_BASE
#define DUMMY_2 AIP_2_BASE
#define DUMMY_MEM_SIZE 8

volatile int edge_val = 0;
volatile int start_state=0;
void int_isr(void * context);
void start_isr(void * context);
void int_setup();
void start_setup();


int main(void)
{

	start_state = 0;
    uint32_t dataFlit = 0;
    uint32_t dataFlits[DUMMY_MEM_SIZE];

    uint32_t data[4];

    printf("MasterSoC AIP Controller Test Application\n");
    int_setup();
    printf("Int Setup Done\n");
    start_setup();

    ID00001001_init(DUMMY_0);
    ID00001001_init(DUMMY_1);
    ID00001001_init(DUMMY_2);
  
   /* ID00001001_getStatus(DUMMY_0, &dataFlit);

    for (uint32_t i = 0; i < DUMMY_MEM_SIZE; i++)
    {
        dataFlits[i] = 1<<i;
    }

    ID00001001_writeData(DUMMY_0, dataFlits, DUMMY_MEM_SIZE, 0);


    for (uint32_t i = 0; i < DUMMY_MEM_SIZE; i++)
    {
        dataFlits[i] = i;
    }

    ID00001001_writeData(DUMMY_1, dataFlits, DUMMY_MEM_SIZE, 0);


    for (uint32_t i = 0; i < DUMMY_MEM_SIZE; i++)
    {
        dataFlits[i] = 7-i;
    }

    ID00001001_writeData(DUMMY_2, dataFlits, DUMMY_MEM_SIZE, 0);

    ID00001001_enableDelay(DUMMY_0, 10000);

    ID00001001_startIP(DUMMY_0);

    ID00001001_getStatus(DUMMY_0, &dataFlit);

    ID00001001_waitirq(DUMMY_0);

    ID00001001_getStatus(DUMMY_0, &dataFlit);

    for (uint32_t i = 0; i < DUMMY_MEM_SIZE; i++)
    {
        dataFlits[i] = 0;
    }*/
   /* ID00001001_readData(DUMMY_0, dataFlits, DUMMY_MEM_SIZE, 0);
    
    printf("Data in Dummy 0:\n");

    for (uint32_t i = 0; i < DUMMY_MEM_SIZE; i++)
    {
        printf("Data[%i]: %x ", i, dataFlits[i]);
    }
    printf("\n");

    ID00001001_enableDelay(DUMMY_1, 10000);

    ID00001001_startIP(DUMMY_1);

    ID00001001_getStatus(DUMMY_1, &dataFlit);

    ID00001001_waitirq(DUMMY_1);

   // ID00001001_waitDone(DUMMY_1);

    ID00001001_getStatus(DUMMY_1, &dataFlit);

    for (uint32_t i = 0; i < DUMMY_MEM_SIZE; i++)
    {
        dataFlits[i] = 0;
    }
    ID00001001_readData(DUMMY_1, dataFlits, DUMMY_MEM_SIZE, 0);
  
    printf("Data in Dummy 1:\n");

    for (uint32_t i = 0; i < DUMMY_MEM_SIZE; i++)
    {
        printf("Data[%i]: %x ", i, dataFlits[i]);
    }
    printf("\n");

    ID00001001_enableDelay(DUMMY_2, 10000);

    ID00001001_startIP(DUMMY_2);

    ID00001001_getStatus(DUMMY_2, &dataFlit);

    ID00001001_waitirq(DUMMY_2);

    ID00001001_getStatus(DUMMY_2, &dataFlit);

    for (uint32_t i = 0; i < DUMMY_MEM_SIZE; i++)
    {
        dataFlits[i] = 0;
    }
    ID00001001_readData(DUMMY_2, dataFlits, DUMMY_MEM_SIZE, 0);
    printf("Data in Dummy 2:\n");

    for (uint32_t i = 0; i < DUMMY_MEM_SIZE; i++)
    {
        printf("Data[%i]: %x ", i, dataFlits[i]);
    }
    printf("\n");*/

    LMS7002M_t *lms = LMS7002M_create(spidev_interface_transact);
    LMS7002M_reset(lms);
    LMS7002M_set_spi_mode(lms, 4); //set 4-wire spi before reading back

    int ret = 0;
    double actualRate = 0.0;
    ret = LMS7002M_set_lo_freq(lms, LMS_TX, REF_FREQ, 2.500e9, &actualRate);


    printf("Waiting to start\n");
	while(1){
		if(start_state != 0){
				ID00004003_readData(AIP_UP_0_BASE, data, 5,0);

				printf("\n The opcode in memory[0]: %lx\n", data[0]);
				printf("\n The data in memory[1]: %lx\n", data[1]);
				printf("\n The data in memory[2]: %lx\n", data[2]);
                printf("\n The data in memory[3]: %lx\n", data[3]);
                printf("\n The data in memory[4]: %lx\n", data[4]);

				//spidev_interface_transact(0xA1A10000, 1);
				//LMS7002M_set_nco_freq(NULL, 0, 0, 0.0);

		   start_state = 0;
		}
    }


    return 0;
}


void int_setup(void) {
    IOWR_ALTERA_AVALON_PIO_IRQ_MASK(INT_IP_S0_BASE, 0x07);
    IOWR_ALTERA_AVALON_PIO_EDGE_CAP(INT_IP_S0_BASE, 0x00);

    alt_ic_isr_register(
        INT_IP_S0_IRQ_INTERRUPT_CONTROLLER_ID,
        INT_IP_S0_IRQ,
		int_isr,
        (void *) INT_IP_S0_BASE,
        0x00);

}

void int_isr(void * context) {
    alt_u32 base = (alt_u32)context;

    alt_u32 edge_status = IORD_ALTERA_AVALON_PIO_EDGE_CAP(base);

    if(edge_status) {
        start_state = 1;

        printf("INT DETECTED!\n");

        for(int i = 0; i < 32; i++) {
            if(edge_status & (1 << i)) {
                printf("Line %d activated!\n", i);
            }
        }
        IOWR_ALTERA_AVALON_PIO_EDGE_CAP(base, edge_status);
    }
}



void start_setup(void){

		IOWR_ALTERA_AVALON_PIO_IRQ_MASK(START_UP_BASE, 0x01);
		IOWR_ALTERA_AVALON_PIO_EDGE_CAP(START_UP_BASE, 0x00);
		void * edge_val_ptr;
		edge_val_ptr = (void *) &edge_val;

		alt_ic_isr_register(
				START_UP_IRQ_INTERRUPT_CONTROLLER_ID,
				START_UP_IRQ,
				start_isr,
				edge_val_ptr,
				0x00);
}


void start_isr(void * context){

		volatile int * edge_ptr;
		edge_ptr = (volatile int *) context;

		*edge_ptr = IORD_ALTERA_AVALON_PIO_EDGE_CAP(START_UP_BASE);

		IOWR_ALTERA_AVALON_PIO_EDGE_CAP(START_UP_BASE, 0);

#ifdef 	PRINTF
		printf("------------start DETECTED ------------ \n");
#endif
		start_state=1;
}

