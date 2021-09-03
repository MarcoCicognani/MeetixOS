REMOTE_ARCHIVE="https://www.meetixos.org/libs/ports/libpng-1.6.18.tar.gz"
UNPACKED_DIR=libpng-1.6.18
ARCHIVE=libpng-1.6.18.tar.gz

port_unpack() {
    tar -xf ${ARCHIVE}
}

port_build() {
    ../$UNPACKED_DIR/configure --host=i686-pc-meetix --prefix="$TOOLCHAIN_ROOT" || exit 1
    make -j$BUILD_JOBS || exit 1
    make -j$BUILD_JOBS DESTDIR="$TOOLCHAIN_ROOT/lib" install || exit 1
}
