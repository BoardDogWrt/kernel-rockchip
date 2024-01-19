#!/bin/bash

# if not in kernel directory
if [ -d kernel-som-rk3399 ] ; then
    cd kernel-som-rk3399
fi

REMOTE="" 
SSHKEY="" 
HERE=`pwd`

# CROSS="aarch64-linux-"

OUT_DIR="../live-update"
KRNL_VER="`make -j8 ARCH=arm64 CROSS_COMPILE=${CROSS} kernelrelease`"

echo "-----------------------------------"
echo "KRNL_VER: $KRNL_VER"
echo "OUT_DIR: $OUT_DIR"
echo "-----------------------------------"

mkdir -p ${OUT_DIR}

if [ ! -f .config_done ] ; then
    make -j4 ARCH=arm64 CROSS_COMPILE=${CROSS} kernelrelease || exit 1
    make -j4 ARCH=arm64 CROSS_COMPILE=${CROSS} eof_defconfig
    touch .config_done
fi

# Bit Power Build Host
make -j4 ARCH=arm64 CROSS_COMPILE=${CROSS} all modules dtbs || exit 1
make -j4 ARCH=arm64 CROSS_COMPILE=${CROSS} INSTALL_OUT_DIR=${OUT_DIR} modules_install || exit 1
make -j4 ARCH=arm64 CROSS_COMPILE=${CROSS} nanopi4-images   || exit 1

rm -vf ${OUT_DIR}/lib/modules/$KRNL_VER/source
rm -vf ${OUT_DIR}/lib/modules/$KRNL_VER/build

if [ ! -e ${REMOTE} ] ; then
    #debug:  --list-only /// #-avz
    OPTS="-zvrau"
    OPTS="-arluvt"
    SOURCE="${OUT_DIR}/lib"
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
    cp -Rpvu ${OUT_DIR}/lib/*        /mnt/ssd/sys-update/lib/
    cp -Rpvu resource.img kernel.img /mnt/ssd/sys-update/
fi

exit $?

