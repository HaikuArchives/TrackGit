#!/bin/sh

GCCVERSION=`gcc -dumpversion | cut -d'.' -f 1`
ARCH=`uname -p`
DEBUG=`cat Makefile | grep DEBUGGER |cut -d ' ' -f 3`

OBJDIR=objects.$ARCH-cc$GCCVERSION

if [ "$DEBUG" == "TRUE" ] 
then
	OBJDIR=$OBJDIR-debug
fi

echo "Compiling TrackGit ..."
make

echo "Installing TrackGit add-on ..."
cp -v $OBJDIR/TrackGit ~/config/non-packaged/add-ons/Tracker/	
