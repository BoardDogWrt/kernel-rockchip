#!/bin/bash

# if not in kernel directory
if [ -d kernel-som-rk3399 ] ; then
    cd kernel-som-rk3399
fi

REMOTE="" 
SSHKEY="" 
HERE=`pwd`

# CROSS="aarch64-linux-"

UPDATE_DIR="/mnt/ssd/emmc-update"
KRNL_VER="`make -j4 ARCH=arm64 CROSS_COMPILE=${CROSS} kernelrelease`"

echo "-----------------------------------"
echo "KRNL_VER: $KRNL_VER"
echo "UPDATE_DIR: $UPDATE_DIR"
echo "-----------------------------------"

mkdir -p ${UPDATE_DIR}

if [ ! -f .config_done ] ; then
    make -j4 ARCH=arm64 CROSS_COMPILE=${CROSS} kernelrelease || exit 1
    make -j4 ARCH=arm64 CROSS_COMPILE=${CROSS} eof_defconfig
    touch .config_done
fi

# Bit Power Build Host
make -j4 ARCH=arm64 CROSS_COMPILE=${CROSS} all modules dtbs || exit 1
make -j4 ARCH=arm64 CROSS_COMPILE=${CROSS} INSTALL_MOD_PATH=${UPDATE_DIR} modules_install || exit 1
make -j4 ARCH=arm64 CROSS_COMPILE=${CROSS} nanopi4-images   || exit 1

rm -vf ${UPDATE_DIR}/lib/modules/$KRNL_VER/source
rm -vf ${UPDATE_DIR}/lib/modules/$KRNL_VER/build

if [ ! -e ${REMOTE} ] ; then
    OPTS="-arluvt"
    SOURCE="${UPDATE_DIR}/"
    TARGET="root@${REMOTE}:${UPDATE_DIR}/"
    echo "-------------------------------------------------"
    echo "++ Enter the following commands to update remote."
    echo rsync ${OPTS} ${SYNC_OPT} ${SOURCE} ${TARGET}
    echo scp resource.img kernel.img root@${REMOTE}:${UPDATE_DIR}/
else
    cp -Rpvu resource.img kernel.img ${UPDATE_DIR}
fi

exit $?

