# pkgIndex.tcl -- tells Tcl how to load my package.
package ifneeded "hello" 1.0 [list load [file join $dir libhello[info sharedlibextension]]]
