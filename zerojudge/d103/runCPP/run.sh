#!/bin/bash

######### Requirement ##########

# xclip required.
# don't have xclip or don't have one of the graphical interface.
if ! command -v xclip >/dev/null 2>&1 || { [ -z "$DISPLAY" ] && [ -z "$WAYLAND_DISPLAY" ]; }; then
    echo "xclip is not installed or there is no graphical interface, the code will not be copied to the clipboard."
fi

if [[ -z "$1" ]]; then
  echo "Error: A input file is needed." >&2
  exit 1
fi

######### Basic Setting ##########

scriptName="$(basename "$(realpath "${BASH_SOURCE[0]}")")"
scriptDir="$(dirname "$(realpath $0)")"
cd $scriptDir


######### Module Script ##########

source $scriptDir/color.h.sh

########## Compile Options ##########

# 對於 gcc/g++ 的優化選項: 請查看 https://gcc.gnu.org/onlinedocs/gcc-10.2.0/gcc/Optimize-Options.html#Optimize-Options

# General
#OPT_FLAGS+=" -O3"                # 最佳化等級3，激進的優化
OPT_FLAGS+=" -Ofast"             # 啟動所有優化，無視嚴格的標準合規性
OPT_FLAGS+=" -funroll-loops"     # 展開迴圈來減少迴圈計數開銷，提高效能
OPT_FLAGS+=" -fprefetch-loop-arrays"    # 開啟陣列預取優化，在迴圈中預取陣列數據，提高記憶體訪問效率 (或更差，取決於程式)

# 分次編譯 (兩次 configure)
#OPT_FLAGS+=" -fprofile-generate"      # 執行一次程式以收集執行資料
#OPT_FLAGS+=" -fprofile-use=$(pwd)"   # 利用收集的資料進行優化


# CPU Specify
OPT_FLAGS+=" -march=native"      # 使用本地CPU架構指令集進行最佳化
OPT_FLAGS+=" -mtune=native"             # 調整編譯選項以最佳化當前CPU
OPT_FLAGS+=" -mfpmath=sse"              # 使用SSE指令集加速浮點運算（對於支持的CPU）
OPT_FLAGS+=" -mavx"                     # 開啟AVX指令集，適用於支援AVX的CPU，可顯著加速浮點數運算
OPT_FLAGS+=" -mavx2"                    # 開啟AVX2指令集，加速矢量運算
OPT_FLAGS+=" -mprefer-vector-width=256" # 優先使用256位的矢量寬度，提高矢量化效率

# LTO
#OPT_FLAGS+=" -fno-lto"                         # 禁用LTO，可用於排除LTO引起的潛在問題，例如: __gttf2@@GCC_4.3.0
# error example: https://aur.archlinux.org/pkgbase/gcc7
#OPT_FLAGS+=" -fwhole-program"                  # 啟用全局變數鏈接時間優化。This option should not be used in combination with -flto. Instead relying on a linker plugin should provide safer and more precise information.

OPT_FLAGS+=" -flto=auto"                        # the standard link-time optimizer
OPT_FLAGS+=" -fuse-linker-plugin"               # 使用鏈接器插件，能使LTO在鏈接階段進行更多優化。不可用時， -fwhole-program 應該使用。
#OPT_FLAGS+=" -flto-partition=one"              # 單一分區的LTO，有助於減少LTO的內存佔用

# Others
OPT_FLAGS+=" -fno-plt"           # 不使用程序鏈接表（PLT）對動態鏈接庫的函數進行直接調用，提高函數調用效率
OPT_FLAGS+=" -pipe"              # 使用管道加速編譯步驟間的數據傳遞

CC="gcc"
CXX="g++"
FC="gfortran"

########## Compile ##########

if [[ $1 == *".c" ]]; then
    compileCMD="$CC $OPT_FLAGS ../$1 -o $1.bin"
elif [[ $1 == *".cpp" ]]; then
    compileCMD="$CXX $OPT_FLAGS ../$1 -o $1.bin"
fi

# TRY
color yellow "$compileCMD"
$compileCMD || {
  # ERR
  color red "Error: Compile Error";
  exit 1;
}
# SUC
color green "Compile Success"



########## Execute ##########

# TRY
color yellow "./$1.bin <input.txt > output.txt"
TIME1=$(date +%s%3N)
./$1.bin <input.txt > output.txt || {
  # ERR
  color red "Error: Execute error.";
  exit 1;
}
TIME2=$(date +%s%3N)
# SUC
color green "Execute Success."

########## Analyze ##########

if [ ! -s answer.txt ]; then
  # 答案沒有東西，不使用答案模式
    color green "Time: $((TIME2 - TIME1)) ms"
    color green "Output."
    color green "----------"
    cat output.txt
else
  # 使用答案模式
  # -w忽略換行符
  diff -w output.txt answer.txt > /dev/null
  if [ $? -eq 0 ]; then
    color bright_green "Accept."
    color green "Time: $((TIME2 - TIME1)) ms"
    color green "Output."
    color green "----------"
    cat output.txt
    # User Input
    read -p "$(color bright_cyan 'Copy code to clipboard? (y/yes to confirm)')" confirmation

    if [[ "$confirmation" =~ ^(y|Y|yes|Yes)$ ]]; then
      xclip -sel clip ../$1
    fi
  else
    color red "Output."
    color red "----------"
    cat output.txt
    color red "Answer."
    color red "----------"
    cat answer.txt
  fi
fi
