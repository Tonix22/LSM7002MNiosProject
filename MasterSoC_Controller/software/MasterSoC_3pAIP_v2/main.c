#include <stdio.h>
#include "system.h"
#include "altera_avalon_pio_regs.h"
#include "sys/alt_irq.h"
#include <unistd.h>
#include "aip.h"
//#include "ID00001001_dummy.h"

// **************************************************************
#include <LMS7002M/LMS7002M.h>
//#include <LMS7002M/LMS7002M_logger.h>

//#include <stdlib.h>

//#include <stdint.h>
//#include <string.h>


#include "platform.h"
#include "parser.h"
#include "reinterpret.h"



//***************************************************************************************

#define FLITS_AIP 8
#define REF_FREQ (61.44e6/2)

volatile int edge_val = 0;
volatile int start_state=0;
//void int_isr(void * context);
void start_isr(void * context);
//void int_setup();
void start_setup();


int main(void)
{
	start_state = 0;
    uint32_t dataFlit = 0;
   // uint32_t dataFlits[DUMMY_MEM_SIZE];

    uint32_t data[FLITS_AIP];

   // int_setup();
    start_setup();

    // ********* INICIALIZACION *****************************

   int ret = 0;
   
    LMS7002M_t *lms = LMS7002M_create(spidev_interface_transact);
    
    LMS7002M_reset(lms);
    LMS7002M_set_spi_mode(lms, 4); //set 4-wire spi before reading back
    /********************************
     **********LMS STATUS************
     ********************************/

    //read info register
  //  LMS7002M_regs_spi_read(lms, 0x002f);
    //printf("rev 0x%x\n", LMS7002M_regs(lms)->reg_0x002f_rev);
    //printf("ver 0x%x\n", LMS7002M_regs(lms)->reg_0x002f_ver);

    /********************************
     ********* TURN ON CLK **********
     ********************************/
  //  while (!start_state);
  //  start_state = 0;
    //turn the clocks on
    // double actualRate = 0.0;
    // ret = LMS7002M_set_data_clock(lms, REF_FREQ, 61.44e6, &actualRate);
    // if (ret != 0)
    // {
    //     printf("clock tune failure %d\n", ret);
    // }
 //   while (!start_state);
  //  start_state = 0;
    //configure data port directions and data clock rates
   // LMS7002M_configure_lml_port(lms, LMS_PORT1, LMS_TX, 2);
   // LMS7002M_configure_lml_port(lms, LMS_PORT2, LMS_RX, 2);


    /********************************
    **********ENABLE COMPONENTS******
    *********************************/
   // while (!start_state);
    // start_state = 0;
    // LMS7002M_afe_enable(lms, LMS_TX, LMS_CHA, true);
    // LMS7002M_afe_enable(lms, LMS_TX, LMS_CHB, true);
    // LMS7002M_afe_enable(lms, LMS_RX, LMS_CHA, true);
    // LMS7002M_afe_enable(lms, LMS_RX, LMS_CHB, true);
    // LMS7002M_rxtsp_enable(lms, LMS_CHAB, true);
    // LMS7002M_txtsp_enable(lms, LMS_CHAB, true);
    // LMS7002M_rbb_enable(lms, LMS_CHAB, true);
    // LMS7002M_tbb_enable(lms, LMS_CHAB, true);
    // LMS7002M_rfe_enable(lms, LMS_CHAB, true);
    // LMS7002M_trf_enable(lms, LMS_CHAB, true);
    // LMS7002M_sxx_enable(lms, LMS_RX, true);
    // LMS7002M_sxx_enable(lms, LMS_TX, true);

   
    uint32_t opcode;
    size_t buffer_size;
    Geric_Parameter buffer[5];   
    double data_pointer;    
    

    printf("Waiting to start\n");
	while(1){
		if(start_state != 0){
				ID00004003_readData(AIP_UP_0_BASE, data, FLITS_AIP, 0);
                
                opcode = data[0];
				// printf("\n The opcode in memory[0]: %lx\n", opcode);
				// printf("\n The data in memory[1]: %lx\n", data[1]);
				// printf("\n The data in memory[2]: %lx\n", data[2]);
                // printf("\n The data in memory[3]: %lx\n", data[3]);
                // printf("\n The data in memory[4]: %lx\n", data[4]);
                // printf("\n The data in memory[5]: %lx\n", data[5]);
                // printf("\n The data in memory[6]: %lx\n", data[6]);
                // printf("\n The data in memory[7]: %lx\n", data[7]);

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
                        buffer[1].value.sint = u32_to_int32(data[1]);   // cast to int
                        buffer[2].value.sint = u32_to_int32(data[2]);
                        // LMS7002M_t *, const int, const int
                        break;
                    case SPI_CONFIG_NUM:
                        buffer_size = 2;
                        buffer[1].value.sint = u32_to_int32(data[1]);   // cast to int
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
                        buffer[3].value.sint = u32_to_int32(data[3]);
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
                        buffer[2].value.sint = u32_to_int32(data[2]);
                        // LMS7002M_t *, const LMS7002M_dir_t, const int
                        break;
                    case LDO_ENABLE_NUM:
                        buffer_size = 3;
                        buffer[1].value.b = data[1];
                        buffer[2].value.sint = u32_to_int32(data[2]);
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
                        buffer[1].value.d = u32_to_double(data[1], data[2]);
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
                    case SET_GFIR_TAPS_NUM:
                        buffer_size = 1;
                        // LMS7002M_t *
                        break;
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
                        buffer[2].value.sint = u32_to_int32(data[2]);
                        buffer[3].value.sint = u32_to_int32(data[3]);
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
                        buffer[3].value.sint = u32_to_int32(data[3]);
                        // LMS7002M_t *, const LMS7002M_chan_t, const bool, const int
                        break;
                    case SET_PATH_AND_BAND_NUM:
                        buffer_size = 3;
                        buffer[1].value.const_chan = data[1];
                        buffer[2].value.sint = u32_to_int32(data[2]);
                        // LMS7002M_t *, const LMS7002M_chan_t, const int
                        break;
                    case TBB_LOOP_BACK_ENABLE_NUM:
                        buffer_size = 4;
                        buffer[1].value.const_chan = data[1];
                        buffer[2].value.sint = u32_to_int32(data[2]);
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
                        printf("Error: Unsupported opcode\n");
                        break;
                }
                
                ret = executeOpcode(lms, opcode, buffer, buffer_size);
                //int i = LMS7002M_spi_read(lms, 0x0123);
               // LMS7002M_spi_write (lms, 0x0020, 0xFFFE); 
               // LMS7002M_spi_write (lms, 0x011C, 0xA941);
              //  LMS7002M_spi_write (lms, 0x011E, 0x0984);
              //  LMS7002M_spi_write (lms, 0x011D, 0x0000);
              //  LMS7002M_spi_write (lms, 0x011F, 0x3600);
              //  LMS7002M_spi_write (lms, 0x0121, 0x8638);
                // LMS7002M_spi_read(lms, 0x0121);
                // LMS7002M_spi_read(lms, 0x0120);
              //  LMS7002M_spi_read(lms, 0x0123);
                // LMS7002M_spi_read(lms, 0x011C);

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

