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
    input  wire                       MISO,
	 output wire                       MOSI,
	 output wire                       SCLK,
	 output wire                       SS_n,	 
	 output wire [9:0]                 leds

);

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
		  .MISO               ( MISO ),
        .MOSI               ( MOSI ),
        .SCLK               ( SCLK ),
        .SS_n               ( SS_n )  
    );
     


    ID00001001_dummy dummy0(
        .clk               ( clk             ),
        .rst_a             ( w_reset         ),
		  .en_s              ( 1'b1            ),

        .data_in           ( w_dataIn_s0     ),
        .data_out          ( w_dataOut_s0    ),		  
        .write             ( w_write_s0      ),
        .read              ( w_read_s0       ),
        .start             ( w_start_s0      ),
        .conf_dbus         ( w_config_s0     ),
		  .int_req           ( w_int_IP[0]     )
       
    );

    ID00001001_dummy dummy1(
        .clk               ( clk             ),
        .rst_a             ( w_reset         ),
		  .en_s              ( 1'b1            ),

        .data_in           ( w_dataIn_s1     ),
        .data_out          ( w_dataOut_s1    ),		  
        .write             ( w_write_s1      ),
        .read              ( w_read_s1       ),
        .start             ( w_start_s1      ),
        .conf_dbus         ( w_config_s1     ),
		  .int_req           ( w_int_IP[1]     )
       
    );

    ID00001001_dummy dummy2(
        .clk               ( clk             ),
        .rst_a             ( w_reset         ),
		  .en_s              ( 1'b1            ),

        .data_in           ( w_dataIn_s2     ),
        .data_out          ( w_dataOut_s2    ),		  
        .write             ( w_write_s2      ),
        .read              ( w_read_s2       ),
        .start             ( w_start_s2      ),
        .conf_dbus         ( w_config_s2     ),
		  .int_req           ( w_int_IP[2]     )
       
    );	 



 
endmodule
