REMOTE_ARCHIVE="https://www.meetixos.org/libs/ports/cairo-1.12.18.tar.xz"
UNPACKED_DIR=cairo-1.12.18
ARCHIVE=cairo-1.12.18.tar.xz

port_unpack() {
    tar -xf ${ARCHIVE}
}

port_build() {
    CFLAGS=-DCAIRO_NO_MUTEX=1 PKG_CONFIG=meetix-pkg-config.sh \
        ../$UNPACKED_DIR/configure --disable-shared           \
                                   --host=i686-pc-meetix      \
                                   --prefix="$TOOLCHAIN_ROOT" \
                                   --enable-xlib=no || exit 1
    make -j$BUILD_JOBS || exit 1
    make -j$BUILD_JOBS DESTDIR="$TOOLCHAIN_ROOT/lib" install || exit 1
}
