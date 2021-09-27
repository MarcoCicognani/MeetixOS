REMOTE_ARCHIVE=https://www.meetixos.org/libs/ports/pixman-0.32.6.tar.gz
UNPACKED_DIR=pixman-0.32.6
ARCHIVE=pixman-0.32.6.tar.gz

port_unpack() {
    tar -xf $ARCHIVE
}

port_build() {
    PKG_CONFIG=meetix-pkg-config.sh \
        ../$UNPACKED_DIR/configure --host=i686-pc-meetix --prefix="$TOOLCHAIN_ROOT" || exit 1

    make -j$BUILD_JOBS || exit 1
    make -j$BUILD_JOBS install || exit 1

    ln -fs "$TOOLCHAIN_ROOT/include/pixman-1" "$SOURCE_DIR/Libs/Headers/pixman-1"
}
