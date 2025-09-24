module avalon_aip_avalon_interface (
    clk,
    resetn,

    address,
    chipselect,
    write,
    writedata,
    read,
    readdata,

    i_aip_dataOut_export,
    o_aip_dataIn_export,
    o_aip_config_export,
    o_aip_read_export,
    o_aip_write_export,
    o_aip_start_export,
    i_aip_int_export,

    o_core_int_export
);


    input clk, resetn, read, write, chipselect;
    input [31:0] writedata;
	 input [7:0] address;
    output [31:0] readdata;

    input [31:0]i_aip_dataOut_export;
    input i_aip_int_export;
    output [31:0] o_aip_dataIn_export;
    output [4:0] o_aip_config_export;
    output o_aip_read_export;
    output o_aip_write_export;
    output o_aip_start_export;
    output o_core_int_export;

    wire [31:0] aip_dataIn;
    wire [4:0] aip_config;
    wire aip_read;
    wire aip_write;
    wire aip_start;
    wire core_int;
    wire [31:0] aip_dataOut;
    wire aip_int;

	avalon_aip u_avalon_aip(
		 .i_clk               ( clk               ),
		 .i_rst               ( resetn            ),
		 .i_avalon_address    ( address           ),
		 .i_avalon_chipselect ( chipselect        ),
		 .i_avalon_write      ( write             ),
		 .i_avalon_writedata  ( writedata         ),
		 .o_avalon_read       ( read              ),
		 .o_avalon_readdata   ( readdata          ),
		 .i_aip_dataOut       ( aip_dataOut       ),
		 .o_aip_dataIn        ( aip_dataIn        ),
		 .o_aip_config        ( aip_config        ),
		 .o_aip_read          ( aip_read          ),
		 .o_aip_write         ( aip_write         ),
		 .o_aip_start         ( aip_start         ),
		 .i_aip_int           ( aip_int           ),
		 .o_core_int          ( core_int          )
	);
  
	 assign aip_dataOut = i_aip_dataOut_export ;
	 assign aip_int = i_aip_int_export;
	 assign o_aip_dataIn_export = aip_dataIn;
	 assign o_aip_config_export = aip_config;
	 assign o_aip_read_export = aip_read;
	 assign o_aip_write_export = aip_write;
	 assign o_aip_start_export = aip_start;
	 assign o_core_int_export = core_int;
	
endmodule 
	
	