#!/bin/bash

ROOT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
TOOLS_DIR="$ROOT_DIR/Tools/Commands"

if [ $# -eq 0 ]; then
    echo "Usage: nken [command]"
    echo "Commands disponibles:"
    echo "  build    - Compile le projet"
    echo "  run      - Execute le programme"
    echo "  clean    - Nettoie les fichiers de compilation"
    echo "  help     - Affiche cette aide"
    exit 1
fi

COMMAND=$1
shift

case $COMMAND in
    "build")
        "$TOOLS_DIR/Build.sh" "$@"
        ;;
    "run")
        "$TOOLS_DIR/Run.sh" "$@"
        ;;
    "clean")
        "$TOOLS_DIR/Clean.sh" "$@"
        ;;
    "help")
        echo "Usage: nken [command]"
        echo "Commands disponibles:"
        echo "  build    - Compile le projet"
        echo "  run      - Execute le programme"
        echo "  clean    - Nettoie les fichiers de compilation"
        echo "  help     - Affiche cette aide"
        ;;
    *)
        echo "Commande inconnue: $COMMAND"
        echo "Utilisez 'nken help' pour voir les commandes disponibles."
        exit 1
        ;;
esac