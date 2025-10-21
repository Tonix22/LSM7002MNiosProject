#!/usr/bin/env bash
# Save current directory
CUR_DIR=$(pwd)

# Go to the Intel FPGA Nios2 EDS directory
cd ~/intelFPGA_lite/18.1/nios2eds || {
  echo "❌ Directory not found!"
  exit 1
}

# Source the Nios II environment
echo "✅ Sourcing Nios II command shell..."
source ./nios2_command_shell.sh

# Go back to your original directory
cd "$CUR_DIR" || exit
echo "✅ Returned to $CUR_DIR"
