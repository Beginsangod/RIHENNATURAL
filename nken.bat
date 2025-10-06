@echo off
setlocal enabledelayedexpansion

set ROOT_DIR=%~dp0
set TOOLS_DIR=%ROOT_DIR%Tools\Commands

if "%~1"=="" (
    echo Usage: nken [command]
    echo Commands disponibles:
    echo   build    - Compile le projet
    echo   run      - Execute le programme
    echo   clean    - Nettoie les fichiers de compilation
    echo   help     - Affiche cette aide
    goto :eof
)

set COMMAND=%~1
shift

if "%COMMAND%"=="build" (
    call "%TOOLS_DIR%\Build.bat" %*
) else if "%COMMAND%"=="run" (
    call "%TOOLS_DIR%\Run.bat" %*
) else if "%COMMAND%"=="clean" (
    call "%TOOLS_DIR%\Clean.bat" %*
) else if "%COMMAND%"=="help" (
    echo Usage: nken [command]
    echo Commands disponibles:
    echo   build    - Compile le projet
    echo   run      - Execute le programme
    echo   clean    - Nettoie les fichiers de compilation
    echo   help     - Affiche cette aide
) else (
    echo Commande inconnue: %COMMAND%
    echo Utilisez 'nken help' pour voir les commandes disponibles.
)

endlocal