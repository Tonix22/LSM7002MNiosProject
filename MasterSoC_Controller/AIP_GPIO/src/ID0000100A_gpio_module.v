module  ID0000100A_gpio_module  
#(parameter DATA_WIDTH = 32, CONFIG_WIDTH = 5)          
(
    input clk_in, rst_in,enable, 

	//--- AIP ---//
	input 	[DATA_WIDTH-1:0] 	dataInAIP,
	output 	[DATA_WIDTH-1:0] 	dataOutAIP,
	input 	[CONFIG_WIDTH-1:0] 	configAIP,
	input 	readAIP,
	input 	writeAIP,
	input 	startAIP,
	output 	intAIP,

//--- IP-core ---//
    input  [15:0] AF_in, 
	output [15:0] AF_out,
    input  [15:0] GPIO_in, 
	output [15:0] GPIO_out, 
	output [15:0] GPIO_we,

	output Interruption_GPIO
);	


localparam STATUS_WIDTH = 8;
localparam CONF_REG_SIZE = 4;
localparam MEM_ADDR_MAX_WIDTH = 16;

localparam MMEMIN 	= 5'd0;    // output data register
localparam AMEMIN 	= 5'd1;
localparam MMEMOUT 	= 5'd2;   // input data register
localparam AMEMOUT 	= 5'd3;
localparam MMEMOUT2 = 5'd4;  // register IRQ
localparam AMEMOUT2 = 5'd5;
localparam CCONFREG = 5'd6;  // MODER register
localparam ACONFREG = 5'd7;
localparam STATUS 	= 5'd30;
localparam IPID 	= 5'd31;
//--- IP-core ---//
wire [DATA_WIDTH-1:0] MODE_register;
wire [DATA_WIDTH-1:0] ODATA_register;
wire [DATA_WIDTH-1:0] IDATA_register;

wire [DATA_WIDTH-1:0] 					rdDataMemIn_0;
wire [MEM_ADDR_MAX_WIDTH-1:0] 			rdAddrMemIn_0;
wire [(CONF_REG_SIZE*DATA_WIDTH)-1:0] 	rdDataConfigReg;
  
  
wire statusIPcore_Busy;
wire intIPCore_Done;
wire startIPcore;
	
wire [15:0] GPIO_edgeP_detect;
wire [15:0] GPIO_edgeN_detect;

wire [15:0] readingIn;
wire [15:0] interrup_out,interrup_out_w;

ID0000100A_aip ID0000100a_GPIO_aip
(
	.clk					(clk_in),
	.rst					(rst_in),
	.en						(enable),

  //--- AIP ---//
	.dataInAIP				(dataInAIP),
	.dataOutAIP				(dataOutAIP),
	.configAIP				(configAIP),
	.readAIP				(readAIP),
	.writeAIP				(writeAIP),
	.startAIP				(startAIP),
	.intAIP					(intAIP),

  //--- IP-core ---//
	.rdDataMemIn_0			(ODATA_register),
	.rdAddrMemIn_0			({(MEM_ADDR_MAX_WIDTH){1'b0}}),
	.wrDataMemOut_0			({16'h0000,GPIO_in}),
	.wrAddrMemOut_0			(16'd0),
	.wrEnMemOut_0			(1'b1),
	.wrDataMemOut_1			({16'h0000,interrup_out_w}),
	.wrAddrMemOut_1			(16'd0),
	.wrEnMemOut_1			(1'b1),
	.rdDataConfigReg		(rdDataConfigReg),
	.statusIPcore_Busy		(1'b0),
	.intIPCore_Done			(intIPCore_Done),
	.startIPcore			(startIPcore)
);
assign interrup_out_w = (readAIP & (configAIP == MMEMOUT2))?16'h0000:interrup_out;

assign Interruption_GPIO = |interrup_out;

wire [1:0] mask_mode0_GPIO;
wire [1:0] mask_mode1_GPIO;
wire [1:0] mask_mode2_GPIO;
wire [1:0] mask_mode3_GPIO;
wire [1:0] mask_mode4_GPIO;
wire [1:0] mask_mode5_GPIO;
wire [1:0] mask_mode6_GPIO;
wire [1:0] mask_mode7_GPIO;
wire [1:0] mask_mode8_GPIO;
wire [1:0] mask_mode9_GPIO;
wire [1:0] mask_mode10_GPIO;
wire [1:0] mask_mode11_GPIO;
wire [1:0] mask_mode12_GPIO;
wire [1:0] mask_mode13_GPIO;
wire [1:0] mask_mode14_GPIO;
wire [1:0] mask_mode15_GPIO;

assign mask_mode0_GPIO[1:0]  = rdDataConfigReg[1:0];
assign mask_mode1_GPIO[1:0]  = rdDataConfigReg[3:2];
assign mask_mode2_GPIO[1:0]  = rdDataConfigReg[5:4];
assign mask_mode3_GPIO[1:0]  = rdDataConfigReg[7:6];
assign mask_mode4_GPIO[1:0]  = rdDataConfigReg[9:8];
assign mask_mode5_GPIO[1:0]  = rdDataConfigReg[11:10];
assign mask_mode6_GPIO[1:0]  = rdDataConfigReg[13:12];
assign mask_mode7_GPIO[1:0]  = rdDataConfigReg[15:14];
assign mask_mode8_GPIO[1:0]  = rdDataConfigReg[17:16];
assign mask_mode9_GPIO[1:0]  = rdDataConfigReg[19:18];
assign mask_mode10_GPIO[1:0] = rdDataConfigReg[21:20];
assign mask_mode11_GPIO[1:0] = rdDataConfigReg[23:22];
assign mask_mode12_GPIO[1:0] = rdDataConfigReg[25:24];
assign mask_mode13_GPIO[1:0] = rdDataConfigReg[27:26];
assign mask_mode14_GPIO[1:0] = rdDataConfigReg[29:28];
assign mask_mode15_GPIO[1:0] = rdDataConfigReg[31:30];

assign GPIO_we[0]  =  ((mask_mode0_GPIO  == 2'b01) || (mask_mode0_GPIO  == 2'b10))? 1'b1 : 1'b0;
assign GPIO_we[1]  =  ((mask_mode1_GPIO  == 2'b01) || (mask_mode1_GPIO  == 2'b10))? 1'b1 : 1'b0;
assign GPIO_we[2]  =  ((mask_mode2_GPIO  == 2'b01) || (mask_mode2_GPIO  == 2'b10))? 1'b1 : 1'b0;
assign GPIO_we[3]  =  ((mask_mode3_GPIO  == 2'b01) || (mask_mode3_GPIO  == 2'b10))? 1'b1 : 1'b0;
assign GPIO_we[4]  =  ((mask_mode4_GPIO  == 2'b01) || (mask_mode4_GPIO  == 2'b10))? 1'b1 : 1'b0;
assign GPIO_we[5]  =  ((mask_mode5_GPIO  == 2'b01) || (mask_mode5_GPIO  == 2'b10))? 1'b1 : 1'b0;
assign GPIO_we[6]  =  ((mask_mode6_GPIO  == 2'b01) || (mask_mode6_GPIO  == 2'b10))? 1'b1 : 1'b0;
assign GPIO_we[7]  =  ((mask_mode7_GPIO  == 2'b01) || (mask_mode7_GPIO  == 2'b10))? 1'b1 : 1'b0;

assign GPIO_we[8]  =  (mask_mode8_GPIO  == 2'b01)? 1'b1 : 1'b0;
assign GPIO_we[9]  =  (mask_mode9_GPIO  == 2'b01)? 1'b1 : 1'b0;
assign GPIO_we[10] =  (mask_mode10_GPIO == 2'b01)? 1'b1 : 1'b0;
assign GPIO_we[11] =  (mask_mode11_GPIO == 2'b01)? 1'b1 : 1'b0;
assign GPIO_we[12] =  (mask_mode12_GPIO == 2'b01)? 1'b1 : 1'b0;
assign GPIO_we[13] =  (mask_mode13_GPIO == 2'b01)? 1'b1 : 1'b0;
assign GPIO_we[14] =  (mask_mode14_GPIO == 2'b01)? 1'b1 : 1'b0;
assign GPIO_we[15] =  (mask_mode15_GPIO == 2'b01)? 1'b1 : 1'b0;

assign GPIO_out[0]  = (mask_mode0_GPIO   == 2'b10)? AF_in[0] : ODATA_register[0];
assign GPIO_out[1]  = (mask_mode1_GPIO   == 2'b10)? AF_in[1] : ODATA_register[1];
assign GPIO_out[2]  = (mask_mode2_GPIO   == 2'b10)? AF_in[2] : ODATA_register[2];
assign GPIO_out[3]  = (mask_mode3_GPIO   == 2'b10)? AF_in[3] : ODATA_register[3];
assign GPIO_out[4]  = (mask_mode4_GPIO   == 2'b10)? AF_in[4] : ODATA_register[4];
assign GPIO_out[5]  = (mask_mode5_GPIO   == 2'b10)? AF_in[5] : ODATA_register[5];
assign GPIO_out[6]  = (mask_mode6_GPIO   == 2'b10)? AF_in[6] : ODATA_register[6];
assign GPIO_out[7]  = (mask_mode7_GPIO   == 2'b10)? AF_in[7] : ODATA_register[7];
assign GPIO_out[8]  = (mask_mode8_GPIO   == 2'b10)? AF_in[8] : ODATA_register[8];
assign GPIO_out[9]  = (mask_mode9_GPIO   == 2'b10)? AF_in[9] : ODATA_register[9];
assign GPIO_out[10] = (mask_mode10_GPIO  == 2'b10)? AF_in[10]: ODATA_register[10];
assign GPIO_out[11] = (mask_mode11_GPIO  == 2'b10)? AF_in[11]: ODATA_register[11];
assign GPIO_out[12] = (mask_mode12_GPIO  == 2'b10)? AF_in[12]: ODATA_register[12];
assign GPIO_out[13] = (mask_mode13_GPIO  == 2'b10)? AF_in[13]: ODATA_register[13];
assign GPIO_out[14] = (mask_mode14_GPIO  == 2'b10)? AF_in[14]: ODATA_register[14];
assign GPIO_out[15] = (mask_mode15_GPIO  == 2'b10)? AF_in[15]: ODATA_register[15];

assign AF_out[0]  = (mask_mode0_GPIO   == 2'b11)? GPIO_in[0] : 1'b0;
assign AF_out[1]  = (mask_mode1_GPIO   == 2'b11)? GPIO_in[1] : 1'b0;
assign AF_out[2]  = (mask_mode2_GPIO   == 2'b11)? GPIO_in[2] : 1'b0;
assign AF_out[3]  = (mask_mode3_GPIO   == 2'b11)? GPIO_in[3] : 1'b0;
assign AF_out[4]  = (mask_mode4_GPIO   == 2'b11)? GPIO_in[4] : 1'b0;
assign AF_out[5]  = (mask_mode5_GPIO   == 2'b11)? GPIO_in[5] : 1'b0;
assign AF_out[6]  = (mask_mode6_GPIO   == 2'b11)? GPIO_in[6] : 1'b0;
assign AF_out[7]  = (mask_mode7_GPIO   == 2'b11)? GPIO_in[7] : 1'b0;
assign AF_out[8]  = (mask_mode8_GPIO   == 2'b11)? GPIO_in[8] : 1'b0;
assign AF_out[9]  = (mask_mode9_GPIO   == 2'b11)? GPIO_in[9] : 1'b0;
assign AF_out[10] = (mask_mode10_GPIO  == 2'b11)? GPIO_in[10]: 1'b0;
assign AF_out[11] = (mask_mode11_GPIO  == 2'b11)? GPIO_in[11]: 1'b0;
assign AF_out[12] = (mask_mode12_GPIO  == 2'b11)? GPIO_in[12]: 1'b0;
assign AF_out[13] = (mask_mode13_GPIO  == 2'b11)? GPIO_in[13]: 1'b0;
assign AF_out[14] = (mask_mode14_GPIO  == 2'b11)? GPIO_in[14]: 1'b0;
assign AF_out[15] = (mask_mode15_GPIO  == 2'b11)? GPIO_in[15]: 1'b0;


wire [1:0] mask_interrupt0_GPIO;
wire [1:0] mask_interrupt1_GPIO;
wire [1:0] mask_interrupt2_GPIO;
wire [1:0] mask_interrupt3_GPIO;
wire [1:0] mask_interrupt4_GPIO;
wire [1:0] mask_interrupt5_GPIO;
wire [1:0] mask_interrupt6_GPIO;
wire [1:0] mask_interrupt7_GPIO;
wire [1:0] mask_interrupt8_GPIO;
wire [1:0] mask_interrupt9_GPIO;
wire [1:0] mask_interrupt10_GPIO;
wire [1:0] mask_interrupt11_GPIO;
wire [1:0] mask_interrupt12_GPIO;
wire [1:0] mask_interrupt13_GPIO;
wire [1:0] mask_interrupt14_GPIO;
wire [1:0] mask_interrupt15_GPIO;

assign mask_interrupt0_GPIO[1:0]  = rdDataConfigReg[33:32];
assign mask_interrupt1_GPIO[1:0]  = rdDataConfigReg[35:34];
assign mask_interrupt2_GPIO[1:0]  = rdDataConfigReg[37:36];
assign mask_interrupt3_GPIO[1:0]  = rdDataConfigReg[39:38];
assign mask_interrupt4_GPIO[1:0]  = rdDataConfigReg[41:40];
assign mask_interrupt5_GPIO[1:0]  = rdDataConfigReg[43:42];
assign mask_interrupt6_GPIO[1:0]  = rdDataConfigReg[45:44];
assign mask_interrupt7_GPIO[1:0]  = rdDataConfigReg[47:46];
assign mask_interrupt8_GPIO[1:0]  = rdDataConfigReg[49:48];
assign mask_interrupt9_GPIO[1:0]  = rdDataConfigReg[51:50];
assign mask_interrupt10_GPIO[1:0] = rdDataConfigReg[53:52];
assign mask_interrupt11_GPIO[1:0] = rdDataConfigReg[55:54];
assign mask_interrupt12_GPIO[1:0] = rdDataConfigReg[57:56];
assign mask_interrupt13_GPIO[1:0] = rdDataConfigReg[59:58];
assign mask_interrupt14_GPIO[1:0] = rdDataConfigReg[61:60];
assign mask_interrupt15_GPIO[1:0] = rdDataConfigReg[63:62];

wire [15:0] pulse_detection;
// mask_interrupt0_GPIO[1:0] == 2'b00  NO detection
// mask_interrupt0_GPIO[1:0] == 2'b01  POS edge detection
// mask_interrupt0_GPIO[1:0] == 2'b10  NEG edge detection

assign pulse_detection[0]  = mask_interrupt0_GPIO[1:0]  == 2'b00 ? 1'b0: mask_interrupt0_GPIO[1:0]  == 2'b01 ? GPIO_edgeP_detect[0]:  mask_interrupt0_GPIO[1:0]  == 2'b10 ? GPIO_edgeN_detect[0]:  1'b0; 
assign pulse_detection[1]  = mask_interrupt1_GPIO[1:0]  == 2'b00 ? 1'b0: mask_interrupt1_GPIO[1:0]  == 2'b01 ? GPIO_edgeP_detect[1]:  mask_interrupt1_GPIO[1:0]  == 2'b10 ? GPIO_edgeN_detect[1]:  1'b0; 
assign pulse_detection[2]  = mask_interrupt2_GPIO[1:0]  == 2'b00 ? 1'b0: mask_interrupt2_GPIO[1:0]  == 2'b01 ? GPIO_edgeP_detect[2]:  mask_interrupt2_GPIO[1:0]  == 2'b10 ? GPIO_edgeN_detect[2]:  1'b0; 
assign pulse_detection[3]  = mask_interrupt3_GPIO[1:0]  == 2'b00 ? 1'b0: mask_interrupt3_GPIO[1:0]  == 2'b01 ? GPIO_edgeP_detect[3]:  mask_interrupt3_GPIO[1:0]  == 2'b10 ? GPIO_edgeN_detect[3]:  1'b0; 
assign pulse_detection[4]  = mask_interrupt4_GPIO[1:0]  == 2'b00 ? 1'b0: mask_interrupt4_GPIO[1:0]  == 2'b01 ? GPIO_edgeP_detect[4]:  mask_interrupt4_GPIO[1:0]  == 2'b10 ? GPIO_edgeN_detect[4]:  1'b0; 
assign pulse_detection[5]  = mask_interrupt5_GPIO[1:0]  == 2'b00 ? 1'b0: mask_interrupt5_GPIO[1:0]  == 2'b01 ? GPIO_edgeP_detect[5]:  mask_interrupt5_GPIO[1:0]  == 2'b10 ? GPIO_edgeN_detect[5]:  1'b0; 
assign pulse_detection[6]  = mask_interrupt6_GPIO[1:0]  == 2'b00 ? 1'b0: mask_interrupt6_GPIO[1:0]  == 2'b01 ? GPIO_edgeP_detect[6]:  mask_interrupt6_GPIO[1:0]  == 2'b10 ? GPIO_edgeN_detect[6]:  1'b0; 
assign pulse_detection[7]  = mask_interrupt7_GPIO[1:0]  == 2'b00 ? 1'b0: mask_interrupt7_GPIO[1:0]  == 2'b01 ? GPIO_edgeP_detect[7]:  mask_interrupt7_GPIO[1:0]  == 2'b10 ? GPIO_edgeN_detect[7]:  1'b0; 
assign pulse_detection[8]  = mask_interrupt8_GPIO[1:0]  == 2'b00 ? 1'b0: mask_interrupt8_GPIO[1:0]  == 2'b01 ? GPIO_edgeP_detect[8]:  mask_interrupt8_GPIO[1:0]  == 2'b10 ? GPIO_edgeN_detect[8]:  1'b0; 
assign pulse_detection[9]  = mask_interrupt9_GPIO[1:0]  == 2'b00 ? 1'b0: mask_interrupt9_GPIO[1:0]  == 2'b01 ? GPIO_edgeP_detect[9]:  mask_interrupt9_GPIO[1:0]  == 2'b10 ? GPIO_edgeN_detect[9]:  1'b0; 
assign pulse_detection[10] = mask_interrupt10_GPIO[1:0] == 2'b00 ? 1'b0: mask_interrupt10_GPIO[1:0] == 2'b01 ? GPIO_edgeP_detect[10]: mask_interrupt10_GPIO[1:0] == 2'b10 ? GPIO_edgeN_detect[10]: 1'b0; 
assign pulse_detection[11] = mask_interrupt11_GPIO[1:0] == 2'b00 ? 1'b0: mask_interrupt11_GPIO[1:0] == 2'b01 ? GPIO_edgeP_detect[11]: mask_interrupt11_GPIO[1:0] == 2'b10 ? GPIO_edgeN_detect[11]: 1'b0; 
assign pulse_detection[12] = mask_interrupt12_GPIO[1:0] == 2'b00 ? 1'b0: mask_interrupt12_GPIO[1:0] == 2'b01 ? GPIO_edgeP_detect[12]: mask_interrupt12_GPIO[1:0] == 2'b10 ? GPIO_edgeN_detect[12]: 1'b0; 
assign pulse_detection[13] = mask_interrupt13_GPIO[1:0] == 2'b00 ? 1'b0: mask_interrupt13_GPIO[1:0] == 2'b01 ? GPIO_edgeP_detect[13]: mask_interrupt13_GPIO[1:0] == 2'b10 ? GPIO_edgeN_detect[13]: 1'b0; 
assign pulse_detection[14] = mask_interrupt14_GPIO[1:0] == 2'b00 ? 1'b0: mask_interrupt14_GPIO[1:0] == 2'b01 ? GPIO_edgeP_detect[14]: mask_interrupt14_GPIO[1:0] == 2'b10 ? GPIO_edgeN_detect[14]: 1'b0; 
assign pulse_detection[15] = mask_interrupt15_GPIO[1:0] == 2'b00 ? 1'b0: mask_interrupt15_GPIO[1:0] == 2'b01 ? GPIO_edgeP_detect[15]: mask_interrupt15_GPIO[1:0] == 2'b10 ? GPIO_edgeN_detect[15]: 1'b0; 

wire [15:0] pulse_detection_enable;

assign pulse_detection_enable[0]   = ((mask_interrupt0_GPIO[1:0]   == 2'b01) || (mask_interrupt0_GPIO[1:0]   == 2'b10))? 1'b1 : 1'b0; 
assign pulse_detection_enable[1]   = ((mask_interrupt1_GPIO[1:0]   == 2'b01) || (mask_interrupt1_GPIO[1:0]   == 2'b10))? 1'b1 : 1'b0; 
assign pulse_detection_enable[2]   = ((mask_interrupt2_GPIO[1:0]   == 2'b01) || (mask_interrupt2_GPIO[1:0]   == 2'b10))? 1'b1 : 1'b0; 
assign pulse_detection_enable[3]   = ((mask_interrupt3_GPIO[1:0]   == 2'b01) || (mask_interrupt3_GPIO[1:0]   == 2'b10))? 1'b1 : 1'b0; 
assign pulse_detection_enable[4]   = ((mask_interrupt4_GPIO[1:0]   == 2'b01) || (mask_interrupt4_GPIO[1:0]   == 2'b10))? 1'b1 : 1'b0; 
assign pulse_detection_enable[5]   = ((mask_interrupt5_GPIO[1:0]   == 2'b01) || (mask_interrupt5_GPIO[1:0]   == 2'b10))? 1'b1 : 1'b0; 
assign pulse_detection_enable[6]   = ((mask_interrupt6_GPIO[1:0]   == 2'b01) || (mask_interrupt6_GPIO[1:0]   == 2'b10))? 1'b1 : 1'b0; 
assign pulse_detection_enable[7]   = ((mask_interrupt7_GPIO[1:0]   == 2'b01) || (mask_interrupt7_GPIO[1:0]   == 2'b10))? 1'b1 : 1'b0; 
assign pulse_detection_enable[8]   = ((mask_interrupt8_GPIO[1:0]   == 2'b01) || (mask_interrupt8_GPIO[1:0]   == 2'b10))? 1'b1 : 1'b0; 
assign pulse_detection_enable[9]   = ((mask_interrupt9_GPIO[1:0]   == 2'b01) || (mask_interrupt9_GPIO[1:0]   == 2'b10))? 1'b1 : 1'b0; 
assign pulse_detection_enable[10]  = ((mask_interrupt10_GPIO[1:0]  == 2'b01) || (mask_interrupt10_GPIO[1:0]  == 2'b10))? 1'b1 : 1'b0; 
assign pulse_detection_enable[11]  = ((mask_interrupt11_GPIO[1:0]  == 2'b01) || (mask_interrupt11_GPIO[1:0]  == 2'b10))? 1'b1 : 1'b0; 
assign pulse_detection_enable[12]  = ((mask_interrupt12_GPIO[1:0]  == 2'b01) || (mask_interrupt12_GPIO[1:0]  == 2'b10))? 1'b1 : 1'b0; 
assign pulse_detection_enable[13]  = ((mask_interrupt13_GPIO[1:0]  == 2'b01) || (mask_interrupt13_GPIO[1:0]  == 2'b10))? 1'b1 : 1'b0; 
assign pulse_detection_enable[14]  = ((mask_interrupt14_GPIO[1:0]  == 2'b01) || (mask_interrupt14_GPIO[1:0]  == 2'b10))? 1'b1 : 1'b0; 
assign pulse_detection_enable[15]  = ((mask_interrupt15_GPIO[1:0]  == 2'b01) || (mask_interrupt15_GPIO[1:0]  == 2'b10))? 1'b1 : 1'b0; 


pos_edge_det pos_edge_det( 
	.input_detect	(GPIO_in),            
	.clk_in			(clk_in),            
    .out_detect		(GPIO_edgeP_detect)
);

neg_edge_det neg_edge_det( 
	.input_detect	(GPIO_in),            
	.clk_in			(clk_in),            
    .out_detect		(GPIO_edgeN_detect)
);


genvar g;

generate
	for(g=0; g<=15; g=g+1) begin: m
	Interrupt_Controller IntepCtrller(
		.clk_in			(clk_in), 
		.rst_in			(rst_in),
		.in_pulse		(pulse_detection[g]),
		.readingIn		(pulse_detection_enable[g] & readAIP & (configAIP == MMEMOUT2)),

		.interrup_out	(interrup_out[g])

	);
	end
endgenerate

endmodule

module pos_edge_det (        
        
	input clk_in,  
	input [15:0] input_detect,               
    output [15:0] out_detect
	);           

    reg [15:0] in_detect_dly;                          

    always @ (posedge clk_in) begin
		in_detect_dly <= input_detect;
	end

	assign out_detect = input_detect & ~in_detect_dly;

endmodule

module neg_edge_det (             
    
	input clk_in,   
	input [15:0] input_detect,         
	output [15:0] out_detect
	);           

    reg [15:0] in_detect_dly;                          

    
	always @ (posedge clk_in) begin
		in_detect_dly <= input_detect;
	end

	assign out_detect = ~input_detect & in_detect_dly;

endmodule

module Interrupt_Controller(
input   clk_in, rst_in,
input   in_pulse, readingIn,

output reg interrup_out

);
reg [1:0] nstate;
reg [1:0] state;

localparam DETECTING = 2'h0, READY2READ = 2'h1, CLEARING = 2'h2;

always @ (posedge clk_in, negedge rst_in)
    if (!rst_in) 
		state <= 2'b0;
    else 
		state <= nstate;

always @ (*)
begin
    //nstate = state;
	    begin
        case(state)
            DETECTING: begin
				nstate = (in_pulse)? READY2READ:DETECTING;
				interrup_out = 1'b0;
			end
            READY2READ: begin 
				nstate = (readingIn)? CLEARING:READY2READ;
				interrup_out = 1'b1;
			end
            CLEARING: begin
				nstate = DETECTING;
				interrup_out = 1'b0;
			end
            default: begin
				nstate = DETECTING;
				interrup_out = 1'b0;
			end
        endcase
    end
end

endmodule