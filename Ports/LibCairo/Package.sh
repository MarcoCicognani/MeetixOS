REMOTE_ARCHIVE=https://www.meetixos.org/Repository/Ports/cairo-1.16.0.tar.xz
UNPACKED_DIR=cairo-1.16.0
ARCHIVE=cairo-1.16.0.tar.xz

port_unpack() {
    tar -xf ${ARCHIVE}
}

port_build() {
    local CUSTOM_INCLUDES="-I$SOURCE_DIR/Libs/Headers/pixman-1 -I$SOURCE_DIR/Libs/Headers/freetype2"

    PKG_CONFIG=meetix-pkg-config.sh              \
    LDFLAGS="-L$MEETIX_LIBC_DIR"                 \
    LIBS="$MEETIX_EXTRA_LIBS"                    \
    CFLAGS="-DCAIRO_NO_MUTEX=1 $CUSTOM_INCLUDES" \
        ../$UNPACKED_DIR/configure --disable-shared           \
                                   --host=i686-pc-meetix      \
                                   --prefix="$TOOLCHAIN_ROOT" \
                                   --enable-xlib=no || exit 1

    make -j$BUILD_JOBS install || exit 1

    ln -fs "$TOOLCHAIN_ROOT/include/cairo" "$SOURCE_DIR/Libs/Headers/cairo"
}
