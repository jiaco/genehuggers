#!/bin/bash
#
#	still need to manually edit the last line '\'

echo "SOURCES +=\\" > sources.pri
ls *.cpp | awk '{printf( "\t%s \\\n", $1 )}' >> sources.pri

