REMOTE_ARCHIVE=https://www.meetixos.org/Repository/Ports/cairo-1.16.0.tar.xz
UNPACKED_DIR=cairo-1.16.0
ARCHIVE=cairo-1.16.0.tar.xz

port_unpack() {
    tar -xf ${ARCHIVE}
}

port_build() {
    PKG_CONFIG=meetix-pkg-config.sh                                                                               \
    CPPFLAGS="$MEETIX_EXTRA_INCLUDE_PATHS -I$TOOLCHAIN_ROOT/include/pixman-1 -I$TOOLCHAIN_ROOT/include/freetype2" \
    CFLAGS="-DCAIRO_NO_MUTEX=1 -Wno-packed $MEETIX_EXTRA_OPTIONS"                                                 \
    LDFLAGS="$MEETIX_EXTRA_LINK_DIRS"                                                                             \
    LIBS="$MEETIX_EXTRA_LIBS"                                                                                     \
        ../$UNPACKED_DIR/configure --disable-shared                                                               \
                                   --host=i686-pc-meetix                                                          \
                                   --prefix="$TOOLCHAIN_ROOT"                                                     \
                                   --enable-xlib=no || exit 1

    make -j$BUILD_JOBS install || exit 1
}
