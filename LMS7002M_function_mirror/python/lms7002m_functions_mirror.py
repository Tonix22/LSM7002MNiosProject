import struct
from functions_AIP import *

timeout = 100000000

# ====================================================
# Conversión de tipos de datos
# ====================================================
def u32(value):
    return int(value) & 0xFFFFFFFF

def double_to_u32_words_be(value: float) -> list[int]:
    b = struct.pack(">d", float(value))

    return [
        int.from_bytes(b[0:4], "big"),
        int.from_bytes(b[4:8], "big")
    ]

def bool_to_u32(value: bool) -> int:
    return 1 if value else 0

def char_to_u32(value):
    return ord(value) & 0xFFFFFFFF

#============================================================
# Función de espera por sofware  
#============================================================
def wait_time(cycles):
    for _ in range(cycles):
        pass

# ============================================================
# Funciones de gestión e inicialización
# ============================================================

def LMS7002M_create(aip):
    opcode = 0x00
    aip_write(aip, [opcode], 1, 0)
    aip_start(aip)


# def LMS7002M_regs(aip):
#     opcode = 0x01
#     aip_write(aip, [opcode], 1, 0)
#     aip_start(aip)


def LMS7002M_destroy(aip):
    opcode = 0x21
    aip_write(aip, [opcode], 1, 0)
    aip_start(aip)


def LMS7002M_reset(aip):
    opcode = 0x81
    aip_write(aip, [opcode], 1, 0)
    aip_start(aip)


def LMS7002M_power_down(aip):
    opcode = 0xA1
    aip_write(aip, [opcode], 1, 0)
    aip_start(aip)


# ============================================================
# Funciones de comunicación SPI y sincronización de registros
# ============================================================

def LMS7002M_regs_to_rfic(aip):
    opcode = 0x41
    aip_write(aip, [opcode], 1, 0)
    aip_start(aip)


def LMS7002M_rfic_to_regs(aip):
    opcode = 0x61
    aip_write(aip, [opcode], 1, 0)
    aip_start(aip)


def LMS7002M_spi_write(aip, addr, data):
    opcode = 0x02
    aip_write(aip, [opcode, addr, data], 3, 0)
    aip_start(aip)


def LMS7002M_spi_read(aip, addr):
    clearDone(aip)
    opcode = 0x03
    aip_write(aip, [opcode, addr], 2, 0)
    aip_start(aip)
    STATUS = 0
    while(STATUS == 0):
        STATUS = getStatus(aip)
        STATUS &= 1 
    clearDone(aip)    
    data = aip_read(aip, 1, 0)
    return data[0]


def LMS7002M_regs_spi_write(aip, addr):
    opcode = 0x23
    aip_write(aip, [opcode, addr], 2, 0)
    aip_start(aip)


def LMS7002M_regs_spi_read(aip, addr):
    opcode = 0x43
    aip_write(aip, [opcode, addr], 2, 0)
    aip_start(aip)


def LMS7002M_set_spi_mode(aip, mode):
    opcode = 0x63
    aip_write(aip, [opcode, mode], 2, 0)
    aip_start(aip)


# ============================================================
# Funciones de configuración de interfaz digital
# ============================================================

def LMS7002M_set_work_mode(aip):
    opcode = 0xE1
    aip_write(aip, [u32(opcode)], 1, 0)
    aip_start(aip)


def LMS7002M_configure_lml_port(aip, port, direction, mclk_div):
    opcode = 0x05
    aip_write(aip, [u32(opcode), u32(port), u32(direction), u32(mclk_div)], 4, 0)
    aip_start(aip)


def LMS7002M_invert_fclk(aip, enable : bool):
    opcode = 0x06
    aip_write(aip, [u32(opcode), bool_to_u32(enable)], 2, 0)
    aip_start(aip)


def LMS7002M_reset_lml_fifo(aip, direction):
    opcode = 0x07
    aip_write(aip, [u32(opcode), u32(direction)], 2, 0)
    aip_start(aip)


def LMS7002M_set_mac_dir(aip, direction):
    opcode = 0x27
    aip_write(aip, [u32(opcode), u32(direction)], 2, 0)
    aip_start(aip)


def LMS7002M_set_mac_ch(aip, channel):
    opcode = 0x47
    aip_write(aip, [u32(opcode), char_to_u32(channel)], 2, 0)
    aip_start(aip)


def LMS7002M_set_diq_mux(aip, direction, positions):
    opcode = 0x08
    aip_write(aip, [u32(opcode), u32(direction), u32(positions)], 3, 0)
    aip_start(aip)


# ============================================================
# Funciones de control auxiliar y señales externas
# ============================================================

def LMS7002M_RESET(aip): 
    opcode = 0x101
    aip_write(aip, [u32(opcode)], 1, 0)
    aip_start(aip)


def LMS7002M_CORE_LDO_ENABLE(aip, enable : bool):
    opcode = 0x86
    aip_write(aip, [u32(opcode), bool_to_u32(enable)], 2, 0)
    aip_start(aip)


def LMS7002M_RXEN(aip, enable : bool):
    opcode = 0xA6
    aip_write(aip, [u32(opcode), bool_to_u32(enable)], 2, 0)
    aip_start(aip)


def LMS7002M_TXEN(aip, enable : bool):
    opcode = 0xC6
    aip_write(aip, [u32(opcode), bool_to_u32(enable)], 2, 0)
    aip_start(aip)


def LMS7002M_TXNRX1(aip, enable : bool):
    opcode = 0xE6
    aip_write(aip, [u32(opcode), bool_to_u32(enable)], 2, 0)
    aip_start(aip)


def LMS7002M_TXNRX2(aip, enable : bool):
    opcode = 0x106
    aip_write(aip, [u32(opcode), bool_to_u32(enable)], 2, 0)
    aip_start(aip)


def LMS7002M_enable_external_loopback(aip, channel, enable : bool):
    opcode = 0x12F
    aip_write(aip, [u32(opcode), char_to_u32(channel), bool_to_u32(enable)], 3, 0)
    aip_start(aip)


# ============================================================
# Funciones de referencias y distribución de reloj
# ============================================================

def LMS7002M_xbuf_share_tx(aip, enable : bool):
    opcode = 0x26
    aip_write(aip, [u32(opcode), bool_to_u32(enable)], 2, 0)
    aip_start(aip)


def LMS7002M_xbuf_enable_bias(aip, enable : bool):
    opcode = 0x46
    aip_write(aip, [u32(opcode), bool_to_u32(enable)], 2, 0)
    aip_start(aip)


def LMS7002M_sxt_to_sxr(aip, enable : bool):
    opcode = 0x66
    aip_write(aip, [u32(opcode), bool_to_u32(enable)], 2, 0)
    aip_start(aip)


# ============================================================
# Funciones de alimentación y AFE
# ============================================================

def LMS7002M_ldo_enable(aip, enable : bool, group):
    group = 0   # esta variable debe ser zero mientras exista un solo grupo general de LDOs.  
    opcode = 0x09
    aip_write(aip, [u32(opcode), bool_to_u32(enable), u32(group)], 3, 0)
    aip_start(aip)


def LMS7002M_afe_enable(aip, direction, channel, enable : bool):
    opcode = 0x0A
    aip_write(aip, [u32(opcode), u32(direction), char_to_u32(channel), bool_to_u32(enable)], 4, 0)
    aip_start(aip)


# ============================================================
# Funciones de frecuencia y generación de reloj
# ============================================================

def LMS7002M_set_lo_freq(aip, direction, fref, freq):
    fref = float(fref)
    freq = float(freq)
    opcode = 0x0E
    aip_write(aip, [u32(opcode), u32(direction), *double_to_u32_words_be(fref), *double_to_u32_words_be(freq)], 6, 0)
    aip_start(aip)


def LMS7002M_sxx_enable(aip, direction, enable : bool):
    opcode = 0x0F
    aip_write(aip, [u32(opcode), u32(direction), bool_to_u32(enable)], 3, 0)
    aip_start(aip)


def LMS7002M_set_data_clock(aip, fref, freq):
    fref = float(fref)
    freq = float(freq)
    opcode = 0x0B
    aip_write(aip, [u32(opcode), *double_to_u32_words_be(fref), *double_to_u32_words_be(freq)], 5, 0)
    aip_start(aip)


# ============================================================
# Funciones de procesamiento digital TX/RX
# ============================================================

def LMS7002M_txtsp_enable(aip, channel, enable : bool):
    opcode = 0x2F
    aip_write(aip, [u32(opcode), char_to_u32(channel), bool_to_u32(enable)], 3, 0)
    aip_start(aip)


def LMS7002M_rxtsp_enable(aip, channel, enable : bool):
    opcode = 0xAF
    aip_write(aip, [u32(opcode), char_to_u32(channel), bool_to_u32(enable)], 3, 0)
    aip_start(aip)


def LMS7002M_txtsp_set_interp(aip, channel, interp):
    opcode = 0x10
    aip_write(aip, [u32(opcode), char_to_u32(channel), u32(interp)], 3, 0)
    aip_start(aip)


def LMS7002M_rxtsp_set_decim(aip, channel, decim):
    opcode = 0x30
    aip_write(aip, [u32(opcode), char_to_u32(channel), u32(decim)], 3, 0)
    aip_start(aip)


def LMS7002M_set_gfir_taps(aip, direction, channel, gfir, taps, ntaps):
    taps = [u32(tap) for tap in taps]

    opcode = 0x0D 
    aip_write(aip, [u32(opcode), u32(direction), char_to_u32(channel), u32(gfir), 0, u32(ntaps), *taps], 6 + ntaps, 0)
    aip_start(aip)

def LMS7002M_set_nco_freq(aip, direction, channel, freq_rel):
    freq_rel = float(freq_rel)
    opcode = 0x0C
    aip_write(aip, [u32(opcode), u32(direction), char_to_u32(channel), *double_to_u32_words_be(freq_rel)], 5, 0)
    aip_start(aip)


def LMS7002M_txtsp_set_freq(aip, channel, freq_rel):
    freq_rel = float(freq_rel)
    opcode = 0x17
    aip_write(aip, [u32(opcode), char_to_u32(channel), *double_to_u32_words_be(freq_rel)], 4, 0)
    aip_start(aip)


def LMS7002M_rxtsp_set_freq(aip, channel, freq_rel):
    freq_rel = float(freq_rel)
    opcode = 0x37
    aip_write(aip, [u32(opcode), char_to_u32(channel), *double_to_u32_words_be(freq_rel)], 4, 0)
    aip_start(aip)


def LMS7002M_rxtsp_set_dc_correction(aip, channel, enable : bool, window):
    opcode = 0x13
    aip_write(aip, [u32(opcode), char_to_u32(channel), bool_to_u32(enable), u32(window)], 4, 0)
    aip_start(aip)


def LMS7002M_txtsp_set_dc_correction(aip, channel, val_i, val_q):
    val_i = float(val_i)
    val_q = float(val_q)
    opcode = 0x12
    aip_write(aip, [u32(opcode), char_to_u32(channel), *double_to_u32_words_be(val_i), *double_to_u32_words_be(val_q)], 6, 0)
    aip_start(aip)


def LMS7002M_txtsp_set_iq_correction(aip, channel, phase, gain):
    phase = float(phase)
    gain = float(gain)
    opcode = 0x32
    aip_write(aip, [u32(opcode), char_to_u32(channel), *double_to_u32_words_be(phase), *double_to_u32_words_be(gain)], 6, 0)
    aip_start(aip)


def LMS7002M_rxtsp_set_iq_correction(aip, channel, phase, gain):
    phase = float(phase)
    gain = float(gain)
    opcode = 0x52
    aip_write(aip, [u32(opcode), char_to_u32(channel), *double_to_u32_words_be(phase), *double_to_u32_words_be(gain)], 6, 0)
    aip_start(aip)


def LMS7002M_txtsp_tsg_tone(aip, channel):
    opcode = 0x67
    aip_write(aip, [u32(opcode), char_to_u32(channel)], 2, 0)
    aip_start(aip)


def LMS7002M_rxtsp_tsg_tone(aip, channel):
    opcode = 0x87
    aip_write(aip, [u32(opcode), char_to_u32(channel)], 2, 0)
    aip_start(aip)


def LMS7002M_txtsp_tsg_const(aip, channel, val_i, val_q):
    opcode = 0x11
    aip_write(aip, [u32(opcode), char_to_u32(channel), u32(val_i), u32(val_q)], 4, 0)
    aip_start(aip)

def LMS7002M_rxtsp_tsg_const(aip, channel, val_i, val_q):
    opcode = 0x31
    aip_write(aip, [u32(opcode), char_to_u32(channel), u32(val_i), u32(val_q)], 4, 0)
    aip_start(aip)


def LMS7002M_rxtsp_read_rssi(aip, channel):
    clearDone(aip)
    opcode = 0x18
    aip_write(aip, [u32(opcode), char_to_u32(channel)], 2, 0)
    aip_start(aip)

    STATUS = 0
    while(STATUS == 0):
        STATUS = getStatus(aip)
        STATUS &= 1
    
    clearDone(aip)
    
    rssi = aip_read(aip, 1, 0) 
    return rssi 


# ============================================================
# Funciones de configuración de la cadena analógica de transmisión
# ============================================================

def LMS7002M_tbb_enable(aip, channel, enable : bool):
    opcode = 0x4F
    aip_write(aip, [u32(opcode), char_to_u32(channel), bool_to_u32(enable)], 3, 0)
    aip_start(aip)


def LMS7002M_trf_enable(aip, channel, enable : bool):
    opcode = 0x6F
    aip_write(aip, [u32(opcode), char_to_u32(channel), bool_to_u32(enable)], 3, 0)
    aip_start(aip)


def LMS7002M_tbb_set_path(aip, channel, path):
    opcode = 0x14
    aip_write(aip, [u32(opcode), char_to_u32(channel), u32(path)], 3, 0)
    aip_start(aip)


def LMS7002M_tbb_set_test_in(aip, channel, path):
    opcode = 0x34
    aip_write(aip, [u32(opcode), char_to_u32(channel), u32(path)], 3, 0)
    aip_start(aip)


def LMS7002M_trf_select_band(aip, channel, band):
    opcode = 0x54
    aip_write(aip, [u32(opcode), char_to_u32(channel), u32(band)], 3, 0)
    aip_start(aip)


def LMS7002M_trf_set_pad(aip, channel, gain):
    gain = float(gain)
    opcode = 0x57
    aip_write(aip, [u32(opcode), char_to_u32(channel), *double_to_u32_words_be(gain)], 4, 0)
    aip_start(aip)


# ============================================================
# Funciones de configuración de la cadena analógica de recepción
# ============================================================

def LMS7002M_rbb_enable(aip, channel, enable : bool):
    opcode = 0xCF
    aip_write(aip, [u32(opcode), char_to_u32(channel), bool_to_u32(enable)], 3, 0)
    aip_start(aip)


def LMS7002M_rfe_enable(aip, channel, enable : bool):
    opcode = 0x10F
    aip_write(aip, [u32(opcode), char_to_u32(channel), bool_to_u32(enable)], 3, 0)
    aip_start(aip)


def LMS7002M_rbb_set_path(aip, channel, path):
    opcode = 0x74
    aip_write(aip, [u32(opcode), char_to_u32(channel), u32(path)], 3, 0)
    aip_start(aip)


def LMS7002M_rfe_set_path(aip, channel, path):
    opcode = 0x94
    aip_write(aip, [u32(opcode), char_to_u32(channel), u32(path)], 3, 0)
    aip_start(aip)


def LMS7002M_rbb_set_pga(aip, channel, gain):
    gain = float(gain)
    opcode = 0x97
    aip_write(aip, [u32(opcode), char_to_u32(channel), *double_to_u32_words_be(gain)], 4, 0)
    aip_start(aip)


def LMS7002M_rfe_set_lna(aip, channel, gain):
    gain = float(gain)
    opcode = 0xB7
    aip_write(aip, [u32(opcode), char_to_u32(channel), *double_to_u32_words_be(gain)], 4, 0)
    aip_start(aip)


def LMS7002M_rfe_set_tia(aip, channel, gain):
    gain = float(gain)
    opcode = 0xF7
    aip_write(aip, [u32(opcode), char_to_u32(channel), *double_to_u32_words_be(gain)], 4, 0)
    aip_start(aip)


def LMS7002M_rbb_set_test_out(aip, channel, enable : bool):
    opcode = 0xEF
    aip_write(aip, [u32(opcode), char_to_u32(channel), bool_to_u32(enable)], 3, 0)
    aip_start(aip)


# ============================================================
# Funciones de lazos de retroalimentación y pruebas internas
# ============================================================

def LMS7002M_setup_digital_loopback(aip):
    opcode = 0xC1
    aip_write(aip, [u32(opcode)], 1, 0)
    aip_start(aip)


def LMS7002M_trf_enable_loopback(aip, channel, enable : bool):
    opcode = 0x8F
    aip_write(aip, [u32(opcode), char_to_u32(channel), bool_to_u32(enable)], 3, 0)
    aip_start(aip)


def LMS7002M_tbb_enable_loopback(aip, channel, mode, swap : bool):
    opcode = 0x15
    aip_write(aip, [u32(opcode), char_to_u32(channel), u32(mode), bool_to_u32(swap)], 4, 0)
    aip_start(aip)


def LMS7002M_trf_set_loopback_pad(aip, channel, gain):
    gain = float(gain)
    opcode = 0x77
    aip_write(aip, [u32(opcode), char_to_u32(channel), *double_to_u32_words_be(gain)], 4, 0)
    aip_start(aip)


def LMS7002M_rfe_set_loopback_lna(aip, channel, gain):
    gain = float(gain)
    opcode = 0xD7
    aip_write(aip, [u32(opcode), char_to_u32(channel), *double_to_u32_words_be(gain)], 4, 0)
    aip_start(aip)


# ============================================================
# Funciones de configuración de filtros y calibración
# ============================================================

def LMS7002M_tbb_set_filter_bw(aip, channel, bw):
    bw = float(bw)
    opcode = 0x16
    aip_write(aip, [u32(opcode), char_to_u32(channel), *double_to_u32_words_be(bw)], 4, 0)
    aip_start(aip)


def LMS7002M_rbb_set_filter_bw(aip, channel, bw):
    bw = float(bw)
    opcode = 0x36
    aip_write(aip, [u32(opcode), char_to_u32(channel), *double_to_u32_words_be(bw)], 4, 0)
    aip_start(aip)


def TuneTxFilter_8051(aip, channel, bw):
    bw = float(bw)
    opcode = 0x56
    aip_write(aip, [u32(opcode), char_to_u32(channel), *double_to_u32_words_be(bw)], 4, 0)
    aip_start(aip)


def TuneRxFilter_8051(aip, channel, bw):
    bw = float(bw)
    opcode = 0x76
    aip_write(aip, [u32(opcode), char_to_u32(channel), *double_to_u32_words_be(bw)], 4, 0)
    aip_start(aip)

# ============================================================
def tx_cal_init(aip, channel):
    opcode = 0xC7
    aip_write(aip, [u32(opcode), char_to_u32(channel)], 2, 0)
    aip_start(aip)


def rx_cal_init(aip, channel):
    opcode = 0xA7
    aip_write(aip, [u32(opcode), char_to_u32(channel)], 2, 0)
    aip_start(aip)
# ============================================================

def CalibrateAll(aip, channel, bw):
    bw = float(bw)
    opcode = 0x117
    aip_write(aip, [u32(opcode), char_to_u32(channel), *double_to_u32_words_be(bw)], 4, 0)
    aip_start(aip)


def CalibrateTx(aip, channel, bw):
    bw = float(bw)
    opcode = 0x137
    aip_write(aip, [u32(opcode), char_to_u32(channel), *double_to_u32_words_be(bw)], 4, 0)
    aip_start(aip)


def CalibrateRx(aip, channel, bw):
    bw = float(bw)
    opcode = 0x157
    aip_write(aip, [u32(opcode), char_to_u32(channel), *double_to_u32_words_be(bw)], 4, 0)
    aip_start(aip)
 