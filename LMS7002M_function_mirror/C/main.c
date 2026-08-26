#include <stdio.h>
#include "lms7002m_functions_mirror.h"

void show_menu(void);
void execute_option(void *aip, int option);

int main() {
    int option = 0;

    show_menu();
    void *aip = aip_init();

    while (1) {
        printf("Seleccione una opcion:\n");
        scanf("%d", &option);
        if (option == 0) {
            printf("Saliendo del programa.\n");
            break;
        }
        execute_option(aip, option); }
        aip_finish(aip);
    return 0;
}




void execute_option(void *aip, int option){

    switch (option) {
    case 1:
        LMS7002M_create(aip);
        break;

    case 2:
        LMS7002M_regs(aip);
        break;

    case 3:
        LMS7002M_destroy(aip);
        break;

    case 4:
        LMS7002M_reset(aip);
        break;

    case 5:
        LMS7002M_power_down(aip);
        break;

    case 6:
        LMS7002M_regs_to_rfic(aip);
        break;

    case 7:
        LMS7002M_rfic_to_regs(aip);
        break;

    case 8: {
        unsigned int addr_tmp, data_tmp;
        uint16_t addr, data;

        printf("addr en hexadecimal, sin 0x: ");
        scanf("%x", &addr_tmp);

        printf("data en hexadecimal, sin 0x: ");
        scanf("%x", &data_tmp);

        addr = (uint16_t)addr_tmp;
        data = (uint16_t)data_tmp;

        LMS7002M_spi_write(aip, addr, data);
        break;
    }

    case 9: {
        unsigned int addr_tmp;
        uint16_t addr;

        printf("addr en hexadecimal, sin 0x: ");
        scanf("%x", &addr_tmp);

        addr = (uint16_t)addr_tmp;

        uint16_t data = LMS7002M_spi_read(aip, addr);
        printf("Data read: 0x%04X\n", data);
        break;
    }

    case 10: {
        unsigned int addr_tmp;
        uint16_t addr;

        printf("addr en hexadecimal, sin 0x: ");
        scanf("%x", &addr_tmp);

        addr = (uint16_t)addr_tmp;

        LMS7002M_regs_spi_write(aip, addr);
        break;
    }

    case 11: {
        unsigned int addr_tmp;
        uint16_t addr;

        printf("addr en hexadecimal, sin 0x: ");
        scanf("%x", &addr_tmp);

        addr = (uint16_t)addr_tmp;

        LMS7002M_regs_spi_read(aip, addr);
        break;
    }

    case 12: {
        int mode;

        printf("mode: ");
        scanf("%d", &mode);

        LMS7002M_set_spi_mode(aip, mode);
        break;
    }

    case 13:
        LMS7002M_set_work_mode(aip);
        break;

    case 14: {
        int port, direction, mclk_div;

        printf("port: ");
        scanf("%d", &port);

        printf("direction: ");
        scanf("%d", &direction);

        printf("mclk_div: ");
        scanf("%d", &mclk_div);

        LMS7002M_configure_lml_port(aip, port, direction, mclk_div);
        break;
    }

    case 15: {
        int enable;

        printf("enable (0/1): ");
        scanf("%d", &enable);

        LMS7002M_invert_fclk(aip, enable);
        break;
    }

    case 16: {
        int direction;

        printf("direction: ");
        scanf("%d", &direction);

        LMS7002M_reset_lml_fifo(aip, direction);
        break;
    }

    case 17: {
        int direction;

        printf("direction: ");
        scanf("%d", &direction);

        LMS7002M_set_mac_dir(aip, direction);
        break;
    }

    case 18: {
        char channel;

        printf("channel (A/B/C): ");
        scanf(" %c", &channel);

        LMS7002M_set_mac_ch(aip, channel);
        break;
    }

    case 19: {
        uint32_t direction;
        uint32_t positions;

        printf("direction: ");
        scanf("%u", &direction);

        printf("positions: ");
        scanf("%u", &positions);

        LMS7002M_set_diq_mux(aip, direction, positions);
        break;
    }

    case 20:
        LMS7002M_RESET(aip);
        break;

    case 21: {
        int enable;

        printf("enable (0/1): ");
        scanf("%d", &enable);

        LMS7002M_CORE_LDO_ENABLE(aip, enable);
        break;
    }

    case 22: {
        int enable;

        printf("enable (0/1): ");
        scanf("%d", &enable);

        LMS7002M_RXEN(aip, enable);
        break;
    }

    case 23: {
        int enable;

        printf("enable (0/1): ");
        scanf("%d", &enable);

        LMS7002M_TXEN(aip, enable);
        break;
    }

    case 24: {
        int enable;

        printf("enable (0/1): ");
        scanf("%d", &enable);

        LMS7002M_TXNRX1(aip, enable);
        break;
    }

    case 25: {
        int enable;

        printf("enable (0/1): ");
        scanf("%d", &enable);

        LMS7002M_TXNRX2(aip, enable);
        break;
    }

    case 26: {
        char channel;
        int enable;

        printf("channel (A/B): ");
        scanf(" %c", &channel);

        printf("enable (0/1): ");
        scanf("%d", &enable);

        LMS7002M_enable_external_loopback(aip, channel, enable);
        break;
    }

    case 27: {
        int enable;

        printf("enable (0/1): ");
        scanf("%d", &enable);

        LMS7002M_xbuf_share_tx(aip, enable);
        break;
    }

    case 28: {
        int enable;

        printf("enable (0/1): ");
        scanf("%d", &enable);

        LMS7002M_xbuf_enable_bias(aip, enable);
        break;
    }

    case 29: {
        int enable;

        printf("enable (0/1): ");
        scanf("%d", &enable);

        LMS7002M_sxt_to_sxr(aip, enable);
        break;
    }

    case 30: {
        int enable;
        int group;

        printf("enable (0/1): ");
        scanf("%d", &enable);

        printf("group: ");
        scanf("%d", &group);

        LMS7002M_ldo_enable(aip, enable, group);
        break;
    }

    case 31: {
        int direction;
        char channel;
        int enable;

        printf("direction: ");
        scanf("%d", &direction);

        printf("channel (A/B): ");
        scanf(" %c", &channel);

        printf("enable (0/1): ");
        scanf("%d", &enable);

        LMS7002M_afe_enable(aip, direction, channel, enable);
        break;
    }

    case 32: {
        int direction;
        double fref;
        double freq;

        printf("direction: ");
        scanf("%d", &direction);

        printf("fref: ");
        scanf("%lf", &fref);

        printf("freq: ");
        scanf("%lf", &freq);

        LMS7002M_set_lo_freq(aip, direction, fref, freq);
        break;
    }

    case 33: {
        int direction;
        int enable;

        printf("direction: ");
        scanf("%d", &direction);

        printf("enable (0/1): ");
        scanf("%d", &enable);

        LMS7002M_sxx_enable(aip, direction, enable);
        break;
    }

    case 34: {
        double fref;
        double freq;

        printf("fref: ");
        scanf("%lf", &fref);

        printf("freq: ");
        scanf("%lf", &freq);

        LMS7002M_set_data_clock(aip, fref, freq);
        break;
    }

    case 35: {
        char channel;
        int enable;

        printf("channel (A/B): ");
        scanf(" %c", &channel);

        printf("enable (0/1): ");
        scanf("%d", &enable);

        LMS7002M_txtsp_enable(aip,  channel, enable);
        break;
    }

    case 36: {
        char channel;
        int enable;

        printf("channel (A/B): ");
        scanf(" %c", &channel);

        printf("enable (0/1): ");
        scanf("%d", &enable);

        LMS7002M_rxtsp_enable(aip, channel, enable);
        break;
    }

    case 37: {
        char channel;
        int interp;

        printf("channel (A/B): ");
        scanf(" %c", &channel);

        printf("interp: ");
        scanf("%d", &interp);

        LMS7002M_txtsp_set_interp(aip, channel, interp);
        break;
    }

    case 38: {
        char channel;
        int decim;

        printf("channel (A/B): ");
        scanf(" %c", &channel);

        printf("decim: ");
        scanf("%d", &decim);

        LMS7002M_rxtsp_set_decim(aip, channel, decim);
        break;
    }

    case 39: {
        uint32_t direction;
        char channel;
        uint32_t gfir;
        uint32_t ntaps;
        uint32_t i;

        printf("direction: ");
        scanf("%u", &direction);

        printf("channel (A/B): ");
        scanf(" %c", &channel);

        printf("gfir: ");
        scanf("%u", &gfir);

        printf("ntaps: ");
        scanf("%u", &ntaps);

        short taps[40] =  {-14499, 0, -32381, -32126, 16360, 0, 13, 0, -6275, 3, -14499, 1, 5, 0, 2383, 0, 40, 0, -6279, 3, -6360, 3, -6152, 3, 4944,0, -6360, -1, 40, 0, -6316, 3, 5, 0, -14499, 1, 13, 0, 19455, 16765};

        // for (i = 0; i < ntaps; i++) {
        //     int32_t temp;

        //     printf("taps[%u]: ", i);
        //     scanf("%d", &temp);

        //     taps[i] = u32(temp);
        // }

        LMS7002M_set_gfir_taps(aip, direction, channel, gfir, taps, ntaps);
        break;
    }

    case 40: {
        int direction;
        char channel;
        double freq_rel;

        printf("direction: ");
        scanf("%d", &direction);

        printf("channel (A/B): ");
        scanf(" %c", &channel);

        printf("freq_rel: ");
        scanf("%lf", &freq_rel);

        LMS7002M_set_nco_freq(aip, direction, channel, freq_rel);
        break;
    }

    case 41: {
        char channel;
        double freq_rel;

        printf("channel (A/B): ");
        scanf(" %c", &channel);

        printf("freq_rel: ");
        scanf("%lf", &freq_rel);

        LMS7002M_txtsp_set_freq(aip, channel, freq_rel);
        break;
    }

    case 42: {
        char channel;
        double freq_rel;

        printf("channel (A/B): ");
        scanf(" %c", &channel);

        printf("freq_rel: ");
        scanf("%lf", &freq_rel);

        LMS7002M_rxtsp_set_freq(aip, channel, freq_rel);
        break;
    }

    case 43: {
        char channel;
        int enable;
        int window;

        printf("channel (A/B): ");
        scanf(" %c", &channel);

        printf("enable (0/1): ");
        scanf("%d", &enable);

        printf("window: ");
        scanf("%d", &window);

        LMS7002M_rxtsp_set_dc_correction(aip, channel, enable, window);
        break;
    }

    case 44: {
        char channel;
        double val_i;
        double val_q;

        printf("channel (A/B): ");
        scanf(" %c", &channel);

        printf("val_i: ");
        scanf("%lf", &val_i);

        printf("val_q: ");
        scanf("%lf", &val_q);

        LMS7002M_txtsp_set_dc_correction(aip, channel, val_i, val_q);
        break;
    }

    case 45: {
        char channel;
        double phase;
        double gain;

        printf("channel (A/B): ");
        scanf(" %c", &channel);

        printf("phase: ");
        scanf("%lf", &phase);

        printf("gain: ");
        scanf("%lf", &gain);

        LMS7002M_txtsp_set_iq_correction(aip, channel, phase, gain);
        break;
    }

    case 46: {
        char channel;
        double phase;
        double gain;

        printf("channel (A/B): ");
        scanf(" %c", &channel);

        printf("phase: ");
        scanf("%lf", &phase);

        printf("gain: ");
        scanf("%lf", &gain);

        LMS7002M_rxtsp_set_iq_correction(aip, channel, phase, gain);
        break;
    }

    case 47: {
        char channel;

        printf("channel (A/B): ");
        scanf(" %c", &channel);

        LMS7002M_txtsp_tsg_tone(aip, channel);
        break;
    }

    case 48: {
        char channel;

        printf("channel (A/B): ");
        scanf(" %c", &channel);

        LMS7002M_rxtsp_tsg_tone(aip, channel);
        break;
    }

    case 49: {
        char channel;
        int val_i;
        int val_q;

        printf("channel (A/B): ");
        scanf(" %c", &channel);

        printf("val_i: ");
        scanf("%d", &val_i);

        printf("val_q: ");
        scanf("%d", &val_q);

        LMS7002M_txtsp_tsg_const(aip, channel, val_i, val_q);
        break;
    }

    case 50: {
        char channel;
        int val_i;
        int val_q;

        printf("channel (A/B): ");
        scanf(" %c", &channel);

        printf("val_i: ");
        scanf("%d", &val_i);

        printf("val_q: ");
        scanf("%d", &val_q);

        LMS7002M_rxtsp_tsg_const(aip, channel, val_i, val_q);
        break;
    }

    case 51: {
        char channel;

        printf("channel (A/B): ");
        scanf(" %c", &channel);

        uint16_t rssi = LMS7002M_rxtsp_read_rssi(aip, channel);
        printf("RSSI: %u\n", rssi);
        break;
    }

    case 52: {
        char channel;
        int enable;

        printf("channel (A/B): ");
        scanf(" %c", &channel);

        printf("enable (0/1): ");
        scanf("%d", &enable);

        LMS7002M_tbb_enable(aip, channel, enable);
        break;
    }

    case 53: {
        char channel;
        int enable;

        printf("channel (A/B): ");
        scanf(" %c", &channel);

        printf("enable (0/1): ");
        scanf("%d", &enable);

        LMS7002M_trf_enable(aip, channel, enable);
        break;
    }

    case 54: {
        char channel;
        int path;

        printf("channel (A/B): ");
        scanf(" %c", &channel);

        printf("path: ");
        scanf("%d", &path);

        LMS7002M_tbb_set_path(aip, channel, path);
        break;
    }

    case 55: {
        char channel;
        int path;

        printf("channel (A/B): ");
        scanf(" %c", &channel);

        printf("path: ");
        scanf("%d", &path);

        LMS7002M_tbb_set_test_in(aip, channel, path);
        break;
    }

    case 56: {
        char channel;
        int band;

        printf("channel (A/B): ");
        scanf(" %c", &channel);

        printf("band: ");
        scanf("%d", &band);

        LMS7002M_trf_select_band(aip, channel, band);
        break;
    }

    case 57: {
        char channel;
        double gain;

        printf("channel (A/B): ");
        scanf(" %c", &channel);

        printf("gain: ");
        scanf("%lf", &gain);

        LMS7002M_trf_set_pad(aip, channel, gain);
        break;
    }

    case 58: {
        char channel;
        int enable;

        printf("channel (A/B): ");
        scanf(" %c", &channel);

        printf("enable (0/1): ");
        scanf("%d", &enable);

        LMS7002M_rbb_enable(aip, channel, enable);
        break;
    }

    case 59: {
        char channel;
        int enable;

        printf("channel (A/B): ");
        scanf(" %c", &channel);

        printf("enable (0/1): ");
        scanf("%d", &enable);

        LMS7002M_rfe_enable(aip, channel, enable);
        break;
    }

    case 60: {
        char channel;
        int path;

        printf("channel (A/B): ");
        scanf(" %c", &channel);

        printf("path: ");
        scanf("%d", &path);

        LMS7002M_rbb_set_path(aip, channel, path);
        break;
    }

    case 61: {
        char channel;
        int path;

        printf("channel (A/B): ");
        scanf(" %c", &channel);

        printf("path: ");
        scanf("%d", &path);

        LMS7002M_rfe_set_path(aip, channel, path);
        break;
    }

    case 62: {
        char channel;
        double gain;

        printf("channel (A/B): ");
        scanf(" %c", &channel);

        printf("gain: ");
        scanf("%lf", &gain);

        LMS7002M_rbb_set_pga(aip, channel, gain);
        break;
    }

    case 63: {
        char channel;
        double gain;

        printf("channel (A/B): ");
        scanf(" %c", &channel);

        printf("gain: ");
        scanf("%lf", &gain);

        LMS7002M_rfe_set_lna(aip, channel, gain);
        break;
    }

    case 64: {
        char channel;
        double gain;

        printf("channel (A/B): ");
        scanf(" %c", &channel);

        printf("gain: ");
        scanf("%lf", &gain);

        LMS7002M_rfe_set_tia(aip, channel, gain);
        break;
    }

    case 65: {
        char channel;
        int enable;

        printf("channel (A/B): ");
        scanf(" %c", &channel);

        printf("enable (0/1): ");
        scanf("%d", &enable);

        LMS7002M_rbb_set_test_out(aip, channel, enable);
        break;
    }

    case 66:
        LMS7002M_setup_digital_loopback(aip);
        break;

    case 67: {
        char channel;
        int enable;

        printf("channel (A/B): ");
        scanf(" %c", &channel);

        printf("enable (0/1): ");
        scanf("%d", &enable);

        LMS7002M_trf_enable_loopback(aip, channel, enable);
        break;
    }

    case 68: {
        char channel;
        int mode;
        int swap;

        printf("channel (A/B): ");
        scanf(" %c", &channel);

        printf("mode: ");
        scanf("%d", &mode);

        printf("swap (0/1): ");
        scanf("%d", &swap);

        LMS7002M_tbb_enable_loopback(aip, channel, mode, swap);
        break;
    }

    case 69: {
        char channel;
        double gain;

        printf("channel (A/B): ");
        scanf(" %c", &channel);

        printf("gain: ");
        scanf("%lf", &gain);

        LMS7002M_trf_set_loopback_pad(aip, channel, gain);
        break;
    }

    case 70: {
        char channel;
        double gain;

        printf("channel (A/B): ");
        scanf(" %c", &channel);

        printf("gain: ");
        scanf("%lf", &gain);

        LMS7002M_rfe_set_loopback_lna(aip, channel, gain);
        break;
    }

    case 71: {
        char channel;
        double bw;

        printf("channel (A/B): ");
        scanf(" %c", &channel);

        printf("bw: ");
        scanf("%lf", &bw);

        LMS7002M_tbb_set_filter_bw(aip, channel, bw);
        break;
    }

    case 72: {
        char channel;
        double bw;

        printf("channel (A/B): ");
        scanf(" %c", &channel);

        printf("bw: ");
        scanf("%lf", &bw);

        LMS7002M_rbb_set_filter_bw(aip, channel, bw);
        break;
    }

    case 73: {
        char channel;
        double bw;

        printf("channel (A/B): ");
        scanf(" %c", &channel);

        printf("bw: ");
        scanf("%lf", &bw);

        TuneTxFilter_8051(aip, channel, bw);
        break;
    }

    case 74: {
        char channel;
        double bw;

        printf("channel (A/B): ");
        scanf(" %c", &channel);

        printf("bw: ");
        scanf("%lf", &bw);

        TuneRxFilter_8051(aip, channel, bw);
        break;
    }

    case 75: {
        char channel;

        printf("channel (A/B): ");
        scanf(" %c", &channel);

        tx_cal_init(aip, channel);
        break;
    }

    case 76: {
        char channel;

        printf("channel (A/B): ");
        scanf(" %c", &channel);

        rx_cal_init(aip, channel);
        break;
    }

    case 77: {
        char channel;
        double bw;

        printf("channel (A/B): ");
        scanf(" %c", &channel);

        printf("bw: ");
        scanf("%lf", &bw);

        CalibrateAll(aip, channel, bw);
        break;
    }

    case 78: {
        char channel;
        double bw;

        printf("channel (A/B): ");
        scanf(" %c", &channel);

        printf("bw: ");
        scanf("%lf", &bw);

        CalibrateTx(aip, channel, bw);
        break;
    }

    case 79: {
        char channel;
        double bw;

        printf("channel (A/B): ");
        scanf(" %c", &channel);

        printf("bw: ");
        scanf("%lf", &bw);

        CalibrateRx(aip, channel, bw);
        break;
    }

    default:
        printf("Opción no válida.\n");
        break;
} }






void show_menu(void)
{
    printf("\n");
    printf("=========== MENU LMS7002M ===========\n");
    printf("0. Salir\n");

    printf("\n--- Funciones de gestion e inicializacion ---\n");
    printf("1.  LMS7002M_create\n");
    printf("2.  LMS7002M_regs\n");
    printf("3.  LMS7002M_destroy\n");
    printf("4.  LMS7002M_reset\n");
    printf("5.  LMS7002M_power_down\n");

    printf("\n--- Funciones de comunicacion SPI y sincronizacion de registros ---\n");
    printf("6.  LMS7002M_regs_to_rfic\n");
    printf("7.  LMS7002M_rfic_to_regs\n");
    printf("8.  LMS7002M_spi_write\n");
    printf("9.  LMS7002M_spi_read\n");
    printf("10. LMS7002M_regs_spi_write\n");
    printf("11. LMS7002M_regs_spi_read\n");
    printf("12. LMS7002M_set_spi_mode\n");

    printf("\n--- Funciones de configuracion de interfaz digital ---\n");
    printf("13. LMS7002M_set_work_mode\n");
    printf("14. LMS7002M_configure_lml_port\n");
    printf("15. LMS7002M_invert_fclk\n");
    printf("16. LMS7002M_reset_lml_fifo\n");
    printf("17. LMS7002M_set_mac_dir\n");
    printf("18. LMS7002M_set_mac_ch\n");
    printf("19. LMS7002M_set_diq_mux\n");

    printf("\n--- Funciones de control auxiliar y senales externas ---\n");
    printf("20. LMS7002M_RESET\n");
    printf("21. LMS7002M_CORE_LDO_ENABLE\n");
    printf("22. LMS7002M_RXEN\n");
    printf("23. LMS7002M_TXEN\n");
    printf("24. LMS7002M_TXNRX1\n");
    printf("25. LMS7002M_TXNRX2\n");
    printf("26. LMS7002M_enable_external_loopback\n");

    printf("\n--- Funciones de referencias y distribucion de reloj ---\n");
    printf("27. LMS7002M_xbuf_share_tx\n");
    printf("28. LMS7002M_xbuf_enable_bias\n");
    printf("29. LMS7002M_sxt_to_sxr\n");

    printf("\n--- Funciones de alimentacion y AFE ---\n");
    printf("30. LMS7002M_ldo_enable\n");
    printf("31. LMS7002M_afe_enable\n");

    printf("\n--- Funciones de frecuencia y generacion de reloj ---\n");
    printf("32. LMS7002M_set_lo_freq\n");
    printf("33. LMS7002M_sxx_enable\n");
    printf("34. LMS7002M_set_data_clock\n");

    printf("\n--- Funciones de procesamiento digital TX/RX ---\n");
    printf("35. LMS7002M_txtsp_enable\n");
    printf("36. LMS7002M_rxtsp_enable\n");
    printf("37. LMS7002M_txtsp_set_interp\n");
    printf("38. LMS7002M_rxtsp_set_decim\n");
    printf("39. LMS7002M_set_gfir_taps\n");
    printf("40. LMS7002M_set_nco_freq\n");
    printf("41. LMS7002M_txtsp_set_freq\n");
    printf("42. LMS7002M_rxtsp_set_freq\n");
    printf("43. LMS7002M_rxtsp_set_dc_correction\n");
    printf("44. LMS7002M_txtsp_set_dc_correction\n");
    printf("45. LMS7002M_txtsp_set_iq_correction\n");
    printf("46. LMS7002M_rxtsp_set_iq_correction\n");
    printf("47. LMS7002M_txtsp_tsg_tone\n");
    printf("48. LMS7002M_rxtsp_tsg_tone\n");
    printf("49. LMS7002M_txtsp_tsg_const\n");
    printf("50. LMS7002M_rxtsp_tsg_const\n");
    printf("51. LMS7002M_rxtsp_read_rssi\n");

    printf("\n--- Funciones de configuracion de la cadena analogica de transmision ---\n");
    printf("52. LMS7002M_tbb_enable\n");
    printf("53. LMS7002M_trf_enable\n");
    printf("54. LMS7002M_tbb_set_path\n");
    printf("55. LMS7002M_tbb_set_test_in\n");
    printf("56. LMS7002M_trf_select_band\n");
    printf("57. LMS7002M_trf_set_pad\n");

    printf("\n--- Funciones de configuracion de la cadena analogica de recepcion ---\n");
    printf("58. LMS7002M_rbb_enable\n");
    printf("59. LMS7002M_rfe_enable\n");
    printf("60. LMS7002M_rbb_set_path\n");
    printf("61. LMS7002M_rfe_set_path\n");
    printf("62. LMS7002M_rbb_set_pga\n");
    printf("63. LMS7002M_rfe_set_lna\n");
    printf("64. LMS7002M_rfe_set_tia\n");
    printf("65. LMS7002M_rbb_set_test_out\n");

    printf("\n--- Funciones de lazos de retroalimentacion y pruebas internas ---\n");
    printf("66. LMS7002M_setup_digital_loopback\n");
    printf("67. LMS7002M_trf_enable_loopback\n");
    printf("68. LMS7002M_tbb_enable_loopback\n");
    printf("69. LMS7002M_trf_set_loopback_pad\n");
    printf("70. LMS7002M_rfe_set_loopback_lna\n");

    printf("\n--- Funciones de configuracion de filtros y calibracion ---\n");
    printf("71. LMS7002M_tbb_set_filter_bw\n");
    printf("72. LMS7002M_rbb_set_filter_bw\n");
    printf("73. TuneTxFilter_8051\n");
    printf("74. TuneRxFilter_8051\n");
    printf("75. tx_cal_init\n");
    printf("76. rx_cal_init\n");
    printf("77. CalibrateAll\n");
    printf("78. CalibrateTx\n");
    printf("79. CalibrateRx\n");

    printf("======================================\n");
}
