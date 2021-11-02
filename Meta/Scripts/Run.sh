#!/bin/bash

for CMD_ARG in "$@"; do
    case $CMD_ARG in
        --iso-dir=*)
            ISO_DIR=${CMD_ARG#*=}
            if [ ! -d "$ISO_DIR" ]; then
                echo "$$ISO_DIR seems to be un-existing, be sure to build for this target"
                exit 1
            fi
            ;;
        *)
            echo "Run MeetixOS ISO image script"
            echo "USAGE:"
            echo "    Run.sh --iso-dir=/Path/To/OutputDir"
            exit 0
            ;;
    esac
done

# run the iso image
qemu-system-i386 -enable-kvm -m 512M -cpu host -smp 4 -serial stdio -cdrom "$ISO_DIR/MeetixOS.iso" || exit 1