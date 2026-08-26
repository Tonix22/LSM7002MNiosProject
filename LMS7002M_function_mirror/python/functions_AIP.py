from ipdi.ip.pyaip import pyaip, pyaip_init

import sys

def aip_init ():
    try:
        connector = 'COM7'
        nic_addr = 1
        port = 0
        csv_file = 'C:/WorkspaceDDigital/LSM7002MNiosProject/IpdiDriver/ID0000200F_config.csv'

        aip = pyaip_init(connector, nic_addr, port, csv_file)

        aip.reset()

        return aip

    except:
        e = sys.exc_info()
        print('ERROR: ', e)

        aip.finish()
        raise

def aip_finish (aip):
    if aip is not None:
        aip.finish()
   
def aip_write (aip, parameters, nflits, offset):
    if aip is not None:
        aip.writeMem('MProgramMEMIN', parameters, nflits, offset)
    
def aip_start (aip):
    if aip is not None:
        aip.start()

def aip_read (aip, nflits, offset):
    if aip is not None:
        ctrl_answer = aip.readMem('MMEMOUT', nflits, offset)  
        return ctrl_answer
    return None

def aip_reset (aip):
    if aip is not None:
        aip.reset()   

def clearDone (aip):
    if aip is not None:
        aip.clearINT(0)
 

def getStatus (aip):
    if aip is not None:
        STATUS = aip.getStatus()
        return STATUS
    return None
 
    

    
 