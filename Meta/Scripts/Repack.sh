#!/bin/bash

for CMD_ARG in "$@"; do
    case $CMD_ARG in
        --out-dir=*)
            OUT_DIR=${CMD_ARG#*=}
            if [ ! -d "$OUT_DIR" ]; then
                echo "out-dir seems to be un-existing, be sure to build for this target"
                exit 1
            fi
            ;;
        *)
            echo "Bootable image build script"
            echo "USAGE:"
            echo "    Repack.sh --out-dir=/Path/To/OutputDir"
            exit 0
            ;;
    esac
done

echo DONE