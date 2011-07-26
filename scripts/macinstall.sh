#!/bin/bash

LF=/Library/Frameworks
AP=/Applications


tp=$1
ver=$2

LF_SD="Versions/"$ver
AP_SD="Contents/MacOS"

if [ -e $LF/$tp".framework" ]; then
	tp2=`echo $tp | tr '[A-Z]' '[a-z]'`
	tgt=$LF"/"$tp".framework/"$LF_SD"/"$tp2;
	ttype="LIB";
elif [ -e $AP/$tp".app" ]; then
	tgt=$AP"/"$tp".app/"$AP_SD"/"$tp;
	ttype="APP";
else
	echo "Nada";
	exit
fi

echo "BEGIN WORK FOR " $ttype " : " $tgt
libs=( `otool -L $tgt | grep -v '^/' | grep -v Qt | grep -v '/usr' | awk '{print $1}'` )

for lib in ${libs[@]}
do
	echo "INSTALL NAME FOR " $lib
	install_name_tool -change $lib $LF"/"$lib $tgt
done
