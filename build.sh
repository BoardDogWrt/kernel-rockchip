#!/bin/bash

# ---------------------------------------------------------
# Simple kernel build script (c) by EoF Software Labs  
# ---------------------------------------------------------

# if not in kernel directory
if [ -d kernel -a ! -f Kbuild ] ; then
    cd kernel
fi

SRC_DIR=`pwd`
OS_HOST=`uname -m`

if [ ${OS_HOST} == "x86_64" ] ; then
    #using Ubuntu gcc-10-aarch64
    #TOOL_CHAIN="/opt/FriendlyARM/toolchain/6.4-aarch64"
	CC="aarch64-linux-gnu-gcc-10"
	KRNL_ARCH="arm64"
	REMOTE_SSHKEY="${HOME}/.ssh/id_rsa_tux_deploy" 
	REMOTE_HOST="192.168.181.210" 
    REMOTE_TARGET="root@${REMOTE_HOST}:/mnt/ssd/emmc-update/"
    if [ -e ${TOOL_CHAIN} ] ; then 
        if [[ ! ${PATH} =~ "${TOOL_CHAIN}" ]]; then
            export PATH=${TOOL_CHAIN}/bin/:$PATH
        fi
    fi
    # Output directory
    UPDATE_DIR="${SRC_DIR}/../live-update"
else
    UPDATE_DIR="/mnt/ssd/emmc-update"
	KRNL_ARCH="arm64"
    CC="gcc"
fi

# Used kernel branch
#KRNL_BRANCH="nanopi4-linux-v4.4.y"
KRNL_BRANCH=`git branch`

# Get kernel version
KRNL_VER="`make -j4 CC=${CC} ARCH=${KRNL_ARCH} kernelrelease`"

echo "----------------------------------------------------"
echo "SRC_DIR......: ${SRC_DIR}"
echo "OS_HOST......: ${OS_HOST}"
echo "UPDATE_DIR...: ${UPDATE_DIR}"
echo "KERNEL_VER...: ${KRNL_VER}"
echo "KERNEL_ARCH..: ${KRNL_ARCH}"
echo "KERNEL_BRANCH: ${KRNL_BRANCH}"
echo "CROSS_COMPILE: ${CROSS}"
echo "TOOL_CHAIN...: ${TOOL_CHAIN}"
echo "REMOTE_TARGET: ${REMOTE_TARGET}"
echo "----------------------------------------------------"
#echo | ${CC} -dM -E -
#echo "----------------------------------------------------"

mkdir -p ${UPDATE_DIR}

if [ ! -f .config ] ; then
    echo "----------------------------------------------------"
    echo "++ Configure kernel from eof_defconfig"
    make -j4 CC=${CC} ARCH=${KRNL_ARCH} clean || exit 1
    make -j4 CC=${CC} ARCH=${KRNL_ARCH} kernelrelease || exit 1
    make -j4 CC=${CC} ARCH=${KRNL_ARCH} eof_defconfig
fi

# Bit Power Build Host
echo "----------------------------------------------------"
echo "++ Build kernel targets: Image modules dtbs"
make -j4 CC=${CC} ARCH=${KRNL_ARCH} Image modules dtbs || exit 1

echo "----------------------------------------------------"
echo "++ Install modules to: ${UPDATE_DIR}"
make -j4 CC=${CC} ARCH=${KRNL_ARCH} \
		INSTALL_MOD_PATH=${UPDATE_DIR} modules_install || exit 1

echo "----------------------------------------------------"
echo "++ Install kernel headers to: ${UPDATE_DIR}"
make -j4 CC=${CC} ARCH=${KRNL_ARCH}  \
        INSTALL_HDR_PATH=${UPDATE_DIR}/usr/src headers_install || exit 1

echo "----------------------------------------------------"
echo "++ Generate nanopi4 kernel images..."
make -j4 CC=${CC} ARCH=${KRNL_ARCH} nanopi4-images || exit 1

rm -vf ${UPDATE_DIR}/lib/modules/$KRNL_VER/source
rm -vf ${UPDATE_DIR}/lib/modules/$KRNL_VER/build

if [ ! -e ${REMOTE_TARGET} ] ; then
    eval $(ssh-agent) # Create agent and environment variables
    ssh-add ${REMOTE_SSHKEY}
    OPTS="-arluvt"
    SOURCE="${UPDATE_DIR}/lib"
    echo "Enter manual following commands:"
    echo rsync ${OPTS} ${SOURCE} ${REMOTE_TARGET}
    echo "scp -r -i ${REMOTE_SSHKEY} resource.img kernel.img ${REMOTE_TARGET}"
else
    cp -Rpvu resource.img kernel.img /mnt/ssd/emmc-update/
fi

exit $?

