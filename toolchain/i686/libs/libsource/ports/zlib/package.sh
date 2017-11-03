# variables
REMOTE_ARCHIVE="https://www.meetixos.org/libs/ports/zlib-1.2.8.tar.gz"
UNPACKED_DIR=zlib-1.2.8
ARCHIVE=zlib-1.2.8.tar.gz
REQUIRES_INSTALL_IN_SOURCE_DIR=1

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
	export CC=i686-mx-gcc
	export AR=i686-mx-ar
	../$UNPACKED_DIR/configure --static --prefix=$PREFIX
	make
	make DESTDIR=$SYSROOT install
}