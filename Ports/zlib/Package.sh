REMOTE_ARCHIVE=https://www.meetixos.org/libs/ports/zlib-1.2.8.tar.gz
UNPACKED_DIR=zlib-1.2.8
ARCHIVE=zlib-1.2.8.tar.gz
BUILD_IN_SOURCE_DIR=1

port_unpack() {
    tar -xf $ARCHIVE
}

port_build() {
    CC=i686-pc-meetix-gcc AR=i686-pc-meetix-ar \
        ../$UNPACKED_DIR/configure --static --prefix="$TOOLCHAIN_ROOT" || exit 1

    make -j$BUILD_JOBS || exit 1
    make -j$BUILD_JOBS DESTDIR="$TOOLCHAIN_ROOT/lib" install || exit 1
}
