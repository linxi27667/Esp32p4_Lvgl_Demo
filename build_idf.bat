@echo off
set "MSYSTEM="
set "IDF_PATH=E:\MCU\esp32\.espressif\v5.5.3\esp-idf"
set "IDF_PYTHON_ENV_PATH=C:\Users\30817\.espressif\python_env\idf5.5_py3.11_env"
set "ESP_ROM_ELF_DIR=C:\Espressif\tools\esp-rom-elfs\20241011\"
set "PATH=C:\Espressif\tools\riscv32-esp-elf\esp-14.2.0_20251107\riscv32-esp-elf\bin;C:\Espressif\tools\ninja\1.12.1;C:\Espressif\tools\cmake\3.30.2\bin;C:\Espressif\tools\idf-exe\1.0.3;C:\Espressif\tools\ccache\4.12.1;C:\Espressif\tools\dfu-util\0.11;C:\Espressif\tools\esp-rom-elfs\20241011;C:\Users\30817\.espressif\python_env\idf5.5_py3.11_env\Scripts;%PATH%"
cd /d E:\MCU\esp32\p4\lvgl_demo_v9
python "%IDF_PATH%\tools\idf.py" %*
