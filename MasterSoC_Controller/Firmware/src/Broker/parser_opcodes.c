#include "parser_typedefs.h"
#include "parser.h"
#include "LMS7002M_filter_cal.h"
#include "LMS7002M_set_work_mode.h"
/* Auto-generated file: Grouped OpcodeDescriptor arrays and global all_descriptors array */

/* Opcode descriptors for group CREATE_NUM */
OpcodeDescriptor CREATE_NUM_opcodes[] = {
    {
        .opcode = 0x0,
        .QT_Label = "Internal",
        .num_params = 1,
        .args = NULL, // LMS7002M_spi_transact_t
        .callback = (void*)LMS7002M_create
    },
};

/* Opcode descriptors for group ONE_PARAM_LMS7002M_T_NUM */
OpcodeDescriptor ONE_PARAM_LMS7002M_T_NUM_opcodes[] = {
    {
        .opcode = 0x1,
        .QT_Label = "Regs",
        .num_params = 1,
        .args = NULL, // LMS7002M_t *
        .callback = (void*)LMS7002M_regs
    },
    {
        .opcode = 0x21,
        .QT_Label = "Internal",
        .num_params = 1,
        .args = NULL, // LMS7002M_t *
        .callback = (void*)LMS7002M_destroy
    },
    {
        .opcode = 0x41,
        .QT_Label = "Regs",
        .num_params = 1,
        .args = NULL, // LMS7002M_t *
        .callback = (void*)LMS7002M_regs_to_rfic
    },
    {
        .opcode = 0x61,
        .QT_Label = "Regs",
        .num_params = 1,
        .args = NULL, // LMS7002M_t *
        .callback = (void*)LMS7002M_rfic_to_regs
    },
    {
        .opcode = 0x81,
        .QT_Label = "Power",
        .num_params = 1,
        .args = NULL, // LMS7002M_t *
        .callback = (void*)LMS7002M_reset
    },
    {
        .opcode = 0xA1,
        .QT_Label = "Power",
        .num_params = 1,
        .args = NULL, // LMS7002M_t *
        .callback = (void*)LMS7002M_power_down
    },
    {
        .opcode = 0xC1,
        .QT_Label = "loopback",
        .num_params = 1,
        .args = NULL, // LMS7002M_t *
        .callback = (void*)LMS7002M_setup_digital_loopback
    },
    {
        .opcode = 0xE1,
        .QT_Label = "WORK MODE",
        .num_params = 1,
        .args = NULL, // LMS7002M_t *
        .callback = (void*)LMS7002M_set_work_mode
    },
};

/* Opcode descriptors for group SPI_WRITE_NUM */
OpcodeDescriptor SPI_WRITE_NUM_opcodes[] = {
    {
        .opcode = 0x2,
        .QT_Label = "SPI",
        .num_params = 3,
        .args = NULL, // LMS7002M_t *, const int, const int
        .callback = (void*)LMS7002M_spi_write
    },
};

/* Opcode descriptors for group SPI_CONFIG_NUM */
OpcodeDescriptor SPI_CONFIG_NUM_opcodes[] = {
    {
        .opcode = 0x3,
        .QT_Label = "SPI",
        .num_params = 2,
        .args = NULL, // LMS7002M_t *, const int
        .callback = (void*)LMS7002M_spi_read
    },
    {
        .opcode = 0x23,
        .QT_Label = "SPI",
        .num_params = 2,
        .args = NULL, // LMS7002M_t *, const int
        .callback = (void*)LMS7002M_regs_spi_write
    },
    {
        .opcode = 0x43,
        .QT_Label = "SPI",
        .num_params = 2,
        .args = NULL, // LMS7002M_t *, const int
        .callback = (void*)LMS7002M_regs_spi_read
    },
    {
        .opcode = 0x63,
        .QT_Label = "SPI",
        .num_params = 2,
        .args = NULL, // LMS7002M_t *, const int
        .callback = (void*)LMS7002M_set_spi_mode
    },
};

/* Opcode descriptors for group INI_NUM */
OpcodeDescriptor INI_NUM_opcodes[] = {
    {
        .opcode = 0x4,
        .QT_Label = "INI",
        .num_params = 2,
        .args = NULL, // LMS7002M_t *, const char *
        .callback = (void*)LMS7002M_dump_ini
    },
    {
        .opcode = 0x24,
        .QT_Label = "INI",
        .num_params = 2,
        .args = NULL, // LMS7002M_t *, const char *
        .callback = (void*)LMS7002M_load_ini
    },
};

/* Opcode descriptors for group CONFIGURE_LML_PORT_NUM */
OpcodeDescriptor CONFIGURE_LML_PORT_NUM_opcodes[] = {
    {
        .opcode = 0x5,
        .QT_Label = "loopback",
        .num_params = 4,
        .args = NULL, // LMS7002M_t *, const LMS7002M_port_t, const LMS7002M_dir_t, const int
        .callback = (void*)LMS7002M_configure_lml_port
    },
};

/* Opcode descriptors for group ONE_PARAM_CONST_BOOL_NUM */
OpcodeDescriptor ONE_PARAM_CONST_BOOL_NUM_opcodes[] = {
    {
        .opcode = 0x6,
        .QT_Label = "Other",
        .num_params = 2,
        .args = NULL, // LMS7002M_t *, const bool
        .callback = (void*)LMS7002M_invert_fclk
    },
    {
        .opcode = 0x26,
        .QT_Label = "BUFF",
        .num_params = 2,
        .args = NULL, // LMS7002M_t *, const bool
        .callback = (void*)LMS7002M_xbuf_share_tx
    },
    {
        .opcode = 0x46,
        .QT_Label = "BUFF",
        .num_params = 2,
        .args = NULL, // LMS7002M_t *, const bool
        .callback = (void*)LMS7002M_xbuf_enable_bias
    },
    {
        .opcode = 0x66,
        .QT_Label = "Other",
        .num_params = 2,
        .args = NULL, // LMS7002M_t *, const bool
        .callback = (void*)LMS7002M_sxt_to_sxr
    },
};

/* Opcode descriptors for group ONE_PARAM_LMS7002M_CHAN_NUM */
OpcodeDescriptor ONE_PARAM_LMS7002M_CHAN_NUM_opcodes[] = {
    {
        .opcode = 0x7,
        .QT_Label = "BUFF",
        .num_params = 2,
        .args = NULL, // LMS7002M_t *, const LMS7002M_dir_t
        .callback = (void*)LMS7002M_reset_lml_fifo
    },
    {
        .opcode = 0x27,
        .QT_Label = "MAC",
        .num_params = 2,
        .args = NULL, // LMS7002M_t *, const LMS7002M_dir_t
        .callback = (void*)LMS7002M_set_mac_dir
    },
    {
        .opcode = 0x47,
        .QT_Label = "MAC",
        .num_params = 2,
        .args = NULL, // LMS7002M_t *, const LMS7002M_chan_t
        .callback = (void*)LMS7002M_set_mac_ch
    },
    {
        .opcode = 0x67,
        .QT_Label = "Enable Channel",
        .num_params = 2,
        .args = NULL, // LMS7002M_t *, const LMS7002M_chan_t
        .callback = (void*)LMS7002M_txtsp_tsg_tone
    },
    {
        .opcode = 0x87,
        .QT_Label = "Enable Channel",
        .num_params = 2,
        .args = NULL, // LMS7002M_t *, const LMS7002M_chan_t
        .callback = (void*)LMS7002M_rxtsp_tsg_tone
    },
    {
        .opcode = 0xA7,
        .QT_Label = "Enable Channel",
        .num_params = 2,
        .args = NULL, // LMS7002M_t *, const LMS7002M_chan_t
        .callback = (void*)rx_cal_init
    },
    {
        .opcode = 0xC7,
        .QT_Label = "Enable Channel",
        .num_params = 2,
        .args = NULL, // LMS7002M_t *, const LMS7002M_chan_t
        .callback = (void*)tx_cal_init
    },
};

/* Opcode descriptors for group TWO_PARAM_LMS7002M_DIR_INT_NUM */
OpcodeDescriptor TWO_PARAM_LMS7002M_DIR_INT_NUM_opcodes[] = {
    {
        .opcode = 0x8,
        .QT_Label = "IQ",
        .num_params = 3,
        .args = NULL, // LMS7002M_t *, const LMS7002M_dir_t, const int
        .callback = (void*)LMS7002M_set_diq_mux
    },
};

/* Opcode descriptors for group LDO_ENABLE_NUM */
OpcodeDescriptor LDO_ENABLE_NUM_opcodes[] = {
    {
        .opcode = 0x9,
        .QT_Label = "Enable",
        .num_params = 3,
        .args = NULL, // LMS7002M_t *, const bool, const int
        .callback = (void*)LMS7002M_ldo_enable
    },
};

/* Opcode descriptors for group AFE_ENABLE_NUM */
OpcodeDescriptor AFE_ENABLE_NUM_opcodes[] = {
    {
        .opcode = 0xA,
        .QT_Label = "Enable",
        .num_params = 4,
        .args = NULL, // LMS7002M_t *, const LMS7002M_dir_t, const LMS7002M_chan_t, const bool
        .callback = (void*)LMS7002M_afe_enable
    },
};

/* Opcode descriptors for group SET_DATA_CLOCK_NUM */
OpcodeDescriptor SET_DATA_CLOCK_NUM_opcodes[] = {
    {
        .opcode = 0xB,
        .QT_Label = "Sampling",
        .num_params = 4,
        .args = NULL, // LMS7002M_t *, const double, const double, double *
        .callback = (void*)LMS7002M_set_data_clock
    },
};

/* Opcode descriptors for group SET_NCO_FREQ_NUM */
OpcodeDescriptor SET_NCO_FREQ_NUM_opcodes[] = {
    {
        .opcode = 0xC,
        .QT_Label = "Frequency Tunning",
        .num_params = 4,
        .args = NULL, // LMS7002M_t *, const LMS7002M_dir_t, const LMS7002M_chan_t, const double
        .callback = (void*)LMS7002M_set_nco_freq
    },
};

/* Opcode descriptors for group SET_GFIR_TAPS_NUM */
OpcodeDescriptor SET_GFIR_TAPS_NUM_opcodes[] = {
    {
        .opcode = 0xD,
        .QT_Label = "FIR",
        .num_params = 1,
        .args = NULL, // LMS7002M_t *
        .callback = (void*)LMS7002M_set_gfir_taps
    },
};

/* Opcode descriptors for group SET_LO_FREQ_NUM */
OpcodeDescriptor SET_LO_FREQ_NUM_opcodes[] = {
    {
        .opcode = 0xE,
        .QT_Label = "Frequency Tunning",
        .num_params = 5,
        .args = NULL, // LMS7002M_t *, const LMS7002M_dir_t, const double, const double, double *
        .callback = (void*)LMS7002M_set_lo_freq
    },
};

/* Opcode descriptors for group TWO_PARAM_LMS_CONST_BOOL_NUM */
OpcodeDescriptor TWO_PARAM_LMS_CONST_BOOL_NUM_opcodes[] = {
    {
        .opcode = 0xF,
        .QT_Label = "Enable",
        .num_params = 3,
        .args = NULL, // LMS7002M_t *, const LMS7002M_dir_t, const bool
        .callback = (void*)LMS7002M_sxx_enable
    },
    {
        .opcode = 0x2F,
        .QT_Label = "Enable",
        .num_params = 3,
        .args = NULL, // LMS7002M_t *, const LMS7002M_chan_t, const bool
        .callback = (void*)LMS7002M_txtsp_enable
    },
    {
        .opcode = 0x4F,
        .QT_Label = "Enable",
        .num_params = 3,
        .args = NULL, // LMS7002M_t *, const LMS7002M_chan_t, const bool
        .callback = (void*)LMS7002M_tbb_enable
    },
    {
        .opcode = 0x6F,
        .QT_Label = "Enable",
        .num_params = 3,
        .args = NULL, // LMS7002M_t *, const LMS7002M_chan_t, const bool
        .callback = (void*)LMS7002M_trf_enable
    },
    {
        .opcode = 0x8F,
        .QT_Label = "Enable",
        .num_params = 3,
        .args = NULL, // LMS7002M_t *, const LMS7002M_chan_t, const bool
        .callback = (void*)LMS7002M_trf_enable_loopback
    },
    {
        .opcode = 0xAF,
        .QT_Label = "Enable",
        .num_params = 3,
        .args = NULL, // LMS7002M_t *, const LMS7002M_chan_t, const bool
        .callback = (void*)LMS7002M_rxtsp_enable
    },
    {
        .opcode = 0xCF,
        .QT_Label = "Enable",
        .num_params = 3,
        .args = NULL, // LMS7002M_t *, const LMS7002M_chan_t, const bool
        .callback = (void*)LMS7002M_rbb_enable
    },
    {
        .opcode = 0xEF,
        .QT_Label = "Test",
        .num_params = 3,
        .args = NULL, // LMS7002M_t *, const LMS7002M_chan_t, const bool
        .callback = (void*)LMS7002M_rbb_set_test_out
    },
    {
        .opcode = 0x10F,
        .QT_Label = "Enable",
        .num_params = 3,
        .args = NULL, // LMS7002M_t *, const LMS7002M_chan_t, const bool
        .callback = (void*)LMS7002M_rfe_enable
    },
};

/* Opcode descriptors for group TWO_PARAM_CHANT_SIZET_NUM */
OpcodeDescriptor TWO_PARAM_CHANT_SIZET_NUM_opcodes[] = {
    {
        .opcode = 0x10,
        .QT_Label = "Sampling",
        .num_params = 3,
        .args = NULL, // LMS7002M_t *, const LMS7002M_chan_t, const size_t
        .callback = (void*)LMS7002M_txtsp_set_interp
    },
    {
        .opcode = 0x30,
        .QT_Label = "Sampling",
        .num_params = 3,
        .args = NULL, // LMS7002M_t *, const LMS7002M_chan_t, const size_t
        .callback = (void*)LMS7002M_rxtsp_set_decim
    },
};

/* Opcode descriptors for group SP_TSG_NUM */
OpcodeDescriptor SP_TSG_NUM_opcodes[] = {
    {
        .opcode = 0x11,
        .QT_Label = "IQ",
        .num_params = 4,
        .args = NULL, // LMS7002M_t *, const LMS7002M_chan_t, const int, const int
        .callback = (void*)LMS7002M_txtsp_tsg_const
    },
    {
        .opcode = 0x31,
        .QT_Label = "IQ",
        .num_params = 4,
        .args = NULL, // LMS7002M_t *, const LMS7002M_chan_t, const int, const int
        .callback = (void*)LMS7002M_rxtsp_tsg_const
    },
};

/* Opcode descriptors for group TXSTP_CORRECTION_NUM */
OpcodeDescriptor TXSTP_CORRECTION_NUM_opcodes[] = {
    {
        .opcode = 0x12,
        .QT_Label = "Calibrate",
        .num_params = 4,
        .args = NULL, // LMS7002M_t *, const LMS7002M_chan_t, const double, const double
        .callback = (void*)LMS7002M_txtsp_set_dc_correction
    },
    {
        .opcode = 0x32,
        .QT_Label = "Calibrate",
        .num_params = 4,
        .args = NULL, // LMS7002M_t *, const LMS7002M_chan_t, const double, const double
        .callback = (void*)LMS7002M_txtsp_set_iq_correction
    },
};

/* Opcode descriptors for group RXTSP_NUM */
OpcodeDescriptor RXTSP_NUM_opcodes[] = {
    {
        .opcode = 0x13,
        .QT_Label = "Calibrate",
        .num_params = 4,
        .args = NULL, // LMS7002M_t *, const LMS7002M_chan_t, const bool, const int
        .callback = (void*)LMS7002M_rxtsp_set_dc_correction
    },
    {
        .opcode = 0x33,
        .QT_Label = "Calibrate",
        .num_params = 4,
        .args = NULL, // LMS7002M_t *, const LMS7002M_chan_t, const bool, const int
        .callback = (void*)LMS7002M_rxtsp_set_iq_correction
    },
};

/* Opcode descriptors for group SET_PATH_AND_BAND_NUM */
OpcodeDescriptor SET_PATH_AND_BAND_NUM_opcodes[] = {
    {
        .opcode = 0x14,
        .QT_Label = "Band",
        .num_params = 3,
        .args = NULL, // LMS7002M_t *, const LMS7002M_chan_t, const int
        .callback = (void*)LMS7002M_tbb_set_path
    },
    {
        .opcode = 0x34,
        .QT_Label = "Test",
        .num_params = 3,
        .args = NULL, // LMS7002M_t *, const LMS7002M_chan_t, const int
        .callback = (void*)LMS7002M_tbb_set_test_in
    },
    {
        .opcode = 0x54,
        .QT_Label = "Band",
        .num_params = 3,
        .args = NULL, // LMS7002M_t *, const LMS7002M_chan_t, const int
        .callback = (void*)LMS7002M_trf_select_band
    },
    {
        .opcode = 0x74,
        .QT_Label = "Band",
        .num_params = 3,
        .args = NULL, // LMS7002M_t *, const LMS7002M_chan_t, const int
        .callback = (void*)LMS7002M_rbb_set_path
    },
    {
        .opcode = 0x94,
        .QT_Label = "Enable Channel",
        .num_params = 3,
        .args = NULL, // LMS7002M_t *, const LMS7002M_chan_t, const int
        .callback = (void*)LMS7002M_rfe_set_path
    },
};

/* Opcode descriptors for group TBB_LOOP_BACK_ENABLE_NUM */
OpcodeDescriptor TBB_LOOP_BACK_ENABLE_NUM_opcodes[] = {
    {
        .opcode = 0x15,
        .QT_Label = "IQ",
        .num_params = 4,
        .args = NULL, // LMS7002M_t *, const LMS7002M_chan_t, const int, const bool
        .callback = (void*)LMS7002M_tbb_enable_loopback
    },
};

/* Opcode descriptors for group BB_FILER_SET_NUM */
OpcodeDescriptor BB_FILER_SET_NUM_opcodes[] = {
    {
        .opcode = 0x16,
        .QT_Label = "Calibrate",
        .num_params = 4,
        .args = NULL, // LMS7002M_t *, const LMS7002M_chan_t, const double, double *
        .callback = (void*)LMS7002M_tbb_set_filter_bw
    },
    {
        .opcode = 0x36,
        .QT_Label = "Calibrate",
        .num_params = 4,
        .args = NULL, // LMS7002M_t *, const LMS7002M_chan_t, const double, double *
        .callback = (void*)LMS7002M_rbb_set_filter_bw
    },
};

/* Opcode descriptors for group TRF_RBB_RFE_NUM */
OpcodeDescriptor TRF_RBB_RFE_NUM_opcodes[] = {
    {
        .opcode = 0x17,
        .QT_Label = "Frequency Tunning",
        .num_params = 3,
        .args = NULL, // LMS7002M_t *, const LMS7002M_chan_t, const double
        .callback = (void*)LMS7002M_txtsp_set_freq
    },
    {
        .opcode = 0x37,
        .QT_Label = "Frequency Tunning",
        .num_params = 3,
        .args = NULL, // LMS7002M_t *, const LMS7002M_chan_t, const double
        .callback = (void*)LMS7002M_rxtsp_set_freq
    },
    {
        .opcode = 0x57,
        .QT_Label = "Gain",
        .num_params = 3,
        .args = NULL, // LMS7002M_t *, const LMS7002M_chan_t, const double
        .callback = (void*)LMS7002M_trf_set_pad
    },
    {
        .opcode = 0x77,
        .QT_Label = "Gain",
        .num_params = 3,
        .args = NULL, // LMS7002M_t *, const LMS7002M_chan_t, const double
        .callback = (void*)LMS7002M_trf_set_loopback_pad
    },
    {
        .opcode = 0x97,
        .QT_Label = "Gain",
        .num_params = 3,
        .args = NULL, // LMS7002M_t *, const LMS7002M_chan_t, const double
        .callback = (void*)LMS7002M_rbb_set_pga
    },
    {
        .opcode = 0xB7,
        .QT_Label = "Gain",
        .num_params = 3,
        .args = NULL, // LMS7002M_t *, const LMS7002M_chan_t, const double
        .callback = (void*)LMS7002M_rfe_set_lna
    },
    {
        .opcode = 0xD7,
        .QT_Label = "Gain",
        .num_params = 3,
        .args = NULL, // LMS7002M_t *, const LMS7002M_chan_t, const double
        .callback = (void*)LMS7002M_rfe_set_loopback_lna
    },
    {
        .opcode = 0xF7,
        .QT_Label = "Gain",
        .num_params = 3,
        .args = NULL, // LMS7002M_t *, const LMS7002M_chan_t, const double
        .callback = (void*)LMS7002M_rfe_set_tia
    },
};

/* Opcode descriptors for group READRSSI_NUM */
OpcodeDescriptor READRSSI_NUM_opcodes[] = {
    {
        .opcode = 0x18,
        .QT_Label = "RSSI",
        .num_params = 2,
        .args = NULL, // LMS7002M_t *, const LMS7002M_chan_t
        .callback = (void*)LMS7002M_rxtsp_read_rssi
    },
};

// Create a global array of all OpcodeDescriptor arrays
OpcodeDescriptor* all_descriptors[] = {
    CREATE_NUM_opcodes,
    ONE_PARAM_LMS7002M_T_NUM_opcodes,
    SPI_WRITE_NUM_opcodes,
    SPI_CONFIG_NUM_opcodes,
    INI_NUM_opcodes,
    CONFIGURE_LML_PORT_NUM_opcodes,
    ONE_PARAM_CONST_BOOL_NUM_opcodes,
    ONE_PARAM_LMS7002M_CHAN_NUM_opcodes,
    TWO_PARAM_LMS7002M_DIR_INT_NUM_opcodes,
    LDO_ENABLE_NUM_opcodes,
    AFE_ENABLE_NUM_opcodes,
    SET_DATA_CLOCK_NUM_opcodes,
    SET_NCO_FREQ_NUM_opcodes,
    SET_GFIR_TAPS_NUM_opcodes,
    SET_LO_FREQ_NUM_opcodes,
    TWO_PARAM_LMS_CONST_BOOL_NUM_opcodes,
    TWO_PARAM_CHANT_SIZET_NUM_opcodes,
    SP_TSG_NUM_opcodes,
    TXSTP_CORRECTION_NUM_opcodes,
    RXTSP_NUM_opcodes,
    SET_PATH_AND_BAND_NUM_opcodes,
    TBB_LOOP_BACK_ENABLE_NUM_opcodes,
    BB_FILER_SET_NUM_opcodes,
    TRF_RBB_RFE_NUM_opcodes,
    READRSSI_NUM_opcodes,
};

//Define sizes for each group
const size_t group_sizes[] = {
    sizeof(CREATE_NUM_opcodes) / sizeof(OpcodeDescriptor),
    sizeof(ONE_PARAM_LMS7002M_T_NUM_opcodes) / sizeof(OpcodeDescriptor),
    sizeof(SPI_WRITE_NUM_opcodes) / sizeof(OpcodeDescriptor),
    sizeof(SPI_CONFIG_NUM_opcodes) / sizeof(OpcodeDescriptor),
    sizeof(INI_NUM_opcodes) / sizeof(OpcodeDescriptor),
    sizeof(CONFIGURE_LML_PORT_NUM_opcodes) / sizeof(OpcodeDescriptor),
    sizeof(ONE_PARAM_CONST_BOOL_NUM_opcodes) / sizeof(OpcodeDescriptor),
    sizeof(ONE_PARAM_LMS7002M_CHAN_NUM_opcodes) / sizeof(OpcodeDescriptor),
    sizeof(TWO_PARAM_LMS7002M_DIR_INT_NUM_opcodes) / sizeof(OpcodeDescriptor),
    sizeof(LDO_ENABLE_NUM_opcodes) / sizeof(OpcodeDescriptor),
    sizeof(AFE_ENABLE_NUM_opcodes) / sizeof(OpcodeDescriptor),
    sizeof(SET_DATA_CLOCK_NUM_opcodes) / sizeof(OpcodeDescriptor),
    sizeof(SET_NCO_FREQ_NUM_opcodes) / sizeof(OpcodeDescriptor),
    sizeof(SET_GFIR_TAPS_NUM_opcodes) / sizeof(OpcodeDescriptor),
    sizeof(SET_LO_FREQ_NUM_opcodes) / sizeof(OpcodeDescriptor),
    sizeof(TWO_PARAM_LMS_CONST_BOOL_NUM_opcodes) / sizeof(OpcodeDescriptor),
    sizeof(TWO_PARAM_CHANT_SIZET_NUM_opcodes) / sizeof(OpcodeDescriptor),
    sizeof(SP_TSG_NUM_opcodes) / sizeof(OpcodeDescriptor),
    sizeof(TXSTP_CORRECTION_NUM_opcodes) / sizeof(OpcodeDescriptor),
    sizeof(RXTSP_NUM_opcodes) / sizeof(OpcodeDescriptor),
    sizeof(SET_PATH_AND_BAND_NUM_opcodes) / sizeof(OpcodeDescriptor),
    sizeof(TBB_LOOP_BACK_ENABLE_NUM_opcodes) / sizeof(OpcodeDescriptor),
    sizeof(BB_FILER_SET_NUM_opcodes) / sizeof(OpcodeDescriptor),
    sizeof(TRF_RBB_RFE_NUM_opcodes) / sizeof(OpcodeDescriptor),
    sizeof(READRSSI_NUM_opcodes) / sizeof(OpcodeDescriptor),
};

/*
 * @brief Searches for an OpcodeDescriptor by opcode using a mask for efficiency.
 *
 * This function extracts the group ID and sub-index (SN) from the opcode
 * using bitwise operations, allowing direct access to the descriptor.
 *
 * @param opcode The opcode to search for.
 * @return A pointer to the matching OpcodeDescriptor, or NULL if not found.
 */
OpcodeDescriptor* getOpcodeDescriptor(uint32_t opcode) {
    // Extract the group ID (first 5 bits of the opcode)
    uint8_t Group_ID = opcode & 31; // Mask to get the lower 5 bits
    // Extract the sub-index (remaining bits of the opcode)
    uint8_t SN = opcode >> 5; // Shift right to get the sub-index

    // Validate the group ID to ensure it is within bounds
    if (Group_ID >= sizeof(all_descriptors) / sizeof(all_descriptors[0])) {
        return NULL; // Invalid group ID
    }

    // Get the group of descriptors
    OpcodeDescriptor* group_desc = all_descriptors[Group_ID];
    // Get the size of the group
    size_t group_size = group_sizes[Group_ID];

    // Validate the sub-index to ensure it is within bounds
    if (SN >= group_size) {
        return NULL; // Invalid sub-index
    }

    // Return the matching descriptor
    return &group_desc[SN];
}
