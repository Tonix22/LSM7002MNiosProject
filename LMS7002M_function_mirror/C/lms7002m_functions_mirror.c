#include "lms7002m_functions_mirror.h"
#include <string.h>


/* ====================================================
   Conversión de tipos de datos
   ==================================================== */

uint32_t u32(int32_t value) 
{
    uint32_t w;
    memcpy(&w, &value, sizeof(uint32_t));
    return w;
}

void double_to_u32_words_be(double value, uint32_t words[2])
{
    uint64_t u;

    memcpy(&u, &value, sizeof(double));

    words[0] = (uint32_t)((u >> 32) & 0xFFFFFFFF);
    words[1] = (uint32_t)(u & 0xFFFFFFFF);
}

uint32_t bool_to_u32(bool value)
{
    return value ? 1 : 0;
}

uint32_t char_to_u32(char value)
{
    uint32_t w = 0;
    memcpy(&w, &value, sizeof(char));
    return w;
}

/* ===========================================================
   Funcion de espera por software
   =========================================================== */
void wait_time(int cycles){
    for(int i = 0; i < cycles; i++){
        __asm__ volatile ("nop");
    }
}

/* ============================================================
   Funciones de gestión e inicialización
   ============================================================ */

void LMS7002M_create(void *aip)
{
    uint32_t opcode = 0x00;
    uint32_t buffer[] = {opcode};
    aip_write(aip, buffer, 1, 0);
    aip_start(aip);
}

void LMS7002M_regs(void *aip)
{
    uint32_t opcode = 0x01;
    uint32_t buffer[] = {opcode};
    aip_write(aip, buffer, 1, 0);
    aip_start(aip);
}

void LMS7002M_destroy(void *aip)
{
    uint32_t opcode = 0x21;
    uint32_t buffer[] = {opcode};
    aip_write(aip, buffer, 1, 0);
    aip_start(aip);
}

void LMS7002M_reset(void *aip)
{
    uint32_t opcode = 0x81;
    uint32_t buffer[] = {opcode};
    aip_write(aip, buffer, 1, 0);
    aip_start(aip);
}

void LMS7002M_power_down(void *aip)
{
    uint32_t opcode = 0xA1;
    uint32_t buffer[] = {opcode};
    aip_write(aip, buffer, 1, 0);
    aip_start(aip);
}

/* ============================================================
   Funciones de comunicación SPI y sincronización de registros
   ============================================================ */

void LMS7002M_regs_to_rfic(void *aip)
{
    uint32_t opcode = 0x41;
    uint32_t buffer[] = {opcode};
    aip_write(aip, buffer, 1, 0);
    aip_start(aip);
}

void LMS7002M_rfic_to_regs(void *aip)
{
    uint32_t opcode = 0x61;
    uint32_t buffer[] = {opcode};
    aip_write(aip, buffer, 1, 0);
    aip_start(aip);
}

void LMS7002M_spi_write(void *aip, uint16_t addr, uint16_t data)
{
    uint32_t opcode = 0x02;
    uint32_t buffer[] = {opcode, (uint32_t)addr, (uint32_t)data};
    aip_write(aip, buffer, 3, 0);
    aip_start(aip);
}

uint16_t LMS7002M_spi_read(void *aip, uint16_t addr)
{
    clearDone(aip);
    uint32_t opcode = 0x03;
    uint32_t buffer[] = {opcode, (uint32_t)addr};
    aip_write(aip, buffer, 2, 0);
    aip_start(aip);

    uint32_t STATUS = 0;
    while(STATUS == 0){
        STATUS = getStatus(aip);
        STATUS &= 1;
    }
    clearDone(aip);

    uint32_t data = aip_read(aip, 1, 0);
    data = data & 0x0000FFFF;
    return (uint16_t)data;
}

void LMS7002M_regs_spi_write(void *aip, uint16_t addr)
{
    uint32_t opcode = 0x23;
    uint32_t buffer[] = {opcode, (uint32_t)addr};
    aip_write(aip, buffer, 2, 0);
    aip_start(aip);
}

void LMS7002M_regs_spi_read(void *aip, uint16_t addr)
{
    uint32_t opcode = 0x43;
    uint32_t buffer[] = {opcode, (uint32_t)addr};
    aip_write(aip, buffer, 2, 0);
    aip_start(aip);
}

void LMS7002M_set_spi_mode(void *aip, uint32_t mode)
{
    uint32_t opcode = 0x63;
    uint32_t buffer[] = {opcode, mode};
    aip_write(aip, buffer, 2, 0);
    aip_start(aip);
}

/* ============================================================
   Funciones de configuración de interfaz digital
   ============================================================ */

void LMS7002M_set_work_mode(void *aip)
{
    uint32_t opcode = 0xE1;
    uint32_t buffer[] = {opcode};
    aip_write(aip, buffer, 1, 0);
    aip_start(aip);
}

void LMS7002M_configure_lml_port(void *aip, uint32_t port, uint32_t direction, uint32_t mclk_div)
{
    uint32_t opcode = 0x05;
    uint32_t buffer[] = {opcode, port, direction, mclk_div};
    aip_write(aip, buffer, 4, 0);
    aip_start(aip);
}

void LMS7002M_invert_fclk(void *aip, bool enable)
{
    uint32_t opcode = 0x06;
    uint32_t buffer[] = {opcode, bool_to_u32(enable)};
    aip_write(aip, buffer, 2, 0);
    aip_start(aip);
}

void LMS7002M_reset_lml_fifo(void *aip, uint32_t direction)
{
    uint32_t opcode = 0x07;
    uint32_t buffer[] = {opcode, direction};
    aip_write(aip, buffer, 2, 0);
    aip_start(aip);
}

void LMS7002M_set_mac_dir(void *aip, uint32_t direction)
{
    uint32_t opcode = 0x27;
    uint32_t buffer[] = {opcode, direction};
    aip_write(aip, buffer, 2, 0);
    aip_start(aip);
}

void LMS7002M_set_mac_ch(void *aip, char channel)
{
    uint32_t opcode = 0x47;
    uint32_t buffer[] = {opcode, char_to_u32(channel)};
    aip_write(aip, buffer, 2, 0);
    aip_start(aip);
}

void LMS7002M_set_diq_mux(void *aip, uint32_t direction, uint32_t positions)
{
    uint32_t opcode = 0x08;
    uint32_t buffer[] = {opcode, direction, positions};
    aip_write(aip, buffer, 3, 0);
    aip_start(aip);
}

/* ============================================================
   Funciones de control auxiliar y señales externas
   ============================================================ */

void LMS7002M_RESET(void *aip)
{
    uint32_t opcode = 0x101;
    uint32_t buffer[] = {u32(opcode)};
    aip_write(aip, buffer, 1, 0);
    aip_start(aip);
}

void LMS7002M_CORE_LDO_ENABLE(void *aip, bool enable)
{
    uint32_t opcode = 0x86;
    uint32_t buffer[] = {u32(opcode), bool_to_u32(enable)};
    aip_write(aip, buffer, 2, 0);
    aip_start(aip);
}

void LMS7002M_RXEN(void *aip, bool enable)
{
    uint32_t opcode = 0xA6;
    uint32_t buffer[] = {u32(opcode), bool_to_u32(enable)};
    aip_write(aip, buffer, 2, 0);
    aip_start(aip);
}

void LMS7002M_TXEN(void *aip, bool enable)
{
    uint32_t opcode = 0xC6;
    uint32_t buffer[] = {u32(opcode), bool_to_u32(enable)};
    aip_write(aip, buffer, 2, 0);
    aip_start(aip);
}

void LMS7002M_TXNRX1(void *aip, bool enable)
{
    uint32_t opcode = 0xE6;
    uint32_t buffer[] = {u32(opcode), bool_to_u32(enable)};
    aip_write(aip, buffer, 2, 0);
    aip_start(aip);
}

void LMS7002M_TXNRX2(void *aip, bool enable)
{
    uint32_t opcode = 0x106;
    uint32_t buffer[] = {u32(opcode), bool_to_u32(enable)};
    aip_write(aip, buffer, 2, 0);
    aip_start(aip);
}

void LMS7002M_enable_external_loopback(void *aip, char channel, bool enable)
{
    uint32_t opcode = 0x12F;
    uint32_t buffer[] = {u32(opcode), char_to_u32(channel), bool_to_u32(enable)};
    aip_write(aip, buffer, 3, 0);
    aip_start(aip);
}

/* ============================================================
   Funciones de referencias y distribución de reloj
   ============================================================ */

void LMS7002M_xbuf_share_tx(void *aip, bool enable)
{
    uint32_t opcode = 0x26;
    uint32_t buffer[] = {u32(opcode), bool_to_u32(enable)};
    aip_write(aip, buffer, 2, 0);
    aip_start(aip);
}

void LMS7002M_xbuf_enable_bias(void *aip, bool enable)
{
    uint32_t opcode = 0x46;
    uint32_t buffer[] = {u32(opcode), bool_to_u32(enable)};
    aip_write(aip, buffer, 2, 0);
    aip_start(aip);
}

void LMS7002M_sxt_to_sxr(void *aip, bool enable)
{
    uint32_t opcode = 0x66;
    uint32_t buffer[] = {u32(opcode), bool_to_u32(enable)};
    aip_write(aip, buffer, 2, 0);
    aip_start(aip);
}

/* ============================================================
   Funciones de alimentación y AFE
   ============================================================ */

void LMS7002M_ldo_enable(void *aip, bool enable, uint32_t group)
{
    group = 0;

    uint32_t opcode = 0x09;
    uint32_t buffer[] = {u32(opcode), bool_to_u32(enable), u32(group)};
    aip_write(aip, buffer, 3, 0);
    aip_start(aip);
}

void LMS7002M_afe_enable(void *aip, uint32_t direction, char channel, bool enable)
{
    uint32_t opcode = 0x0A;
    uint32_t buffer[] = {u32(opcode), u32(direction), char_to_u32(channel), bool_to_u32(enable)};
    aip_write(aip, buffer, 4, 0);
    aip_start(aip);
}

/* ============================================================
   Funciones de frecuencia y generación de reloj
   ============================================================ */

void LMS7002M_set_lo_freq(void *aip, uint32_t direction, double fref, double freq)
{
    uint32_t fref_words[2];
    uint32_t freq_words[2];

    double_to_u32_words_be(fref, fref_words);
    double_to_u32_words_be(freq, freq_words);

    uint32_t opcode = 0x0E;
    uint32_t buffer[] = {u32(opcode), u32(direction), fref_words[0], fref_words[1], freq_words[0], freq_words[1]};
    aip_write(aip, buffer, 6, 0);
    aip_start(aip);
}

void LMS7002M_sxx_enable(void *aip, uint32_t direction, bool enable)
{
    uint32_t opcode = 0x0F;
    uint32_t buffer[] = {u32(opcode), u32(direction), bool_to_u32(enable)};
    aip_write(aip, buffer, 3, 0);
    aip_start(aip);
}

void LMS7002M_set_data_clock(void *aip, double fref, double freq)
{
    uint32_t fref_words[2];
    uint32_t freq_words[2];

    double_to_u32_words_be(fref, fref_words);
    double_to_u32_words_be(freq, freq_words);

    uint32_t opcode = 0x0B;
    uint32_t buffer[] = {u32(opcode), fref_words[0], fref_words[1], freq_words[0], freq_words[1]};
    aip_write(aip, buffer, 5, 0);
    aip_start(aip);
}

/* ============================================================
   Funciones de procesamiento digital TX/RX
   ============================================================ */

void LMS7002M_txtsp_enable(void *aip, char channel, bool enable)
{
    uint32_t opcode = 0x2F;
    uint32_t buffer[] = {u32(opcode), char_to_u32(channel), bool_to_u32(enable)};
    aip_write(aip, buffer, 3, 0);
    aip_start(aip);
}

void LMS7002M_rxtsp_enable(void *aip, char channel, bool enable)
{
    uint32_t opcode = 0xAF;
    uint32_t buffer[] = {u32(opcode), char_to_u32(channel), bool_to_u32(enable)};
    aip_write(aip, buffer, 3, 0);
    aip_start(aip);
}

void LMS7002M_txtsp_set_interp(void *aip, char channel, uint32_t interp)
{
    uint32_t opcode = 0x10;
    uint32_t buffer[] = {u32(opcode), char_to_u32(channel), u32(interp)};
    aip_write(aip, buffer, 3, 0);
    aip_start(aip);
}

void LMS7002M_rxtsp_set_decim(void *aip, char channel, uint32_t decim)
{
    uint32_t opcode = 0x30;
    uint32_t buffer[] = {u32(opcode), char_to_u32(channel), u32(decim)};
    aip_write(aip, buffer, 3, 0);
    aip_start(aip);
}

void LMS7002M_set_gfir_taps(void *aip, uint32_t direction, char channel, uint32_t gfir, short taps[], uint32_t ntaps)
{
    uint32_t opcode = 0x0D;
    uint32_t buffer[6 + ntaps];

    buffer[0] = u32(opcode);
    buffer[1] = u32(direction);
    buffer[2] = char_to_u32(channel);
    buffer[3] = u32(gfir);
    buffer[4] = 0;
    buffer[5] = u32(ntaps);

    for (size_t i = 0; i < ntaps; i++)
    {
        buffer[6 + i] = (uint32_t)(uint16_t)taps[i];
    }


    aip_write(aip, buffer, 6 + ntaps, 0);
    aip_start(aip);
}

void LMS7002M_set_nco_freq(void *aip, uint32_t direction, char channel, double freq_rel)
{
    uint32_t freq_rel_words[2];

    double_to_u32_words_be(freq_rel, freq_rel_words);

    uint32_t opcode = 0x0C;
    uint32_t buffer[] = {u32(opcode), u32(direction), char_to_u32(channel), freq_rel_words[0], freq_rel_words[1]};
    aip_write(aip, buffer, 5, 0);
    aip_start(aip);
}

void LMS7002M_txtsp_set_freq(void *aip, char channel, double freq_rel)
{
    uint32_t freq_rel_words[2];

    double_to_u32_words_be(freq_rel, freq_rel_words);

    uint32_t opcode = 0x17;
    uint32_t buffer[] = {u32(opcode), char_to_u32(channel), freq_rel_words[0], freq_rel_words[1]};
    aip_write(aip, buffer, 4, 0);
    aip_start(aip);
}

void LMS7002M_rxtsp_set_freq(void *aip, char channel, double freq_rel)
{
    uint32_t freq_rel_words[2];

    double_to_u32_words_be(freq_rel, freq_rel_words);

    uint32_t opcode = 0x37;
    uint32_t buffer[] = {u32(opcode), char_to_u32(channel), freq_rel_words[0], freq_rel_words[1]};
    aip_write(aip, buffer, 4, 0);
    aip_start(aip);
}

void LMS7002M_rxtsp_set_dc_correction(void *aip, char channel, bool enable, uint32_t window)
{
    uint32_t opcode = 0x13;
    uint32_t buffer[] = {u32(opcode), char_to_u32(channel), bool_to_u32(enable), u32(window)};
    aip_write(aip, buffer, 4, 0);
    aip_start(aip);
}

void LMS7002M_txtsp_set_dc_correction(void *aip, char channel, double val_i, double val_q)
{
    uint32_t val_i_words[2];
    uint32_t val_q_words[2];

    double_to_u32_words_be(val_i, val_i_words);
    double_to_u32_words_be(val_q, val_q_words);

    uint32_t opcode = 0x12;
    uint32_t buffer[] = {u32(opcode), char_to_u32(channel), val_i_words[0], val_i_words[1], val_q_words[0], val_q_words[1]};
    aip_write(aip, buffer, 6, 0);
    aip_start(aip);
}

void LMS7002M_txtsp_set_iq_correction(void *aip, char channel, double phase, double gain)
{
    uint32_t phase_words[2];
    uint32_t gain_words[2];

    double_to_u32_words_be(phase, phase_words);
    double_to_u32_words_be(gain, gain_words);

    uint32_t opcode = 0x32;
    uint32_t buffer[] = {u32(opcode), char_to_u32(channel), phase_words[0], phase_words[1], gain_words[0], gain_words[1]};
    aip_write(aip, buffer, 6, 0);
    aip_start(aip);
}

void LMS7002M_rxtsp_set_iq_correction(void *aip, char channel, double phase, double gain)
{
    uint32_t phase_words[2];
    uint32_t gain_words[2];

    double_to_u32_words_be(phase, phase_words);
    double_to_u32_words_be(gain, gain_words);

    uint32_t opcode = 0x52;
    uint32_t buffer[] = {u32(opcode), char_to_u32(channel), phase_words[0], phase_words[1], gain_words[0], gain_words[1]};
    aip_write(aip, buffer, 6, 0);
    aip_start(aip);
}

void LMS7002M_txtsp_tsg_tone(void *aip, char channel)
{
    uint32_t opcode = 0x67;
    uint32_t buffer[] = {u32(opcode), char_to_u32(channel)};
    aip_write(aip, buffer, 2, 0);
    aip_start(aip);
}

void LMS7002M_rxtsp_tsg_tone(void *aip, char channel)
{
    uint32_t opcode = 0x87;
    uint32_t buffer[] = {u32(opcode), char_to_u32(channel)};
    aip_write(aip, buffer, 2, 0);
    aip_start(aip);
}

void LMS7002M_txtsp_tsg_const(void *aip, char channel, uint16_t val_i, uint16_t val_q)
{
    uint32_t opcode = 0x11;
    uint32_t buffer[] = {u32(opcode), char_to_u32(channel), u32(val_i), u32(val_q)};
    aip_write(aip, buffer, 4, 0);
    aip_start(aip);
}

void LMS7002M_rxtsp_tsg_const(void *aip, char channel, uint16_t val_i, uint16_t val_q)
{
    uint32_t opcode = 0x31;
    uint32_t buffer[] = {u32(opcode), char_to_u32(channel), u32(val_i), u32(val_q)};
    aip_write(aip, buffer, 4, 0);
    aip_start(aip);
}

uint16_t LMS7002M_rxtsp_read_rssi(void *aip, char channel)
{
    clearDone(aip);
    uint16_t rssi = 0;

    uint32_t opcode = 0x18;
    uint32_t buffer[] = {u32(opcode), char_to_u32(channel)};
    aip_write(aip, buffer, 2, 0);
    aip_start(aip);

    uint32_t STATUS = 0;
    while(STATUS == 0){
        STATUS = getStatus(aip);
        STATUS &= 1;
    }
    clearDone(aip);
    rssi = aip_read(aip, 1, 0);
    return rssi;
}

/* ============================================================
   Funciones de configuración de la cadena analógica de transmisión
   ============================================================ */

void LMS7002M_tbb_enable(void *aip, char channel, bool enable)
{
    uint32_t opcode = 0x4F;
    uint32_t buffer[] = {u32(opcode), char_to_u32(channel), bool_to_u32(enable)};
    aip_write(aip, buffer, 3, 0);
    aip_start(aip);
}

void LMS7002M_trf_enable(void *aip, char channel, bool enable)
{
    uint32_t opcode = 0x6F;
    uint32_t buffer[] = {u32(opcode), char_to_u32(channel), bool_to_u32(enable)};
    aip_write(aip, buffer, 3, 0);
    aip_start(aip);
}

void LMS7002M_tbb_set_path(void *aip, char channel, uint32_t path)
{
    uint32_t opcode = 0x14;
    uint32_t buffer[] = {u32(opcode), char_to_u32(channel), u32(path)};
    aip_write(aip, buffer, 3, 0);
    aip_start(aip);
}

void LMS7002M_tbb_set_test_in(void *aip, char channel, uint32_t path)
{
    uint32_t opcode = 0x34;
    uint32_t buffer[] = {u32(opcode), char_to_u32(channel), u32(path)};
    aip_write(aip, buffer, 3, 0);
    aip_start(aip);
}

void LMS7002M_trf_select_band(void *aip, char channel, uint32_t band)
{
    uint32_t opcode = 0x54;
    uint32_t buffer[] = {u32(opcode), char_to_u32(channel), u32(band)};
    aip_write(aip, buffer, 3, 0);
    aip_start(aip);
}

void LMS7002M_trf_set_pad(void *aip, char channel, double gain)
{
    uint32_t gain_words[2];

    double_to_u32_words_be(gain, gain_words);

    uint32_t opcode = 0x57;
    uint32_t buffer[] = {u32(opcode), char_to_u32(channel), gain_words[0], gain_words[1]};
    aip_write(aip, buffer, 4, 0);
    aip_start(aip);
}

/* ============================================================
   Funciones de configuración de la cadena analógica de recepción
   ============================================================ */

void LMS7002M_rbb_enable(void *aip, char channel, bool enable)
{
    uint32_t opcode = 0xCF;
    uint32_t buffer[] = {u32(opcode), char_to_u32(channel), bool_to_u32(enable)};
    aip_write(aip, buffer, 3, 0);
    aip_start(aip);
}

void LMS7002M_rfe_enable(void *aip, char channel, bool enable)
{
    uint32_t opcode = 0x10F;
    uint32_t buffer[] = {u32(opcode), char_to_u32(channel), bool_to_u32(enable)};
    aip_write(aip, buffer, 3, 0);
    aip_start(aip);
}

void LMS7002M_rbb_set_path(void *aip, char channel, uint32_t path)
{
    uint32_t opcode = 0x74;
    uint32_t buffer[] = {u32(opcode), char_to_u32(channel), u32(path)};
    aip_write(aip, buffer, 3, 0);
    aip_start(aip);
}

void LMS7002M_rfe_set_path(void *aip, char channel, uint32_t path)
{
    uint32_t opcode = 0x94;
    uint32_t buffer[] = {u32(opcode), char_to_u32(channel), u32(path)};
    aip_write(aip, buffer, 3, 0);
    aip_start(aip);
}

void LMS7002M_rbb_set_pga(void *aip, char channel, double gain)
{
    uint32_t gain_words[2];

    double_to_u32_words_be(gain, gain_words);

    uint32_t opcode = 0x97;
    uint32_t buffer[] = {u32(opcode), char_to_u32(channel), gain_words[0], gain_words[1]};
    aip_write(aip, buffer, 4, 0);
    aip_start(aip);
}

void LMS7002M_rfe_set_lna(void *aip, char channel, double gain)
{
    uint32_t gain_words[2];

    double_to_u32_words_be(gain, gain_words);

    uint32_t opcode = 0xB7;
    uint32_t buffer[] = {u32(opcode), char_to_u32(channel), gain_words[0], gain_words[1]};
    aip_write(aip, buffer, 4, 0);
    aip_start(aip);
}

void LMS7002M_rfe_set_tia(void *aip, char channel, double gain)
{
    uint32_t gain_words[2];

    double_to_u32_words_be(gain, gain_words);

    uint32_t opcode = 0xF7;
    uint32_t buffer[] = {u32(opcode), char_to_u32(channel), gain_words[0], gain_words[1]};
    aip_write(aip, buffer, 4, 0);
    aip_start(aip);
}

void LMS7002M_rbb_set_test_out(void *aip, char channel, bool enable)
{
    uint32_t opcode = 0xEF;
    uint32_t buffer[] = {u32(opcode), char_to_u32(channel), bool_to_u32(enable)};
    aip_write(aip, buffer, 3, 0);
    aip_start(aip);
}

/* ============================================================
   Funciones de lazos de retroalimentación y pruebas internas
   ============================================================ */

void LMS7002M_setup_digital_loopback(void *aip)
{
    uint32_t opcode = 0xC1;
    uint32_t buffer[] = {u32(opcode)};
    aip_write(aip, buffer, 1, 0);
    aip_start(aip);
}

void LMS7002M_trf_enable_loopback(void *aip, char channel, bool enable)
{
    uint32_t opcode = 0x8F;
    uint32_t buffer[] = {u32(opcode), char_to_u32(channel), bool_to_u32(enable)};
    aip_write(aip, buffer, 3, 0);
    aip_start(aip);
}

void LMS7002M_tbb_enable_loopback(void *aip, char channel, uint32_t mode, bool swap)
{
    uint32_t opcode = 0x15;
    uint32_t buffer[] = {u32(opcode), char_to_u32(channel), u32(mode), bool_to_u32(swap)};
    aip_write(aip, buffer, 4, 0);
    aip_start(aip);
}

void LMS7002M_trf_set_loopback_pad(void *aip, char channel, double gain)
{
    uint32_t gain_words[2];

    double_to_u32_words_be(gain, gain_words);

    uint32_t opcode = 0x77;
    uint32_t buffer[] = {u32(opcode), char_to_u32(channel), gain_words[0], gain_words[1]};
    aip_write(aip, buffer, 4, 0);
    aip_start(aip);
}

void LMS7002M_rfe_set_loopback_lna(void *aip, char channel, double gain)
{
    uint32_t gain_words[2];

    double_to_u32_words_be(gain, gain_words);

    uint32_t opcode = 0xD7;
    uint32_t buffer[] = {u32(opcode), char_to_u32(channel), gain_words[0], gain_words[1]};
    aip_write(aip, buffer, 4, 0);
    aip_start(aip);
}

/* ============================================================
   Funciones de configuración de filtros y calibración
   ============================================================ */

void LMS7002M_tbb_set_filter_bw(void *aip, char channel, double bw)
{
    uint32_t bw_words[2];

    double_to_u32_words_be(bw, bw_words);

    uint32_t opcode = 0x16;
    uint32_t buffer[] = {u32(opcode), char_to_u32(channel), bw_words[0], bw_words[1]};
    aip_write(aip, buffer, 4, 0);
    aip_start(aip);
}

void LMS7002M_rbb_set_filter_bw(void *aip, char channel, double bw)
{
    uint32_t bw_words[2];

    double_to_u32_words_be(bw, bw_words);

    uint32_t opcode = 0x36;
    uint32_t buffer[] = {u32(opcode), char_to_u32(channel), bw_words[0], bw_words[1]};
    aip_write(aip, buffer, 4, 0);
    aip_start(aip);
}

void TuneTxFilter_8051(void *aip, char channel, double bw)
{
    uint32_t bw_words[2];

    double_to_u32_words_be(bw, bw_words);

    uint32_t opcode = 0x56;
    uint32_t buffer[] = {u32(opcode), char_to_u32(channel), bw_words[0], bw_words[1]};
    aip_write(aip, buffer, 4, 0);
    aip_start(aip);
}

void TuneRxFilter_8051(void *aip, char channel, double bw)
{
    uint32_t bw_words[2];

    double_to_u32_words_be(bw, bw_words);

    uint32_t opcode = 0x76;
    uint32_t buffer[] = {u32(opcode), char_to_u32(channel), bw_words[0], bw_words[1]};
    aip_write(aip, buffer, 4, 0);
    aip_start(aip);
}

/* ============================================================ */

void tx_cal_init(void *aip, char channel)
{
    uint32_t opcode = 0xC7;
    uint32_t buffer[] = {u32(opcode), char_to_u32(channel)};
    aip_write(aip, buffer, 2, 0);
    aip_start(aip);
}

void rx_cal_init(void *aip, char channel)
{
    uint32_t opcode = 0xA7;
    uint32_t buffer[] = {u32(opcode), char_to_u32(channel)};
    aip_write(aip, buffer, 2, 0);
    aip_start(aip);
}

/* ============================================================ */

void CalibrateAll(void *aip, char channel, double bw)
{
    uint32_t bw_words[2];

    double_to_u32_words_be(bw, bw_words);

    uint32_t opcode = 0x117;
    uint32_t buffer[] = {u32(opcode), char_to_u32(channel), bw_words[0], bw_words[1]};
    aip_write(aip, buffer, 4, 0);
    aip_start(aip);
}

void CalibrateTx(void *aip, char channel, double bw)
{
    uint32_t bw_words[2];

    double_to_u32_words_be(bw, bw_words);

    uint32_t opcode = 0x137;
    uint32_t buffer[] = {u32(opcode), char_to_u32(channel), bw_words[0], bw_words[1]};
    aip_write(aip, buffer, 4, 0);
    aip_start(aip);
}

void CalibrateRx(void *aip, char channel, double bw)
{
    uint32_t bw_words[2];

    double_to_u32_words_be(bw, bw_words);

    uint32_t opcode = 0x157;
    uint32_t buffer[] = {u32(opcode), char_to_u32(channel), bw_words[0], bw_words[1]};
    aip_write(aip, buffer, 4, 0);
    aip_start(aip);
}
