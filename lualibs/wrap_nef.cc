
#include <stdlib.h>
#include <string.h>
 
 
#include "nef.h"
#include "nef2.h"

extern "C" {
    #include <lua.h>
    #include <lauxlib.h>
    int luaopen_nef(lua_State *L);
}

static int nefnum=0;
 

static int lnef_null(lua_State *L)
{
    lnef_userdata_t *cu;

    std::cout<<"NEF NULL "<<nefnum<<"\n";
             
    cu       = (lnef_userdata_t *)lua_newuserdata(L, sizeof(*cu));
    cu->n    = new nef();
    cu->num  = nefnum++;
                                 
    luaL_getmetatable(L, "LNef");
    lua_setmetatable(L, -2);
    return 1;
}


// duplicate another nef
static int lnef_dup(lua_State *L)
{
    lnef_userdata_t *cu;
    lnef_userdata_t *newcu;

    cu  = (lnef_userdata_t *)luaL_checkudata(L, 1, "LNef");
                             
    newcu       = (lnef_userdata_t *)lua_newuserdata(L, sizeof(*cu));
    luaL_getmetatable(L, "LNef");
    lua_setmetatable(L, -2);
                                         
    newcu->n    = new nef(*(cu->n));
    newcu->num  = nefnum++;

    std::cout<<"NEF DUP "<<newcu->num<<" from nef "<<cu->num<<"\n";
    return 1;
}

// polyhedron from vertices and triangular facets
// {points={...},faces={...},convexity=3}
// points are flat array of coords, faces are flat array of indexed of triangles
static int lnef_polyhedron(lua_State *L)
{
    lnef_userdata_t *cu;

    luaL_checktype(L, 1,LUA_TTABLE);
    lua_getfield(L, 1, "points"); // -2
    lua_getfield(L, 1, "faces"); // -1

    int nbV=(int)lua_rawlen(L,-2);
    int nbF=(int)lua_rawlen(L,-1);

    float *vertices=(float *)malloc(nbV*sizeof(float));
    int *faces=(int *)malloc(nbF*sizeof(int));

    for(int i=0;i<nbV;i++) {
        lua_rawgeti(L,-2,i+1);
        vertices[i] = lua_tonumber(L, -1);
        lua_pop(L,1);
    }

    for(int i=0;i<nbF;i++) {
        lua_rawgeti(L,-1,i+1);
        faces[i] = lua_tointeger(L, -1);
        lua_pop(L,1);
    }

    cu       = (lnef_userdata_t *)lua_newuserdata(L, sizeof(*cu));
    cu->n    = new nef();
    cu->num  = nefnum++;

    luaL_getmetatable(L, "LNef");
    lua_setmetatable(L, -2);

    std::cout<<"NEF POLYHEDRON nbV="<<nbV<<" nbF="<<nbF<<"\n";

    //cu->n->cube(1.0,1.0,1.0);
    cu->n->polyhedron(vertices,faces,nbV,nbF);
    free(vertices);
    free(faces);
    std::cout<<"done polyhedron\n";
    return 1;
}

// setup nef as a cube
static int lnef_cube(lua_State *L)
{
    lnef_userdata_t *cu;

    luaL_checktype(L, 1,LUA_TTABLE);
    lua_getfield(L, 1, "x");
    lua_getfield(L, 1, "y");
    lua_getfield(L, 1, "z");
    lua_getfield(L, 1, "center");

    double x = lua_isnil(L,-4)?1.0:lua_tonumber(L, -4);
    double y = lua_isnil(L,-3)?1.0:lua_tonumber(L, -3);
    double z = lua_isnil(L,-2)?1.0:lua_tonumber(L, -2);
    int center = lua_toboolean(L, -1);

    cu       = (lnef_userdata_t *)lua_newuserdata(L, sizeof(*cu));
    cu->n    = new nef();
    cu->num  = nefnum++;

    luaL_getmetatable(L, "LNef");
    lua_setmetatable(L, -2);

    std::cout<<"NEF CUBE "<<x<<"x"<<y<<"x"<<z<<" c"<<center<<" : "<<cu->num<<"\n";

    cu->n->cube(x,y,z,center);
    //std::cout<<"done cube\n";
    return 1;
}

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


// setup nef as a cylinder
static int lnef_cylinder(lua_State *L)
{
    lnef_userdata_t *cu;

    luaL_checktype(L, 1,LUA_TTABLE);
    lua_getfield(L, 1, "triangles");
    lua_getfield(L, 1, "h");
    lua_getfield(L, 1, "r1");
    lua_getfield(L, 1, "r2");
    lua_getfield(L, 1, "n");

    int tri = lua_tointeger(L, -5);
    double h = lua_isnil(L,-4)?1.0:lua_tonumber(L, -4);
    double r1 = lua_isnil(L,-3)?1.0:lua_tonumber(L, -3); 
    double r2 = lua_isnil(L,-2)?1.0:lua_tonumber(L, -2);
    int n = lua_isnil(L,-1)?8:lua_tointeger(L, -1);

    if( n<3 ) n=3;
    if( tri!=0 ) tri=1;

    cu       = (lnef_userdata_t *)lua_newuserdata(L, sizeof(*cu));
    cu->n    = new nef();
    cu->num  = nefnum++;

    luaL_getmetatable(L, "LNef");
    lua_setmetatable(L, -2);

    std::cout<<"NEF CYLINDER "<<cu->num<<"\n";

    cu->n->cylinder(r1,r2,h,n,tri);
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
// transform({{1,2,3,4},{5,6,7,8},{9,10,11,12},{13,14,15,16}})
static int lnef_transform(lua_State *L)
{
    lnef_userdata_t *cu;

    cu     = (lnef_userdata_t *)luaL_checkudata(L, 1, "LNef");
    luaL_checktype(L, 2,LUA_TTABLE);

    lua_len(L,2);
    int len=lua_tointeger(L,-1);
    lua_pop(L,1);
    //std::cout << "len is "<<len<<"\n";
    if(len!=12) { luaL_error(L, "transform: expected 12 numbers"); }
    double affine[12];
    for(int i=0;i<len;i++) {
        lua_rawgeti(L,2,i+1);
        affine[i] = lua_tonumber(L, -1);
        lua_pop(L,1);
        //std::cout << "number "<<i<<" is "<<affine[i]<<"\n";
    }
    cu->n->transform(affine);
    return 0;
}


//
// return the polygon data of this nef
// {points={},faces={}}=poly(nef)
//
static int lnef_poly(lua_State *L)
{
    lnef_userdata_t *cu;

    cu     = (lnef_userdata_t *)luaL_checkudata(L, 1, "LNef");

    float *vertices=NULL;
    int *facets=NULL;
    int nbV,nbF;

    cu->n->renderGL(vertices,facets,nbV,nbF);
    printf("%d V and %d F\n",nbV,nbF);

    lua_createtable(L,0,2); // {points and faces} -4

    lua_pushstring(L,"points"); // [nbV*3] -3
    lua_createtable(L,nbV*3,0); // -2
    for(int i=0;i<nbV*3;i++) {
        lua_pushnumber(L,vertices[i]);
        lua_rawseti(L,-2,i+1);
    }
    lua_rawset(L,-3); // {points=...

    lua_pushstring(L,"faces"); // [nbf*3] -3
    lua_createtable(L,nbF*3,0); // -2
    for(int i=0;i<nbF*3;i++) {
        lua_pushinteger(L,facets[i]+1); // +1 pour array qui commence a 1 en lua
        lua_rawseti(L,-2,i+1);
    }
    lua_rawset(L,-3); // {faces=...

    if( vertices ) free(vertices);
    if( facets ) free(facets);

    return 1;
}

#if 0
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
    return 0;
}
#endif

#if 0
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
#endif


#if 0
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
#endif

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
 
/*
static int lnef_tostring(lua_State *L)
{
    lnef_userdata_t *cu;
         
    cu = (lnef_userdata_t *)luaL_checkudata(L, 1, "LNef");
             
    lua_pushfstring(L, "num=%d", cu->num);
                 
    return 1;
}
*/
 
static const struct luaL_Reg lnef_methods[] = {
    { "save",        lnef_save      },
    { "export",      lnef_export    },
    { "info",        lnef_info      },
    { "volume",      lnef_volume    },
    { "bbox",        lnef_bbox      },
    { "dup",         lnef_dup       },
    { "regularize",  lnef_regularize },
//    { "rotate",      lnef_rotate },
//    { "scale",       lnef_scale },
//    { "translate",   lnef_translate },
    { "transform",   lnef_transform },
    { "poly",        lnef_poly },
    { "__gc",        lnef_destroy   },
    { "__add",       lnef_union     },
    { "__sub",       lnef_difference },
    { "__mul",       lnef_intersection },
 //   { "__tostring",  lnef_tostring  },
    { NULL,          NULL               },
};
 
static const struct luaL_Reg lnef_functions[] = {
    { "null",       lnef_null },
    { "polyhedron", lnef_polyhedron },
    { "cube",       lnef_cube },
    { "sphere",     lnef_sphere },
    { "cylinder",   lnef_cylinder },
    { "load",       lnef_load },
    { "import",     lnef_import },
    { "hull",       lnef_hull1 },
    { "union",      lnef_union1 },
    { "intersection",lnef_inter1    },
    { "difference",  lnef_difference1    },
    { NULL,  NULL         }
};
 
int luaopen_nef(lua_State *L)
{
    printf("nef library loaded!\n");
    /* Create the metatable and put it on the stack. */
    luaL_newmetatable(L, "LNef");
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
    luaL_setfuncs(L, lnef_methods, 0);
                     
    /* Register the object.func functions into the table that is at the top of the
     * stack. */
    luaL_newlib(L, lnef_functions);

    // definir cube comme la fonction cube, global
    /*
    lua_register(L,"cube",lnef_cube);
    lua_register(L,"sphere",lnef_sphere);
    lua_register(L,"cylinder",lnef_cylinder);

    lua_register(L,"hull",lnef_hull1);
    lua_register(L,"union",lnef_union1);
    lua_register(L,"intersection",lnef_inter1);
    lua_register(L,"difference",lnef_difference1);
    */

    return 1;
}


