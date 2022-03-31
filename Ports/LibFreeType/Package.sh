REMOTE_ARCHIVE=https://www.meetixos.org/Repository/Ports/freetype-2.11.0.tar.gz
UNPACKED_DIR=freetype-2.11.0
ARCHIVE=freetype-2.11.0.tar.gz

port_unpack() {
    tar -xf ${ARCHIVE}
}

port_build() {
    PKG_CONFIG=meetix-pkg-config.sh        \
    CPPFLAGS="$MEETIX_EXTRA_INCLUDE_PATHS" \
    CFLAGS="$MEETIX_EXTRA_OPTIONS"         \
    LDFLAGS="$MEETIX_EXTRA_LINK_DIRS"      \
    LIBS="$MEETIX_EXTRA_LIBS"              \
        ../$UNPACKED_DIR/configure --host=i686-pc-meetix --prefix="$TOOLCHAIN_ROOT" --with-harfbuzz=no

    make -j$BUILD_JOBS # make all is necessary or install fails...mmh
    make -j$BUILD_JOBS install
}
