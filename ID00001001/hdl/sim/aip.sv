//localparam AIP_DATA_WIDTH = 32;
//localparam AIP_CONFIG_WIDTH = 5;

interface aip_if;
    logic [31:0] aip_dataIn;
    logic [31:0] aip_dataOut;
    logic [4:0] aip_config;
    logic aip_read;
    logic aip_write;
    logic aip_start;
    logic aip_int;

    task automatic getID;
        output logic [31:0] id;
        begin
            $display("Data %x", id);
            aipRead('d31, id);
            $display("Data %x", id);
        end
    endtask // getID

    task automatic writeMem;
        input logic [4:0] configMem;
        input logic [31:0] dataWrite [];
        input int amountData;
        input int offset;
        begin
            // set address
            aipWrite(configMem+1, offset);

            // write data
            for (int idx = 0; idx < amountData; idx++) begin
                aipWrite(configMem, dataWrite[idx]);
            end
        end
    endtask // writeMem

    task automatic readMem;
        input logic [4:0] configMem;
        output logic [31:0] dataRead [];
        input int amountData;
        input int offset;
        reg [31:0] dataRX;
        begin
            dataRead = new [amountData];

            // set address
            aipWrite(configMem+1, offset);

            // write datas
            for (int idx = 0; idx < amountData; idx++) begin
                aipRead('d2, dataRX);
                dataRead[idx] = dataRX;
            end
        end
    endtask // readMem

    task automatic writeConfReg;
        input logic [4:0] configMem;
        input logic [31:0] dataWrite [];
        input int amountData;
        input int offset;
        begin
            // set address
            aipWrite(configMem+1, offset);

            // write data
            for (int idx = 0; idx < amountData; idx++) begin
                aipWrite(configMem, dataWrite[idx]);
            end
        end
    endtask // writeConfReg

    task automatic start;
        begin
            aipStart();
        end
    endtask // start

    task automatic clearINT;
        input int idx;
        reg [31:0] status;
        begin
            // get status
            aipRead('d30, status);

            status = (status & ('d1 << idx));

            // set status
            aipWrite('d30, status);
        end
    endtask // clearINT

    task automatic getStatus;
        output logic [31:0] status;    
        begin
            aipRead('d30, status);
        end
    endtask // getStatus

    task automatic aipRead;
        input logic [4:0] configReg;
        output logic [31:0] data;
        begin
            aip_config = configReg;

            #2;
            aip_read = 1'b1;
            
            #2;
            aip_read = 1'b0;
            data = aip_dataOut;

            #2;
        end
    endtask // aipRead

    task automatic aipWrite;
        input logic [4:0] configReg;
        input logic [31:0] dataWR;
        begin
            aip_config = configReg;
            aip_dataIn = dataWR;

            #2;
            aip_write = 1'b1;

            #2;
            aip_write = 1'b0;

            #2;
        end
    endtask // aipWrite

    task automatic aipStart;
        begin
            aip_start = 1'b1;

            #2;
            aip_start = 1'b0;

            #2;
        end
    endtask
endinterface

//    localparam AIP_DATA_WIDTH = 32;
//    localparam AIP_CONFIG_WIDTH = 5;






    


    // void (*enableINT)(uint8_t idxInt, const char* addrs);
    
    // void (*disableINT)(uint8_t idxInt, const char* addrs);
    
    // void (*getINT)(uint8_t* intVector, const char* addrs);
    
    // void (*getNotification)(uint8_t* notificationsVector, const char* addrs);

//endpackage // aip

// void (*reset)();
// void (*version)(char *version);
// void (*readconfigs)(const char *filename, const char *addrs);
// void (*enableNicInts)(const char *addrs);
// void (*waitForINT)();
// void (*finish)();
