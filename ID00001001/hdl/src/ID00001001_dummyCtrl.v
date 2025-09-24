module ID00001001_dummyCtrl
(
  clk,
  rst_a,
  en_s,

  start,

  confReg,

  addrRD,

  addrWR,
  enWR,

  done_f,
  data_rdy,
  data_read,

  busy_f
);

  parameter ADDR_WIDTH_MEMI = 'd6;
	parameter ADDR_WIDTH_MEMO = 'd6;
	parameter SIZE_CR = 'd1;

  localparam DATA_WIDTH = 'd32;

  input wire clk;
  input wire rst_a;
  input wire en_s;

  input wire start;

  input wire [(SIZE_CR*DATA_WIDTH)-1:0] confReg;

  output reg [ADDR_WIDTH_MEMI-1:0] addrRD;

  output reg [ADDR_WIDTH_MEMO-1:0] addrWR;
  output reg enWR;

  output reg busy_f;
  output reg data_rdy;
  output reg data_read;

  output reg done_f;

  localparam STANDBY = 'd0;
  localparam CONFIG = 'd1;
  localparam DELAY = 'd2;
  localparam IP_PROC = 'd3;
  localparam WAIT_B = 'd4;
  localparam WAIT_A = 'd5;
  localparam EN_DELAY = 1'b1;
  localparam WAIT = 4'hF;

  reg [2:0] state;
  reg [15:0] count_b;
  reg [31:0] count;
  reg en_count;

  /* counter for delay */
  always @(posedge clk or negedge rst_a) begin
    if(!rst_a) begin
      count <= 'd0;
      count_b <= 'd0;
    end
    else begin
      if(en_count) begin
        if (count_b == 16'hC350) begin
          count <= count + 1'b1;
          count_b <= 'd0;
        end
        else begin
          count_b <= count_b + 1'b1;
        end
      end
      else begin
        count <= 'd0;
        count_b <= 'd0;
      end
    end
  end

  always @(posedge clk or negedge rst_a) begin
    if (!rst_a) begin
      state <= STANDBY;
      addrRD <= 'd0;
      addrWR <= 'd0;
      enWR <= 1'b0;
      busy_f <= 1'b0;
      done_f <= 1'b0;
      data_rdy <= 1'b0;
      data_read <= 1'b0;
      en_count <= 'd0;
    end
    else begin
      if (en_s) begin
        case (state)
          STANDBY: begin
            if (start) begin
              state <= CONFIG;
              busy_f <= 1'b1;
            end
            done_f <= 1'b0;
            data_rdy <= 1'b0;
            data_read <= 1'b0;
          end // STANDBY
          CONFIG: begin
            if (confReg[0] == EN_DELAY) begin
              state <= DELAY;
              en_count <= 1'b1;
            end
            else begin
              state <= WAIT_B;
            end
          end // CONFIG
          DELAY: begin
            if (count == confReg[31:1]) begin //delay in milliseconds
              state <= WAIT_B;
              en_count <= 1'b0;
            end
            else begin
              state <= DELAY;
            end
          end // DELAY
          WAIT_B: begin
              state <= IP_PROC;
              enWR <= 1'b1;
          end // WAIT_B
          IP_PROC: begin
            state <= WAIT_A;
            enWR <= 1'b0;
          end // IP_PROC
          WAIT_A: begin
            if (addrWR == {ADDR_WIDTH_MEMO{1'b1}}) begin
              state <= STANDBY;
              addrRD <= 'd0;
              addrWR <= 'd0;
              enWR <= 1'b0;
              busy_f <= 1'b0;
              done_f <= 1'b1;
              data_read <= 1'b1;
            end
            else begin
              if (addrWR == 'd8)
                data_rdy <= 1'b1;
              state <= WAIT_B;
              addrRD <= addrRD + 1'b1;
              addrWR <= addrWR + 1'b1;
              enWR <= 1'b0;
            end
          end // WAIT_A
          default: begin
            state <= STANDBY;
            addrRD <= 'd0;
            addrWR <= 'd0;
            enWR <= 1'b0;
            busy_f <= 1'b0;
            done_f <= 1'b0;
            data_rdy <= 1'b0;
            data_read <= 1'b0;
            en_count <= 'd0;
          end // default
        endcase
      end
    end
  end

endmodule
