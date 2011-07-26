#!/bin/bash

echo "HEADERS +=\\" > headers.pri
ls *.h | grep -v Gh | awk '{printf( "\t%s \\\n", $1 )}' >> headers.pri
ls *.h | grep Gh | awk '{printf( "\t%s\n", $1 ) }' >> headers.pri

