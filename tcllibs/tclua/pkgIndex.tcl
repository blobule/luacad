# pkgIndex.tcl -- tells Tcl how to load my package.
package ifneeded "tclua" 1.0 [list load [file join $dir libtclua[info sharedlibextension]]]
