REMOTE_ARCHIVE="https://www.meetixos.org/libs/ports/freetype-2.5.3.tar.gz"
UNPACKED_DIR=freetype-2.5.3
ARCHIVE=freetype-2.5.3.tar.gz

port_unpack() {
    tar -xf ${ARCHIVE}
}

port_build() {
    ../$UNPACKED_DIR/configure --host=i686-pc-meetix      \
                               --prefix="$TOOLCHAIN_ROOT" \
                               --with-png=no              \
                               --with-zlib=no             \
                               --with-harfbuzz=no || exit 1
    make -j$BUILD_JOBS || exit 1
    make -j$BUILD_JOBS DESTDIR="$TOOLCHAIN_ROOT/lib" install || exit 1
}
