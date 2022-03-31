REMOTE_ARCHIVE=https://www.meetixos.org/Repository/Ports/pixman-0.40.0.tar.gz
UNPACKED_DIR=pixman-0.40.0
ARCHIVE=pixman-0.40.0.tar.gz

port_unpack() {
    tar -xf $ARCHIVE
}

port_build() {
    PKG_CONFIG=meetix-pkg-config.sh        \
    CPPFLAGS="$MEETIX_EXTRA_INCLUDE_PATHS" \
    LDFLAGS="$MEETIX_EXTRA_LINK_DIRS"      \
    LIBS="$MEETIX_EXTRA_LIBS"              \
        ../$UNPACKED_DIR/configure --host=i686-pc-meetix --prefix="$TOOLCHAIN_ROOT"

    make -j$BUILD_JOBS install
}
