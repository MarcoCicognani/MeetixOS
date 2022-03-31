REMOTE_ARCHIVE=https://www.meetixos.org/Repository/Ports/libpng-1.6.37.tar.gz
UNPACKED_DIR=libpng-1.6.37
ARCHIVE=libpng-1.6.37.tar.gz

port_unpack() {
    tar -xf ${ARCHIVE}
}

port_build() {
    CPPFLAGS="$MEETIX_EXTRA_INCLUDE_PATHS" \
    CFLAGS="$MEETIX_EXTRA_OPTIONS"         \
    LDFLAGS="$MEETIX_EXTRA_LINK_DIRS"      \
    LIBS="$MEETIX_EXTRA_LIBS"              \
        ../$UNPACKED_DIR/configure --host=i686-pc-meetix --prefix="$TOOLCHAIN_ROOT"

    make -j$BUILD_JOBS install
}
