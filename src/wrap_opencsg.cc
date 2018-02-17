//
// wrpping for opencsg
//
//
//
//
//

#include <stdlib.h>
#include <string.h>
#include <iostream>
 
#include <lua.h>
#include <lauxlib.h>

// our support library
#include <ocsg.h>
 
#include <nef.h>

//#include "nef.h"

extern "C" {
    int luaopen_opencsg(lua_State *L);
}


/* Userdata object that will hold the nef-polyhedron. */
/*
typedef struct {
        nef *n;
        int num;
} lnef_userdata_t;
*/



//
// generic opencsg data
//
 
ocsg csg;


// return the window id
static int lopencsg_init(lua_State *L)
{
    int wid; // window id
    std::cout<<"OpenCSG init\n";

    if( (wid=csg.init())<0 ) {
        luaL_error(L, "opencsg: init error.");
    }

    lua_pushinteger(L,wid);
    return 1;
}

static int lopencsg_uninit(lua_State *L)
{
    std::cout<<"OpenCSG uninit\n";

    csg.uninit();

    return 0;
}

static int lopencsg_clear(lua_State *L)
{
    //std::cout<<"OpenCSG clear\n";

    csg.clear();

    return 0;
}

static int lopencsg_newProduct(lua_State *L)
{
    //std::cout<<"OpenCSG newProduct\n";

    csg.newProduct();

    return 0;
}


static int lopencsg_draw(lua_State *L)
{
    const char *op=lua_tostring(L,1);

    //printf("op = '%s'\n",op);

    //std::cout<<"OpenCSG draw\n";
    lnef_userdata_t *cu;
    cu  = (lnef_userdata_t *)luaL_checkudata(L, 2, "LNef");


    float *vertices=NULL;
    int *facets=NULL;
    int nbV,nbF;

    cu->n->renderGL(vertices,facets,nbV,nbF);
    //printf("%d V and %d F\n",nbV,nbF);

    csg.draw(op[0],vertices,facets,nbV,nbF);

    //double bb[6];
    //cu->n->bbox(bb);
    //printf("bb x %lf .. %lf\n",bb[0],bb[1]);

    //int k=csg.draw();
    //std::cout << "returned "<<k<<"\n";

    if( vertices ) free(vertices);
    if( facets ) free(facets);

    return 0;
}
static int lopencsg_render(lua_State *L)
{
    //std::cout<<"OpenCSG render\n";

    int k=csg.render();

    return 0;
}

#if 0
// setup nef as a sphere
static int lnef_sphere(lua_State *L)
{
    lnef_userdata_t *cu;

    luaL_checktype(L, 1,LUA_TTABLE);
    lua_getfield(L, 1, "triangles");
    lua_getfield(L, 1, "r");
    lua_getfield(L, 1, "n"); // nba = n, nbb=nba/2-2

    int tri = lua_tointeger(L, -3);
    double r = lua_isnil(L,-2)?1.0:lua_tonumber(L, -2);
    int n = lua_isnil(L,-1)?8:lua_tointeger(L, -1);

    int nba=n;
    if( nba<3 ) nba=3;
    int nbb=(n+1)/2-1;
    if( nbb<1 ) nbb=1;
    if( tri!=0 ) tri=1;

    cu       = (lnef_userdata_t *)lua_newuserdata(L, sizeof(*cu));
    cu->n    = new nef();
    cu->num  = nefnum++;

    luaL_getmetatable(L, "LNef");
    lua_setmetatable(L, -2);

    std::cout<<"NEF SPHERE "<<cu->num<<"\n";

    cu->n->sphere(r,nba,nbb,tri);
    return 1;
}


// setup nef as a sphere
static int lnef_cylinder(lua_State *L)
{
    lnef_userdata_t *cu;

    luaL_checktype(L, 1,LUA_TTABLE);
    lua_getfield(L, 1, "triangles");
    lua_getfield(L, 1, "h");
    lua_getfield(L, 1, "r");
    lua_getfield(L, 1, "n");

    int tri = lua_tointeger(L, -4);
    double h = lua_isnil(L,-3)?1.0:lua_tonumber(L, -3);
    double r = lua_isnil(L,-2)?1.0:lua_tonumber(L, -2);
    int n = lua_isnil(L,-1)?8:lua_tointeger(L, -1);

    if( n<3 ) n=3;
    if( tri!=0 ) tri=1;

    cu       = (lnef_userdata_t *)lua_newuserdata(L, sizeof(*cu));
    cu->n    = new nef();
    cu->num  = nefnum++;

    luaL_getmetatable(L, "LNef");
    lua_setmetatable(L, -2);

    std::cout<<"NEF CYLINDER "<<cu->num<<"\n";

    cu->n->cylinder(r,h,n,tri);
    return 1;
}


// save internal
static int lnef_save(lua_State *L)
{
    lnef_userdata_t *cu;

    cu    = (lnef_userdata_t *)luaL_checkudata(L, 1, "LNef");
    luaL_checktype(L, 2,LUA_TTABLE);

    lua_getfield(L, 2, "name");
    if( lua_isnil(L,-1) ) { luaL_error(L, "save: name cannot be empty"); }
    const char *name=lua_tostring(L,-1);

    cu->n->save(name);
    return 0;
}
 
// load internal
static int lnef_load(lua_State *L)
{
    lnef_userdata_t *cu;

    luaL_checktype(L, 1,LUA_TTABLE);

    lua_getfield(L, 1, "name");
    if( lua_isnil(L,-1) ) { luaL_error(L, "load: name cannot be empty"); }
    const char *name=lua_tostring(L,-1);

    cu       = (lnef_userdata_t *)lua_newuserdata(L, sizeof(*cu));
    cu->n    = new nef();
    cu->num  = nefnum++;

    luaL_getmetatable(L, "LNef");
    lua_setmetatable(L, -2);

    cu->n->load(name);
    return 1;
}

// export en STL
static int lnef_export(lua_State *L)
{
    lnef_userdata_t *cu;

    cu    = (lnef_userdata_t *)luaL_checkudata(L, 1, "LNef");
    luaL_checktype(L, 2,LUA_TTABLE);

    lua_getfield(L, 2, "name");
    lua_getfield(L, 2, "binary");

    if( lua_isnil(L,-2) ) { luaL_error(L, "export: name cannot be empty"); }

    const char *name=lua_tostring(L,-2);
    int bin = lua_tointeger(L, -1);

    cu->n->myexport(name,bin);
    return 0;
}

// import un STL
static int lnef_import(lua_State *L)
{
    lnef_userdata_t *cu;

    luaL_checktype(L, 1,LUA_TTABLE);
    lua_getfield(L, 1, "name");
    if( lua_isnil(L,-1) ) { luaL_error(L, "import: name cannot be empty"); }

    const char *name=lua_tostring(L,-1);

    cu       = (lnef_userdata_t *)lua_newuserdata(L, sizeof(*cu));
    cu->n    = new nef();
    cu->num  = nefnum++;

    luaL_getmetatable(L, "LNef");
    lua_setmetatable(L, -2);

    cu->n->myimport(name);
    return 1;
}

// info sur le nef
static int lnef_info(lua_State *L)
{
    lnef_userdata_t *cu;

    cu    = (lnef_userdata_t *)luaL_checkudata(L, 1, "LNef");

    int valid,simple,volumes;

    cu->n->info(cu->num,valid,simple,volumes);
    lua_pushinteger(L,valid);
    lua_pushinteger(L,simple);
    lua_pushinteger(L,volumes);
    return 3;
}

// volume
static int lnef_volume(lua_State *L)
{
    lnef_userdata_t *cu;

    cu    = (lnef_userdata_t *)luaL_checkudata(L, 1, "LNef");

    double v;
    double center[3];
    cu->n->volume(v,center);

    lua_pushnumber(L,v);
    /*
    lua_createtable(L,3,0);
    lua_pushinteger(L,1); lua_pushnumber(L,center[0]); lua_settable(L,-3);
    lua_pushinteger(L,2); lua_pushnumber(L,center[1]); lua_settable(L,-3);
    lua_pushinteger(L,3); lua_pushnumber(L,center[2]); lua_settable(L,-3);
    */
    lua_pushnumber(L,center[0]);
    lua_pushnumber(L,center[1]);
    lua_pushnumber(L,center[2]);

    return 4;
}

// boudingbox
static int lnef_bbox(lua_State *L)
{
    lnef_userdata_t *cu;

    cu    = (lnef_userdata_t *)luaL_checkudata(L, 1, "LNef");

    double box[6];
    cu->n->bbox(box);

    for(int i=0;i<6;i++) lua_pushnumber(L,box[i]);
    return 6;
}

static int lnef_regularize(lua_State *L)
{
    lnef_userdata_t *cu;
             
    cu     = (lnef_userdata_t *)luaL_checkudata(L, 1, "LNef");

    cu->n->regularize();
    return 0;
}


// update the current nef...
static int lnef_translate(lua_State *L)
{
    lnef_userdata_t *cu;
             
    cu     = (lnef_userdata_t *)luaL_checkudata(L, 1, "LNef");
    luaL_checktype(L, 2,LUA_TTABLE);

    lua_getfield(L, 2, "x");
    lua_getfield(L, 2, "y");
    lua_getfield(L, 2, "z");

    double tx = lua_tonumber(L, -3);
    double ty = lua_tonumber(L, -2);
    double tz = lua_tonumber(L, -1);

    cu->n->translate(tx,ty,tz);
    return 1;
}



static int lnef_rotate(lua_State *L)
{
    lnef_userdata_t *cu;
             
    cu     = (lnef_userdata_t *)luaL_checkudata(L, 1, "LNef");
    luaL_checktype(L, 2,LUA_TTABLE);

    lua_getfield(L, 2, "x");
    lua_getfield(L, 2, "y");
    lua_getfield(L, 2, "z");

    double rx = lua_tonumber(L, -3);
    double ry = lua_tonumber(L, -2);
    double rz = lua_tonumber(L, -1);

    std::cout << "rotate "<<rx<<","<<ry<<","<<rz<<"\n";

    cu->n->rotate(rx,ry,rz);
    return 0;
}


static int lnef_scale(lua_State *L)
{
    lnef_userdata_t *cu;
             
    cu     = (lnef_userdata_t *)luaL_checkudata(L, 1, "LNef");
    luaL_checktype(L, 2,LUA_TTABLE);

    lua_getfield(L, 2, "x");
    lua_getfield(L, 2, "y");
    lua_getfield(L, 2, "z");

    double sx = lua_isnil(L,-3)?1.0:lua_tonumber(L,-3);
    double sy = lua_isnil(L,-2)?1.0:lua_tonumber(L,-2);
    double sz = lua_isnil(L,-1)?1.0:lua_tonumber(L,-1);

    cu->n->scale(sx,sy,sz);
    return 0;
}

// union{a,b,c,d,e,...} union de tout les elements
static int lnef_hull1(lua_State *L)
{
    lnef_userdata_t *cu,*ecu;

    luaL_checktype(L, 1,LUA_TTABLE);
    lua_len(L,-1);
    int len=lua_tointeger(L,-1);
    lua_pop(L,1);
    std::cout << "nb param is "<<len<<"\n";

    cu       = (lnef_userdata_t *)lua_newuserdata(L, sizeof(*cu));
    cu->n    = new nef();
    cu->num  = nefnum++;

    luaL_getmetatable(L, "LNef");
    lua_setmetatable(L, -2);

    std::vector<nef *> obj;
    for(int i=1;i<=len;i++) {
        lua_rawgeti(L,1,i);
        ecu = (lnef_userdata_t *)luaL_checkudata(L, -1, "LNef");
        lua_pop(L,1);
        obj.push_back(ecu->n);
    }
    cu->n->hull(obj);
    std::cout<<"NEF HULl "<<cu->num<<"\n";
    std::cout << "sz="<<obj.size()<<"\n";
    return 1;
}


// union{a,b,c,d,e,...} union de tout les elements
static int lnef_union1(lua_State *L)
{
    lnef_userdata_t *cu,*ecu;

    luaL_checktype(L, 1,LUA_TTABLE);
    lua_len(L,-1);
    int len=lua_tointeger(L,-1);
    lua_pop(L,1);
    std::cout << "nb param is "<<len<<"\n";

    cu       = (lnef_userdata_t *)lua_newuserdata(L, sizeof(*cu));
    cu->n    = new nef();
    cu->num  = nefnum++;

    luaL_getmetatable(L, "LNef");
    lua_setmetatable(L, -2);

    for(int i=1;i<=len;i++) {
        lua_rawgeti(L,1,i);
        ecu = (lnef_userdata_t *)luaL_checkudata(L, -1, "LNef");
        lua_pop(L,1);
        cu->n->myunion(*(ecu->n));
    }
    return 1;
}

// union(a) -> retourne un nouveau nef
// on pourrait aussi avoir :union(..)
static int lnef_difference1(lua_State *L)
{
    lnef_userdata_t *cu,*ecu;

    luaL_checktype(L, 1,LUA_TTABLE);
    lua_len(L,-1);
    int len=lua_tointeger(L,-1);
    lua_pop(L,1);
    std::cout << "nb param is "<<len<<"\n";

    cu       = (lnef_userdata_t *)lua_newuserdata(L, sizeof(*cu));
    cu->n    = new nef();
    cu->num  = nefnum++;

    luaL_getmetatable(L, "LNef");
    lua_setmetatable(L, -2);

    for(int i=1;i<=len;i++) {
        lua_rawgeti(L,1,i);
        ecu = (lnef_userdata_t *)luaL_checkudata(L, -1, "LNef");
        lua_pop(L,1);
        if( i==1 ) cu->n=new nef(*(ecu->n));
        else cu->n->difference(*(ecu->n));
    }
    return 1;
}

// union(a) -> retourne un nouveau nef
// on pourrait aussi avoir :union(..)
static int lnef_inter1(lua_State *L)
{
    lnef_userdata_t *cu,*ecu;

    luaL_checktype(L, 1,LUA_TTABLE);
    lua_len(L,-1);
    int len=lua_tointeger(L,-1);
    lua_pop(L,1);
    std::cout << "nb param is "<<len<<"\n";

    cu       = (lnef_userdata_t *)lua_newuserdata(L, sizeof(*cu));
    cu->n    = new nef();
    cu->num  = nefnum++;

    luaL_getmetatable(L, "LNef");
    lua_setmetatable(L, -2);

    for(int i=1;i<=len;i++) {
        lua_rawgeti(L,1,i);
        ecu = (lnef_userdata_t *)luaL_checkudata(L, -1, "LNef");
        lua_pop(L,1);
        if( i==1 ) cu->n=new nef(*(ecu->n));
        else cu->n->intersection(*(ecu->n));
    }
    return 1;
}



// __add(a,b) -> retourne un nouveau nef
// on pourrait aussi avoir :union(..)
static int lnef_union(lua_State *L)
{
    lnef_userdata_t *cu,*cu1,*cu2;
             
    cu1     = (lnef_userdata_t *)luaL_checkudata(L, 1, "LNef");
    cu2     = (lnef_userdata_t *)luaL_checkudata(L, 2, "LNef");

    cu       = (lnef_userdata_t *)lua_newuserdata(L, sizeof(*cu));
    luaL_getmetatable(L, "LNef");
    lua_setmetatable(L, -2);
                                         
    /* Create the data that comprises the userdata (the nef). */
    cu->n    = new nef();
    cu->n->myunion(*(cu1->n),*(cu2->n));
    cu->num  = nefnum++;

    std::cout << "union ("<<cu1->num<<" + "<<cu2->num<<") -> nef "<<cu->num<<"\n";

    return 1;
}

// __sub(a,b) -> retourne un nouveau nef
// on pourrait aussi avoir :union(..)
static int lnef_difference(lua_State *L)
{
    lnef_userdata_t *cu,*cu1,*cu2;
             
    cu1     = (lnef_userdata_t *)luaL_checkudata(L, 1, "LNef");
    cu2     = (lnef_userdata_t *)luaL_checkudata(L, 2, "LNef");

    cu       = (lnef_userdata_t *)lua_newuserdata(L, sizeof(*cu));
    luaL_getmetatable(L, "LNef");
    lua_setmetatable(L, -2);

    cu->n   = new nef();
    cu->n->difference(*(cu1->n),*(cu2->n));
    cu->num  = nefnum++;

    std::cout << "dif ("<<cu1->num<<" - "<<cu2->num<<") -> nef "<<cu->num<<"\n";
    return 1;
}

// __mul(a,b) -> retourne un nouveau nef
// on pourrait aussi avoir :union(..)
static int lnef_intersection(lua_State *L)
{
    lnef_userdata_t *cu,*cu1,*cu2;
             
    cu1     = (lnef_userdata_t *)luaL_checkudata(L, 1, "LNef");
    cu2     = (lnef_userdata_t *)luaL_checkudata(L, 2, "LNef");

    cu       = (lnef_userdata_t *)lua_newuserdata(L, sizeof(*cu));
    luaL_getmetatable(L, "LNef");
    lua_setmetatable(L, -2);

    // the new nef

    cu->n   = new nef();
    cu->n->intersection(*(cu1->n),*(cu2->n));
    cu->num  = nefnum++;

    std::cout << "inter ("<<cu1->num<<" * "<<cu2->num<<") -> nef "<<cu->num<<"\n";
    return 1;
}

 
static int lnef_destroy(lua_State *L)
{
    lnef_userdata_t *cu;
         
    cu = (lnef_userdata_t *)luaL_checkudata(L, 1, "LNef");

    if (cu->n != NULL) {
        std::cout<<"destroy nef "<<cu->num<<"\n";
        delete cu->n;
        cu->n = NULL;
        cu->num=-1;
    }
                     
    return 0;
}
#endif
 
/*
static int lnef_tostring(lua_State *L)
{
    lnef_userdata_t *cu;
         
    cu = (lnef_userdata_t *)luaL_checkudata(L, 1, "LNef");
             
    lua_pushfstring(L, "num=%d", cu->num);
                 
    return 1;
}
*/
 
static const struct luaL_Reg lopencsg_functions[] = {
    { "init",       lopencsg_init },
    { "uninit",     lopencsg_uninit },
    { "clear",      lopencsg_clear },
    { "newProduct", lopencsg_newProduct },
    { "draw",       lopencsg_draw },
    { "render",     lopencsg_render },
    { NULL,  NULL         }
};
 

int luaopen_opencsg(lua_State *L)
{
    printf("opencsg library loaded!\n");
#ifndef USE_OPENCSG
    luaL_error(L, "compilation was done without OpenCSG. Sorry.");
#endif
    printf("Using OpenCSG!\n");
    /* Create the metatable and put it on the stack. */
    luaL_newmetatable(L, "LOpenCSG");
    /* Duplicate the metatable on the stack (We know have 2). */
    lua_pushvalue(L, -1);
    /* Pop the first metatable off the stack and assign it to __index
     * of the second one. We set the metatable for the table to itself.
     * This is equivalent to the following in lua:
     * metatable = {}
     * metatable.__index = metatable
     */
    lua_setfield(L, -2, "__index");
                 
    /* Set the methods to the metatable that should be accessed via object:func */
    //luaL_setfuncs(L, lopencsg_methods, 0);
                     
    /* Register the object.func functions into the table that is at the top of the
     * stack. */
    luaL_newlib(L, lopencsg_functions);

    return 1;
}


