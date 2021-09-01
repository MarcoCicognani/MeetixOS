# variables
REMOTE_ARCHIVE="https://www.meetixos.org/libs/ports/pixman-0.32.6.tar.gz"
UNPACKED_DIR=pixman-0.32.6
ARCHIVE=pixman-0.32.6.tar.gz

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
	export PKG_CONFIG=meetix-pkg-config.sh
	../$UNPACKED_DIR/configure --host=$HOST --prefix=$PREFIX
	make -j8
	make DESTDIR=$SYSROOT install
}