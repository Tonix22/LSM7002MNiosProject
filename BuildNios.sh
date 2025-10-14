#!/usr/bin/env bash
# nios_build.sh — simple y directo (con raíz común)

set -euo pipefail

# ====== AJUSTA SOLO ESTO ======
PROJECT_ROOT="/home/tonix/Documents/MasterSoC_DE1SoC/MasterSoC_Controller"
NIOS_ACDS="/home/tonix/intelFPGA_lite/18.1"   # raíz de IntelFPGA (18.1)
# ==============================

# Derivados (puedes sobreescribir con variables de entorno si quieres)
BSP_DIR="${BSP_DIR:-$PROJECT_ROOT/software/MasterSoC_3pAIP_v2_bsp}"
APP_DIR="${APP_DIR:-$PROJECT_ROOT/software/MasterSoC_3pAIP_v2}"
ELF_NAME="${ELF_NAME:-app.elf}"

cmd="${1:-}"

# Bootstrap mínimo del entorno Nios II solo si hace falta
ensure_nios_env() {
  if command -v nios2-bsp-generate-files >/dev/null 2>&1; then
    return
  fi
  local NIOS="$NIOS_ACDS/nios2eds"
  local QUARTUS="$NIOS_ACDS/quartus"
  local GNU64="$NIOS/bin/gnu/H-x86_64-pc-linux-gnu/bin"
  local GNU32="$NIOS/bin/gnu/H-i686-pc-linux-gnu/bin"

  export SOPC_KIT_NIOS2="$NIOS"
  export QUARTUS_ROOTDIR="$QUARTUS"

  local ADD="$NIOS/bin:$NIOS/sdk2/bin:$QUARTUS/bin"
  if [ -d "$GNU64" ]; then ADD="$GNU64:$ADD"
  elif [ -d "$GNU32" ]; then ADD="$GNU32:$ADD"; fi

  export PATH="$ADD:$PATH"
}

usage() {
  cat <<EOF
Uso: $0 {bsp|mk|list|build}

  bsp   : nios2-bsp-generate-files (desde settings.bsp en:
          $BSP_DIR)
  mk    : nios2-app-update-makefile (genera/actualiza Makefile para:
          $APP_DIR, elf=$ELF_NAME)
  list  : lista fuentes detectadas por la app
  build : make clean all en el directorio de la app
EOF
}

if [[ -z "$cmd" ]]; then
  usage; exit 1
fi

ensure_nios_env

case "$cmd" in
  bsp)
    nios2-bsp-generate-files \
      --settings "$BSP_DIR/settings.bsp" \
      --bsp-dir "$BSP_DIR"
    ;;

  mk)
    nios2-app-update-makefile \
      --app-dir "${APP_DIR}" \
      --bsp-dir "${BSP_DIR}" \
      --src-dir "${APP_DIR}" \
      --elf-name "${ELF_NAME}"
    ;;

  list)
    nios2-app-update-makefile \
      --list-src-files \
      --app-dir "$APP_DIR"
    ;;

  build)
    make -C "$APP_DIR" clean all
    ;;

  *)
    echo "Comando desconocido: $cmd"; echo
    usage; exit 1
    ;;
esac
