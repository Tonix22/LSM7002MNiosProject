
`define MEM_IN_DEF 1

`define MEM_OUT_DEF 1

`define CONF_REG_DEF 1 // MAX 1

module aipCoprocessor
(
  clk,
  rst,
  en,
  //----- net -----//
  dataInAIP_net,
  dataOutAIP_net,
  configAIP_net,
  readAIP_net,
  writeAIP_net,
  startAIP_net,
  intAIP_net,
  //----- uP -----//
  dataInAIP_uP,
  dataOutAIP_uP,
  configAIP_uP,
  readAIP_uP,
  writeAIP_uP,
  //startAIP_uP,//O.COMENTADO
  //intAIP_uP,//O.COMENTADO

  startIPcore
);

  localparam IP_ID = 32'h00004003; // ip-core id value

  localparam SEL_BITS = 'd2; // mux selector width

  localparam DATA_WIDTH = 32;

  localparam MEM_ADDR_MAX_WIDTH = 16;

  localparam CONF_REG_ADDR_MAX_WIDTH = 3;

  localparam CONFIG_WIDTH = 5; // config parameter width

  localparam STATUS_WIDTH = 8;

  `ifdef MEM_IN_DEF
  localparam [(`MEM_IN_DEF*MEM_ADDR_MAX_WIDTH)-1:0] MEM_IN_ADDR_WIDTH = {16'd7}; // size of mem in

  localparam [((`MEM_IN_DEF*2)*CONFIG_WIDTH)-1:0] CONFIG_MEM_IN = {5'b00001, 5'b00000}; // mem in configs
  `endif

  `ifdef MEM_OUT_DEF
  localparam [(`MEM_OUT_DEF*MEM_ADDR_MAX_WIDTH)-1:0] MEM_OUT_ADDR_WIDTH = {16'd7}; // size of mem out

  localparam [((`MEM_OUT_DEF*2)*CONFIG_WIDTH)-1:0] CONFIG_MEM_OUT = {5'b00011, 5'b00010}; // mem out configs
  `endif

  `ifdef CONF_REG_DEF
  localparam [(`CONF_REG_DEF*CONF_REG_ADDR_MAX_WIDTH)-1:0] CONF_REG_ADDR_WIDTH = {3'd1}; // size of conf reg

  localparam [(2*CONFIG_WIDTH)-1:0] CONFIG_CONF_REG = {5'b00101, 5'b00100}; // conf reg configs
  `endif

  input clk;
  input rst;
  input en;

  input [DATA_WIDTH-1:0] dataInAIP_net;
  output [DATA_WIDTH-1:0] dataOutAIP_net;
  input [CONFIG_WIDTH-1:0] configAIP_net;
  input readAIP_net;
  input writeAIP_net;
  input startAIP_net;
  output intAIP_net;

  input [DATA_WIDTH-1:0] dataInAIP_uP;
  output [DATA_WIDTH-1:0] dataOutAIP_uP;
  input [CONFIG_WIDTH-1:0] configAIP_uP;
  input readAIP_uP;
  input writeAIP_uP;
//  input startAIP_uP;//O.COMENTADO
//  output intAIP_uP;//O.COMENTADO

  `ifdef MEM_IN_DEF
  wire [(`MEM_IN_DEF*DATA_WIDTH)-1:0] rdDataMemIn_uP;
  wire [(`MEM_IN_DEF*MEM_ADDR_MAX_WIDTH)-1:0] rdAddrMemIn_uP;

  wire [(`MEM_IN_DEF*MEM_ADDR_MAX_WIDTH)-1:0] wrAddrMemIn_net; //address write for memory in
  wire [`MEM_IN_DEF-1:0] wrEnMemIn_net; //enable write for memory in

  wire [DATA_WIDTH-1:0] rdDataMemIn0;
  `endif // MEM_IN_DEF

  `ifdef MEM_OUT_DEF
  // input [(`MEM_OUT_DEF*DATA_WIDTH)-1:0] wrDataMemOut;
  wire [(`MEM_OUT_DEF*MEM_ADDR_MAX_WIDTH)-1:0] wrAddrMemOut_uP;
  wire [`MEM_OUT_DEF-1:0] wrEnMemOut_uP;

  wire [(`MEM_OUT_DEF*DATA_WIDTH)-1:0] rdDataMemOut_net; // data read for memory in
  wire [(`MEM_OUT_DEF*MEM_ADDR_MAX_WIDTH)-1:0] rdAddrMemOut_net; // address read for memory in
  `endif // MEM_OUT_DEF

  `ifdef CONF_REG_DEF
  wire [(CONF_REG_ADDR_WIDTH*DATA_WIDTH)-1:0] rdDataConfigReg; //output
  wire [DATA_WIDTH-1:0] rdDataConfigRegMux;
  wire [DATA_WIDTH-1:0] configsAIP;

  wire [CONF_REG_ADDR_MAX_WIDTH-1:0] wrAddrConfigReg_net; //addres write for conf reg
  wire wrEnConfigReg_net; //enable write for configuration register

  wire [1:0] rdAddrConfigReg_uP; //addres write for conf reg
  `endif //CONF_REG_DEF

  output startIPcore;

  wire [SEL_BITS-1:0] selMux_net;
  wire [((2**SEL_BITS)*DATA_WIDTH)-1:0] dataMux_net;

  wire [DATA_WIDTH-1:0] wireIpId_net;

  wire setStatus_net;
  wire [DATA_WIDTH-1:0] wireStatus_net;

  wire [SEL_BITS-1:0] selMux_uP;
  wire [((2**SEL_BITS)*DATA_WIDTH)-1:0] dataMux_uP;

  reg [DATA_WIDTH-1:0] status_uP;
  wire [7:0] wireInt_uP;
  wire setStatus_uP;

    assign startIPcore = startAIP_net;

  genvar i;
  //---------- Mem In ----------//
  `ifdef MEM_IN_DEF
  generate
    for(i=0; i<`MEM_IN_DEF; i = i + 1) begin :  MEMIN
      if (0 == i)
        simple_dual_port_ram_single_clk
        #(
          .DATA_WIDTH (DATA_WIDTH),
          .ADDR_WIDTH (MEM_IN_ADDR_WIDTH[MEM_ADDR_MAX_WIDTH*i+:MEM_ADDR_MAX_WIDTH])
        )
        MEMIN
        (
          .Write_clock__i (clk),

          .Write_enable_i (wrEnMemIn_net[i]), // ctrl
          .Write_addres_i (wrAddrMemIn_net[MEM_ADDR_MAX_WIDTH*i+:MEM_IN_ADDR_WIDTH[MEM_ADDR_MAX_WIDTH*i+:MEM_ADDR_MAX_WIDTH]]), // ctrl
          .data_input___i (dataInAIP_net), // interface

          .Read_address_i (rdAddrMemIn_uP[MEM_ADDR_MAX_WIDTH*i+:MEM_IN_ADDR_WIDTH[MEM_ADDR_MAX_WIDTH*i+:MEM_ADDR_MAX_WIDTH]]), // ip
          .data_output__o (rdDataMemIn0) // ip
        );
      else
        simple_dual_port_ram_single_clk
        #(
          .DATA_WIDTH (DATA_WIDTH),
          .ADDR_WIDTH (MEM_IN_ADDR_WIDTH[MEM_ADDR_MAX_WIDTH*i+:MEM_ADDR_MAX_WIDTH])
        )
        MEMIN
        (
          .Write_clock__i (clk),

          .Write_enable_i (wrEnMemIn_net[i]), // ctrl
          .Write_addres_i (wrAddrMemIn_net[MEM_ADDR_MAX_WIDTH*i+:MEM_IN_ADDR_WIDTH[MEM_ADDR_MAX_WIDTH*i+:MEM_ADDR_MAX_WIDTH]]), // ctrl
          .data_input___i (dataInAIP_net), // interface

          .Read_address_i (rdAddrMemIn_uP[MEM_ADDR_MAX_WIDTH*i+:MEM_IN_ADDR_WIDTH[MEM_ADDR_MAX_WIDTH*i+:MEM_ADDR_MAX_WIDTH]]), // ip
          .data_output__o (rdDataMemIn_uP[DATA_WIDTH*i+:DATA_WIDTH]) // ip
        );
    end //for
  endgenerate
  `endif

    `ifdef MEM_IN_DEF
    assign rdDataMemIn_uP[0+:DATA_WIDTH] = rdDataMemIn0; // MemIn
    `endif

  //---------- Mem Out ----------//
  `ifdef MEM_OUT_DEF
  generate
    for(i=0; i<`MEM_OUT_DEF; i = i+1) begin : MEMOUT
      simple_dual_port_ram_single_clk
      #(
        .DATA_WIDTH (DATA_WIDTH),
        .ADDR_WIDTH (MEM_OUT_ADDR_WIDTH[MEM_ADDR_MAX_WIDTH*i+:MEM_ADDR_MAX_WIDTH])
      )
      MEMOUT
      (
        .Write_clock__i (clk),

        .Write_enable_i (wrEnMemOut_uP[i]), // ip
        .Write_addres_i (wrAddrMemOut_uP[MEM_ADDR_MAX_WIDTH*i+:MEM_OUT_ADDR_WIDTH[MEM_ADDR_MAX_WIDTH*i+:MEM_ADDR_MAX_WIDTH]]), // ctrl
        .data_input___i (dataInAIP_uP), // ip

        .Read_address_i (rdAddrMemOut_net[MEM_ADDR_MAX_WIDTH*i+:MEM_OUT_ADDR_WIDTH[MEM_ADDR_MAX_WIDTH*i+:MEM_ADDR_MAX_WIDTH]]), // ip
        .data_output__o (rdDataMemOut_net[DATA_WIDTH*i+:DATA_WIDTH]) // mux
      );
    end //for
  endgenerate
  `endif

  //---------- Conf Reg ----------//
  `ifdef CONF_REG_DEF
    aipConfigurationRegister
    #(
      .DATAWIDTH (DATA_WIDTH),
      .REGISTERS (CONF_REG_ADDR_WIDTH)
    )
    CONFREG
    (
      .reset(rst),
      .writeClock (clk),

      .writeEnable (wrEnConfigReg_net), // cntrl
      .writeAddress (wrAddrConfigReg_net), // cntrl

      .dataInput (dataInAIP_net), // interface
      .dataOutput ({configsAIP,rdDataConfigReg}) // ip
    );

    aipParametricMux
    #(
      .DATAWIDTH (DATA_WIDTH),
      .SELBITS (2)
    )
    MUX_CONFREG_UP
    (
      .data_in ({{128-(CONF_REG_ADDR_WIDTH*DATA_WIDTH){1'b0}},rdDataConfigReg}),
      .sel (rdAddrConfigReg_uP),
      .data_out (rdDataConfigRegMux)
    );
  `endif

  //---------- Control ----------//
  aipCtrl
  #(
    `ifdef MEM_IN_DEF
    .CONFIG_MEM_IN(CONFIG_MEM_IN),
    `endif
    `ifdef MEM_OUT_DEF
    .CONFIG_MEM_OUT(CONFIG_MEM_OUT),
    `endif
    `ifdef CONF_REG_DEF
    .CONF_REG_ADDR_WIDTH(CONF_REG_ADDR_WIDTH),
    .CONFIG_CONF_REG(CONFIG_CONF_REG),
    `endif
    .SIZE_MUX(SEL_BITS)
  )
  CNTRL_NET
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .readAIP(readAIP_net),
    .writeAIP(writeAIP_net),
    .configAIP(configAIP_net),

    `ifdef MEM_IN_DEF
    .wrEnMemIn(wrEnMemIn_net),
    .wrAddrMemIn(wrAddrMemIn_net),
    `endif

    `ifdef MEM_OUT_DEF
    .rdAddrMemOut(rdAddrMemOut_net),
    `endif

    `ifdef CONF_REG_DEF
    .wrAddrConfigReg(wrAddrConfigReg_net),
    .wrEnConfigReg(wrEnConfigReg_net),
    `endif

    .memAddr(dataInAIP_net[MEM_ADDR_MAX_WIDTH-1:0]),
    .setStatus(setStatus_net),
    .selMux(selMux_net)
  );

  aipCtrlCoprocessor
  #(
    `ifdef MEM_IN_DEF
    .CONFIG_MEM_IN(CONFIG_MEM_IN),
    `endif
    `ifdef MEM_OUT_DEF
    .CONFIG_MEM_OUT(CONFIG_MEM_OUT),
    `endif
    `ifdef CONF_REG_DEF
    .CONF_REG_ADDR_WIDTH(CONF_REG_ADDR_WIDTH),
    .CONFIG_CONF_REG(CONFIG_CONF_REG),
    `endif
    .SIZE_MUX(SEL_BITS)
  )
  CNTRL_UP
  (
    .clk (clk),
    .rst (rst),
    .en (en),
    .readAIP (readAIP_uP),
    .writeAIP (writeAIP_uP),
    .configAIP (configAIP_uP),

    `ifdef MEM_OUT_DEF
    .wrEnMemOut (wrEnMemOut_uP),
    .wrAddrMemOut (wrAddrMemOut_uP),
    `endif

    `ifdef MEM_IN_DEF
    .rdAddrMemIn (rdAddrMemIn_uP),
    `endif

    `ifdef CONF_REG_DEF
    .rdAddrConfigReg (rdAddrConfigReg_uP),
    `endif

    .memAddr (dataInAIP_uP[MEM_ADDR_MAX_WIDTH-1:0]),
    .setStatus (setStatus_uP),
    .selMux (selMux_uP)
  );

  genvar j;
  generate
    for (j=((2**SEL_BITS)-1); j>=0; j = j - 1) begin : SEL_MUX_NET
      if (j == ((2**SEL_BITS)-2))
        assign dataMux_net[DATA_WIDTH*j+:DATA_WIDTH] = wireStatus_net;
      else if (j == ((2**SEL_BITS)-1))
        assign dataMux_net[DATA_WIDTH*j+:DATA_WIDTH] = wireIpId_net;
      `ifdef MEM_OUT_DEF
      else if (j<`MEM_OUT_DEF)
        assign dataMux_net[DATA_WIDTH*j+:DATA_WIDTH] = rdDataMemOut_net[DATA_WIDTH*j+:DATA_WIDTH];
      `endif
      else
        assign dataMux_net[DATA_WIDTH*j+:DATA_WIDTH] = 32'h00000000;
    end // for

    for (j=((2**SEL_BITS)-1); j>=0; j = j - 1) begin : SEL_MUX_UP
      if (j == ((2**SEL_BITS)-1))
        assign dataMux_uP[DATA_WIDTH*j+:DATA_WIDTH] = wireStatus_net;
      `ifdef MEM_IN_DEF
      else if (j<`MEM_IN_DEF)
          assign dataMux_uP[DATA_WIDTH*j+:DATA_WIDTH] = rdDataMemIn_uP[DATA_WIDTH*j+:DATA_WIDTH];
      `ifdef CONF_REG_DEF
      else if (j==`MEM_IN_DEF)
          assign dataMux_uP[DATA_WIDTH*j+:DATA_WIDTH] = rdDataConfigRegMux;
      `endif
      `elsif CONF_REG_DEF
      else if (j==0)
        assign dataMux_uP[DATA_WIDTH*j+:DATA_WIDTH] = rdDataConfigRegMux;
      `endif
      else
        assign dataMux_uP[DATA_WIDTH*j+:DATA_WIDTH] = 32'h00000000;
    end // for
  endgenerate

  //---------- Mux ----------//
  aipParametricMux
  #(
    .DATAWIDTH (DATA_WIDTH),
    .SELBITS (SEL_BITS)
  )
  MUX_NET
  (
    .data_in (dataMux_net),
    .sel (selMux_net),
    .data_out (dataOutAIP_net)
  );

  aipParametricMux
  #(
    .DATAWIDTH (DATA_WIDTH),
    .SELBITS (SEL_BITS)
  )
  MUX_UP
  (
    .data_in (dataMux_uP),
    .sel (selMux_uP),
    .data_out (dataOutAIP_uP)
  );

  //---------- ID Reg ----------//
  aipId
  #(
    .ID (IP_ID)
  )
  ID
  (
    .clk (clk),
    .data_IP_ID (wireIpId_net)
  );

  //---------- Status Reg ----------//
  aipStatus STATUS
  (
    .clk (clk),
    .rst (rst),
    .enSet (setStatus_net),
    .dataIn (dataInAIP_net),
    .intIP (status_uP[7:0]),
    .statusIP (status_uP[15:8]),
    .dataStatus (wireStatus_net),
    .intReq (intAIP_net)
  );

  always @ (posedge clk or negedge rst ) begin
    if (!rst) begin
      status_uP = {DATA_WIDTH{1'b0}};
    end
    else begin
      if (setStatus_uP)
        status_uP <= dataInAIP_uP;
    end
  end

endmodule

module aipCtrl
(
  clk,
  rst,
  en,
  readAIP, // AIP read
  writeAIP, // AIP write
  configAIP, // AIP Config

  `ifdef MEM_IN_DEF
  wrEnMemIn,
  wrAddrMemIn,
  `endif

  `ifdef MEM_OUT_DEF
  rdAddrMemOut,
  `endif

  `ifdef CONF_REG_DEF
  wrAddrConfigReg,
  wrEnConfigReg,
  `endif

  memAddr, // address to set
  setStatus, // status register enable signal
  selMux // mux selector for data out
);

  localparam MEM_ADDR_MAX_WIDTH = 16; // address width for memories in/out

  localparam CONF_REG_ADDR_MAX_WIDTH = 3; // address width for configuration register

  localparam CONFIG_WIDTH = 5; // config parameter width

  localparam STAT_REG = 5'b11110; // config value for status register

  localparam ID_REG = 5'b11111; // config value for id register

  parameter SIZE_MUX = 'd2;

  `ifdef MEM_IN_DEF
  parameter [((`MEM_IN_DEF*2)*CONFIG_WIDTH)-1:0] CONFIG_MEM_IN = {5'b00011, 5'b00010, 5'b00001, 5'b00000};
  `endif

  `ifdef MEM_OUT_DEF
  parameter [((`MEM_OUT_DEF*2)*CONFIG_WIDTH)-1:0] CONFIG_MEM_OUT = {5'b00101, 5'b00100};
  `endif

  `ifdef CONF_REG_DEF
  parameter [CONF_REG_ADDR_MAX_WIDTH-1:0] CONF_REG_ADDR_WIDTH = {3'd2}; // size of conf reg

  parameter [(2*CONFIG_WIDTH)-1:0] CONFIG_CONF_REG = {5'b01011, 5'b01010};
  `endif

  input clk;
  input rst;
  input en;
  input readAIP;
  input writeAIP;
  input [CONFIG_WIDTH-1:0] configAIP;

  `ifdef MEM_IN_DEF
  output wire [(`MEM_IN_DEF*MEM_ADDR_MAX_WIDTH)-1:0] wrAddrMemIn;
  output wire [`MEM_IN_DEF-1:0] wrEnMemIn;

  reg [MEM_ADDR_MAX_WIDTH-1:0] regWrAddrMemIn [0:`MEM_IN_DEF];
  `endif

  `ifdef MEM_OUT_DEF
  output wire [(`MEM_OUT_DEF*MEM_ADDR_MAX_WIDTH)-1:0] rdAddrMemOut;

  reg [MEM_ADDR_MAX_WIDTH-1:0] regRdAddrMemOut [0:`MEM_OUT_DEF];
  `endif

  `ifdef CONF_REG_DEF
  output wire [CONF_REG_ADDR_MAX_WIDTH-1:0] wrAddrConfigReg;
  output wrEnConfigReg;

  reg [CONF_REG_ADDR_MAX_WIDTH-1:0] regWrAddrConfigReg;
  `endif

  output wire [SIZE_MUX-1:0] selMux;

  input [MEM_ADDR_MAX_WIDTH-1:0] memAddr;

  output wire setStatus;

  wire [SIZE_MUX-1:0] wireSelMux [0:2**SIZE_MUX-2];

  assign setStatus = writeAIP & (configAIP == STAT_REG);

  genvar indexEn;
  `ifdef MEM_IN_DEF
  generate
      for (indexEn=0; indexEn<`MEM_IN_DEF; indexEn= indexEn + 1) begin : WR_EN_MEMIN
          assign wrEnMemIn[indexEn] = writeAIP & (configAIP == CONFIG_MEM_IN[CONFIG_WIDTH*(indexEn*2)+:CONFIG_WIDTH]);
      end // for MEM_IN_DEF
   endgenerate
  `endif

  `ifdef CONF_REG_DEF
  generate
      assign wrEnConfigReg = writeAIP & (configAIP == CONFIG_CONF_REG[0+:CONFIG_WIDTH]);
   endgenerate
  `endif

  wire [31:0] idValue;
  wire [31:0] statusValue;
  assign statusValue = ((2**SIZE_MUX)-2);
  assign idValue = ((2**SIZE_MUX)-1);

  genvar j;
  generate
    if(SIZE_MUX > 1) begin
    for (j=((2**SIZE_MUX)-2); j>=0; j = j - 1) begin : SEL_MUX
      if (j==(2**SIZE_MUX)-2) begin
        assign wireSelMux[j] = configAIP == STAT_REG ? statusValue[SIZE_MUX-1:0] : idValue[SIZE_MUX-1:0];
      end // if (j==(2**SIZE_MUX)-2)
      else if (j==0) begin
        `ifdef MEM_OUT_DEF
        if (`MEM_OUT_DEF == 1) begin
          assign selMux = (configAIP == CONFIG_MEM_OUT[CONFIG_WIDTH*(j*2)+:CONFIG_WIDTH]) ? j[SIZE_MUX-1:0] : wireSelMux[(2**SIZE_MUX)-2];
        end
        else begin
          assign selMux = (configAIP == CONFIG_MEM_OUT[CONFIG_WIDTH*(j*2)+:CONFIG_WIDTH]) ? j[SIZE_MUX-1:0] : wireSelMux[j+1];
        end
        `else
        assign selMux = wireSelMux[(2**SIZE_MUX)-2];
        `endif
      end // if (j==0)
      `ifdef MEM_OUT_DEF
      else if (j == `MEM_OUT_DEF - 1) begin
        assign wireSelMux[j] = configAIP == CONFIG_MEM_OUT[CONFIG_WIDTH*(j*2)+:CONFIG_WIDTH] ? j[SIZE_MUX-1:0] : wireSelMux[(2**SIZE_MUX)-2];
      end // if (j == `MEM_OUT_DEF - 1)
      else if (j<`MEM_OUT_DEF) begin
        assign wireSelMux[j] = configAIP == CONFIG_MEM_OUT[CONFIG_WIDTH*(j*2)+:CONFIG_WIDTH] ? j[SIZE_MUX-1:0] : wireSelMux[j+1];
      end // if (j<`MEM_OUT_DEF)
      `endif
    end // for
    end // if(SEL_MUX > 1)
    else begin
      assign selMux = configAIP == STAT_REG ? statusValue[SIZE_MUX-1:0] : idValue[SIZE_MUX-1:0];
    end
  endgenerate

  integer i;
  always @(posedge clk or negedge rst) begin
    if (!rst) begin
      `ifdef MEM_IN_DEF
      for (i=0; i<`MEM_IN_DEF; i= i + 1) begin
        regWrAddrMemIn[i] <= 'd0;
      end
      `endif

      `ifdef MEM_OUT_DEF
      for (i=0; i<`MEM_OUT_DEF; i= i + 1) begin
        regRdAddrMemOut[i] <= 'd0;
      end
      `endif

      `ifdef CONF_REG_DEF
      regWrAddrConfigReg <= 'd0;
      `endif
    end // if (!rst)
    else begin
      if (en) begin
        `ifdef MEM_IN_DEF
        for (i=0; i<`MEM_IN_DEF; i= i + 1) begin
          if (writeAIP && configAIP == CONFIG_MEM_IN[CONFIG_WIDTH*(i*2+1)+:CONFIG_WIDTH])
            regWrAddrMemIn[i] <= memAddr[MEM_ADDR_MAX_WIDTH-1:0];
          else if (writeAIP && (configAIP == CONFIG_MEM_IN[CONFIG_WIDTH*(i*2)+:CONFIG_WIDTH]))
            regWrAddrMemIn[i] <= regWrAddrMemIn[i] + 1'b1;
        end // for MEM_IN_DEF
        `endif

        `ifdef MEM_OUT_DEF
        for (i=0; i<`MEM_OUT_DEF; i= i + 1) begin
          if (writeAIP && configAIP == CONFIG_MEM_OUT[CONFIG_WIDTH*(i*2+1)+:CONFIG_WIDTH])
            regRdAddrMemOut[i] <= memAddr[MEM_ADDR_MAX_WIDTH-1:0];
          else if (readAIP && (configAIP == CONFIG_MEM_OUT[CONFIG_WIDTH*(i*2)+:CONFIG_WIDTH]))
            regRdAddrMemOut[i] <= regRdAddrMemOut[i] + 1'b1;
        end // for MEM_OUT_DEF
        `endif

        `ifdef CONF_REG_DEF
        if (writeAIP && configAIP == CONFIG_CONF_REG[CONFIG_WIDTH+:CONFIG_WIDTH])
          regWrAddrConfigReg <= memAddr[CONF_REG_ADDR_MAX_WIDTH-1:0];
        else if (writeAIP && (configAIP == CONFIG_CONF_REG[0+:CONFIG_WIDTH]))
          if ((CONF_REG_ADDR_WIDTH[0+:CONF_REG_ADDR_MAX_WIDTH]-1) > regWrAddrConfigReg)
            regWrAddrConfigReg <= regWrAddrConfigReg + 1'b1;
          else
            regWrAddrConfigReg <= 'd0;
        `endif
      end // if (en)
    end // else
  end // always

  genvar indexAddr;
  `ifdef MEM_IN_DEF
  generate
    for (indexAddr=0; indexAddr<(`MEM_IN_DEF); indexAddr=indexAddr+1) begin: MEM_IN_ADDR
      assign wrAddrMemIn[MEM_ADDR_MAX_WIDTH*indexAddr +: MEM_ADDR_MAX_WIDTH] = regWrAddrMemIn[indexAddr][MEM_ADDR_MAX_WIDTH-1:0];
    end
  endgenerate
  `endif

  `ifdef MEM_OUT_DEF
  generate
    for (indexAddr=0; indexAddr<(`MEM_OUT_DEF); indexAddr=indexAddr+1) begin: MEM_OUT_ADDR
      assign rdAddrMemOut[MEM_ADDR_MAX_WIDTH*indexAddr +: MEM_ADDR_MAX_WIDTH] = regRdAddrMemOut[indexAddr][MEM_ADDR_MAX_WIDTH-1:0];
    end
  endgenerate
  `endif

  `ifdef CONF_REG_DEF
  generate
    assign wrAddrConfigReg = regWrAddrConfigReg;
  endgenerate
  `endif

endmodule

module aipCtrlCoprocessor
(
  clk,
  rst,
  en,
  readAIP,
  writeAIP,
  configAIP,

  `ifdef MEM_OUT_DEF
  wrEnMemOut, //wrEnMemIn
  wrAddrMemOut, //wrAddrMemIn
  `endif

  `ifdef MEM_IN_DEF
  rdAddrMemIn, //rdAddrMemOut
  `endif

  `ifdef CONF_REG_DEF
  rdAddrConfigReg, //wrAddrConfigReg
  `endif

  memAddr,
  setStatus,
  selMux
);

  localparam MEM_ADDR_MAX_WIDTH = 16; // address width for memories in/out

  localparam CONF_REG_ADDR_MAX_WIDTH = 3; // address width for configuration register

  localparam CONFIG_WIDTH = 5; // config parameter width

  localparam STAT_REG = 5'b11110; // config value for status register

  // localparam ID_REG = 5'b11111; // config value for id register

  parameter SIZE_MUX = 'd2;

  `ifdef MEM_IN_DEF
  parameter [((`MEM_IN_DEF*2)*CONFIG_WIDTH)-1:0] CONFIG_MEM_IN = {5'b00101, 5'b00100, 5'b00011, 5'b00010, 5'b00001, 5'b00000};
  `endif

  `ifdef MEM_OUT_DEF
  parameter [((`MEM_OUT_DEF*2)*CONFIG_WIDTH)-1:0] CONFIG_MEM_OUT = {5'b00111, 5'b00110, 5'b00101, 5'b00100};
  `endif

  `ifdef CONF_REG_DEF
  parameter [CONF_REG_ADDR_MAX_WIDTH-1:0] CONF_REG_ADDR_WIDTH = {3'd3}; // size of conf reg

  parameter [(2*CONFIG_WIDTH)-1:0] CONFIG_CONF_REG = {5'b01001, 5'b01000};
  `endif

  input clk;
  input rst;
  input en;
  input readAIP;
  input writeAIP;
  input [CONFIG_WIDTH-1:0] configAIP;

  `ifdef MEM_OUT_DEF
  output wire [(`MEM_OUT_DEF*MEM_ADDR_MAX_WIDTH)-1:0] wrAddrMemOut;
  output wire [`MEM_OUT_DEF-1:0] wrEnMemOut;

  reg [MEM_ADDR_MAX_WIDTH-1:0] regWrAddrMemOut [0:`MEM_OUT_DEF];
  `endif

  `ifdef MEM_IN_DEF
  output wire [(`MEM_IN_DEF*MEM_ADDR_MAX_WIDTH)-1:0] rdAddrMemIn;

  reg [MEM_ADDR_MAX_WIDTH-1:0] regRdAddrMemIn [0:`MEM_IN_DEF]; //regRdAddrMemOut
  `endif

  `ifdef CONF_REG_DEF
  output wire [1:0] rdAddrConfigReg; //wrAddrConfigReg

  reg [1:0] regRdAddrConfigReg; //regWrAddrConfigReg
  `endif

  output wire [SIZE_MUX-1:0] selMux;

  input [MEM_ADDR_MAX_WIDTH-1:0] memAddr;

  wire [SIZE_MUX-1:0] wireSelMux [0:2**SIZE_MUX-1];

  output wire setStatus;

  assign setStatus = writeAIP & (configAIP == STAT_REG);

  genvar indexEn;
  `ifdef MEM_OUT_DEF
  generate
    for (indexEn=0; indexEn<`MEM_OUT_DEF; indexEn= indexEn + 1) begin : WR_EN_MEMOUT
      assign wrEnMemOut[indexEn] = writeAIP & (configAIP == CONFIG_MEM_OUT[CONFIG_WIDTH*(indexEn*2)+:CONFIG_WIDTH]);
    end // for MEM_OUT_DEF
  endgenerate
  `endif

  wire [32-1:0] statusValue;
  assign statusValue = ((2**SIZE_MUX)-1);

  genvar j;
  generate
    for (j=((2**SIZE_MUX)-1); j>=0; j = j - 1) begin : SEL_MUX
      if (j==(2**SIZE_MUX)-1) begin
        // assign wireSelMux[j] = ((2**SIZE_MUX)-1); //status
        assign wireSelMux[j] = statusValue[SIZE_MUX-1:0]; //status
      end //if (j==(2**SIZE_MUX)-1)
      else if (j==0) begin
        `ifdef MEM_IN_DEF
        assign selMux = (configAIP == CONFIG_MEM_IN[CONFIG_WIDTH*(j*2)+:CONFIG_WIDTH]) ? j[SIZE_MUX-1:0] : wireSelMux[j+1]; //memIn
        `elsif CONF_REG_DEF
        assign selMux = (configAIP == CONFIG_CONF_REG[0+:CONFIG_WIDTH]) ? j[SIZE_MUX-1:0] : wireSelMux[(2**SIZE_MUX)-1]; //conf
        `else
        assign selMux = wireSelMux[(2**SIZE_MUX)-1]; //status
        `endif
      end //else if (j==0)
      `ifdef MEM_IN_DEF
      else if (j == `MEM_IN_DEF) begin
        `ifdef CONF_REG_DEF
        assign wireSelMux[j] = configAIP == CONFIG_CONF_REG[0+:CONFIG_WIDTH] ? j[SIZE_MUX-1:0] : wireSelMux[(2**SIZE_MUX)-1]; //memIn - conf
        `else
        assign wireSelMux[j] = wireSelMux[(2**SIZE_MUX)-1]; // memIn
        `endif
      end //else if (j == `MEM_IN_DEF)
      else if (j<`MEM_IN_DEF) begin
        assign wireSelMux[j] = configAIP == CONFIG_MEM_IN[CONFIG_WIDTH*(j*2)+:CONFIG_WIDTH] ? j[SIZE_MUX-1:0] : wireSelMux[j+1]; // memIn
      end //else if (j<`MEM_IN_DEF)
      `endif
    end // for
  endgenerate

  integer i;
  always @(posedge clk or negedge rst) begin
    if (!rst) begin
      `ifdef MEM_OUT_DEF
      for (i=0; i<`MEM_OUT_DEF; i= i + 1) begin
          regWrAddrMemOut[i] <= 'd0;
      end
      `endif

      `ifdef MEM_IN_DEF
      for (i=0; i<`MEM_IN_DEF; i= i + 1) begin
        regRdAddrMemIn[i] <= 'd0;
      end
      `endif

      `ifdef CONF_REG_DEF
      regRdAddrConfigReg <= 'd0;
      `endif
    end // if (!rst)
    else begin
      if (en) begin
        `ifdef MEM_OUT_DEF
        for (i=0; i<`MEM_OUT_DEF; i= i + 1) begin
          if (writeAIP && configAIP == CONFIG_MEM_OUT[CONFIG_WIDTH*(i*2+1)+:CONFIG_WIDTH])
            regWrAddrMemOut[i] <= memAddr[MEM_ADDR_MAX_WIDTH-1:0];
          else if (writeAIP && (configAIP == CONFIG_MEM_OUT[CONFIG_WIDTH*(i*2)+:CONFIG_WIDTH]))
            regWrAddrMemOut[i] <= regWrAddrMemOut[i] + 1'b1;
        end // for MEM_OUT_DEF
        `endif

        `ifdef MEM_IN_DEF
        for (i=0; i<`MEM_IN_DEF; i= i + 1) begin
          if (writeAIP && configAIP == CONFIG_MEM_IN[CONFIG_WIDTH*(i*2+1)+:CONFIG_WIDTH])
            regRdAddrMemIn[i] <= memAddr[MEM_ADDR_MAX_WIDTH-1:0];
          else if (readAIP && (configAIP == CONFIG_MEM_IN[CONFIG_WIDTH*(i*2)+:CONFIG_WIDTH]))
            regRdAddrMemIn[i] <= regRdAddrMemIn[i] + 1'b1;
        end // for MEM_IN_DEF
        `endif

        `ifdef CONF_REG_DEF
        if (writeAIP && configAIP == CONFIG_CONF_REG[CONFIG_WIDTH+:CONFIG_WIDTH])
            regRdAddrConfigReg <= memAddr[1:0];
        else if (readAIP && (configAIP == CONFIG_CONF_REG[0+:CONFIG_WIDTH]))
            if ((CONF_REG_ADDR_WIDTH[0+:CONF_REG_ADDR_MAX_WIDTH]-1) > regRdAddrConfigReg)
                regRdAddrConfigReg <= regRdAddrConfigReg + 1'b1;
            else
                regRdAddrConfigReg <= 'd0;
        `endif
      end // if (en)
    end // else
  end // always

  genvar indexAddr;
  `ifdef MEM_OUT_DEF
  generate
    for (indexAddr=0; indexAddr<(`MEM_OUT_DEF); indexAddr=indexAddr+1) begin: MEM_OUT_ADDR
      assign wrAddrMemOut[MEM_ADDR_MAX_WIDTH*indexAddr +: MEM_ADDR_MAX_WIDTH] = regWrAddrMemOut[indexAddr][MEM_ADDR_MAX_WIDTH-1:0];
    end
  endgenerate
  `endif

  `ifdef MEM_IN_DEF
  generate
    for (indexAddr=0; indexAddr<(`MEM_IN_DEF); indexAddr=indexAddr+1) begin: MEM_IN_ADDR
      assign rdAddrMemIn[MEM_ADDR_MAX_WIDTH*indexAddr +: MEM_ADDR_MAX_WIDTH] = regRdAddrMemIn[indexAddr][MEM_ADDR_MAX_WIDTH-1:0];
    end
  endgenerate
  `endif

  `ifdef CONF_REG_DEF
  generate
      assign rdAddrConfigReg = regRdAddrConfigReg;
  endgenerate
  `endif

endmodule
