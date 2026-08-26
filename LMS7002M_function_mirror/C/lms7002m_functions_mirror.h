#ifndef LMS7002M_FUNCTIONS_MIRROR_H
#define LMS7002M_FUNCTIONS_MIRROR_H

#include <stdint.h>
#include <stdbool.h>
#include "functions_aip.h"

/* ====================================================
   Conversión de tipos de datos
   ==================================================== */
uint32_t u32(int32_t value);
void double_to_u32_words_be(double value, uint32_t words[2]);
uint32_t bool_to_u32(bool value);
uint32_t char_to_u32(char value);

/* ===========================================================
   Funcion de espera por software
   =========================================================== */
void wait_time(int cycles);

/* ============================================================
   Funciones de gestión e inicialización
   ============================================================ */
void LMS7002M_create(void *aip);
void LMS7002M_regs(void *aip);
void LMS7002M_destroy(void *aip);
void LMS7002M_reset(void *aip);
void LMS7002M_power_down(void *aip);

/* ============================================================
   Funciones de comunicación SPI y sincronización de registros
   ============================================================ */
void LMS7002M_regs_to_rfic(void *aip);
void LMS7002M_rfic_to_regs(void *aip);
void LMS7002M_spi_write(void *aip, uint16_t addr, uint16_t data);
uint16_t LMS7002M_spi_read(void *aip, uint16_t addr);
void LMS7002M_regs_spi_write(void *aip, uint16_t addr);
void LMS7002M_regs_spi_read(void *aip, uint16_t addr);
void LMS7002M_set_spi_mode(void *aip, uint32_t mode);

/* ============================================================
   Funciones de configuración de interfaz digital
   ============================================================ */
void LMS7002M_set_work_mode(void *aip);
void LMS7002M_configure_lml_port(void *aip, uint32_t port, uint32_t direction, uint32_t mclk_div);
void LMS7002M_invert_fclk(void *aip, bool enable);
void LMS7002M_reset_lml_fifo(void *aip, uint32_t direction);
void LMS7002M_set_mac_dir(void *aip, uint32_t direction);
void LMS7002M_set_mac_ch(void *aip, char channel);
void LMS7002M_set_diq_mux(void *aip, uint32_t direction, uint32_t positions);

/* ============================================================
   Funciones de control auxiliar y señales externas
   ============================================================ */
void LMS7002M_RESET(void *aip);
void LMS7002M_CORE_LDO_ENABLE(void *aip, bool enable);
void LMS7002M_RXEN(void *aip, bool enable);
void LMS7002M_TXEN(void *aip, bool enable);
void LMS7002M_TXNRX1(void *aip, bool enable);
void LMS7002M_TXNRX2(void *aip, bool enable);
void LMS7002M_enable_external_loopback(void *aip, char channel, bool enable);

/* ============================================================
   Funciones de referencias y distribución de reloj
   ============================================================ */
void LMS7002M_xbuf_share_tx(void *aip, bool enable);
void LMS7002M_xbuf_enable_bias(void *aip, bool enable);
void LMS7002M_sxt_to_sxr(void *aip, bool enable);

/* ============================================================
   Funciones de alimentación y AFE
   ============================================================ */
void LMS7002M_ldo_enable(void *aip, bool enable, uint32_t group);
void LMS7002M_afe_enable(void *aip, uint32_t direction, char channel, bool enable);

/* ============================================================
   Funciones de frecuencia y generación de reloj
   ============================================================ */
void LMS7002M_set_lo_freq(void *aip, uint32_t direction, double fref, double freq);
void LMS7002M_sxx_enable(void *aip, uint32_t direction, bool enable);
void LMS7002M_set_data_clock(void *aip, double fref, double freq);

/* ============================================================
   Funciones de procesamiento digital TX/RX
   ============================================================ */
void LMS7002M_txtsp_enable(void *aip, char channel, bool enable);
void LMS7002M_rxtsp_enable(void *aip, char channel, bool enable);
void LMS7002M_txtsp_set_interp(void *aip, char channel, uint32_t interp);
void LMS7002M_rxtsp_set_decim(void *aip, char channel, uint32_t decim);
void LMS7002M_set_gfir_taps(void *aip, uint32_t direction, char channel, uint32_t gfir, short taps[], uint32_t ntaps);
void LMS7002M_set_nco_freq(void *aip, uint32_t direction, char channel, double freq_rel);
void LMS7002M_txtsp_set_freq(void *aip, char channel, double freq_rel);
void LMS7002M_rxtsp_set_freq(void *aip, char channel, double freq_rel);
void LMS7002M_rxtsp_set_dc_correction(void *aip, char channel, bool enable, uint32_t window);
void LMS7002M_txtsp_set_dc_correction(void *aip, char channel, double val_i, double val_q);
void LMS7002M_txtsp_set_iq_correction(void *aip, char channel, double phase, double gain);
void LMS7002M_rxtsp_set_iq_correction(void *aip, char channel, double phase, double gain);
void LMS7002M_txtsp_tsg_tone(void *aip, char channel); 
void LMS7002M_rxtsp_tsg_tone(void *aip, char channel); 
void LMS7002M_txtsp_tsg_const(void *aip, char channel, uint16_t val_i, uint16_t val_q);
void LMS7002M_rxtsp_tsg_const(void *aip, char channel, uint16_t val_i, uint16_t val_q);
uint16_t LMS7002M_rxtsp_read_rssi(void *aip, char channel);

/* ============================================================
   Funciones de configuración de la cadena analógica de transmisión
   ============================================================ */
void LMS7002M_tbb_enable(void *aip, char channel, bool enable);
void LMS7002M_trf_enable(void *aip, char channel, bool enable);
void LMS7002M_tbb_set_path(void *aip, char channel, uint32_t path);
void LMS7002M_tbb_set_test_in(void *aip, char channel, uint32_t path);
void LMS7002M_trf_select_band(void *aip, char channel, uint32_t band);
void LMS7002M_trf_set_pad(void *aip, char channel, double gain);

/* ============================================================
   Funciones de configuración de la cadena analógica de recepción
   ============================================================ */
void LMS7002M_rbb_enable(void *aip, char channel, bool enable);
void LMS7002M_rfe_enable(void *aip, char channel, bool enable);
void LMS7002M_rbb_set_path(void *aip, char channel, uint32_t path);
void LMS7002M_rfe_set_path(void *aip, char channel, uint32_t path);
void LMS7002M_rbb_set_pga(void *aip, char channel, double gain);
void LMS7002M_rfe_set_lna(void *aip, char channel, double gain);
void LMS7002M_rfe_set_tia(void *aip, char channel, double gain);
void LMS7002M_rbb_set_test_out(void *aip, char channel, bool enable);

/* ============================================================
   Funciones de lazos de retroalimentación y pruebas internas
   ============================================================ */
void LMS7002M_setup_digital_loopback(void *aip);
void LMS7002M_trf_enable_loopback(void *aip, char channel, bool enable);
void LMS7002M_tbb_enable_loopback(void *aip, char channel, uint32_t mode, bool swap);
void LMS7002M_trf_set_loopback_pad(void *aip, char channel, double gain);
void LMS7002M_rfe_set_loopback_lna(void *aip, char channel, double gain);

/* ============================================================
   Funciones de configuración de filtros y calibración
   ============================================================ */
void LMS7002M_tbb_set_filter_bw(void *aip, char channel, double bw);
void LMS7002M_rbb_set_filter_bw(void *aip, char channel, double bw);
void TuneTxFilter_8051(void *aip, char channel, double bw);
void TuneRxFilter_8051(void *aip, char channel, double bw);
void tx_cal_init(void *aip, char channel);
void rx_cal_init(void *aip, char channel);
void CalibrateAll(void *aip, char channel, double bw);
void CalibrateTx(void *aip, char channel, double bw);
void CalibrateRx(void *aip, char channel, double bw);

#endif
