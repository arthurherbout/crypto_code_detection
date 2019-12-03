#!/bin/bash

if [ $# -lt 2 ]
then
  echo "usage is: ${0##*/} <path to coverity scan> <extra compiler options>"
  echo "e.g. \"${0##*/} \"/usr/local/bin/coverity\" \"-DLTM_DESC -I/path/to/libtommath/\"\""
  exit -1
fi

PATH=$PATH:$1/bin

make clean
rm -r cov-int/

myCflags=""
myCflags="$myCflags -O2 ${2}"
myCflags="$myCflags -pipe -Werror -Wpointer-arith -Winit-self -Wextra -Wall -Wformat -Wformat-security"

CFLAGS="$myCflags" cov-build --dir cov-int  make -f makefile.unix $MAKE_OPTS IGNORE_SPEED=1 1>gcc_1.txt

if [ $? -ne 0 ]
then
  echo "make failed"
  exit -1
fi

# zipup everything
tar caf libtomcrypt.lzma cov-int

mytoken=$(cat .coverity_token)
mymail=$(cat .coverity_mail)
myversion=$(git describe --dirty)

curl -k --form project=libtomcrypt \
  --form token=${mytoken} \
  --form email=${mymail} \
  --form file=@libtomcrypt.lzma \
  --form version=\"${myversion}\" \
  --form description="\"libtomcrypt version ${myversion}\"" \
  https://scan.coverity.com/builds?project=libtom%2Flibtomcrypt

# ref:         HEAD -> develop
# git commit:  1937f412605e1b04ddb41ef9c2f2f0aab7e61548
# commit time: 2019-11-22 12:03:27 +0100
