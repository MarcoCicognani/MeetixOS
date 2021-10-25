REMOTE_ARCHIVE=https://www.meetixos.org/Repository/Ports/pixman-0.40.0.tar.gz
UNPACKED_DIR=pixman-0.40.0
ARCHIVE=pixman-0.40.0.tar.gz

port_unpack() {
    tar -xf $ARCHIVE
}

port_build() {
    PKG_CONFIG=meetix-pkg-config.sh \
    LDFLAGS="-L$MEETIX_LIBC_DIR"    \
    LIBS="$MEETIX_EXTRA_LIBS"       \
        ../$UNPACKED_DIR/configure --host=i686-pc-meetix --prefix="$TOOLCHAIN_ROOT" || exit 1

    make -j$BUILD_JOBS install || exit 1

    ln -fs "$TOOLCHAIN_ROOT/include/pixman-1" "$SOURCE_DIR/Libs/Headers/pixman-1"
}
