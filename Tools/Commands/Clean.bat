@echo off
setlocal

set BUILD_DIR=.\Build

echo Cleaning build files...

if exist "%BUILD_DIR%" (
    rmdir /s /q "%BUILD_DIR%"
    echo Removed %BUILD_DIR%
)

echo Clean completed.

endlocal