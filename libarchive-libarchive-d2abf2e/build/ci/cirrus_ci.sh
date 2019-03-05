#!/bin/sh
UNAME=`uname`
if [ "$1" = "install" ]
then
	if [ "$UNAME" = "FreeBSD" ]
	then
		set -x -e
		sed -i.bak -e 's,pkg+http://pkg.FreeBSD.org/\${ABI}/quarterly,pkg+http://pkg.FreeBSD.org/\${ABI}/latest,' /etc/pkg/FreeBSD.conf
		mount -u -o acls /
		mkdir /tmp_acl_nfsv4
		MD=`mdconfig -a -t swap -s 128M`
		newfs /dev/$MD
		tunefs -N enable /dev/$MD
		mount /dev/$MD /tmp_acl_nfsv4
		chmod 1777 /tmp_acl_nfsv4
		pkg install -y autoconf automake cmake libiconv libtool pkgconf expat libxml2 liblz4 zstd
	elif [ "$UNAME" = "Darwin" ]
	then
		set -x -e
		brew update
		brew install autoconf automake libtool pkg-config cmake xz lz4 zstd
	elif [ "$UNAME" = "Linux" -a -f /etc/debian_version ]
	then
		apt-get -y update
		apt-get -y install build-essential automake libtool bison sharutils pkgconf libacl1-dev libbz2-dev libzip-dev zlib1g-dev liblzma-dev liblz4-dev libzstd-dev lrzip cmake
	fi
elif [ "$1" = "test" ]
then
	if [ "$UNAME" = "FreeBSD" -a "$BUILD_SYSTEM" != "cmake" ]
	then
		set -e
		echo "Additional NFSv4 ACL tests"
		CURDIR=`pwd`
		BUILDDIR="${CURDIR}/build_ci/autotools"
		cd "${BUILDDIR}"
		TMPDIR=/tmp_acl_nfsv4 ./libarchive_test -r "${CURDIR}/libarchive/test" -v test_acl_platform_nfs4
	fi
else
	echo "Usage $0 install | test_nfsv4_acls"
	exit 1
fi
