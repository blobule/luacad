 /*
  * hello.c -- A minimal Tcl C extension.
  */
#include <tcl.h>

#define NAMESPACE "yo"

 static int 
 Hello_Cmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
 {
     Tcl_SetObjResult(interp, Tcl_NewStringObj("Salut!", -1));
     return TCL_OK;
 }

 /*
  * Hello_Init -- Called when Tcl loads your extension.
  */
 int DLLEXPORT
 Hello_Init(Tcl_Interp *interp)
 {
#ifdef NAMESPACE
     Tcl_Namespace *nsPtr; /* pointer to hold our own new namespace */
#endif



     if (Tcl_InitStubs(interp, TCL_VERSION, 0) == NULL) {
         return TCL_ERROR;
     }

#ifdef NAMESPACE
    /* create the namespace named 'hello' */
    nsPtr = Tcl_CreateNamespace(interp, NAMESPACE, NULL, NULL);
    if (nsPtr == NULL) { return TCL_ERROR; }
#endif

     /* changed this to check for an error - GPS */
     if (Tcl_PkgProvide(interp, "hello", "1.0") == TCL_ERROR) {
         return TCL_ERROR;
     }

     Tcl_CreateObjCommand(interp,
#ifdef NAMESPACE
             NAMESPACE "::hello",
#else
             "hello",
#endif
             Hello_Cmd, NULL, NULL);

     return TCL_OK;
 }
