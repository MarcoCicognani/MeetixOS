#!/bin/bash
. ../../../MXbase.sh

echo "Installing pkg-config wrapper"
OUT_PATH=$TOOLCHAIN_BASE/bin
mkdir -p $OUT_PATH
OUT_FILE=$OUT_PATH/mx-pkg-config.sh
sed -e 's|__SYSROOT__|'$SYSROOT'|' mx-pkg-config.sh.template > $OUT_FILE
chmod +x $OUT_FILE
