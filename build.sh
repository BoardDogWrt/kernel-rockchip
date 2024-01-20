#!/bin/bash

# ---------------------------------------------------------
# Simple kernel build script (c) by EoF Software Labs  
# ---------------------------------------------------------

# if not in kernel directory
if [ -d kernel -a ! -f Kbuild ] ; then
    cd kernel
fi

echo "----------------------------------------------------"

SRC_DIR=`pwd`
OS_HOST=`uname -m`

# Output directory
UPDATE_DIR="${SRC_DIR}/../live-update"

echo "SRC_DIR......: ${SRC_DIR}"
echo "UPDATE_DIR...: ${UPDATE_DIR}"
echo "OS_HOST......: ${OS_HOST}"

if [ ${OS_HOST} == "x86_64" ] ; then
    export TOOL_CHAIN="/opt/FriendlyARM/toolchain/6.4-aarch64"
	export CROSS=aarch64-linux-
	export KRNL_ARCH="arm64"
	export REMOTE_SSHKEY="${HOME}/.ssh/id_rsa_tux_deploy" 
	export REMOTE_HOST="192.168.181.210" 
    export REMOTE_TARGET="root@${REMOTE_HOST}:/mnt/ssd/emmc-update/"
    if [[ ! ${PATH} =~ "${TOOL_CHAIN}" ]]; then
        export PATH=${TOOL_CHAIN}/bin/:$PATH
    fi
else
	export KRNL_ARCH="arm64"
fi

# Used kernel branch
#KRNL_BRANCH="nanopi4-linux-v4.4.y"
KRNL_BRANCH=`git branch`

# Get kernel version
KRNL_VER="`make -j8 ARCH=${KRNL_ARCH} CROSS_COMPILE=${CROSS} kernelrelease`"

echo "KERNEL_VER...: ${KRNL_VER}"
echo "KERNEL_ARCH..: ${KRNL_ARCH}"
echo "KERNEL_BRANCH: ${KRNL_BRANCH}"
echo "CROSS_COMPILE: ${CROSS}"
echo "TOOL_CHAIN...: ${TOOL_CHAIN}"
echo "REMOTE_TARGET: ${REMOTE_TARGET}"
echo "----------------------------------------------------"

mkdir -p ${UPDATE_DIR}

if [ ! -f .config ] ; then
    echo "----------------------------------------------------"
    echo "++ Configure kernel from eof_defconfig"
    make -j4 ARCH=${KRNL_ARCH} CROSS_COMPILE=${CROSS} kernelrelease || exit 1
    make -j4 ARCH=${KRNL_ARCH} CROSS_COMPILE=${CROSS} eof_defconfig
fi

# Bit Power Build Host
echo "----------------------------------------------------"
echo "++ Build kernel targets: Image modules dtbs"
make -j4 ARCH=${KRNL_ARCH} CROSS_COMPILE=${CROSS} Image modules dtbs || exit 1

echo "----------------------------------------------------"
echo "++ Install modules to: ${UPDATE_DIR}"
make -j4 ARCH=${KRNL_ARCH} CROSS_COMPILE=${CROSS} \
		INSTALL_MOD_PATH=${UPDATE_DIR} modules_install || exit 1

echo "----------------------------------------------------"
echo "++ Generate nanopi4 kernel images..."
make -j4 ARCH=${KRNL_ARCH} CROSS_COMPILE=${CROSS} nanopi4-images   || exit 1

rm -vf ${UPDATE_DIR}/lib/modules/$KRNL_VER/source
rm -vf ${UPDATE_DIR}/lib/modules/$KRNL_VER/build

if [ ! -e ${REMOTE_TARGET} ] ; then
    #debug:  --list-only /// #-avz
    #OPTS="-zvrau"
    eval $(ssh-agent) # Create agent and environment variables
    ssh-add ${REMOTE_SSHKEY}
    OPTS="-arluvt"
    SOURCE="${UPDATE_DIR}/lib"
    INCLUDE=""
    EXCLUDE=""
    echo "Enter manual following commands:"
    echo "rsync: ${OPTS} ${SOURCE} -> ${REMOTE_TARGET}"
    rsync ${OPTS} ${SYNC_OPT} ${INCLUDE} ${EXCLUDE} ${SOURCE} ${REMOTE_TARGET}
    echo "scp -r -i ${REMOTE_SSHKEY} resource.img kernel.img ${REMOTE_TARGET}"
    scp -r -i ${REMOTE_SSHKEY} resource.img kernel.img ${REMOTE_TARGET}
else
    mkdir -p /mnt/ssd/emmc-update/lib
    cp -Rpvu ${UPDATE_DIR}/lib/*     /mnt/ssd/emmc-update/lib/
    cp -Rpvu resource.img kernel.img /mnt/ssd/emmc-update/
fi

exit $?

