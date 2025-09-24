/*
    1 tab == 4 spaces!
*/

`include "../aip.sv"

`timescale 10ns/1ns

`define DELAY

module ID00001001_dummyAIPTb ();
    localparam DATA_WIDTH = 32;
    localparam SIZE_MEM = 'd6;

    reg clk;
    reg rst_a;
    reg en_s;
    //reg [DATA_WIDTH-1:0] data_in;
    //wire [DATA_WIDTH-1:0] data_out;
    //reg write;
    //reg read;
    //reg start;
    //reg [4:0] conf_dbus;
    //wire int_req;

    reg [DATA_WIDTH-1:0] tb_data;
    reg [DATA_WIDTH-1:0] dataSet [2**SIZE_MEM];

    logic [31:0] temp [];
    logic [31:0] dataTX [];
    logic [31:0] dataRX [];

    aip_if aipCon();

    ID00001001_dummy
    DUT
    (
        .clk(clk),
        .rst_a(rst_a),
        .en_s(en_s),
        .data_in(aipCon.aip_dataIn),
        .data_out(aipCon.aip_dataOut),
        .write(aipCon.aip_write),
        .read(aipCon.aip_read),
        .start(aipCon.aip_start),
        .conf_dbus(aipCon.aip_config),
        .int_req(aipCon.aip_int)
    );

    always begin
        #1 clk = ~clk;
    end

    initial begin
        rst_a = 0;
        clk = 1;

        aipCon.aip_dataIn = 0;
        aipCon.aip_config = 0;
        aipCon.aip_read = 0;
        aipCon.aip_write = 0;
        aipCon.aip_start = 0;

        en_s = 0;

        // Wait for global reset to finish
        #10;
        rst_a = 1'b1;
        en_s = 1'b1;
        #10;

        #10 // GET ID
        temp = new [1];
        aipCon.getID(temp[0]);
        $display(" %0t | ID = %x", $time, temp[0]);

        $display("--------------------------------------------------");

        dataTX = new [64];
        dataRX = new [64];

        // WRITE MEM IN
        for (int idx = 0; idx < 64; idx++) begin
            dataTX[idx] = $urandom;
        end

        #2
        aipCon.writeMem('d0, dataTX, 64, 0);

//        `ifdef DELAY
//        // SET PTR CONF REG
//        write_interface(5'b00101, 32'h00000000);
//
//        // WRITE CONF REG
//        // tb_data = 00001771;
//        tb_data = 000003E9;
//        write_interface(5'b00100, tb_data);
//        `endif // DELAY
//
        // START PROCESS
        $display("%7t Sending start", $time);
        aipCon.start();

//        `ifdef DELAY
//        tb_data = 0;
//        // $display("%08x", tb_data);
//        while (!tb_data[0]) begin
//            read_interface(5'b11110, tb_data);
//            $display("%7T Status - %08x", $time, tb_data);
//            #100;
//        end
//        `endif // DELAY
//
//        // SET PTR MEM OUT
//        write_interface(5'b00011, 32'h00000000);

        // READ MEM OUT
        #10
        $display ("\t\tI \t\tO \t\tResult");
        aipCon.readMem('d2, dataRX, 64, 0);

        for (int i = 0; i < 64; i++) begin
            $display ("Read data %2d \t%8h \t%8h \t%s", i, dataTX[i], dataRX[i], (dataTX[i] === dataRX[i] ? "OK": "ERROR"));
        end

        #500;
        $finish;
    end

`ifdef IVERILOG
    initial begin
        $dumpfile("ID00001001_dummyAIPTb.vcd");
        $dumpvars(0, ID00001001_dummyAIPTb);

        for (integer index = 0; index < 64; index = index + 1) begin
            $dumpvars(1, ID00001001_dummyAIPTb.DUT.INTERFACE.AIP.MEMIN[0].genblk1.MEMIN.RAM_Structure[index]);
        end
        

        $dumpall;
    end
`endif

endmodule
