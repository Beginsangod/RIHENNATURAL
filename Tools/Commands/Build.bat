@echo off
setlocal enabledelayedexpansion

:: Verifier si l'argument principal est "build"
if /i "%1" neq "build" (
    echo Usage: %0 build [compilateur] [debug^|release] [clean]
    exit /b 1
)

:: Compilateur par defaut clang++, ou passe en second argument
set "COMPILER=%~2"
if "%COMPILER%"=="" set "COMPILER=clang++"

:: Mode par defaut release, ou passe en troisieme argument
set "MODE=%~3"
if /i "!MODE!"=="debug" (
    set "CFLAGS=-g -O0 -std=c++20"
) else (
    set "CFLAGS=-O2 -std=c++20"
    set "MODE=release"
)

:: Annoncer le compilateur et le mode
echo Building with %COMPILER% in %MODE% mode...

:: Option clean pour forcer recompilation complete
if /i "%4"=="clean" (
    if exist ".\Build\Obj" rmdir /s /q ".\Build\Obj" & mkdir ".\Build\Obj"
    echo Mode clean active: all .o files deleted.
)

:: Chemins pour SDL3 (adaptez si MSYS2 est ailleurs)
set "INCLUDE_DIR=C:\msys64\ucrt64\include\SDL3"
set "LIB_DIR=C:\msys64\ucrt64\lib"

:: Dossiers
set "SRC_DIR=.\Sources"
set "OBJ_DIR=.\Build\Obj"
set "BIN_DIR=.\Build\Bin"
set "RES_DIR=.\Resources"

:: Nom de l'executable de sortie
set "OUT=RihenNatural.exe"

:: Verifier l'existence du dossier Sources
if not exist "%SRC_DIR%" (
    echo Error: Directory %SRC_DIR% does not exist.
    exit /b 1
)

:: Creer les dossiers si inexistants
if not exist "%OBJ_DIR%" mkdir "%OBJ_DIR%"
if not exist "%BIN_DIR%" mkdir "%BIN_DIR%"

:: Verifier s'il y a des fichiers .cpp (recursivement)
set "CPP_COUNT=0"
for /r "%SRC_DIR%" %%f in (*.cpp) do (
    set /a CPP_COUNT+=1
)
if !CPP_COUNT! equ 0 (
    echo Error: No .cpp files found in %SRC_DIR% or its subdirectories.
    exit /b 1
)

:: Compiler les .cpp modifies uniquement (recursivement)
set "OBJ_FILES="
for /r "%SRC_DIR%" %%f in (*.cpp) do (
    set "cpp_full=%%~ff"
    set "base=%%~nf"
    set "o=%OBJ_DIR%\!base!.o"
    set "compile=no"

    call :FileModTime "!cpp_full!" cpp_date
    call :FileModTime "!o!" o_date
    
    :: Si le fichier objet n'existe pas, compiler
    if "!o_date!"=="" (
        set "compile=yes"
    ) else (
        :: Si le .cpp est plus recent, compiler
        if "!cpp_date!" GTR "!o_date!" (
            set "compile=yes"
        ) else (
            :: Verifier si les fichiers .h inclus sont plus recents (version simplifiee)
            call :CheckHeadersSimple "!cpp_full!" "!o!" compile
        )
    )

    if "!compile!"=="yes" (
        echo Compiling !base!.cpp
        "%COMPILER%" -c "!cpp_full!" -I "%SRC_DIR%" -I "%INCLUDE_DIR%" !CFLAGS! -o "!o!"
        if errorlevel 1 (
            echo Error compiling !base!.cpp
            exit /b 1
        )
    ) else (
        echo Ignoring !base!.cpp
    )
    set "OBJ_FILES=!OBJ_FILES! "!o!""
)

:: Verifier s'il y a des fichiers objets
if "!OBJ_FILES!"=="" (
    echo Error: No object files generated for linking. Check files in %SRC_DIR%.
    exit /b 1
)

:: Lier tous les .o en executable
echo Linking...
"%COMPILER%" !OBJ_FILES! -L "%LIB_DIR%" -lSDL3 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lversion -luuid -ladvapi32 -lshell32 !CFLAGS! -o "%BIN_DIR%\%OUT%"
if errorlevel 1 (
    echo Error during linking
    exit /b 1
)

:: Copier les ressources
if exist "%RES_DIR%" (
    echo Copying resources to %BIN_DIR%...
    xcopy /E /I /Y "%RES_DIR%\*" "%BIN_DIR%\" >nul || (
        echo Error copying resources
        exit /b 1
    )
) else (
    echo Warning: Directory %RES_DIR% does not exist, no resources copied.
)

echo Build completed: %BIN_DIR%\%OUT%

goto :EOF

:CheckHeadersSimple CppFile ObjFile ResultVar
:: Version simplifiee qui verifie tous les fichiers .h dans le dossier Sources
setlocal
set "needs_recompile=no"

call :FileModTime "%~2" obj_date

:: Verifier tous les fichiers .h dans Sources
for /r "%SRC_DIR%" %%h in (*.h) do (
    call :FileModTime "%%h" header_date
    if "!header_date!" GTR "!obj_date!" (
        set "needs_recompile=yes"
        goto :HeaderCheckDone
    )
)

:HeaderCheckDone
endlocal & set "%~3=%needs_recompile%"
exit /b

:FileModTime  File  DateVar
:: Utilise dir pour obtenir la date de modification dans un format comparable
setlocal
set "ts="
if not exist "%~1" (
    set "ts="
) else (
    for /f "tokens=1-2" %%A in ('dir "%~1" /t:w 2^>nul ^| findstr /r /c:"%~nx1$"') do (
        set "date=%%A"
        set "time=%%B"
    )
    if defined date (
        :: Reformater la date pour comparaison lexicographique (YYYYMMDDHHMMSS)
        for /f "tokens=1-3 delims=/" %%X in ("!date!") do (
            set "day=%%X"
            set "month=%%Y"
            set "year=%%Z"
        )
        for /f "tokens=1-2 delims=:." %%X in ("!time!") do (
            set "hour=%%X"
            set "minute=%%Y"
        )
        :: Completer avec des zeros pour un format fixe
        set "day=00!day!" & set "day=!day:~-2!"
        set "month=00!month!" & set "month=!month:~-2!"
        set "hour=00!hour!" & set "hour=!hour:~-2!"
        set "minute=00!minute!" & set "minute=!minute:~-2!"
        set "ts=!year!!month!!day!!hour!!minute!"
    )
)
endlocal & set "%~2=%ts%"
exit /b