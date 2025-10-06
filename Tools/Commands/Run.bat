@echo off
setlocal

set BUILD_DIR=.\Build
set BIN_DIR=!BUILD_DIR!\Bin
set OUTPUT_FILE=RihenNatural.exe

if not exist "%BIN_DIR%\%OUTPUT_FILE%" (
    echo Error: Executable not found. Run 'nken build' first.
    exit /b 1
)

echo Running RihenNatural...
cd "%BIN_DIR%"
".\%OUTPUT_FILE%"
cd ..\..

endlocal