#!/bin/sh

echo allo

lua() {
   echo cp $1 /usr/local/lib/lua/5.3/
   cp $1 /usr/local/lib/lua/5.3/
   chmod ugo+r /usr/local/lib/lua/5.3/$1
}
tclbin() {
   echo cp $1 /usr/local/bin/
   cp $1 /usr/local/bin/
   chmod ugo+rx /usr/local/bin/$1
}
tcl() {
   mkdir -p /usr/local/lib/tcltk/$1
   cp $1.tcl /usr/local/lib/tcltk/$1/
   echo "package ifneeded \"dohelp\" 1.0 [list source [file join \$dir dohelp.tcl]]" >/usr/local/lib/tcltk/$1/pkgIndex.tcl
   chmod ugo+rx /usr/local/lib/tcltk/$1
   chmod ugo+r /usr/local/lib/tcltk/$1/*
}

tclbin luacad-editor.tcl
tcl dohelp

lua editor-csg.lua
lua luacad.lua
lua matrix-extra.lua
lua matrix.lua
lua table-extra.lua
lua complex.lua
lua md5.lua
lua cache.lua

