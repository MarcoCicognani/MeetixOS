REMOTE_ARCHIVE=https://www.meetixos.org/Repository/Ports/freetype-2.11.0.tar.gz
UNPACKED_DIR=freetype-2.11.0
ARCHIVE=freetype-2.11.0.tar.gz

port_unpack() {
    tar -xf ${ARCHIVE}
}

port_build() {
    PKG_CONFIG="meetix-pkg-config.sh" \
    LDFLAGS="-L$MEETIX_LIBC_DIR"      \
    LIBS="$MEETIX_EXTRA_LIBS"         \
        ../$UNPACKED_DIR/configure --host=i686-pc-meetix --prefix="$TOOLCHAIN_ROOT" --with-harfbuzz=no || exit 1

    make -j$BUILD_JOBS         || exit 1 # make all is necessary or install fails...mmh
    make -j$BUILD_JOBS install || exit 1

    ln -sf "$TOOLCHAIN_ROOT/include/freetype2" "$SOURCE_DIR/Libs/Headers/freetype2"
}
