/*
    1 tab == 4 spaces!
*/

`timescale 10ns/1ns

`define DELAY

module ID00001001_dummyTb ();
    localparam DATA_WIDTH = 32;
    localparam SIZE_MEM = 'd6;
    reg clk;
    reg rst_a;
    reg en_s;
    reg [DATA_WIDTH-1:0] data_in;
    wire [DATA_WIDTH-1:0] data_out;
    reg write;
    reg read;
    reg start;
    reg [4:0] conf_dbus;
    wire int_req;

    reg [DATA_WIDTH-1:0] tb_data;
    reg [DATA_WIDTH-1:0] dataSet [2**SIZE_MEM];

    ID00001001_dummy
    DUT
    (
        .clk(clk),
        .rst_a(rst_a),
        .en_s(en_s),
        .data_in(data_in),
        .data_out(data_out),
        .write(write),
        .read(read),
        .start(start),
        .conf_dbus(conf_dbus),
        .int_req(int_req)
    );

    task write_interface;
        input [4:0] write_conf_dbus;
        input [31:0] write_data;
        begin
            conf_dbus = write_conf_dbus;
            data_in = write_data;
            #2
            write = 1'b1;
            #2;
            write = 1'b0;
            #10;
        end
    endtask

    task read_interface;
        input [4:0] read_conf_dbus;
        output [31:0] read_data;
        begin
            conf_dbus = read_conf_dbus;
            #2
            read = 1'b1;
            #2;
            read_data = data_out;
            read = 1'b0;
            #10;
        end
    endtask

    task start_interface;
        begin
            start = 1'b1;
            #2;
            start = 1'b0;
            #10;
        end
    endtask

    always begin
        #1 clk = ~clk;
    end

    initial begin
        rst_a = 0;
        clk = 0;

        conf_dbus = 0;
        read = 0;
        write = 0;
        start = 0;
        data_in = 0;

        en_s = 0;

        // Wait for global reset to finish
        #10;
        rst_a = 1'b1;
        en_s = 1'b1;
        #10;

        // READ IP_ID
        read_interface(5'b11111, tb_data);
        $display ("%7T Read ID %h", $time, tb_data);

        // SET PTR MEM IN
        write_interface(5'b00001, 32'h00000000);

        // WRITE MEM IN
        for (int i = 0; i < 2**SIZE_MEM; i++) begin
            dataSet[i] = $urandom();
            write_interface(5'b00000, dataSet[i]);
        end

        `ifdef DELAY
        // SET PTR CONF REG
        write_interface(5'b00101, 32'h00000000);

        // WRITE CONF REG
        // tb_data = 00001771;
        tb_data = 000003E9;
        write_interface(5'b00100, tb_data);
        `endif // DELAY

        // START PROCESS
        $display("%7T Sending start", $time);
        start_interface();

        `ifdef DELAY
        tb_data = 0;
        // $display("%08x", tb_data);
        while (!tb_data[0]) begin
            read_interface(5'b11110, tb_data);
            $display("%7T Status - %08x", $time, tb_data);
            #100;
        end
        `endif // DELAY

        // SET PTR MEM OUT
        write_interface(5'b00011, 32'h00000000);

        // READ MEM OUT
        $display ("\t\tI \t\tO \t\tResult");
        for (int i = 0; i < 2**SIZE_MEM; i++) begin
            read_interface(5'b00010, tb_data);
            $display ("Read data %2d \t%8h \t%8h \t%s", i, dataSet[i], tb_data, (dataSet[i] === tb_data ? "OK": "ERROR"));
        end

        #100;
        $finish;
    end

`ifdef IVERILOG
    initial begin
        $dumpfile("ID00001001_dummyTb.vcd");
        $dumpvars(0, ID00001001_dummyTb);

        for (integer index = 0; index < 64; index = index + 1) begin
            $dumpvars(1, ID00001001_dummyTb.DUT.INTERFACE.AIP.MEMIN[0].genblk1.MEMIN.RAM_Structure[index]);
        end
        

        $dumpall;
    end
`endif

endmodule
