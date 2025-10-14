#!/usr/bin/env bash
# nios_build.sh — simple y directo

# AJUSTA ESTAS RUTAS SI LO NECESITAS:
BSP_DIR="${BSP_DIR:-/home/tonix/Documents/MasterSoC_DE1SoC/MasterSoC_Controller/software/MasterSoC_3pAIP_v1_bsp}"
APP_DIR="${APP_DIR:-/home/tonix/Documents/MasterSoC_DE1SoC/MasterSoC_Controller/software/MasterSoC_3pAIP_v1}"
ELF_NAME="${ELF_NAME:-app.elf}"

cmd="$1"

if [[ -z "$cmd" ]]; then
  echo "Uso: $0 {bsp|mk|list|build}"
  echo "  bsp   : nios2-bsp-generate-files (desde settings.bsp)"
  echo "  mk    : nios2-app-update-makefile (genera/actualiza Makefile)"
  echo "  list  : lista fuentes detectadas por la app"
  echo "  build : make (compila la app)"
  exit 1
fi

case "$cmd" in
  bsp)
    # Regenera archivos del BSP a partir de settings.bsp
    nios2-bsp-generate-files \
      --settings "${BSP_DIR}/settings.bsp" \
      --bsp-dir "${BSP_DIR}"
    ;;

  mk)
    # Genera/actualiza Makefile de la app usando el BSP existente
    nios2-app-update-makefile \
      --app-dir "${APP_DIR}" \
      --bsp-dir "${BSP_DIR}" \
      --src-dir "${APP_DIR}" \
      --elf-name "${ELF_NAME}"
    ;;

  list)
    # Lista los .c que la herramienta ve para la app
    nios2-app-update-makefile \
      --list-src-files \
      --app-dir "${APP_DIR}"
    ;;

  build)
    # Compila la app con el Makefile actual
    make -C "${APP_DIR}" clean all
    ;;

  *)
    echo "Comando desconocido: $cmd (usa: bsp|mk|list|build)"
    exit 1
    ;;
esac
