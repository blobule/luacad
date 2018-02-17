 /*
  * extension Lua
  *
  */
#include <tcl.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

typedef struct {
    int a,b,c;
    lua_State *lua;
} t_client;

t_client client;


static int 
Hello_Cmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]) {
    Tcl_SetObjResult(interp, Tcl_NewStringObj("Salut!", -1));
    t_client *client=(t_client *)cdata;
    printf("client %d\n",client->a);
    client->a++;
    return TCL_OK;
}


static int 
Lua_Cmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]) {
	int i;
	int len;
	char *s;

    Tcl_Obj *list=Tcl_NewListObj(0,NULL);

    t_client *c=(t_client *)cdata;
    s = Tcl_GetStringFromObj(objv[1], &len);
    //printf("-- %s\n",s);
    int k=luaL_dostring(c->lua, s);
    if( k ) {
        printf("**** %s\n",lua_tostring(c->lua,1));
    }else{

        int nbresults=lua_gettop(c->lua);


        for(int i=1;i<=nbresults;i++) {
            int t=lua_type(c->lua,i);
            //printf("type[%d] is ",i);
            switch(t) {
                case LUA_TNIL:
                    //printf("nil\n");
                    Tcl_ListObjAppendElement(interp,list,Tcl_NewStringObj("",-1));
                    break;
                case LUA_TBOOLEAN:
                    {
                    int b=lua_toboolean(c->lua,i);
                    //printf("bool %d\n",b);
                    Tcl_ListObjAppendElement(interp,list,Tcl_NewBooleanObj(b));
                    break;
                    }
                case LUA_TNUMBER:
                    //printf("number ");
                    if( lua_isinteger(c->lua,i) ) {
                        int num=(int)lua_tointeger(c->lua,i);
                        //printf("integer %d\n",num);
                        Tcl_ListObjAppendElement(interp,list,Tcl_NewIntObj(num));
                    }else{
                        double d=lua_tonumber(c->lua,i);
                        //printf("double %f\n",d);
                        Tcl_ListObjAppendElement(interp,list,Tcl_NewDoubleObj(d));
                    }
                    break;
                case LUA_TSTRING:
                    {
                    const char *str=lua_tostring(c->lua,i);
                    //printf("string %s\n",str);
                    Tcl_ListObjAppendElement(interp,list,Tcl_NewStringObj(str,-1));
                    }
                    break;
                case LUA_TTABLE:
                    //printf("table\n");
                    break;
                case LUA_TFUNCTION:
                    //printf("function\n");
                    break;
                case LUA_TUSERDATA:
                    //printf("userdata\n");
                    break;
                case LUA_TTHREAD:
                    //printf("thread\n");
                    break;
            }
        }
    }

    // remove everything that was returned
    lua_settop(c->lua,0);

    //const char * str = lua_tostring(c->lua, -1);

    Tcl_SetObjResult(interp, list);
	return TCL_OK;
}

// return nothing if ok, else the error
static int 
LuaCheck_Cmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]) {
	int i;
	int len;
	char *s;

    t_client *c=(t_client *)cdata;
    s = Tcl_GetStringFromObj(objv[1], &len);
    int k=luaL_loadstring(c->lua, s);
    //printf("-- checking %s returned %d\n",s,k);
    if( k ) {
        //printf("**** check: %s\n",lua_tostring(c->lua,1));
        const char *str=lua_tostring(c->lua,1);
        Tcl_SetObjResult(interp, Tcl_NewStringObj(str,-1));
    }
    // remove everything that was returned
    lua_settop(c->lua,0);
	return TCL_OK;
}



///////////////////////////////////////////////////

int DLLEXPORT
Tclua_Init(Tcl_Interp *interp)
{
    if (Tcl_InitStubs(interp, TCL_VERSION, 0) == NULL) { return TCL_ERROR; }

    /* changed this to check for an error - GPS */
    if (Tcl_PkgProvide(interp, "tclua", "1.0") == TCL_ERROR) { return TCL_ERROR; }

    // create interpreter
    client.lua = luaL_newstate();
    luaL_openlibs(client.lua);
    //luaL_openlib(lua, "Map", map_lib, 0);

    client.a=111;

    Tcl_CreateObjCommand(interp, "hello", Hello_Cmd, (void *)&client, NULL);
    Tcl_CreateObjCommand(interp, "lua", Lua_Cmd, (void *)&client, NULL);
    Tcl_CreateObjCommand(interp, "luacheck", LuaCheck_Cmd, (void *)&client, NULL);

    return TCL_OK;
 }
