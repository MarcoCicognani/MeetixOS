REMOTE_ARCHIVE=https://www.meetixos.org/Repository/Ports/zlib-1.2.11.tar.gz
UNPACKED_DIR=zlib-1.2.11
ARCHIVE=zlib-1.2.11.tar.gz
BUILD_IN_SOURCE_DIR=1

port_unpack() {
    tar -xf $ARCHIVE
}

port_build() {
    local TEST_LDFLAGS="-L$MEETIX_LIBC_DIR $MEETIX_EXTRA_LIBS -L. libz.a"

    CC=i686-pc-meetix-gcc \
    AR=i686-pc-meetix-ar  \
        ../$UNPACKED_DIR/configure --static --prefix="$TOOLCHAIN_ROOT" || exit 1

    make -j$BUILD_JOBS TEST_LDFLAGS="$TEST_LDFLAGS" install || exit 1
}
