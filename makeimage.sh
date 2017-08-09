#!/bin/sh

DISKIMAGE="c.img"
echo "Mounting image"
DISKLOCATION=`hdiutil attach ${DISKIMAGE} | head -n1 | cut -d " " -f1`
echo "Mounted at ${DISKLOCATION}"
cp ./build/bin/kernel.elf /Volumes/MUSTARD/boot
hdiutil detach $DISKLOCATION
echo "Unmounted"