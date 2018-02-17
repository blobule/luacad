--
-- test the demonstrate all primitives
--

nef=require("nef")

--
-- the only function is new(), which generates an empty nef
-- the only way to get a nef is:
--    - use nef.new()
--    - use dup() to duplicate another nef
--    - use binary operators (+/-/*)
--
-- all other operations change the current nef
--
a=nef.null()
b=a:dup()
c=nef.null()

--
-- Primitives
-- (notice that arguments are a table, so parameters are optional)
-- 
-- in general:
--      n is number of divisions
--      triangles (0/1) specify to use triangles or quads
--
--      ref=
--
a=nef.cube{x=1,y=1,z=1}
b=nef.sphere{r=1,n=8,triangles=0}
c=nef.cylinder{r=1,h=1,n=8,triangles=0}

--collectgarbage()

--
-- I/O
--
a:export{name="yo-a.stl",binary=0}
b:export{name="yo-b.stl",binary=1}
c:export{name="yo-c.stl",binary=1}

d=nef.import{name="yo-b.stl"}
d:export{name="yo-bb.stl"}

-- en format interne exact
a:save{name="yo-a.off"}
a=nef.load{name="yo-a.off"}


--
-- Info
--
print("bbox is ",a:bbox())  -- xmin,xmax,ymin,ymax,zmin,zmax
print("volume and center of mass are ",a:volume()) -- vol,cx,cy,cz
print("info is ",a:info()) -- valid,simple,volumes


--
--
-- transformations
--
a:translate{x=1}
a:rotate{x=90}
a:scale{z=1}

--
--
-- operations
--
e=a+b
f=a-b
g=a*b

e:export{name="yo-e.stl"}
f:export{name="yo-f.stl"}
g:export{name="yo-g.stl"}

-- on peut aussi executer "in place", mais so ca brise la cache
--a:union(b)
--a:intersection(b)
--a:difference(b)


a=nef.load{name="yoyo.off"}
a:save{name="out.off"}


