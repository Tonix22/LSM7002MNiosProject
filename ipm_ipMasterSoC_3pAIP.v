module ipm_ipMasterSoC_3pAIP
#(
    parameter   CONF_WIDTH          = 5,
    parameter   DATA_WIDTH          = 32
)
(
    // Main
    input  wire                       clk,
    input  wire                       rst,
    
    // MCU
    input  wire [3:0]                 addressMCU,
    input  wire                       rstMCU,
    input  wire                       rdMCU,
    input  wire                       wrMCU,
    inout  wire [7:0]                 dataMCU,
    output wire                       intMCU,
	 input  wire                       uart_rxd,             
	 output wire                       uart_txd,
    input  wire                       MISO_lms7002,
	 output wire                       MOSI,
	 output wire                       SCLK,
	 output wire [1:0]                 SS_n,	 
	 output wire [9:0]                 leds,
	 
	 // FPGACFG
	 output wire LMS1_CORE_LDO_EN,
	 output wire LMS1_TXNRX1,
	 output wire LMS1_TXNRX2,
	 output wire LMS1_TXEN,
	 output wire LMS1_RXEN,
	 
	 // loopback externo
	 	output wire TX1_2_LB_AT,
		output wire TX1_2_LB_H,
		output wire TX1_2_LB_SH,
		output wire TX1_2_LB_L,
	
		output wire TX2_2_LB_AT,
		output wire TX2_2_LB_H,
		output wire TX2_2_LB_SH,
		output wire TX2_2_LB_L,
		
		output wire LMS_RESET,
	 
		 // mux_spi
	 input wire                        sel_switch,
	 output wire                       sel_mux_spi,
	 
	 // uart que viene de la lime
	 output wire                       uart_lime,
	 
	 // GPIO
	 output wire [15:0]                GPIO_out

);
wire MISO_master;

	assign sel_mux_spi = sel_switch;
	assign uart_lime = MISO_master;
	
	wire MISO_fpgacfg;
 
	assign MISO_master = MISO_fpgacfg | MISO_lms7002;  //
	
	assign LMS_RESET = LMS1_RESET && GPIO_out[0];
	

	 
    wire                  w_reset;
    wire [DATA_WIDTH-1:0] w_DataIPtoMCU;
    wire [DATA_WIDTH-1:0] w_DataMCUtoIP;
    wire [CONF_WIDTH-1:0] w_Conf;
    wire                  w_ReadIP;
    wire                  w_WriteIP;
    wire                  w_StartIP;
    wire                  w_INT;

	 wire [7:0]            w_int_IP;

    wire [DATA_WIDTH-1:0] w_dataIn_s0;
    wire                  w_read_s0;
    wire [4:0]            w_config_s0;
    wire                  w_write_s0;
    wire [DATA_WIDTH-1:0] w_dataOut_s0;
    wire                  w_start_s0;

    wire [DATA_WIDTH-1:0] w_dataIn_s1;
    wire                  w_read_s1;
    wire [4:0]            w_config_s1;
    wire                  w_write_s1;
    wire [DATA_WIDTH-1:0] w_dataOut_s1;
    wire                  w_start_s1;
 
    wire [DATA_WIDTH-1:0] w_dataIn_s2;
    wire                  w_read_s2;
    wire [4:0]            w_config_s2;
    wire                  w_write_s2;
    wire [DATA_WIDTH-1:0] w_dataOut_s2;
    wire                  w_start_s2;
 


    assign w_reset = rst;
    
    ipm IPM
    (
        .clk_n_Hz           ( clk             ),
        .ipm_RstIn          ( w_reset         ),
        
        // MCU
        .ipmMCUDataInout    ( dataMCU         ),
        .ipmMCUAddrsIn      ( addressMCU      ),
        .ipmMCURdIn         ( rdMCU           ),
        .ipmMCUWrIn         ( wrMCU           ),
        .ipmMCUINTOut       ( intMCU          ),
        
        // IP
        .ipmPIPDataIn       ( w_DataIPtoMCU   ),
        .ipmPIPConfOut      ( w_Conf          ),
        .ipmPIPReadOut      ( w_ReadIP        ),
        .ipmPIPWriteOut     ( w_WriteIP       ),
        .ipmPIPStartOut     ( w_StartIP       ),
        .ipmPIPDataOut      ( w_DataMCUtoIP   ),
        .ipmPIPINTIn        ( w_INT           )
    );

    ip_master_soc_controller IPCORE_MASTER 
    (
        .i_clk              ( clk             ),
        .i_rst_a            ( rst             ),
        .i_en_s             ( 1'b1            ),
  
        .i_data_in          ( w_DataMCUtoIP   ),
        .o_data_out         ( w_DataIPtoMCU   ),
        .i_write            ( w_WriteIP       ),
        .i_read             ( w_ReadIP        ),
        .i_start            ( w_StartIP       ),
        .i_conf_dbus        ( w_Conf          ),
        .o_int_req          ( w_INT           ),
        
		  .i_int_IP           ( w_int_IP        ),

        .o_dataInAIP_IP_s0  ( w_dataIn_s0     ),  
        .o_readAIP_IP_s0    ( w_read_s0       ),   
        .o_configAIP_IP_s0  ( w_config_s0     ),  
        .o_writeAIP_IP_s0   ( w_write_s0      ),   
        .i_dataOutAIP_IP_s0 ( w_dataOut_s0    ),
        .o_start_IP_s0      ( w_start_s0      ),  

        .o_dataInAIP_IP_s1  ( w_dataIn_s1     ),  
        .o_readAIP_IP_s1    ( w_read_s1       ),   
        .o_configAIP_IP_s1  ( w_config_s1     ),  
        .o_writeAIP_IP_s1   ( w_write_s1      ),   
        .i_dataOutAIP_IP_s1 ( w_dataOut_s1    ),
        .o_start_IP_s1      ( w_start_s1      ), 

        .o_dataInAIP_IP_s2  ( w_dataIn_s2     ),  
        .o_readAIP_IP_s2    ( w_read_s2       ),   
        .o_configAIP_IP_s2  ( w_config_s2     ),  
        .o_writeAIP_IP_s2   ( w_write_s2      ),   
        .i_dataOutAIP_IP_s2 ( w_dataOut_s2    ),
        .o_start_IP_s2      ( w_start_s2      ),
		  
        .uart_rxd           ( uart_rxd ),       
        .uart_txd           ( uart_txd ),

        .leds               ( leds ), 
		  .MISO               ( MISO_master ),
        .MOSI               ( MOSI ),
        .SCLK               ( SCLK ),
        .SS_n               ( SS_n )  
    );
     


	     
		  ID0000100A_gpio_module GPIO(
        .clk_in              ( clk             ),
        .rst_in              ( w_reset         ),
		  .enable              ( 1'b1            ),

        .dataInAIP           ( w_dataIn_s0     ),
        .dataOutAIP          ( w_dataOut_s0    ),		  
        .writeAIP            ( w_write_s0      ),
        .readAIP             ( w_read_s0       ),
        .startAIP            ( w_start_s0      ),
        .configAIP           ( w_config_s0     ),
		  .intAIP              ( w_int_IP[0]     ),
		  .GPIO_out            ( GPIO_out        )
       
    );
		// FPGA direct clocking	
		wire [15:0] phase_reg_sel;
		wire [15:0] drct_clk_en;
		wire [4:0]  clk_ind;
		wire [4:0]  cnt_ind;
		wire        load_phase_reg;

	// interface config
		wire [15:0] ch_en;
		wire [1:0]  smpl_width;
		wire        mode;
		wire        ddr_en;
		wire        trxiq_pulse;
		wire        mimo_int_en;
		wire        synch_dis;
		wire        synch_mode;
		wire        smpl_nr_clr;
		wire        txpct_loss_clr;
		wire        rx_en;
		wire        tx_en;
		wire        rx_ptrn_en;
		wire        tx_ptrn_en;
		wire        tx_cnt_en;

	
		wire [15:0] sync_size;
		wire [31:0] sync_pulse_period;

	
		wire [15:0] txant_pre;
		wire [15:0] txant_post;

	
		wire [15:0] SPI_SS;

		wire LMS1_SS;
		wire LMS1_RESET;

	
		wire [15:0] GPIO_fpga;

		wire [3:0] CLK_ENA;
		
		assign TX1_2_LB_AT = GPIO_fpga[1];
		assign TX1_2_LB_H = GPIO_fpga[0];
		assign TX1_2_LB_SH = GPIO_fpga[2];
		assign TX1_2_LB_L = ~ GPIO_fpga[0]; 
		
		assign TX2_2_LB_AT = GPIO_fpga[5];
		assign TX2_2_LB_H = GPIO_fpga[4];
		assign TX2_2_LB_SH = GPIO_fpga[6];
		assign TX2_2_LB_L = ~ GPIO_fpga[4];
		
		
	 
	 fpgacfg u_fpgacfg (

    .maddress		(1'b0),
    .mimo_en		(1'b1),

    .sdin		(MOSI),
    .sclk		(SCLK),
    .sen			(SS_n[1]),
    .sdout		(MISO_fpgacfg),

    .lreset		(rst),
    .mreset		(rst),

   // .oen(oen),
   // .stateo(stateo),

    // -------------------------
    // FPGA direct clocking
    // -------------------------
    .from_fpgacfg_phase_reg_sel   (phase_reg_sel),
    .from_fpgacfg_drct_clk_en		(drct_clk_en),
    .from_fpgacfg_clk_ind			(clk_ind),
    .from_fpgacfg_cnt_ind			(cnt_ind),
    .from_fpgacfg_load_phase_reg	(load_phase_reg),

    // -------------------------
    // Interface config
    // -------------------------
    .from_fpgacfg_ch_en				(ch_en),
    .from_fpgacfg_smpl_width		(smpl_width),
    .from_fpgacfg_mode				(mode),
    .from_fpgacfg_ddr_en			(ddr_en),
    .from_fpgacfg_trxiq_pulse		(trxiq_pulse),
    .from_fpgacfg_mimo_int_en		(mimo_int_en),
    .from_fpgacfg_synch_dis		(synch_dis),
    .from_fpgacfg_synch_mode		(synch_mode),
    .from_fpgacfg_smpl_nr_clr		(smpl_nr_clr),
    .from_fpgacfg_txpct_loss_clr	(txpct_loss_clr),
    .from_fpgacfg_rx_en				(rx_en),
    .from_fpgacfg_tx_en				(tx_en),
    .from_fpgacfg_rx_ptrn_en		(rx_ptrn_en),
    .from_fpgacfg_tx_ptrn_en		(tx_ptrn_en),
    .from_fpgacfg_tx_cnt_en		(tx_cnt_en),


    .from_fpgacfg_sync_size				(sync_size),
    .from_fpgacfg_sync_pulse_period		(sync_pulse_period),

    .from_fpgacfg_txant_pre				(txant_pre),
    .from_fpgacfg_txant_post				(txant_post),

    .from_fpgacfg_SPI_SS					(SPI_SS),
    .from_fpgacfg_LMS1_SS					(LMS1_SS),
    .from_fpgacfg_LMS1_RESET				(LMS1_RESET),
    .from_fpgacfg_LMS1_CORE_LDO_EN		(LMS1_CORE_LDO_EN),
    .from_fpgacfg_LMS1_TXNRX1				(LMS1_TXNRX1),
    .from_fpgacfg_LMS1_TXNRX2				(LMS1_TXNRX2),
    .from_fpgacfg_LMS1_TXEN				(LMS1_TXEN),
    .from_fpgacfg_LMS1_RXEN				(LMS1_RXEN),

    .from_fpgacfg_GPIO				(GPIO_fpga),
  
    .from_fpgacfg_CLK_ENA			(CLK_ENA)
);
	   



 
endmodule
