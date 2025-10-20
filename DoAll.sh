#!/usr/bin/env bash

quartus_pgm -m jtag -o "P;output_files/MasterSoC_DE1SoC.sof@2"
./BuildNios.sh build
./BuildNios.sh download
sudo chmod 777 /dev/ttyACM0