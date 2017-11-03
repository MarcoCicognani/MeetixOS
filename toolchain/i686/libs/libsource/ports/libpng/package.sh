# variables
REMOTE_ARCHIVE="https://www.meetixos.org/libs/ports/libpng-1.6.18.tar.gz"
UNPACKED_DIR=libpng-1.6.18
ARCHIVE=libpng-1.6.18.tar.gz

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
	../$UNPACKED_DIR/configure --host=$HOST --prefix=$PREFIX
	make
	make DESTDIR=$SYSROOT install
}