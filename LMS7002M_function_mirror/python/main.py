from lms7002m_functions_mirror import *


def main():
    aip = aip_init()
    show_menu()
    while True:    
        option = read_int("Seleccione una opción")
        keep_running = execute_option(option, aip)
        if not keep_running:
            break
    aip_finish(aip) 


def read_int(name):
    return int(input(f"{name}: "))


def read_float(name):
    return float(input(f"{name}: "))


def read_bool(name):
    value = int(input(f"{name} (0/1): "))
    return bool(value)


def read_channel(name):
    value = input(f"{name} (A/B/C): ").strip()
    if len(value) != 1:
        raise ValueError("El canal debe ser un solo caracter, por ejemplo A o B.")
    return value


def read_hex(name):
    value = input(f"{name} en hexadecimal: ").strip()

    if value.lower().startswith("0x"):
        value = value[2:]

    return int(value, 16)


def read_taps():
    ntaps = int(input("ntaps: "))
    taps = []
    for i in range(ntaps):
        taps.append(int(input(f"taps[{i}]: ")))
    return taps, ntaps


def show_menu():
    print()
    print("=========== MENU LMS7002M ===========")
    print("0. Salir")
    print("\n--- Funciones de gestión e inicialización ---")
    print("1. LMS7002M_create")
    print("2. LMS7002M_regs")
    print("3. LMS7002M_destroy")
    print("4. LMS7002M_reset")
    print("5. LMS7002M_power_down")
    print("\n--- Funciones de comunicación SPI y sincronización de registros ---")
    print("6. LMS7002M_regs_to_rfic")
    print("7. LMS7002M_rfic_to_regs")
    print("8. LMS7002M_spi_write")
    print("9. LMS7002M_spi_read")
    print("10. LMS7002M_regs_spi_write")
    print("11. LMS7002M_regs_spi_read")
    print("12. LMS7002M_set_spi_mode")
    print("\n--- Funciones de configuración de interfaz digital ---")
    print("13. LMS7002M_set_work_mode")
    print("14. LMS7002M_configure_lml_port")
    print("15. LMS7002M_invert_fclk")
    print("16. LMS7002M_reset_lml_fifo")
    print("17. LMS7002M_set_mac_dir")
    print("18. LMS7002M_set_mac_ch")
    print("19. LMS7002M_set_diq_mux")
    print("\n--- Funciones de control auxiliar y señales externas ---")
    print("20. LMS7002M_RESET")
    print("21. LMS7002M_CORE_LDO_ENABLE")
    print("22. LMS7002M_RXEN")
    print("23. LMS7002M_TXEN")
    print("24. LMS7002M_TXNRX1")
    print("25. LMS7002M_TXNRX2")
    print("26. LMS7002M_enable_external_loopback")
    print("\n--- Funciones de referencias y distribución de reloj ---")
    print("27. LMS7002M_xbuf_share_tx")
    print("28. LMS7002M_xbuf_enable_bias")
    print("29. LMS7002M_sxt_to_sxr")
    print("\n--- Funciones de alimentación y AFE ---")
    print("30. LMS7002M_ldo_enable")
    print("31. LMS7002M_afe_enable")
    print("\n--- Funciones de frecuencia y generación de reloj ---")
    print("32. LMS7002M_set_lo_freq")
    print("33. LMS7002M_sxx_enable")
    print("34. LMS7002M_set_data_clock")
    print("\n--- Funciones de procesamiento digital TX/RX ---")
    print("35. LMS7002M_txtsp_enable")
    print("36. LMS7002M_rxtsp_enable")
    print("37. LMS7002M_txtsp_set_interp")
    print("38. LMS7002M_rxtsp_set_decim")
    print("39. LMS7002M_set_gfir_taps")
    print("40. LMS7002M_set_nco_freq")
    print("41. LMS7002M_txtsp_set_freq")
    print("42. LMS7002M_rxtsp_set_freq")
    print("43. LMS7002M_rxtsp_set_dc_correction")
    print("44. LMS7002M_txtsp_set_dc_correction")
    print("45. LMS7002M_txtsp_set_iq_correction")
    print("46. LMS7002M_rxtsp_set_iq_correction")
    print("47. LMS7002M_txtsp_tsg_tone")
    print("48. LMS7002M_rxtsp_tsg_tone")
    print("49. LMS7002M_txtsp_tsg_const")
    print("50. LMS7002M_rxtsp_tsg_const")
    print("51. LMS7002M_rxtsp_read_rssi")
    print("\n--- Funciones de configuración de la cadena analógica de transmisión ---")
    print("52. LMS7002M_tbb_enable")
    print("53. LMS7002M_trf_enable")
    print("54. LMS7002M_tbb_set_path")
    print("55. LMS7002M_tbb_set_test_in")
    print("56. LMS7002M_trf_select_band")
    print("57. LMS7002M_trf_set_pad")
    print("\n--- Funciones de configuración de la cadena analógica de recepción ---")
    print("58. LMS7002M_rbb_enable")
    print("59. LMS7002M_rfe_enable")
    print("60. LMS7002M_rbb_set_path")
    print("61. LMS7002M_rfe_set_path")
    print("62. LMS7002M_rbb_set_pga")
    print("63. LMS7002M_rfe_set_lna")
    print("64. LMS7002M_rfe_set_tia")
    print("65. LMS7002M_rbb_set_test_out")
    print("\n--- Funciones de lazos de retroalimentación y pruebas internas ---")
    print("66. LMS7002M_setup_digital_loopback")
    print("67. LMS7002M_trf_enable_loopback")
    print("68. LMS7002M_tbb_enable_loopback")
    print("69. LMS7002M_trf_set_loopback_pad")
    print("70. LMS7002M_rfe_set_loopback_lna")
    print("\n--- Funciones de configuración de filtros y calibración ---")
    print("71. LMS7002M_tbb_set_filter_bw")
    print("72. LMS7002M_rbb_set_filter_bw")
    print("73. TuneTxFilter_8051")
    print("74. TuneRxFilter_8051")
    print("75. tx_cal_init")
    print("76. rx_cal_init")
    print("77. CalibrateAll")
    print("78. CalibrateTx")
    print("79. CalibrateRx")
    print("=====================================")


def execute_option(option, aip):
    if option == 0:
        return False

    if option == 1:
        print("\nEjecutando LMS7002M_create")
        LMS7002M_create(aip)
        print("LMS7002M_create ejecutada.")
        return True

    elif option == 2:
        print("\nEjecutando LMS7002M_regs")
        LMS7002M_regs(aip)
        print("LMS7002M_regs ejecutada.")
        return True

    elif option == 3:
        print("\nEjecutando LMS7002M_destroy")
        LMS7002M_destroy(aip)
        print("LMS7002M_destroy ejecutada.")
        return True

    elif option == 4:
        print("\nEjecutando LMS7002M_reset")
        LMS7002M_reset(aip)
        print("LMS7002M_reset ejecutada.")
        return True

    elif option == 5:
        print("\nEjecutando LMS7002M_power_down")
        LMS7002M_power_down(aip)
        print("LMS7002M_power_down ejecutada.")
        return True

    elif option == 6:
        print("\nEjecutando LMS7002M_regs_to_rfic")
        LMS7002M_regs_to_rfic(aip)
        print("LMS7002M_regs_to_rfic ejecutada.")
        return True

    elif option == 7:
        print("\nEjecutando LMS7002M_rfic_to_regs")
        LMS7002M_rfic_to_regs(aip)
        print("LMS7002M_rfic_to_regs ejecutada.")
        return True

    elif option == 8:
        print("\nEjecutando LMS7002M_spi_write")
        addr = read_hex("addr")
        data = read_hex("data")
        LMS7002M_spi_write(aip, addr, data)
        print("LMS7002M_spi_write ejecutada.")
        return True

    elif option == 9:
        print("\nEjecutando LMS7002M_spi_read")
        addr = read_hex("addr")
        data = LMS7002M_spi_read(aip, addr)
        print(f"LMS7002M_spi_read ejecutada. Data: 0x{data:08X}")
        return True

    elif option == 10:
        print("\nEjecutando LMS7002M_regs_spi_write")
        addr = read_hex("addr")
        LMS7002M_regs_spi_write(aip, addr)
        print("LMS7002M_regs_spi_write ejecutada.")
        return True

    elif option == 11:
        print("\nEjecutando LMS7002M_regs_spi_read")
        addr = read_hex("addr")
        LMS7002M_regs_spi_read(aip, addr)
        print("LMS7002M_regs_spi_read ejecutada.")
        return True

    elif option == 12:
        print("\nEjecutando LMS7002M_set_spi_mode")
        mode = read_int("mode")
        LMS7002M_set_spi_mode(aip, mode)
        print("LMS7002M_set_spi_mode ejecutada.")
        return True

    elif option == 13:
        print("\nEjecutando LMS7002M_set_work_mode")
        LMS7002M_set_work_mode(aip)
        print("LMS7002M_set_work_mode ejecutada.")
        return True

    elif option == 14:
        print("\nEjecutando LMS7002M_configure_lml_port")
        port = read_int("port")
        direction = read_int("direction")
        mclk_div = read_int("mclk_div")
        LMS7002M_configure_lml_port(aip, port, direction, mclk_div)
        print("LMS7002M_configure_lml_port ejecutada.")
        return True

    elif option == 15:
        print("\nEjecutando LMS7002M_invert_fclk")
        enable = read_bool("enable")
        LMS7002M_invert_fclk(aip, enable)
        print("LMS7002M_invert_fclk ejecutada.")
        return True

    elif option == 16:
        print("\nEjecutando LMS7002M_reset_lml_fifo")
        direction = read_int("direction")
        LMS7002M_reset_lml_fifo(aip, direction)
        print("LMS7002M_reset_lml_fifo ejecutada.")
        return True

    elif option == 17:
        print("\nEjecutando LMS7002M_set_mac_dir")
        direction = read_int("direction")
        LMS7002M_set_mac_dir(aip, direction)
        print("LMS7002M_set_mac_dir ejecutada.")
        return True

    elif option == 18:
        print("\nEjecutando LMS7002M_set_mac_ch")
        channel = read_channel("channel")
        LMS7002M_set_mac_ch(aip, channel)
        print("LMS7002M_set_mac_ch ejecutada.")
        return True

    elif option == 19:
        print("\nEjecutando LMS7002M_set_diq_mux")
        direction = read_int("direction")
        positions = read_int("positions")
        LMS7002M_set_diq_mux(aip, direction, positions)
        print("LMS7002M_set_diq_mux ejecutada.")
        return True

    elif option == 20:
        print("\nEjecutando LMS7002M_RESET")
        LMS7002M_RESET(aip)
        print("LMS7002M_RESET ejecutada.")
        return True

    elif option == 21:
        print("\nEjecutando LMS7002M_CORE_LDO_ENABLE")
        enable = read_bool("enable")
        LMS7002M_CORE_LDO_ENABLE(aip, enable)
        print("LMS7002M_CORE_LDO_ENABLE ejecutada.")
        return True

    elif option == 22:
        print("\nEjecutando LMS7002M_RXEN")
        enable = read_bool("enable")
        LMS7002M_RXEN(aip, enable)
        print("LMS7002M_RXEN ejecutada.")
        return True

    elif option == 23:
        print("\nEjecutando LMS7002M_TXEN")
        enable = read_bool("enable")
        LMS7002M_TXEN(aip, enable)
        print("LMS7002M_TXEN ejecutada.")
        return True

    elif option == 24:
        print("\nEjecutando LMS7002M_TXNRX1")
        enable = read_bool("enable")
        LMS7002M_TXNRX1(aip, enable)
        print("LMS7002M_TXNRX1 ejecutada.")
        return True

    elif option == 25:
        print("\nEjecutando LMS7002M_TXNRX2")
        enable = read_bool("enable")
        LMS7002M_TXNRX2(aip, enable)
        print("LMS7002M_TXNRX2 ejecutada.")
        return True

    elif option == 26:
        print("\nEjecutando LMS7002M_enable_external_loopback")
        channel = read_channel("channel")
        enable = read_bool("enable")
        LMS7002M_enable_external_loopback(aip, channel, enable)
        print("LMS7002M_enable_external_loopback ejecutada.")
        return True

    elif option == 27:
        print("\nEjecutando LMS7002M_xbuf_share_tx")
        enable = read_bool("enable")
        LMS7002M_xbuf_share_tx(aip, enable)
        print("LMS7002M_xbuf_share_tx ejecutada.")
        return True

    elif option == 28:
        print("\nEjecutando LMS7002M_xbuf_enable_bias")
        enable = read_bool("enable")
        LMS7002M_xbuf_enable_bias(aip, enable)
        print("LMS7002M_xbuf_enable_bias ejecutada.")
        return True

    elif option == 29:
        print("\nEjecutando LMS7002M_sxt_to_sxr")
        enable = read_bool("enable")
        LMS7002M_sxt_to_sxr(aip, enable)
        print("LMS7002M_sxt_to_sxr ejecutada.")
        return True

    elif option == 30:
        print("\nEjecutando LMS7002M_ldo_enable")
        enable = read_bool("enable")
        group = read_int("group")
        LMS7002M_ldo_enable(aip, enable, group)
        print("LMS7002M_ldo_enable ejecutada.")
        return True

    elif option == 31:
        print("\nEjecutando LMS7002M_afe_enable")
        direction = read_int("direction")
        channel = read_channel("channel")
        enable = read_bool("enable")
        LMS7002M_afe_enable(aip, direction, channel, enable)
        print("LMS7002M_afe_enable ejecutada.")
        return True

    elif option == 32:
        print("\nEjecutando LMS7002M_set_lo_freq")
        direction = read_int("direction")
        fref = read_float("fref")
        freq = read_float("freq")
        LMS7002M_set_lo_freq(aip, direction, fref, freq)
        print("LMS7002M_set_lo_freq ejecutada.")
        return True

    elif option == 33:
        print("\nEjecutando LMS7002M_sxx_enable")
        direction = read_int("direction")
        enable = read_bool("enable")
        LMS7002M_sxx_enable(aip, direction, enable)
        print("LMS7002M_sxx_enable ejecutada.")
        return True

    elif option == 34:
        print("\nEjecutando LMS7002M_set_data_clock")
        fref = read_float("fref")
        freq = read_float("freq")
        LMS7002M_set_data_clock(aip, fref, freq)
        print("LMS7002M_set_data_clock ejecutada.")
        return True

    elif option == 35:
        print("\nEjecutando LMS7002M_txtsp_enable")
        channel = read_channel("channel")
        enable = read_bool("enable")
        LMS7002M_txtsp_enable(aip, channel, enable)
        print("LMS7002M_txtsp_enable ejecutada.")
        return True

    elif option == 36:
        print("\nEjecutando LMS7002M_rxtsp_enable")
        channel = read_channel("channel")
        enable = read_bool("enable")
        LMS7002M_rxtsp_enable(aip, channel, enable)
        print("LMS7002M_rxtsp_enable ejecutada.")
        return True

    elif option == 37:
        print("\nEjecutando LMS7002M_txtsp_set_interp")
        channel = read_channel("channel")
        interp = read_int("interp")
        LMS7002M_txtsp_set_interp(aip, channel, interp)
        print("LMS7002M_txtsp_set_interp ejecutada.")
        return True

    elif option == 38:
        print("\nEjecutando LMS7002M_rxtsp_set_decim")
        channel = read_channel("channel")
        decim = read_int("decim")
        LMS7002M_rxtsp_set_decim(aip, channel, decim)
        print("LMS7002M_rxtsp_set_decim ejecutada.")
        return True

    elif option == 39:
        print("\nEjecutando LMS7002M_set_gfir_taps")
        direction = read_int("direction")
        channel = read_channel("channel")
        gfir = read_int("gfir")
        taps, ntaps = read_taps()
        LMS7002M_set_gfir_taps(aip, direction, channel, gfir, taps, ntaps)
        print("LMS7002M_set_gfir_taps ejecutada.")
        return True

    elif option == 40:
        print("\nEjecutando LMS7002M_set_nco_freq")
        direction = read_int("direction")
        channel = read_channel("channel")
        freq_rel = read_float("freq_rel")
        LMS7002M_set_nco_freq(aip, direction, channel, freq_rel)
        print("LMS7002M_set_nco_freq ejecutada.")
        return True

    elif option == 41:
        print("\nEjecutando LMS7002M_txtsp_set_freq")
        channel = read_channel("channel")
        freq_rel = read_float("freq_rel")
        LMS7002M_txtsp_set_freq(aip, channel, freq_rel)
        print("LMS7002M_txtsp_set_freq ejecutada.")
        return True

    elif option == 42:
        print("\nEjecutando LMS7002M_rxtsp_set_freq")
        channel = read_channel("channel")
        freq_rel = read_float("freq_rel")
        LMS7002M_rxtsp_set_freq(aip, channel, freq_rel)
        print("LMS7002M_rxtsp_set_freq ejecutada.")
        return True

    elif option == 43:
        print("\nEjecutando LMS7002M_rxtsp_set_dc_correction")
        channel = read_channel("channel")
        enable = read_bool("enable")
        window = read_int("window")
        LMS7002M_rxtsp_set_dc_correction(aip, channel, enable, window)
        print("LMS7002M_rxtsp_set_dc_correction ejecutada.")
        return True

    elif option == 44:
        print("\nEjecutando LMS7002M_txtsp_set_dc_correction")
        channel = read_channel("channel")
        val_i = read_float("val_i")
        val_q = read_float("val_q")
        LMS7002M_txtsp_set_dc_correction(aip, channel, val_i, val_q)
        print("LMS7002M_txtsp_set_dc_correction ejecutada.")
        return True

    elif option == 45:
        print("\nEjecutando LMS7002M_txtsp_set_iq_correction")
        channel = read_channel("channel")
        phase = read_float("phase")
        gain = read_float("gain")
        LMS7002M_txtsp_set_iq_correction(aip, channel, phase, gain)
        print("LMS7002M_txtsp_set_iq_correction ejecutada.")
        return True

    elif option == 46:
        print("\nEjecutando LMS7002M_rxtsp_set_iq_correction")
        channel = read_channel("channel")
        phase = read_float("phase")
        gain = read_float("gain")
        LMS7002M_rxtsp_set_iq_correction(aip, channel, phase, gain)
        print("LMS7002M_rxtsp_set_iq_correction ejecutada.")
        return True

    elif option == 47:
        print("\nEjecutando LMS7002M_txtsp_tsg_tone")
        channel = read_channel("channel")
        LMS7002M_txtsp_tsg_tone(aip, channel)
        print("LMS7002M_txtsp_tsg_tone ejecutada.")
        return True

    elif option == 48:
        print("\nEjecutando LMS7002M_rxtsp_tsg_tone")
        channel = read_channel("channel")
        LMS7002M_rxtsp_tsg_tone(aip, channel)
        print("LMS7002M_rxtsp_tsg_tone ejecutada.")
        return True

    elif option == 49:
        print("\nEjecutando LMS7002M_txtsp_tsg_const")
        channel = read_channel("channel")
        val_i = read_int("val_i")
        val_q = read_int("val_q")
        LMS7002M_txtsp_tsg_const(aip, channel, val_i, val_q)
        print("LMS7002M_txtsp_tsg_const ejecutada.")
        return True

    elif option == 50:
        print("\nEjecutando LMS7002M_rxtsp_tsg_const")
        channel = read_channel("channel")
        val_i = read_int("val_i")
        val_q = read_int("val_q")
        LMS7002M_rxtsp_tsg_const(aip, channel, val_i, val_q)
        print("LMS7002M_rxtsp_tsg_const ejecutada.")
        return True

    elif option == 51:
        print("\nEjecutando LMS7002M_rxtsp_read_rssi")
        channel = read_channel("channel")
        rssi = LMS7002M_rxtsp_read_rssi(aip, channel)
        print("LMS7002M_rxtsp_read_rssi ejecutada.")
        print(f"RSSI: {rssi}")
        return True

    elif option == 52:
        print("\nEjecutando LMS7002M_tbb_enable")
        channel = read_channel("channel")
        enable = read_bool("enable")
        LMS7002M_tbb_enable(aip, channel, enable)
        print("LMS7002M_tbb_enable ejecutada.")
        return True

    elif option == 53:
        print("\nEjecutando LMS7002M_trf_enable")
        channel = read_channel("channel")
        enable = read_bool("enable")
        LMS7002M_trf_enable(aip, channel, enable)
        print("LMS7002M_trf_enable ejecutada.")
        return True

    elif option == 54:
        print("\nEjecutando LMS7002M_tbb_set_path")
        channel = read_channel("channel")
        path = read_int("path")
        LMS7002M_tbb_set_path(aip, channel, path)
        print("LMS7002M_tbb_set_path ejecutada.")
        return True

    elif option == 55:
        print("\nEjecutando LMS7002M_tbb_set_test_in")
        channel = read_channel("channel")
        path = read_int("path")
        LMS7002M_tbb_set_test_in(aip, channel, path)
        print("LMS7002M_tbb_set_test_in ejecutada.")
        return True

    elif option == 56:
        print("\nEjecutando LMS7002M_trf_select_band")
        channel = read_channel("channel")
        band = read_int("band")
        LMS7002M_trf_select_band(aip, channel, band)
        print("LMS7002M_trf_select_band ejecutada.")
        return True

    elif option == 57:
        print("\nEjecutando LMS7002M_trf_set_pad")
        channel = read_channel("channel")
        gain = read_float("gain")
        LMS7002M_trf_set_pad(aip, channel, gain)
        print("LMS7002M_trf_set_pad ejecutada.")
        return True

    elif option == 58:
        print("\nEjecutando LMS7002M_rbb_enable")
        channel = read_channel("channel")
        enable = read_bool("enable")
        LMS7002M_rbb_enable(aip, channel, enable)
        print("LMS7002M_rbb_enable ejecutada.")
        return True

    elif option == 59:
        print("\nEjecutando LMS7002M_rfe_enable")
        channel = read_channel("channel")
        enable = read_bool("enable")
        LMS7002M_rfe_enable(aip, channel, enable)
        print("LMS7002M_rfe_enable ejecutada.")
        return True

    elif option == 60:
        print("\nEjecutando LMS7002M_rbb_set_path")
        channel = read_channel("channel")
        path = read_int("path")
        LMS7002M_rbb_set_path(aip, channel, path)
        print("LMS7002M_rbb_set_path ejecutada.")
        return True

    elif option == 61:
        print("\nEjecutando LMS7002M_rfe_set_path")
        channel = read_channel("channel")
        path = read_int("path")
        LMS7002M_rfe_set_path(aip, channel, path)
        print("LMS7002M_rfe_set_path ejecutada.")
        return True

    elif option == 62:
        print("\nEjecutando LMS7002M_rbb_set_pga")
        channel = read_channel("channel")
        gain = read_float("gain")
        LMS7002M_rbb_set_pga(aip, channel, gain)
        print("LMS7002M_rbb_set_pga ejecutada.")
        return True

    elif option == 63:
        print("\nEjecutando LMS7002M_rfe_set_lna")
        channel = read_channel("channel")
        gain = read_float("gain")
        LMS7002M_rfe_set_lna(aip, channel, gain)
        print("LMS7002M_rfe_set_lna ejecutada.")
        return True

    elif option == 64:
        print("\nEjecutando LMS7002M_rfe_set_tia")
        channel = read_channel("channel")
        gain = read_float("gain")
        LMS7002M_rfe_set_tia(aip, channel, gain)
        print("LMS7002M_rfe_set_tia ejecutada.")
        return True

    elif option == 65:
        print("\nEjecutando LMS7002M_rbb_set_test_out")
        channel = read_channel("channel")
        enable = read_bool("enable")
        LMS7002M_rbb_set_test_out(aip, channel, enable)
        print("LMS7002M_rbb_set_test_out ejecutada.")
        return True

    elif option == 66:
        print("\nEjecutando LMS7002M_setup_digital_loopback")
        LMS7002M_setup_digital_loopback(aip)
        print("LMS7002M_setup_digital_loopback ejecutada.")
        return True

    elif option == 67:
        print("\nEjecutando LMS7002M_trf_enable_loopback")
        channel = read_channel("channel")
        enable = read_bool("enable")
        LMS7002M_trf_enable_loopback(aip, channel, enable)
        print("LMS7002M_trf_enable_loopback ejecutada.")
        return True

    elif option == 68:
        print("\nEjecutando LMS7002M_tbb_enable_loopback")
        channel = read_channel("channel")
        mode = read_int("mode")
        swap = read_bool("swap")
        LMS7002M_tbb_enable_loopback(aip, channel, mode, swap)
        print("LMS7002M_tbb_enable_loopback ejecutada.")
        return True

    elif option == 69:
        print("\nEjecutando LMS7002M_trf_set_loopback_pad")
        channel = read_channel("channel")
        gain = read_float("gain")
        LMS7002M_trf_set_loopback_pad(aip, channel, gain)
        print("LMS7002M_trf_set_loopback_pad ejecutada.")
        return True

    elif option == 70:
        print("\nEjecutando LMS7002M_rfe_set_loopback_lna")
        channel = read_channel("channel")
        gain = read_float("gain")
        LMS7002M_rfe_set_loopback_lna(aip, channel, gain)
        print("LMS7002M_rfe_set_loopback_lna ejecutada.")
        return True

    elif option == 71:
        print("\nEjecutando LMS7002M_tbb_set_filter_bw")
        channel = read_channel("channel")
        bw = read_float("bw")
        LMS7002M_tbb_set_filter_bw(aip, channel, bw)
        print("LMS7002M_tbb_set_filter_bw ejecutada.")
        return True

    elif option == 72:
        print("\nEjecutando LMS7002M_rbb_set_filter_bw")
        channel = read_channel("channel")
        bw = read_float("bw")
        LMS7002M_rbb_set_filter_bw(aip, channel, bw)
        print("LMS7002M_rbb_set_filter_bw ejecutada.")
        return True

    elif option == 73:
        print("\nEjecutando TuneTxFilter_8051")
        channel = read_channel("channel")
        bw = read_float("bw")
        TuneTxFilter_8051(aip, channel, bw)
        print("TuneTxFilter_8051 ejecutada.")
        return True

    elif option == 74:
        print("\nEjecutando TuneRxFilter_8051")
        channel = read_channel("channel")
        bw = read_float("bw")
        TuneRxFilter_8051(aip, channel, bw)
        print("TuneRxFilter_8051 ejecutada.")
        return True

    elif option == 75:
        print("\nEjecutando tx_cal_init")
        channel = read_channel("channel")
        tx_cal_init(aip, channel)
        print("tx_cal_init ejecutada.")
        return True

    elif option == 76:
        print("\nEjecutando rx_cal_init")
        channel = read_channel("channel")
        rx_cal_init(aip, channel)
        print("rx_cal_init ejecutada.")
        return True

    elif option == 77:
        print("\nEjecutando CalibrateAll")
        channel = read_channel("channel")
        bw = read_float("bw")
        CalibrateAll(aip, channel, bw)
        print("CalibrateAll ejecutada.")
        return True

    elif option == 78:
        print("\nEjecutando CalibrateTx")
        channel = read_channel("channel")
        bw = read_float("bw")
        CalibrateTx(aip, channel, bw)
        print("CalibrateTx ejecutada.")
        return True

    elif option == 79:
        print("\nEjecutando CalibrateRx")
        channel = read_channel("channel")
        bw = read_float("bw")
        CalibrateRx(aip, channel, bw)
        print("CalibrateRx ejecutada.")
        return True

    else:
        print("Opción no válida.")
        return True





if __name__ == "__main__":
    main()
