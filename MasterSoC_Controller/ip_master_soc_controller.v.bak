module ip_master_soc_controller
#(
    // Parameter Declarations
    parameter DATA_WORD = 32
)

(
    // FPGA
    input  wire                 i_clk,
    input  wire                 i_rst_a,
    input  wire                 i_en_s,
	 
	 input  wire                 uart_rxd,       
	 output wire                 uart_txd,
	 output wire [9:0]           leds,    
    input  wire                 MISO,
	 output wire                 MOSI,
	 output wire                 SCLK,
	 output wire [1:0]           SS_n,
	 

    // MCU
    input  wire [4:0]           i_conf_dbus,
    input  wire                 i_read,
    input  wire                 i_write,
    input  wire                 i_start,
    input  wire [DATA_WORD-1:0] i_data_in,
    output wire                 o_int_req,
    output wire [DATA_WORD-1:0] o_data_out, 

    //AIP port 0 
    output wire [DATA_WORD-1:0] o_dataInAIP_IP_s0,  
    output wire                 o_readAIP_IP_s0,   
    output wire [4:0]           o_configAIP_IP_s0,  
    output wire                 o_writeAIP_IP_s0,   
    input  wire [DATA_WORD-1:0] i_dataOutAIP_IP_s0,
    input  wire [7:0]           i_int_IP,
    output wire                 o_start_IP_s0,

    // AIP port 1
    output wire [DATA_WORD-1:0] o_dataInAIP_IP_s1,  
    output wire                 o_readAIP_IP_s1,   
    output wire [4:0]           o_configAIP_IP_s1,  
    output wire                 o_writeAIP_IP_s1,   
    input  wire [DATA_WORD-1:0] i_dataOutAIP_IP_s1,
    input  wire                 i_int_IP_s1,
    output wire                 o_start_IP_s1, 

    //AIP port 2 
    output wire [DATA_WORD-1:0] o_dataInAIP_IP_s2,  
    output wire                 o_readAIP_IP_s2,   
    output wire [4:0]           o_configAIP_IP_s2,  
    output wire                 o_writeAIP_IP_s2,   
    input  wire [DATA_WORD-1:0] i_dataOutAIP_IP_s2,
    input  wire                 i_int_IP_s2,
    output wire                 o_start_IP_s2

);
    wire                        w_startIPcore_uP;
    wire [DATA_WORD-1:0]        w_dataInAIP_uP;
    wire [DATA_WORD-1:0]        w_dataOutAIP_uP;
    wire [4:0]                  w_configAIP_uP;
    wire                        w_readAIP_uP;
    wire                        w_writeAIP_uP;
    wire                        w_startIPcore;

    aipCoprocessor INTERFACE_Coprocessor
    (
        .clk                                 ( i_clk             ),
        .rst                                 ( i_rst_a           ),
        .en                                  ( i_en_s            ),
    //-------------------------- To/From NIc --------------------------//
        .configAIP_net                       ( i_conf_dbus       ),   //Used for protocol to determine different actions types
        .readAIP_net                         ( i_read            ),   //Used for protocol to read different information types
        .writeAIP_net                        ( i_write           ),   //Used for protocol to write different information types
        .startAIP_net                        ( i_start           ),   //Used to start the IP-core
        .dataInAIP_net                       ( i_data_in         ),   //different data in information types
        .dataOutAIP_net                      ( o_data_out        ),   //different data out information types
        .intAIP_net                          ( o_int_req         ),   //Interruption request        
    //------------------------ To/From IP-core ------------------------//
        .dataInAIP_uP                        ( w_dataInAIP_uP    ),
        .dataOutAIP_uP                       ( w_dataOutAIP_uP   ),
        .configAIP_uP                        ( w_configAIP_uP    ),
        .readAIP_uP                          ( w_readAIP_uP      ),
        .writeAIP_uP                         ( w_writeAIP_uP     ),
        .startIPcore                         ( w_startIPcore_uP  )
    );
        

    master_nios_multiple_slave u0 (
        .clk_clk                              ( i_clk              ),
        .reset_reset_n                        ( i_rst_a            ),
 
        .aip_up_0_aip_dataout                 ( w_dataOutAIP_uP    ),
        .aip_up_0_aip_config                  ( w_configAIP_uP     ),
        .aip_up_0_aip_datain                  ( w_dataInAIP_uP     ),
        .aip_up_0_aip_read                    ( w_readAIP_uP       ),
        .aip_up_0_aip_write                   ( w_writeAIP_uP      ),
		  .start_up_0_export                    ( w_startIPcore_uP   ),  

        .int_ip_s0_export                     ( i_int_IP           ),
		  
        .port_s0_aip_dataout                  ( i_dataOutAIP_IP_s0 ),
        .port_s0_aip_int                      ( 1'b0               ),// Input Int
        .port_s0_aip_config                   ( o_configAIP_IP_s0  ),
        .port_s0_aip_datain                   ( o_dataInAIP_IP_s0  ),
        .port_s0_aip_read                     ( o_readAIP_IP_s0    ),
        .port_s0_aip_start                    ( o_start_IP_s0      ),
        .port_s0_aip_write                    ( o_writeAIP_IP_s0   ),
     
        .port_s1_aip_dataout                  ( i_dataOutAIP_IP_s1 ),
        .port_s1_aip_int                      ( 1'b0               ),// Input Int
        .port_s1_aip_config                   ( o_configAIP_IP_s1  ),
        .port_s1_aip_datain                   ( o_dataInAIP_IP_s1  ),
        .port_s1_aip_read                     ( o_readAIP_IP_s1    ),
        .port_s1_aip_start                    ( o_start_IP_s1      ),
        .port_s1_aip_write                    ( o_writeAIP_IP_s1   ),
  
        .port_s2_aip_dataout                  ( i_dataOutAIP_IP_s2 ),
        .port_s2_aip_int                      ( 1'b0               ),// Input Int
        .port_s2_aip_config                   ( o_configAIP_IP_s2  ),
        .port_s2_aip_datain                   ( o_dataInAIP_IP_s2  ),
        .port_s2_aip_read                     ( o_readAIP_IP_s2    ),
        .port_s2_aip_start                    ( o_start_IP_s2      ),
        .port_s2_aip_write                    ( o_writeAIP_IP_s2   ),
		  
        .spi_MISO                             ( MISO               ),            
        .spi_MOSI                             ( MOSI               ),             
        .spi_SCLK                             ( SCLK               ),          
        .spi_SS_n                             ( SS_n               ),            
		  
        .uart_rxd                             ( uart_rxd           ),             
        .uart_txd                             ( uart_txd           ),    
        .leds_export                          ( leds               )          

 );

 endmodule