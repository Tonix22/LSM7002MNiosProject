#!/usr/bin/env bash
# mastersoc.sh — flujo por flags para Cyclone V / Nios II (Quartus 18.1/19.1)
# Ejemplos:
#   ./mastersoc.sh --quartus --program
#   ./mastersoc.sh --app-mk --build --download --terminal
#   ./mastersoc.sh --all
#   ./mastersoc.sh --download --force --device 2 --cable "DE-SoC [1-12]"
set -euo pipefail

### === Defaults (ajusta si quieres) ===
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJ_DIR="${PROJ_DIR:-$SCRIPT_DIR}"
PROJ_NAME="${PROJ_NAME:-MasterSoC_DE1SoC}"
BSP_DIR="${BSP_DIR:-MasterSoC_Controller/software/MasterSoC_3pAIP_v1_bsp}"
APP_DIR="${APP_DIR:-MasterSoC_Controller/software/MasterSoC_3pAIP_v1}"
ELF_NAME="${ELF_NAME:-app.elf}"

# JTAG / selección de dispositivo
CABLE="${CABLE:-}"           # e.g. "DE-SoC [1-12]"  (vacío = primero)
DEVICE="${DEVICE:-2}"        # posición del FPGA/CPU en la cadena (HPS suele ser 1, FPGA 2)
CPU_NAME="${CPU_NAME:-}"     # opcional: instancia Nios, p.ej. nios2_gen2_0

# Ruta Nios shell (se intentan 19.1 y 18.1 automáticamente si no está cargada)
NIOS_SHELL_19="${HOME}/intelFPGA_lite/19.1/nios2eds/nios2_command_shell.sh"
NIOS_SHELL_18="${HOME}/intelFPGA_lite/18.1/nios2eds/nios2_command_shell.sh"

# Flags
DO_QUARTUS=0
DO_PROGRAM=0
DO_APP_MK=0
DO_BUILD=0
DO_DOWNLOAD=0
DO_FORCE=0
DO_TERMINAL=0
DO_GDB=0
DO_ALL=0

usage() {
  cat <<EOF
Uso: $0 [flags]

Etapas:
  --quartus        Ejecuta síntesis+fit+asm (quartus_sh --flow compile)
  --program        Programa el .sof en la FPGA (usa @<DEVICE>)
  --app-mk         Genera/actualiza Makefile de la APP Nios II
  --build          Compila la APP (evitando rebuild del BSP)
  --download       Descarga y arranca el ELF en RAM
  --force          Si hay System ID mismatch, reintenta con --sidp/--id/--timestamp desde system.h
  --terminal       Abre nios2-terminal (JTAG UART)
  --gdb            Debug: lanza --gdb-stub y abre nios2-elf-gdb
  --all            quartus + program + app-mk + build + download + terminal

Opciones:
  --proj-dir <dir>     Raíz del proyecto   (default: $PROJ_DIR)
  --proj-name <name>   Nombre del proyecto (default: $PROJ_NAME)
  --bsp-dir <dir>      Dir BSP             (default: $BSP_DIR)
  --app-dir <dir>      Dir APP             (default: $APP_DIR)
  --elf-name <name>    Nombre ELF          (default: $ELF_NAME)
  --cable <name>       Nombre del cable JTAG (de 'jtagconfig')
  --device <N>         Índice del TAP (default: $DEVICE)
  --cpu-name <name>    Instancia Nios (opcional)
  --help               Muestra esta ayuda

Tips:
  - Ejecuta dentro de la Nios II shell, o deja que el script la cargue automáticamente.
  - Si tu diseño cambió y no tienes .sopcinfo, el script evitará regenerar el BSP y solo compilará la APP.
EOF
}

# Parseo simple de flags
while [[ $# -gt 0 ]]; do
  case "$1" in
    --quartus) DO_QUARTUS=1; shift;;
    --program) DO_PROGRAM=1; shift;;
    --app-mk) DO_APP_MK=1; shift;;
    --build) DO_BUILD=1; shift;;
    --download) DO_DOWNLOAD=1; shift;;
    --force) DO_FORCE=1; shift;;
    --terminal) DO_TERMINAL=1; shift;;
    --gdb) DO_GDB=1; shift;;
    --all) DO_ALL=1; shift;;
    --proj-dir) PROJ_DIR="$2"; shift 2;;
    --proj-name) PROJ_NAME="$2"; shift 2;;
    --bsp-dir) BSP_DIR="$2"; shift 2;;
    --app-dir) APP_DIR="$2"; shift 2;;
    --elf-name) ELF_NAME="$2"; shift 2;;
    --cable) CABLE="$2"; shift 2;;
    --device) DEVICE="$2"; shift 2;;
    --cpu-name) CPU_NAME="$2"; shift 2;;
    --help|-h) usage; exit 0;;
    *) echo "Flag desconocida: $1"; usage; exit 1;;
  esac
done

# Si --all, activa etapas principales
if [[ $DO_ALL -eq 1 ]]; then
  DO_QUARTUS=1
  DO_PROGRAM=1
  DO_APP_MK=1
  DO_BUILD=1
  DO_DOWNLOAD=1
  DO_TERMINAL=1
fi

# Funciones auxiliares
ensure_nios_env() {
  if command -v nios2-download >/dev/null 2>&1; then return 0; fi
  # intenta 19.1 y luego 18.1
  [[ -f "$NIOS_SHELL_19" ]] && source "$NIOS_SHELL_19" || true
  if ! command -v nios2-download >/dev/null 2>&1; then
    [[ -f "$NIOS_SHELL_18" ]] && source "$NIOS_SHELL_18" || true
  fi
  if ! command -v nios2-download >/dev/null 2>&1; then
    echo "[ERROR] No encuentro herramientas Nios II. Carga la shell:"
    echo "  source $NIOS_SHELL_18   # o la de 19.1 si la tienes"
    exit 1
  fi
}

quartus_compile() {
  echo "[QUARTUS] Compilando $PROJ_NAME en $PROJ_DIR"
  (cd "$PROJ_DIR" && quartus_sh --flow compile "$PROJ_NAME")
}

program_sof() {
  local sof="$PROJ_DIR/output_files/${PROJ_NAME}.sof"
  [[ -f "$sof" ]] || { echo "[ERROR] No encuentro $sof"; exit 2; }
  local atidx="@${DEVICE}"
  local cableopt=()
  [[ -n "$CABLE" ]] && cableopt=(-c "$CABLE")
  echo "[PGM] quartus_pgm ${cableopt[*]} -m jtag -o P;${sof}${atidx}"
  quartus_pgm "${cableopt[@]}" -m jtag -o "P;${sof}${atidx}"
}

app_makefiles() {
  ensure_nios_env
  local app="$PROJ_DIR/$APP_DIR"
  local bsp="$PROJ_DIR/$BSP_DIR"
  echo "[APP-MK] Generando/actualizando Makefile en $app (BSP: $bsp)"
  if command -v nios2-app-generate-makefile >/dev/null 2>&1; then
    nios2-app-generate-makefile --app-dir "$app" --bsp-dir "$bsp" --src-dir "$app" --elf-name "$ELF_NAME"
  elif command -v nios2-app-update-makefiles >/dev/null 2>&1; then
    nios2-app-update-makefiles --app-dir "$app" --bsp-dir "$bsp" --src-dir "$app" --elf-name "$ELF_NAME"
  else
    echo "[WARN] No están nios2-app-generate-makefile ni nios2-app-update-makefiles. Continuo con Makefile existente."
  fi
}

build_app() {
  ensure_nios_env
  local app="$PROJ_DIR/$APP_DIR"
  echo "[BUILD] Compilando APP en $app (evitando rebuild del BSP)"
  if make -C "$app" clean all SKIP_BSP_BUILD=1; then
    return 0
  fi
  echo "[PATCH] Parcheando Makefile para saltar BSP…"
  cp -a "$app/Makefile" "$app/Makefile.bak"
  sed -i 's@^\s*make\s\+-C\s\+\.\./[^ ]\+_bsp@# &@' "$app/Makefile"
  grep -q "libhal_bsp.a" "$app/Makefile" || \
    sed -i "s@^\(LIBS *=.*\)\$@\1 $PROJ_DIR/$BSP_DIR/libhal_bsp.a@" "$app/Makefile"
  make -C "$app" clean all
}

download_elf() {
  ensure_nios_env
  local app="$PROJ_DIR/$APP_DIR"
  local elf="$app/$ELF_NAME"
  [[ -f "$elf" ]] || { echo "[ERROR] No existe ELF: $elf"; exit 3; }
  local devopt=(--device "$DEVICE")
  local cableopt=()
  local cpuopt=()
  [[ -n "$CABLE" ]] && cableopt=(--cable "$CABLE")
  [[ -n "$CPU_NAME" ]] && cpuopt=(--cpu-name "$CPU_NAME")

  echo "[DL] nios2-download -g ${cableopt[*]} ${devopt[*]} ${cpuopt[*]} $elf"
  if nios2-download -g "$elf" "${cableopt[@]}" "${devopt[@]}" "${cpuopt[@]}"; then
    return 0
  fi

  if [[ $DO_FORCE -eq 1 ]]; then
    local sys_h="$PROJ_DIR/$BSP_DIR/public/system.h"
    [[ -f "$sys_h" ]] || { echo "[ERROR] No hallo $sys_h para forzar System ID"; exit 4; }
    local SIDP ID TS
    SIDP=$(awk '/#define[[:space:]]+SYSTEM_ID_BASE/{print $3}' "$sys_h")
    ID=$(awk   '/#define[[:space:]]+SYSTEM_ID_ID/{print $3}'   "$sys_h")
    TS=$(awk   '/#define[[:space:]]+SYSTEM_ID_TIMESTAMP/{print $3}' "$sys_h")
    echo "[FORCE] Reintentando con --sidp=$SIDP --id=$ID --timestamp=$TS"
    nios2-download -g "$elf" "${cableopt[@]}" "${devopt[@]}" "${cpuopt[@]}" --sidp "$SIDP" --id "$ID" --timestamp "$TS"
  else
    echo "[HINT] Usa --force para ignorar System ID mismatch leyendo system.h del BSP."
    exit 5
  fi
}

open_terminal() {
  ensure_nios_env
  local devopt=(--device "$DEVICE")
  local cableopt=()
  local cpuopt=()
  [[ -n "$CABLE" ]] && cableopt=(--cable "$CABLE")
  [[ -n "$CPU_NAME" ]] && cpuopt=(--cpu-name "$CPU_NAME")
  echo "[TERM] nios2-terminal ${cableopt[*]} ${devopt[*]} ${cpuopt[*]}"
  nios2-terminal "${cableopt[@]}" "${devopt[@]}" "${cpuopt[@]}"
}

run_gdb() {
  ensure_nios_env
  local app="$PROJ_DIR/$APP_DIR"
  local elf="$app/$ELF_NAME"
  [[ -f "$elf" ]] || { echo "[ERROR] No existe ELF: $elf"; exit 6; }
  local devopt=(--device "$DEVICE")
  local cableopt=()
  local cpuopt=()
  [[ -n "$CABLE" ]] && cableopt=(--cable "$CABLE")
  [[ -n "$CPU_NAME" ]] && cpuopt=(--cpu-name "$CPU_NAME")

  echo "[GDB] Lanzando stub en :3333 y conectando gdb…"
  nios2-download "$elf" "${cableopt[@]}" "${devopt[@]}" "${cpuopt[@]}" --gdb-stub &
  sleep 1
  nios2-elf-gdb "$elf" \
    -ex "target remote :3333" \
    -ex "monitor reset halt" \
    -ex "load" \
    -ex "b main" \
    -ex "c"
}

# === Ejecutar según flags ===
cd "$PROJ_DIR"

if [[ $DO_QUARTUS -eq 1 ]]; then quartus_compile; fi
if [[ $DO_PROGRAM -eq 1 ]]; then program_sof; fi
if [[ $DO_APP_MK -eq 1 ]]; then app_makefiles; fi
if [[ $DO_BUILD -eq 1 ]]; then build_app; fi
if [[ $DO_DOWNLOAD -eq 1 ]]; then download_elf; fi
if [[ $DO_TERMINAL -eq 1 ]]; then open_terminal; fi
if [[ $DO_GDB -eq 1 ]]; then run_gdb; fi

if [[ $DO_QUARTUS$DO_PROGRAM$DO_APP_MK$DO_BUILD$DO_DOWNLOAD$DO_TERMINAL$DO_GDB == 0000000 ]]; then
  usage
fi
