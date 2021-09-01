# variables
REMOTE_ARCHIVE="https://www.meetixos.org/libs/ports/freetype-2.5.3.tar.gz"
UNPACKED_DIR=freetype-2.5.3
ARCHIVE=freetype-2.5.3.tar.gz

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
	../$UNPACKED_DIR/configure --host=$HOST --prefix=$PREFIX --with-png=no --with-zlib=no --with-harfbuzz=no
	make -j8
	make DESTDIR=$SYSROOT install
}