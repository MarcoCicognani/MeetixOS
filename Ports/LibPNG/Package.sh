REMOTE_ARCHIVE=https://www.meetixos.org/libs/ports/libpng-1.6.18.tar.gz
UNPACKED_DIR=libpng-1.6.18
ARCHIVE=libpng-1.6.18.tar.gz

port_unpack() {
    tar -xf ${ARCHIVE}
}

port_build() {
    LDFLAGS="-L$MEETIX_LIBC_DIR" \
    LIBS="$MEETIX_EXTRA_LIBS"    \
        ../$UNPACKED_DIR/configure --host=i686-pc-meetix --prefix="$TOOLCHAIN_ROOT" || exit 1

    make -j$BUILD_JOBS install || exit 1

    ln -fs "$TOOLCHAIN_ROOT/include/png.h" "$SOURCE_DIR/Libs/Headers/png.h"
    ln -fs "$TOOLCHAIN_ROOT/include/pngconf.h" "$SOURCE_DIR/Libs/Headers/pngconf.h"
    ln -fs "$TOOLCHAIN_ROOT/include/pnglibconf.h" "$SOURCE_DIR/Libs/Headers/pnglibconf.h"
}
