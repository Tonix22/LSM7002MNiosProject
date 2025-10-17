from ipdi.ip.pyaip import pyaip, pyaip_init

import sys

try:
    connector = 'COM10'
    nic_addr = 1
    port = 0
    csv_file = 'C:/WorkspaceDDigital/LSM7002MNiosProject/config/ID00004003_config.csv'

    aip = pyaip_init(connector, nic_addr, port, csv_file)

    aip.reset()

    #==========================================
    # Code generated with IPAccelerator 

   # DATA_IN = [0x00003456, 0x00001234]
    DATA_IN = []

    with open("DATA_IN.ipd", "r") as f:
        for linea in f:
            # Divide en bytes y elimina espacios extra
            bytes_str = linea.strip().split()
            if len(bytes_str) == 4:
                # Convierte cada byte a entero y combina los 4 en un solo valor de 32 bits
                val = (int(bytes_str[0], 16) << 24) | \
                      (int(bytes_str[1], 16) << 16) | \
                      (int(bytes_str[2], 16) << 8)  | \
                       int(bytes_str[3], 16)
                DATA_IN.append(val)


    print('Write memory: MDATAIN')
    aip.writeMem('MDATAIN', DATA_IN, len(DATA_IN), 0)
    print(f'DATA_IN Data: {[f"{x:08X}" for x in DATA_IN]}\n')

    print('Start IP\n')
    aip.start()

   # print('Read memory: MDATAOUT')
   # DATA_OUT = aip.readMem('MDATAOUT', 2, 0)
   # print(f'DATA_OUT Data: {[f"{x:08X}" for x in DATA_OUT]}\n')

    #==========================================

    aip.finish()

except:
    e = sys.exc_info()
    print('ERROR: ', e)

    aip.finish()
    raise
