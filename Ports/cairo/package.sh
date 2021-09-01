# variables
REMOTE_ARCHIVE="https://www.meetixos.org/libs/ports/cairo-1.12.18.tar.xz"
UNPACKED_DIR=cairo-1.12.18
ARCHIVE=cairo-1.12.18.tar.xz

##
# definition for portUnpack needed by build script
##
function portUnpack()
{
	tar -xf $ARCHIVE
}

##
# definition for portInstall needed by build script
##
function portInstall()
{
	export CFLAGS="-DCAIRO_NO_MUTEX=1"
	export PKG_CONFIG=meetix-pkg-config.sh

	../$UNPACKED_DIR/configure --disable-shared --host=$HOST --prefix=$PREFIX --enable-xlib=no
	make -j8
	make DESTDIR=$SYSROOT install
}