#include <stdio.h>
#include "system.h"
#include "altera_avalon_pio_regs.h"
#include "sys/alt_irq.h"
#include <unistd.h>
#include "aip.h"
#include "ID0000100A_gpio.h"

// **************************************************************
#include <LMS7002M/LMS7002M.h>
//#include <LMS7002M/LMS7002M_logger.h>

//#include <stdlib.h>

//#include <stdint.h>
//#include <string.h>


#include "platform.h"
#include "parser.h"
#include "reinterpret.h"
#include "LMS7002M_set_work_mode.h"




// Procesador

        #include "sys/alt_timestamp.h"
        #include <sys/alt_alarm.h>
 
         #include "altera_avalon_timer_regs.h"
 
       
    
#include <sys/time.h>
#include <LMS7002M/LMS7002M_time.h>
 
//***************************************************************************************
#define GPIO_PORT AIP_0_BASE
#define FLITS_AIP 128
#define REF_FREQ (61.44e6/2)

volatile int edge_val = 0;
volatile int start_state = 0;
//void int_isr(void * context);
void start_isr(void * context);
//void int_setup();
void start_setup();

#include "Tune_Filter_8051.h"


int main(void)
{ alt_timestamp_start();
    int ts_status;
    unsigned int ts_freq;
    alt_timestamp_type ts0, ts1;
    struct timeval tv0, tv1;
    long long lms_t0, lms_t1;
    int i;
    printf("Hello from Nios II!\n");

    for (i = 0; i < 10; i++)
    {
        lms_t0 = LMS7_time_now();
        printf("Current time: %lld ticks\n", lms_t0);
        LMS7_sleep_for(LMS7_time_tps()/100);   // 10 ms
        lms_t1 = LMS7_time_now();

        printf("[%d] delta 10ms = %lld\n", i, lms_t1 - lms_t0);
    }

	start_state = 0;
    uint32_t dataFlit = 0;
   // uint32_t dataFlits[DUMMY_MEM_SIZE];

    uint32_t data[FLITS_AIP];

    //int_setup();
    start_setup();
    ID0000100A_init(GPIO_PORT);
    ID0000100A_GPIO_MODER_Write(GPIO_PORT,0x00000001);
   // ID0000100A_startIP(GPIO_PORT);

   // reset inactivo
    uint32_t dataO = 1;
    ID0000100A_wrGPIO_OUTDATA(GPIO_PORT,&dataO,1,0);



    // ********* INICIALIZACION *****************************

   int ret = 0;
   
    LMS7002M_t *lms = LMS7002M_create(spidev_interface_transact);
    LMS7002M_reset(lms);
    LMS7002M_set_spi_mode(lms, 4); //set 4-wire spi before reading 


    //  test
    LMS7002M_set_lo_freq(lms, 0, 30720000, 1e9, NULL); 
    LMS7002M_set_work_mode(lms);
    LMS7002M_set_data_clock(lms, 30720000, 60e6, NULL); 
    
   // set_work_mode(lms);


    uint32_t opcode;
    size_t buffer_size;
    Geric_Parameter buffer[5];   
    double data_pointer;    
    
//int sw = 0;
    printf("Waiting 'start'\n");
	while(1){
		if(start_state != 0){
            unsigned char txbuf[4];

//             // if (sw == 0){
           
              txbuf[0] = 0x80;
              txbuf[1] = 0x17;
              txbuf[2] = 0x11;
              txbuf[3] = 0x33;
//             // sw = 1; 
//             // printf("sw a 1\n");   }
            // else {
            // txbuf[0] = 0x80;
            // txbuf[1] = 0x17;
            // txbuf[2] = 0x11;
            // txbuf[3] = 0x44;
            // sw = 0;
            // printf("sw a 0\n");
            // }

            // spi_write_API(txbuf, 4, 1);

				ID00004003_readData(AIP_UP_0_BASE, data, FLITS_AIP, 0);
                
                opcode = data[0];

               // buffer[0] no esta en uso
               
                uint8_t Group_ID = opcode & 31;  
                
                switch (Group_ID) {
                    case CREATE_NUM: 
                        buffer_size = 1;
                         // LMS7002M_spi_transact_t
                        break;
                    case ONE_PARAM_LMS7002M_T_NUM:
                        buffer_size = 1;
                        // LMS7002M_t *
                        break;
                    case SPI_WRITE_NUM:
                        buffer_size = 3;
                        buffer[1].value.const_int = u32_to_int32(data[1]);   // cast to int
                        buffer[2].value.const_int = u32_to_int32(data[2]);
                        // LMS7002M_t *, const int, const int
                        break;
                    case SPI_CONFIG_NUM:
                        buffer_size = 2;
                        buffer[1].value.const_int = u32_to_int32(data[1]);   // cast to int
                        // LMS7002M_t *, const int
                        break;
                    case INI_NUM:
                        buffer_size = 2;
                        buffer[1].value.enum_type = u32_to_char(data[1]);
                        // LMS7002M_t *, const char *
                        break;
                    case CONFIGURE_LML_PORT_NUM:
                        buffer_size = 4;
                        buffer[1].value.const_port = data[1];
                        buffer[2].value.const_dir = data[2];
                        buffer[3].value.const_int = u32_to_int32(data[3]);
                         // LMS7002M_t *, const LMS7002M_port_t, const LMS7002M_dir_t, const int
                        break;
                    case ONE_PARAM_CONST_BOOL_NUM:
                        buffer_size = 2;
                        buffer[1].value.b = data[1];
                        // LMS7002M_t *, const bool
                        break;
                    case ONE_PARAM_LMS7002M_CHAN_NUM:
                        buffer_size = 2;
                        buffer[1].value.const_dir = data[1];
                        // LMS7002M_t *, const LMS7002M_dir_t
                        break;
                    case TWO_PARAM_LMS7002M_DIR_INT_NUM:
                        buffer_size = 3;
                        buffer[1].value.const_dir = data[1];
                        int dat = u32_to_int32(data[2]);

                        int res_1 = (dat % 1000);
                        int res_2 = res_1 % 100;
                        buffer[2].value.const_int_arr4[0] = dat / 1000;
                        buffer[2].value.const_int_arr4[1] = res_1 / 100;
                        buffer[2].value.const_int_arr4[2] = res_2 / 10;
                        buffer[2].value.const_int_arr4[3] = res_2 % 10;

                        // LMS7002M_t *, const LMS7002M_dir_t, const int
                        break;
                    case LDO_ENABLE_NUM:
                        buffer_size = 3;
                        buffer[1].value.b = data[1];
                        buffer[2].value.const_int = u32_to_int32(data[2]);
                        // LMS7002M_t *, const bool, const int
                        break;
                    case AFE_ENABLE_NUM:
                        buffer_size = 4;
                        buffer[1].value.const_dir = data[1];
                        buffer[2].value.const_chan = data[2];
                        buffer[3].value.b = data[3];
                        // LMS7002M_t *, const LMS7002M_dir_t, const LMS7002M_chan_t, const bool
                        break;
                    case SET_DATA_CLOCK_NUM:
                        buffer_size = 4;
                        buffer[1].value.d = 30720000; //u32_to_double(data[1], data[2]);
                        buffer[2].value.d = u32_to_double(data[3], data[4]);                   
                        buffer[3].value.d_pointer = &data_pointer;
                        u32_to_double_ptr(data[5], data[6], buffer[3].value.d_pointer);
                        // LMS7002M_t *, const double, const double, double *
                        break;
                    case SET_NCO_FREQ_NUM:
                        buffer_size = 4;
                        buffer[1].value.const_dir = data[1];
                        buffer[2].value.const_chan = data[2];
                        buffer[3].value.d = u32_to_double(data[3], data[4]);
                        // LMS7002M_t *, const LMS7002M_dir_t, const LMS7002M_chan_t, const double
                        break;
                    case SET_GFIR_TAPS_NUM: {
                        buffer_size = 6;
                        buffer[1].value.const_dir = data[1];
                        buffer[2].value.const_chan = data[2];
                        buffer[3].value.const_int = u32_to_int32(data[3]); 
                        buffer[5].value.size = u32_to_int32(data[5]);

                        buffer[4].value.short_p = (short*)&data[6];
                         
                     // LMS7002M_t *
                        break; }
                    case SET_LO_FREQ_NUM:
                        buffer_size = 5;
                        buffer[1].value.const_dir = data[1];
                       // buffer[2].value.d = u32_to_double(data[2], data[3]);
                        buffer[2].value.d = 30720000.0;
                       // printf(" el valor double es: %.10f\n", buffer[2].value.d);
                        buffer[3].value.d = u32_to_double(data[4], data[5]);
                        double factual;
                        buffer[4].value.d_pointer = &factual;
                       // buffer[4].value.d_pointer = u32_to_double_ptr(data[6], data[7]);
                      //  printf(" el valor double pointer es: %.10f\n", *buffer[4].value.d_pointer);
                        // LMS7002M_t *, const LMS7002M_dir_t, const double, const double, double *
                        break;
                    case TWO_PARAM_LMS_CONST_BOOL_NUM:
                        buffer_size = 3;
                        buffer[1].value.const_dir = data[1];
                        buffer[2].value.b = data[2];
                        // LMS7002M_t *, const LMS7002M_dir_t, const bool
                        break;
                    case TWO_PARAM_CHANT_SIZET_NUM:
                        buffer_size = 3;
                        buffer[1].value.const_chan = data[1];
                        buffer[2].value.size = data[2];
                        // LMS7002M_t *, const LMS7002M_chan_t, const size_t
                        break;
                    case SP_TSG_NUM:
                        buffer_size = 4;
                        buffer[1].value.const_chan = data[1];
                        buffer[2].value.const_int = u32_to_int32(data[2]);
                        buffer[3].value.const_int = u32_to_int32(data[3]);
                       // LMS7002M_t *, const LMS7002M_chan_t, const int, const int
                        break;
                    case TXSTP_CORRECTION_NUM:
                        buffer_size = 4;
                        buffer[1].value.const_chan = data[1];
                        buffer[2].value.d = u32_to_double(data[2], data[3]);
                        buffer[3].value.d = u32_to_double(data[4], data[5]);
                        // LMS7002M_t *, const LMS7002M_chan_t, const double, const double
                        break;
                    case RXTSP_NUM:
                        buffer_size = 4;
                        buffer[1].value.const_chan = data[1];
                        buffer[2].value.b = data[2];
                        buffer[3].value.const_int = u32_to_int32(data[3]);
                        // LMS7002M_t *, const LMS7002M_chan_t, const bool, const int
                        break;
                    case SET_PATH_AND_BAND_NUM:
                        buffer_size = 3;
                        buffer[1].value.const_chan = data[1];
                        buffer[2].value.const_int = u32_to_int32(data[2]);
                        // LMS7002M_t *, const LMS7002M_chan_t, const int
                        break;
                    case TBB_LOOP_BACK_ENABLE_NUM:
                        buffer_size = 4;
                        buffer[1].value.const_chan = data[1];
                        buffer[2].value.const_int = u32_to_int32(data[2]);
                        buffer[3].value.b = data[3];
                        // LMS7002M_t *, const LMS7002M_chan_t, const int, const bool
                        break;
                    case BB_FILER_SET_NUM:
                        buffer_size = 4;
                        buffer[1].value.const_chan = data[1];
                        buffer[2].value.d = u32_to_double(data[2], data[3]);
                        buffer[3].value.d_pointer = &data_pointer;
                        u32_to_double_ptr(data[4], data[5], buffer[3].value.d_pointer);
                        // LMS7002M_t *, const LMS7002M_chan_t, const double, double *
                        break;
                    case TRF_RBB_RFE_NUM:
                        buffer_size = 3;
                        buffer[1].value.const_chan = data[1];
                        buffer[2].value.d = u32_to_double(data[2], data[3]);
                        // LMS7002M_t *, const LMS7002M_chan_t, const double
                        break;
                    case READRSSI_NUM:
                        buffer_size = 2;
                        buffer[1].value.const_chan = data[1];
                        // LMS7002M_t *, const LMS7002M_chan_t
                        break;   
                    default:
                        printf("Error: opcode no válido\n");
                        break;
                }
     
                printf("Nuevo opcode enviado\n");
                ret = executeOpcode(lms, opcode, buffer, buffer_size);
                printf("ret = %d\n", ret);
                //int i = LMS7002M_spi_read(lms, 0x0123);
//                 LMS7002M_spi_read(lms, 0x0105);
//                LMS7002M_spi_read(lms, 0x0106);
//                LMS7002M_spi_read(lms, 0x0107);
//                LMS7002M_spi_read(lms, 0x0109);
//                LMS7002M_spi_read(lms, 0x010A);

// LMS7002M_spi_write (lms, 0x0006, 0x0001); 
//LMS7002M_spi_write (lms, 0x0002, 0x0002);
//                LMS7002M_spi_write (lms, 0x0000, 0x0000); 
//                int i = LMS7002M_spi_read(lms, 0x0002);
//               int val = 0x0008 | i ;
//                LMS7002M_spi_write (lms, 0x0002, val); 
//                LMS7002M_spi_write (lms, 0x0002, i);

//                 LMS7002M_spi_write (lms, 0x0000, 0x0000); 
//                 i = LMS7002M_spi_read(lms, 0x0002);
//                val = 0x0008 | i ;
//                LMS7002M_spi_write (lms, 0x0002, val);
//                LMS7002M_spi_write (lms, 0x0002, i);

//                 LMS7002M_spi_write (lms, 0x0000, 0x0005); 
//                 i = LMS7002M_spi_read(lms, 0x0002);
//                val = 0x0008 | i ;
//                LMS7002M_spi_write (lms, 0x0002, val);
//                LMS7002M_spi_write (lms, 0x0002, i);

//                 LMS7002M_spi_write (lms, 0x0000, 0x0003); 
//                i = LMS7002M_spi_read(lms, 0x0002);
//               val = 0x000C | i ;
//                LMS7002M_spi_write (lms, 0x0002, val);
//                LMS7002M_spi_write (lms, 0x0002, i);
// i = 0x0000;
//                while (i == 0x0000){
//                i = LMS7002M_spi_read(lms, 0x0001);
//                }
//                 while (i == 0x00FF){
//                i = LMS7002M_spi_read(lms, 0x0001);
//                }
               


//                 LMS7002M_spi_write (lms, 0x0000, 0x0006); 
//                 i = LMS7002M_spi_read(lms, 0x0002);
//               val = 0x000C | i ;
//                LMS7002M_spi_write (lms, 0x0002, val);
//                LMS7002M_spi_write (lms, 0x0002, i);

//               i = 0x0000;
//                while (i == 0x0000){
//                i = LMS7002M_spi_read(lms, 0x0001);
//                }
//                 while (i == 0x00FF){
//                i = LMS7002M_spi_read(lms, 0x0001);
//                }

//                LMS7002M_spi_write (lms, 0x0006, 0x0000); 

//                LMS7002M_spi_read(lms, 0x0105);
//                LMS7002M_spi_read(lms, 0x0106);
//                LMS7002M_spi_read(lms, 0x0107);
//                LMS7002M_spi_read(lms, 0x0109);
//                LMS7002M_spi_read(lms, 0x010A);
                
              
              //                                                                                                                                                                                                                         
                // LMS7002M_spi_read(lms, 0x0121);
                // LMS7002M_spi_read(lms, 0x0120);
              //  LMS7002M_spi_read(lms, 0x0123);
              //   LMS7002M_spi_read(lms, 0x011C);

		   start_state = 0;
		}
    }
    return 0;
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


