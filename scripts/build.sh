#!/bin/bash

tgt=$1

for d in GhCore GhBio GhGui GhSalsa GhDbase ghmake
do
	cd $d
	qmake
if [ "$tgt" = "mac" ]; then
	make clean
	make
	make macinstall
elif [ "$tgt" = "win" ]; then
	mingw32-make clean
	mingw32-make install
else
	make clean
	make
	make install
fi

	cd ..
done
