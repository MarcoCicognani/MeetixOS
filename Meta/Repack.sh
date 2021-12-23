#!/bin/bash

RAMDISK_WRITER="$TOOLCHAIN_BIN/RamdiskWriter"

for CMD_ARG in "$@"; do
    case $CMD_ARG in
        --out-dir=*)
            OUT_DIR=${CMD_ARG#*=}
            if [ ! -d "$OUT_DIR" ]; then
                echo "$$OUT_DIR seems to be un-existing, be sure to build for this target"
                exit 1
            fi
            ;;
        --iso-root=*)
            ISO_ROOT=${CMD_ARG#*=}
            if [ ! -d "$ISO_ROOT" ]; then
                echo "$$ISO_ROOT seems to be un-existing, be sure to build for this target"
                exit 1
            fi
            ;;
        --ramdisk-root=*)
            RAMDISK_ROOT=${CMD_ARG#*=}
            if [ ! -d "$RAMDISK_ROOT" ]; then
                echo "$$RAMDISK_ROOT seems to be un-existing, be sure to build for this target"
                exit 1
            fi
            ;;
        *)
            echo "Bootable image build script"
            echo "USAGE:"
            echo "    Repack.sh --out-dir=/Path/To/OutputDir --iso-root=/Path/To/ISO/To/Pack --ramdisk-root=/Path/To/Ramdisk/To/Pack"
            exit 0
            ;;
    esac
done

OUT_ISO_IMAGE="$OUT_DIR/MeetixOS.iso"

# Write the ramdisk image
$RAMDISK_WRITER "$RAMDISK_ROOT" "$ISO_ROOT/boot/Ramdisk.img" || exit 1

# write the ISO image
grub-mkrescue --compress=xz -d /usr/lib/grub/i386-pc -o "$OUT_ISO_IMAGE" "$ISO_ROOT" || exit 1