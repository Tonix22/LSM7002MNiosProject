
module master_nios_multiple_slave (
	aip_up_0_aip_dataout,
	aip_up_0_aip_int,
	aip_up_0_aip_config,
	aip_up_0_aip_datain,
	aip_up_0_aip_read,
	aip_up_0_aip_write,
	aip_up_0_core_int,
	aip_up_0_aip_start,
	clk_clk,
	int_ip_s0_export,
	leds_export,
	port_s0_aip_dataout,
	port_s0_aip_int,
	port_s0_aip_config,
	port_s0_aip_datain,
	port_s0_aip_read,
	port_s0_aip_write,
	port_s0_core_int,
	port_s0_aip_start,
	port_s1_aip_dataout,
	port_s1_aip_int,
	port_s1_aip_config,
	port_s1_aip_datain,
	port_s1_aip_read,
	port_s1_aip_write,
	port_s1_core_int,
	port_s1_aip_start,
	port_s2_aip_dataout,
	port_s2_aip_int,
	port_s2_aip_config,
	port_s2_aip_datain,
	port_s2_aip_read,
	port_s2_aip_write,
	port_s2_core_int,
	port_s2_aip_start,
	reset_reset_n,
	spi_MISO,
	spi_MOSI,
	spi_SCLK,
	spi_SS_n,
	start_up_0_export,
	uart_rxd,
	uart_txd);	

	input	[31:0]	aip_up_0_aip_dataout;
	input		aip_up_0_aip_int;
	output	[4:0]	aip_up_0_aip_config;
	output	[31:0]	aip_up_0_aip_datain;
	output		aip_up_0_aip_read;
	output		aip_up_0_aip_write;
	output		aip_up_0_core_int;
	output		aip_up_0_aip_start;
	input		clk_clk;
	input	[7:0]	int_ip_s0_export;
	output	[9:0]	leds_export;
	input	[31:0]	port_s0_aip_dataout;
	input		port_s0_aip_int;
	output	[4:0]	port_s0_aip_config;
	output	[31:0]	port_s0_aip_datain;
	output		port_s0_aip_read;
	output		port_s0_aip_write;
	output		port_s0_core_int;
	output		port_s0_aip_start;
	input	[31:0]	port_s1_aip_dataout;
	input		port_s1_aip_int;
	output	[4:0]	port_s1_aip_config;
	output	[31:0]	port_s1_aip_datain;
	output		port_s1_aip_read;
	output		port_s1_aip_write;
	output		port_s1_core_int;
	output		port_s1_aip_start;
	input	[31:0]	port_s2_aip_dataout;
	input		port_s2_aip_int;
	output	[4:0]	port_s2_aip_config;
	output	[31:0]	port_s2_aip_datain;
	output		port_s2_aip_read;
	output		port_s2_aip_write;
	output		port_s2_core_int;
	output		port_s2_aip_start;
	input		reset_reset_n;
	input		spi_MISO;
	output		spi_MOSI;
	output		spi_SCLK;
	output		spi_SS_n;
	input		start_up_0_export;
	input		uart_rxd;
	output		uart_txd;
endmodule
