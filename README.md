# MasterSoC_DE1SoC

## Run synthesis

`quartus_sh --flow compile MasterSoC_DE1SoC`

## Download bitstream
1. First test with 

`jtagconfig`

if it doesnt work add persmisions to jtag

`sudo tee /etc/udev/rules.d/51-usbblaster.rules >/dev/null <<'EOF'
SUBSYSTEM=="usb", ATTR{idVendor}=="09fb", MODE:="0666"
SUBSYSTEM=="usb", ATTR{idVendor}=="09fb", ATTR{idProduct}=="6001", MODE:="0666"
SUBSYSTEM=="usb", ATTR{idVendor}=="09fb", ATTR{idProduct}=="6002", MODE:="0666"
SUBSYSTEM=="usb", ATTR{idVendor}=="09fb", ATTR{idProduct}=="6003", MODE:="0666"
SUBSYSTEM=="usb", ATTR{idVendor}=="09fb", ATTR{idProduct}=="6010", MODE:="0666"
SUBSYSTEM=="usb", ATTR{idVendor}=="09fb", ATTR{idProduct}=="6810", MODE:="0666"
EOF

sudo udevadm control --reload-rules
sudo udevadm trigger`

Unplug and plug USB blaster cable

2. Download bitstream
`quartus_pgm -m jtag -o "P;output_files/MasterSoC_DE1SoC.sof"`

if fails try 

`quartus_pgm -m jtag -o "P;output_files/MasterSoC_DE1SoC.sof@2"'


## NIOS Compilation

Run the BuildScript
BuildNios.sh - OPTION

OPTIONS

  echo "Uso: $0 {bsp|mk|list|build}"
  echo "  bsp   : nios2-bsp-generate-files (desde settings.bsp)"
  echo "  mk    : nios2-app-update-makefile (genera/actualiza Makefile)"
  echo "  list  : lista fuentes detectadas por la app"
  echo "  build : make (compila la app)"


## Download Software

```
./BuildNios.sh download
```

nios2-download -g MasterSoC_Controller/software/MasterSoC_3pAIP_v1/app.elf

## NIOS Terminal

nios2-terminal # (Ctrl+] para salir)


## GDB server and client

### SERVER

source ~/intelFPGA_lite/18.1/nios2eds/nios2_command_shell.sh
nios2-gdb-server -c "DE-SoC [1-12]" -d 2 -i 0 --tcpport 2342

### CLIENT

nios2-elf-gdb MasterSoC_Controller/software/MasterSoC_3pAIP_v2/MasterSoC_3pAIP_v2.elf \
  -ex "target remote :2342" \
  -ex "monitor reset" \
  -ex "load" \
  -ex "break main" \
  -ex "continue"

