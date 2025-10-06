#!/bin/bash

# Vérifier si l'argument principal est "build"
if [ "$1" != "build" ]; then
    echo "Usage: $0 build [compiler] [debug|release] [clean]"
    exit 1
fi

# Compilateur par défaut clang++, ou passé en second argument
COMPILER="${2:-clang++}"

# Mode par défaut release, ou passé en troisième argument
MODE="${3:-release}"
if [ "$MODE" = "debug" ]; then
    CFLAGS="-g -O0 -std=c++20"
else
    CFLAGS="-O2 -std=c++20"
    MODE="release"
fi

# Annoncer le compilateur et le mode
echo "Building with $COMPILER in $MODE mode..."

# Option clean pour forcer recompilation complète
if [ "$4" = "clean" ]; then
    if [ -d "./Build/Obj" ]; then
        rm -rf "./Build/Obj"
        mkdir -p "./Build/Obj"
    fi
    echo "Mode clean active: all .o files deleted."
fi

# Chemins pour SDL3 (adaptez selon votre système)
INCLUDE_DIR="/usr/local/include/SDL3"
LIB_DIR="/usr/local/lib"

# Dossiers
SRC_DIR="./Sources"
OBJ_DIR="./Build/Obj"
BIN_DIR="./Build/Bin"
RES_DIR="./Resources"

# Nom de l'exécutable de sortie
OUT="RihenNatural"

# Vérifier l'existence du dossier Sources
if [ ! -d "$SRC_DIR" ]; then
    echo "Error: Directory $SRC_DIR does not exist."
    exit 1
fi

# Créer les dossiers si inexistants
mkdir -p "$OBJ_DIR"
mkdir -p "$BIN_DIR"

# Vérifier s'il y a des fichiers .cpp (récursivement)
CPP_COUNT=$(find "$SRC_DIR" -type f -name "*.cpp" | wc -l)
if [ "$CPP_COUNT" -eq 0 ]; then
    echo "Error: No .cpp files found in $SRC_DIR or its subdirectories."
    exit 1
fi

# Compiler les .cpp modifiés uniquement (récursivement)
OBJ_FILES=""
for cpp_file in $(find "$SRC_DIR" -type f -name "*.cpp"); do
    base=$(basename "$cpp_file" .cpp)
    o_file="$OBJ_DIR/$base.o"
    compile="no"

    # Obtenir les timestamps
    if [ -f "$cpp_file" ]; then
        cpp_date=$(stat -f "%Sm" -t "%Y%m%d%H%M%S" "$cpp_file" 2>/dev/null || stat -c "%Y" "$cpp_file" 2>/dev/null)
    else
        cpp_date=""
    fi
    if [ -f "$o_file" ]; then
        o_date=$(stat -f "%Sm" -t "%Y%m%d%H%M%S" "$o_file" 2>/dev/null || stat -c "%Y" "$o_file" 2>/dev/null)
    else
        o_date=""
    fi

    if [ -z "$o_date" ]; then
        compile="yes"
    elif [ -n "$cpp_date" ] && [ "$cpp_date" -gt "$o_date" ]; then
        compile="yes"
    fi

    if [ "$compile" = "yes" ]; then
        echo "Compiling $base.cpp"
        $COMPILER -c "$cpp_file" -I "$SRC_DIR" -I "$INCLUDE_DIR" $CFLAGS -o "$o_file"
        if [ $? -ne 0 ]; then
            echo "Error compiling $base.cpp"
            exit 1
        fi
    else
        echo "Ignoring $base.cpp"
    fi
    OBJ_FILES="$OBJ_FILES $o_file"
done

# Vérifier s'il y a des fichiers objets
if [ -z "$OBJ_FILES" ]; then
    echo "Error: No object files generated for linking. Check files in $SRC_DIR."
    exit 1
fi

# Lier tous les .o en exécutable
echo "Linking..."
$COMPILER $OBJ_FILES -L "$LIB_DIR" -lSDL3 $CFLAGS -o "$BIN_DIR/$OUT"
if [ $? -ne 0 ]; then
    echo "Error during linking"
    exit 1
fi

# Copier les ressources
if [ -d "$RES_DIR" ]; then
    echo "Copying resources to $BIN_DIR..."
    cp -r "$RES_DIR/"* "$BIN_DIR/" 2>/dev/null || {
        echo "Error copying resources"
        exit 1
    }
else
    echo "Warning: Directory $RES_DIR does not exist, no resources copied."
fi

echo "Build completed: $BIN_DIR/$OUT"