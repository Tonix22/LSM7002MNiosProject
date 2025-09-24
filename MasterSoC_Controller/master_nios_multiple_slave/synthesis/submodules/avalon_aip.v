module avalon_aip (
    i_clk,
    i_rst,

    i_avalon_address,
    i_avalon_chipselect,
    i_avalon_write,
    i_avalon_writedata,
    o_avalon_read,
    o_avalon_readdata,

    i_aip_dataOut,
    o_aip_dataIn,
    o_aip_config,
    o_aip_read,
    o_aip_write,
    o_aip_start,
    i_aip_int,

    o_core_int
);

    input i_clk;
    input i_rst;
    input i_avalon_chipselect;
    input [7:0] i_avalon_address;
    input i_avalon_write;
    input [31:0] i_avalon_writedata;
    output reg [31:0] o_avalon_readdata;
	 output o_avalon_read;

    input [31:0] i_aip_dataOut;
    output [31:0] o_aip_dataIn;
    output [4:0] o_aip_config;
    output reg o_aip_read;
    output reg o_aip_write;
    output o_aip_start;
    input i_aip_int;

    output o_core_int;

    wire [31:0] reg0; // aipDataOut
    reg [31:0] reg4; // aipDataIn
    reg [31:0] reg8; // aipConfig
    reg [31:0] reg12;
    reg start_w;
    wire busCtrl_doWrite;
    wire busCtrl_doRead;

    assign o_aip_dataIn = reg4;
    assign o_aip_config = reg8;
    assign o_aip_start = start_w;
    assign o_core_int = i_aip_int;
	 assign o_avalon_read = 1'b1;
    assign reg0 = i_aip_dataOut;

    always @ (*) begin
        o_avalon_readdata = 32'h0;

        case(i_avalon_address)
            8'b00001100 : begin
            end
            8'b00001000 : begin
            end
            8'b00000100 : begin
            end
            8'b00000000 : begin
                o_avalon_readdata[31:0] = reg0;
            end
            default : begin
            end
        endcase
    end

    assign busCtrl_doWrite = (i_avalon_chipselect && i_avalon_write);
    assign busCtrl_doRead = (i_avalon_chipselect && (! i_avalon_write) && o_avalon_read);

    always @ (posedge i_clk or negedge i_rst) begin
        if (!i_rst) begin
            reg4 <= 32'h0;
            reg8 <= 32'h0;
            reg12 <= 32'h0;
        end
        else begin
            case(i_avalon_address)
                8'b00001100 : begin
                    if(busCtrl_doWrite)begin
                        reg12 <= i_avalon_writedata[31:0];
                    end
                end
                8'b00001000 : begin
                    if(busCtrl_doWrite)begin
                        reg8 <= i_avalon_writedata[31:0];
                    end
                end
                8'b00000100 : begin
                    if(busCtrl_doWrite)begin
                        reg4 <= i_avalon_writedata[31:0];
                    end
                end
                8'b00000000 : begin
                end
                default : begin
                end
            endcase
        end
    end

    // Buffer
    // Write and read should be filtered out
    // when no write_data, write_conf or read_data are issued
    always @(*) begin
        o_aip_read <= busCtrl_doRead && (i_avalon_address == 8'b00000000);
        if (busCtrl_doWrite && (i_avalon_address == 8'b00001100)) start_w <= i_avalon_writedata[0];
        else start_w <= 1'b0;
    end

    always @(posedge i_clk) begin
        o_aip_write <= busCtrl_doWrite && (i_avalon_address == 8'b00000100);
    end

endmodule
