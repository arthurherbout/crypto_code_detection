#!/bin/bash

./helper.pl --update-makefiles || exit 1

makefiles=(makefile makefile_include.mk makefile.shared makefile.unix makefile.mingw makefile.msvc)
vcproj=(libtomcrypt_VS2008.vcproj)

if [ $# -eq 1 ] && [ "$1" == "-c" ]; then
  git add ${makefiles[@]} ${vcproj[@]} doc/Doxyfile && git commit -m 'Update makefiles'
fi

exit 0

# ref:         HEAD -> develop
# git commit:  1937f412605e1b04ddb41ef9c2f2f0aab7e61548
# commit time: 2019-11-22 12:03:27 +0100
