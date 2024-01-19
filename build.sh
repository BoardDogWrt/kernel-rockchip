#!/bin/bash

# if not in kernel directory
if [ -d kernel-som-rk3399 ] ; then
    cd kernel-som-rk3399
fi

REMOTE="" 
SSHKEY="" 
HERE=`pwd`

# CROSS="aarch64-linux-"

MOD_PATH="../live-update"
KRNL_VER="`make -j8 ARCH=arm64 CROSS_COMPILE=${CROSS} kernelrelease`"

echo "-----------------------------------"
echo "KRNL_VER: $KRNL_VER"
echo "MOD_PATH: $MOD_PATH"
echo "-----------------------------------"

mkdir -p ${MOD_PATH}

if [ ! -f .config_done ] ; then
    make -j4 ARCH=arm64 CROSS_COMPILE=${CROSS} kernelrelease || exit 1
    make -j4 ARCH=arm64 CROSS_COMPILE=${CROSS} eof_defconfig
    touch .config_done
fi

if [ -e ${CROSS} ] ; then
    # Local Build
    make -j4 ARCH=arm64 all modules dtbs || exit 1
    make -j4 ARCH=arm64 INSTALL_MOD_PATH=${MOD_PATH} modules_install || exit 1
    make -j4 ARCH=arm64 nanopi4-images   || exit 1
else 
    # Bit Power Build Host
    make -j8 ARCH=arm64 CROSS_COMPILE=${CROSS} all modules dtbs || exit 1
    make -j8 ARCH=arm64 CROSS_COMPILE=${CROSS} INSTALL_MOD_PATH=${MOD_PATH} modules_install || exit 1
    make -j8 ARCH=arm64 CROSS_COMPILE=${CROSS} nanopi4-images   || exit 1
fi

rm -vf ${MOD_PATH}/lib/modules/$KRNL_VER/source
rm -vf ${MOD_PATH}/lib/modules/$KRNL_VER/build

if [ ! -e ${REMOTE} ] ; then
##
## Do it - skip existing target
#debug:  --list-only"
#-avz
    OPTS="-zvrau"
    OPTS="-arluvt"
    SOURCE="${MOD_PATH}/lib"
    TARGET="root@${REMOTE}:/mnt/ssd/sys-update/"
    INCLUDE=""
    EXCLUDE=""
    echo "Enter manual following commands:"
    echo "rsync: ${OPTS} ${SOURCE} -> ${TARGET}"
    echo rsync ${OPTS} ${SYNC_OPT} \
            ${INCLUDE} ${EXCLUDE} ${SOURCE} ${TARGET}
    echo scp -r resource.img kernel.img \
            root@${REMOTE}:/mnt/ssd/sys-update/
else
    mkdir -p /mnt/ssd/sys-update/lib
    cp -Rpvu ${MOD_PATH}/lib/*       /mnt/ssd/sys-update/lib/
    cp -Rpvu resource.img kernel.img /mnt/ssd/sys-update/
fi

exit $?

