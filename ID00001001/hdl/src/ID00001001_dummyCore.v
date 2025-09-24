
module ID00001001_dummyCore
(
  clk,
  rst_a,
  en_s,

  start_IPcore,

  data_MemIn0,
  rd_addr_MemIn0,

  data_ConfigReg,

  data_MemOut0,
  wr_addr_MemOut0,
  wr_en_MemOut0,

  status_IPcore,
  int_IPcore
);

  parameter ADDR_WIDTH_MEMI = 'd6;
  parameter ADDR_WIDTH_MEMO = 'd6;
  parameter SIZE_CR = 'd1;

  localparam DATA_WIDTH = 'd32;
  localparam INT_WIDTH = 'd8;
  localparam STATUS_WIDTH = 'd8;

  input wire clk;
  input wire rst_a;
  input wire en_s;

  input wire start_IPcore;

  input wire [DATA_WIDTH-1:0] data_MemIn0;
  output wire [ADDR_WIDTH_MEMI-1:0] rd_addr_MemIn0;

  input wire [(SIZE_CR*DATA_WIDTH)-1:0] data_ConfigReg;

  output wire [DATA_WIDTH-1:0] data_MemOut0;
  output wire [ADDR_WIDTH_MEMO-1:0] wr_addr_MemOut0;
  output wire wr_en_MemOut0;

  output wire [STATUS_WIDTH-1:0] status_IPcore;
  output wire [INT_WIDTH-1:0] int_IPcore;

  wire done_f;
  wire data_rdy;
  wire data_read;
  wire bsy_f;

  assign status_IPcore = {{(STATUS_WIDTH-1){1'b0}},bsy_f};
  assign int_IPcore = {{(INT_WIDTH-3){1'b0}},data_read,data_rdy,done_f};
  assign data_MemOut0 = data_MemIn0;

  ID00001001_dummyCtrl
  #(
    .ADDR_WIDTH_MEMI (ADDR_WIDTH_MEMI),
    .ADDR_WIDTH_MEMO (ADDR_WIDTH_MEMO),
    .SIZE_CR (SIZE_CR)
  )
  CNTRL
  (
    .clk (clk),
    .rst_a (rst_a),
    .en_s (en_s),

    .start (start_IPcore),

    .confReg (data_ConfigReg),

    .addrRD (rd_addr_MemIn0),

    .addrWR (wr_addr_MemOut0),
    .enWR (wr_en_MemOut0),

    .done_f (done_f),
    .data_rdy (data_rdy),
    .data_read (data_read),

    .busy_f (bsy_f)
  );

endmodule
