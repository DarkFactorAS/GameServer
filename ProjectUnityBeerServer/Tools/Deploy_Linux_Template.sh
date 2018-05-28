#!/bin/bash
NOW=$(date -d "$BuildDate" +"%Y-%m-%d")
SERVER=ValyrianAdventuresServerD
SRC_FOLDER=../Build/ValyrianAdventuresServer.Linux.Debug.x64/Bin
DST_FOLDER=/Services/VAServer
BUILD=$BuildRevision

cp $SRC_FOLDER/$SERVER.exe $DST_FOLDER/$SERVER.$NOW.$BUILD.exe

# Relink the symlink to this one
rm $DST_FOLDER/$SERVER.exe
ln -s $DST_FOLDER/$SERVER.$NOW.$BUILD.exe $DST_FOLDER/$SERVER.exe
