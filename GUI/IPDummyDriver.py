import logging, time, io
from ipdi.ip.pyaip import pyaip, pyaip_init
from typing import Iterable, List, Optional
import os
#from ipdi_lib.ipdi.ip.pyaip import pyaip, pyaip_init
script_dir = os.path.dirname(os.path.abspath(__file__))
file_path = os.path.abspath(os.path.join(script_dir, "../IpdiDriver/ID0000200F_config.csv"))


## IP Dummy driver class
class IPDummyDriver:
    ## Class constructor of IP Dummy driver
    #
    # @param self Object pointer
    # @param targetConn Middleware object
    # @param config Dictionary with IP Dummy configs
    # @param addrs Network address IP
    def __init__(self, connector, nic_addr, port, csv_file):
        ## Pyaip object
        self.__pyaip = pyaip_init(connector, nic_addr, port, csv_file)

        if self.__pyaip is None:
            logging.debug(error)

        ## Array of strings with information read
        self.dataRX = []

        ## IP Dummy IP-ID
        self.IPID = 0

        self.__getID()

        self.__clearStatus()
        self.__PICORV32_instruc = []
        logging.debug(f"IP PICORV32 controller created with IP ID {self.IPID:08x}")

    def OpenTxData(self, name):
        # self.name=""
        with io.open(name, 'r') as Data2TxFile:
            line = Data2TxFile.readline()
            while line:
               line = str(line)
               aa = "0x" + line[0:8]
               #print("string numbers")
               print(aa)
               self.__PICORV32_instruc.append(int(aa, 0))
               # print(int(aa,0))
               line = Data2TxFile.readline()
    
    def sendPICORV32_PROG(self):
        data =  self.__PICORV32_instruc
        dataLen = len(self.__PICORV32_instruc)
        print("Lets fullfil MProgramMEMIN")
        if dataLen != 0:
            self.writeData(self.__PICORV32_instruc)
            print("[{}]".format(', '.join(hex(x) for x in data)))
        else:
            logging.debug("There is no PICORV32 instructions")
    ## Write data in the IP Dummy input memory
    #
    # @param self Object pointer
    # @param data String array with the data to write
    def writeData(self, data):
        self.__pyaip.writeMem('MProgramMEMIN', data, len(data), 0)

        logging.debug("Data captured in Mem Data In")

    def resetPICORV32(self):
        self.__pyaip.writeConfReg('CCONFREG', [1], 1, 0)
        time.sleep(0.1)

        
    
    def assertPICORV32(self):
        self.__pyaip.writeConfReg('CCONFREG', [0], 1, 0)
        time.sleep(0.1)

    ## Read data from the IP Dummy output memory
    #
    # @param self Object pointer
    # @param size Amount of data to read
    def readData(self, size):
        data = self.__pyaip.readMem('MDATAOUT', size, 0)
        logging.debug("Data obtained from Mem Data Out")
        return data

    ## Start processing in IP Dummy
    #
    # @param self Object pointer
    def startIP(self):
        self.__pyaip.start()

        logging.debug("Start sent")



    ## Enable IP Dummy interruptions
    #
    # @param self Object pointer
    def enableINT(self):
        self.__pyaip.enableINT(0,None)

        logging.debug("Int enabled")

    ## Disable IP Dummy interruptions
    #
    # @param self Object pointer
    def disableINT(self):
        self.__pyaip.disableINT(0)

        logging.debug("Int disabled")
    
    ## Show IP Dummy status
    #
    # @param self Object pointer
    def status(self):
        status = self.__pyaip.getStatus()
        logging.info(f"{status:08x}")

        ## Show IP Dummy status
        #
        # @param self Object pointer
        def status(self):
            status = self.__pyaip.getStatus()
            logging.info(f"{status:08x}")

    ## Finish connection
    #
    # @param self Object pointer
    def finish(self):
        self.__pyaip.finish()

    ## Wait for the completion of the process
    #
    # @param self Object pointer
    def waitInt(self):
        waiting = True
        
        while waiting:

            status = self.__pyaip.getStatus()

            logging.debug(f"status {status:08x}")
            
            if status & 0x1:
                waiting = False
            
            time.sleep(0.1)

    ## Get IP ID
    #
    # @param self Object pointer
    def __getID(self):
        self.IPID = self.__pyaip.getID()

    ## Clear status register of IP Dummy
    #
    # @param self Object pointer
    def __clearStatus(self):
        for i in range(8):
            self.__pyaip.clearINT(i)


class IPDIWrapperController:
    def __init__(
        self,
        #connector: str = "/dev/ttyACM0",
        connector: str = "COM7",
        addr: int = 1,
        port: int = 0,
        csv_file: str = file_path,
        aip_mem_size: int = 8,
        *,
        auto_init: bool = False,
        seed: int = 1,
        log_level: int = logging.INFO,
    ) -> None:
        logging.basicConfig(level=log_level, format="%(levelname)s: %(message)s")

        self.connector = connector
        self.addr = addr
        self.port = port
        self.csv_file = csv_file
        self.aip_mem_size = aip_mem_size
        self._driver = None

        if seed is not None:
            import random
            random.seed(seed)

        # create driver
        try:
            self._driver = IPDummyDriver(self.connector, self.addr, self.port, self.csv_file)
            logging.info("PicoRV32: Driver created")
        except Exception as e:
            logging.error(f"PicoRV32: Driver not created: {e}")
            raise

        # run the sequence INSIDE the constructor
        if auto_init:
            try:
                self._driver.disableINT()
                self._driver.resetPICORV32()
                logging.info("PicoRV32: starts")
                self._driver.startIP()
                st = self._driver.status()
                logging.info("PicoRV32 STATUS: %s", st)
            except Exception as e:
                logging.error(f"PicoRV32: init sequence failed: {e}")
                # If you prefer to tear down on failure:
                try:
                    self._driver.finish()
                except Exception:
                    pass
                self._driver = None
                raise

    def finish(self) -> None:
        if self._driver is not None:
            try:
                self._driver.finish()
                logging.info("PicoRV32: Driver finished")
            finally:
                self._driver = None

    def close(self) -> None:
        self.finish()

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc, tb):
        self.close()

    def __del__(self):
        try:
            self.close()
        except Exception:
            pass

    # passthroughs / helpers
    def status(self):
        assert self._driver is not None, "Driver not initialized"
        return self._driver.status()

    def writeData(self, data) -> None:
        data_list = data if isinstance(data, list) else [data]
        self._driver.disableINT()
        self._driver.resetPICORV32()
        self._driver.writeData(data_list)
        self._driver.assertPICORV32()
        self._driver.startIP()
        self._driver.status()

    # If you still want direct access to raw calls:
    def disableINT(self) -> None:
        assert self._driver is not None
        self._driver.disableINT()

    def resetPICORV32(self) -> None:
        assert self._driver is not None
        self._driver.resetPICORV32()

    def startIP(self) -> None:
        assert self._driver is not None
        self._driver.startIP()

"""

if __name__ == "__main__":

    data = ""
    ipdiInstance = IPDIWrapperController()
    
    while data != "00000000":
        data = input("Enter 8 hex digits (00000000 to quit): ").strip()
        if len(data) != 8:
            print("Need exactly 8 hex digits."); continue
        try:
            b = [int(data[i:i+2], 16) for i in range(0, 8, 2)]
        except ValueError:
            print("Invalid hex."); continue
        
        ipdiInstance.writeData(b)

    ipdiInstance.finish()
"""