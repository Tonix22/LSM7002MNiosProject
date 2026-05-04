`timescale 1ns/1ns
`define EOF 32'hFFFF_FFFF
`define NULL 0

module Test_GPIO_id0000100a
#(parameter id0000100a_DATA_WIDTH_OUTPUT_P = 12, 
 parameter CLK_PERIOD=2);

            //----------------------------------------------------------
            //.......MANDATORY TB PARAMETERS............................
            //----------------------------------------------------------
localparam  CYCLE		        = 'd20, // Define the clock work cycle in ns (user)
            DATAWIDTH           = 'd32, // AIP BITWIDTH
            MAX_SIZE_MEM        = 'd8,  // MAX MEMORY SIZE AMONG ALL AIP MEMORIES (Defined by the user)
            //------------------------------------------------------------
            //..................CONFIG VALUES.............................
            //------------------------------------------------------------           
            STATUS   = 5'd30,//Mandatory config
            IP_ID    = 5'd31,//Mandatory config
            id0000100a_MDATAIN0 = 5'd0, // output data register
            id0000100a_ADATAIN0 = 5'd1,
            id0000100a_MMEMOUT0 = 5'd2, // input data register
            id0000100a_AMEMOUT0 = 5'd3,
            id0000100a_MMEMOUT1 = 5'd4, // interruptions
            id0000100a_AMEMOUT1 = 5'd5,
            id0000100a_CCONFREG = 5'd6,
            id0000100a_ACONFREG = 5'd7,
            //------------------------------------------------------------
            //..................PARAMETERS DEFINED BY THE USER............
            //------------------------------------------------------------
            id0000100a_SIZE_MEM0    = 'd8, //Size of the memories of the IP DDS
            INT_BIT_DONE = 'd0; //Bit corresponding to the Int Done flag.

//AIP Interface signals
reg			        readAIP;
reg			        writeAIP;
reg			        startAIP;
reg	[ 4:0] 	        configAIP;
reg	[DATAWIDTH-1:0] dataInAIP;

wire		    	        intAIP;
wire	[DATAWIDTH-1:0]     dataOutAIP;

reg 			 clk, rst_a, en_s;
reg 			 iStartIPcore;

//Auxiliar variables
integer i;
reg [DATAWIDTH-1:0] 	id0000100a_tb_data;

reg [DATAWIDTH-1:0] 		 id0000100a_dataSet [id0000100a_SIZE_MEM0-1:0];
reg [(DATAWIDTH*id0000100a_SIZE_MEM0)-1:0] id0000100a_dataSet_packed;

reg [DATAWIDTH-1:0] 		 id0000100a_result [id0000100a_SIZE_MEM0-1:0];
reg [(DATAWIDTH*id0000100a_SIZE_MEM0)-1:0] id0000100a_result_packed;

reg  [15:0] GPIO_in, AF_in;
wire [15:0] GPIO_out, GPIO_we, AF_out;


 initial begin
        $dumpfile("Test_GPIO_id0000100a.vcd"); 
        $dumpvars(0, Test_GPIO_id0000100a);
        $dumpall;
 end
 

initial 
begin
	clk		        = 1'b1;
	en_s		    = 1'b1;
	readAIP	        = 1'b0;
	writeAIP	    = 1'b0;
	startAIP	    = 1'b0;
	configAIP   	= 5'd0;
	dataInAIP   	= 32'd0;
    iStartIPcore 	= 1'b1;
	rst_a		    = 1'b0;	// reset is active
	#3 rst_a	    = 1'b1;	// at time #n release reset
	#37
    GPIO_in         = 16'hBEAF;
    AF_in           = 16'h000A;
     // READ IP_ID
    getID(id0000100a_tb_data);
    $display ("%7T Read ID %h", $time, id0000100a_tb_data);
     
    // READ STATUS
    getStatus(id0000100a_tb_data);
    $display ("%7T Read STATUS %h", $time, id0000100a_tb_data);
     
    //(INTERRUPTIONS) 
    //FOR ENABLING INTERRUPTIONS
    enableINT(INT_BIT_DONE);
     
    $display("%7T writing to ODATA Register", $time);
    writeMem(id0000100a_MDATAIN0, 32'h0000_beaf, 1,0); // ODATA_Register

    $display("%7T writing to MODER Register", $time);
    writeConfReg(id0000100a_CCONFREG, 32'h55555555, 1,0); // MODER_Register

    #50
    // READ STATUS
    getStatus(id0000100a_tb_data);
    $display ("%7T Read STATUS %h", $time, id0000100a_tb_data);
     
    //CLEAR INT DONE FLAG
    clearINT(INT_BIT_DONE);
     
    // READ STATUS
    getStatus(id0000100a_tb_data);
    $display ("%7T Read STATUS %h", $time, id0000100a_tb_data);  


	#50
     // START PROCESS
    $display("%7T Sending start", $time);
    start();
    #40;


    $display("%7T writing to ODATA", $time);
    writeMem(id0000100a_MDATAIN0, 32'd8, 1,0); // load_psc


    #50
    // READ STATUS
    getStatus(id0000100a_tb_data);
    $display ("%7T Read STATUS %h", $time, id0000100a_tb_data);
     
    //CLEAR INT DONE FLAG
    clearINT(INT_BIT_DONE);

     
    // READ STATUS
    getStatus(id0000100a_tb_data);
    $display ("%7T Read STATUS %h", $time, id0000100a_tb_data);  

	#50
     // START PROCESS
    $display("%7T Sending start", $time);
    start();
    #400;

    $display("%7T writing to MODER Register", $time);
    writeConfReg(id0000100a_CCONFREG, 32'h55550000, 1,0); // MODER_Register  IO[15:8],AF_IO[7:0]
    // START PROCESS
     $display("%7T writing to CONF Interruption MASK Register", $time);
    writeConfReg(id0000100a_CCONFREG, 32'h00000000, 1,1); // CONF_ Interruption Register  IO[15:8],AF_IO[7:0]
    // START PROCESS
    $display("%7T Sending start", $time);
    start();

    #40;
    $display("%7T writing to ODATA", $time);
    writeMem(id0000100a_MDATAIN0, 32'hAAFF, 1,0); // load_psc
    // START PROCESS
    $display("%7T Sending start", $time);
    start();

    #40;
    $display("%7T writing to ODATA", $time);
    writeMem(id0000100a_MDATAIN0, 32'hABFE, 1,0); // load_psc
    // START PROCESS
    $display("%7T Sending start", $time);
    start();

    #40;
    repeat(10) begin
        readMem(id0000100a_MMEMOUT0,id0000100a_tb_data, 1,0); // read MEMin
        $display ("%7T Read GPIO_IN %h", $time, id0000100a_tb_data);
    end


    $display("%7T writing to MODER Register", $time);
    writeConfReg(id0000100a_CCONFREG, 32'h55550000, 1,0); // MODER_Register  IO[15:8],AF_IO[7:0]
    // START PROCESS
     $display("%7T writing to CONF Interruption MASK Register", $time);
    writeConfReg(id0000100a_CCONFREG, 32'h5555AAAA, 1,1); // CONF_ Interruption Register  IO[15:8],AF_IO[7:0]
    // START PROCESS
    $display("%7T Sending start", $time);
    start();
    #810;

    repeat(10) begin

    readMem(id0000100a_MMEMOUT1,id0000100a_tb_data, 1,0); // read MEMin
    $display ("%7T Read INTERRUPTIONS %h", $time, id0000100a_tb_data);
    #8000;
    end
    

    #10000;
	$display($time, " << finishing Simulation >>");
	$finish;
end

//Clock source procedural block
always #(CYCLE/2) clk = !clk;

always@(posedge clk) begin
    #(CYCLE*220) GPIO_in <= GPIO_in + 1'b1;
    AF_in <= {AF_in[14:0],AF_in[15]};
end



ID0000100A_gpio_module AIP_GPIO_Module(
    .clk_in         (clk),     	// Clock
    .rst_in         (rst_a),     // reset low active
    .enable         (1'b1),
    //-------------------------- To/From NIc --------------------------//
    .configAIP      (configAIP),      //Used for protocol to determine different actions types
    .readAIP        (readAIP),      //Used for protocol to read different information types
    .writeAIP       (writeAIP),     //Used for protocol to write different information types
    .startAIP       (startAIP),     //Used to start the IP-core
    .dataInAIP      (dataInAIP),    //different data in information types
    .intAIP         (intAIP),            			  //Interruption request
    .dataOutAIP     (dataOutAIP),   //different data out information types
    // IP signals 
    .AF_in          (AF_in),
    .AF_out         (AF_out),	
	.GPIO_in        (GPIO_in),
	.GPIO_out       (GPIO_out),
	.GPIO_we        (GPIO_we)
    );



//*******************************************************************
//*********************TASKS DEFINITION******************************
//*******************************************************************

task getID;
   output [DATAWIDTH-1:0] read_ID;
      
      begin
         single_read(IP_ID,read_ID);
      end
endtask

task getStatus;
   output [DATAWIDTH-1:0] read_status;
      
      begin
         single_read(STATUS,read_status);
      end
endtask

task writeMem;
        input [                         4:0] config_value;
        input [(DATAWIDTH*MAX_SIZE_MEM)-1:0] write_data;
        input [               DATAWIDTH-1:0] length;
        input [               DATAWIDTH-1:0] offset;
        
        begin        
            //SET POINTER
            single_write(config_value+1, offset);
            
            //WRITE MEMORY
            configAIP = config_value;
            #(CYCLE)
            for(i=0; i < length ; i= i+1) begin
               dataInAIP = write_data[(i*DATAWIDTH)+:DATAWIDTH];
               writeAIP = 1'b1;
               #(CYCLE);
            end
            writeAIP = 1'b0;
            #(CYCLE);
        end
endtask

task writeConfReg;
        input [                         4:0] config_value;
        input [(DATAWIDTH*MAX_SIZE_MEM)-1:0] write_data;
        input [               DATAWIDTH-1:0] length;
        input [               DATAWIDTH-1:0] offset;
        
        begin        
            //SET POINTER
            single_write(config_value+1, offset);
            
            //WRITE MEMORY
            configAIP = config_value;
            #(CYCLE)
            for(i=0; i < length ; i= i+1) begin
               dataInAIP = write_data[(i*DATAWIDTH)+:DATAWIDTH];
               writeAIP = 1'b1;
               #(CYCLE);
            end
            writeAIP = 1'b0;
            #(CYCLE);
        end
endtask



task readMem;
        input [                         4:0] config_value;   
        output[(DATAWIDTH*MAX_SIZE_MEM)-1:0] read_data;     
        input [               DATAWIDTH-1:0] length;
        input [               DATAWIDTH-1:0] offset;        
        
        begin
            //SET POINTER
            single_write(config_value+1, offset);
        
            configAIP = config_value;
            #(CYCLE)
            for(i=0; i < length ; i= i+1) begin               
               readAIP = 1'b1;
               #(CYCLE);
               read_data[(i*DATAWIDTH)+:DATAWIDTH]=dataOutAIP;
            end
            readAIP = 1'b0;
            #(CYCLE);
        end
endtask

task enableINT;
      input [3:0] idxInt;   
      
       reg [DATAWIDTH-1:0] read_status;
       reg [7:0] mask;
       
  begin

       getStatus(read_status);
       
       mask = read_status[23:16]; //previous stored mask
       mask[idxInt] = 1'b1; //enabling INT bit

       single_write(STATUS, {8'd0,mask,16'd0});//write status reg
  end
endtask

task disableINT;
      input [3:0] idxInt;   
      
       reg [DATAWIDTH-1:0] read_status;
       reg [7:0] mask;
  begin
   
       getStatus(read_status);
       
       mask = read_status[23:16]; //previous stored mask
       mask[idxInt] = 1'b0; //disabling INT bit

       single_write(STATUS, {8'd0,mask,16'd0});//write status reg
  end
endtask

task clearINT;
      input [3:0] idxInt;   
      
       reg [DATAWIDTH-1:0] read_status;
       reg [7:0] clear_value;
       reg [7:0] mask;
    
  begin
    
       getStatus(read_status);
       
       mask = read_status[23:16]; //previous stored mask
       clear_value = 7'd1 <<  idxInt;

       single_write(STATUS, {8'd0,mask,8'd0,clear_value});//write status reg
  end
endtask

task start;
  begin
      startAIP = 1'b1;
      #(CYCLE);
      startAIP = 1'b0;
      #(CYCLE);
  end
endtask

task single_write;
        input [          4:0] config_value;
        input [DATAWIDTH-1:0] write_data;
        begin
            configAIP = config_value;
            dataInAIP = write_data;
            #(CYCLE)
            writeAIP = 1'b1;
            #(CYCLE)
            writeAIP = 1'b0;
            #(CYCLE);
        end
endtask

task single_read;
  input  [          4:0] config_value;
  output [DATAWIDTH-1:0] read_data;
  begin
      configAIP = config_value;
      #(CYCLE);
      readAIP = 1'b1;
      #(CYCLE);
      read_data = dataOutAIP;
      readAIP = 1'b0;
      #(CYCLE);
  end
endtask	
endmodule
