
--
-- lua cad
--
-- transform rules


-- 1. x – (y ∪ z ) → (x – y) – z         -> x-y-z
-- 2. x ∩ (y ∪ z) → (x ∩ y) ∪ (x ∩ z)   -> x*y, x*z
-- 3. x – (y ∩ z) → (x – y) ∪ (x – z)   -> x-y, x-z
-- 4. x ∩ (y ∩ z) → (x ∩ y) ∩ z         -> x*y*z
-- 5. x – (y – z) → (x – y) ∪ (x ∩ z)   -> x-y, x*z
-- 6. x ∩ (y – z) → (x ∩ y) – z         -> x*y-z
-- 7. (x – y) ∩ z → (x ∩ z) – y         -> x*z-y
-- 8. (x ∪ y) – z → (x – z) ∪ (y – z)   -> x-z, y-z
-- 9. (x ∪ y) ∩ z → (x ∩ z) ∪ (y ∩ z)   -> x*z, y*z
--
-- 1. x-(y+z) -> (x-y)-z                -{x,+{y,z}} -> -{-{x,y},z}
-- 3. x-(y*z) -> (x-y)+(x-z)            -{x,*{y,z}} -> +{-{x,y},-{x,z}}
-- 5. x-(y-z) -> (x-y)+(x*z)            -{x,-{y,z}} -> +{-{x,y},*{x,z}}
-- 2. x*(y+z) -> (x*y)+(x*z)            *{x,+{y,z}} -> +{*{x,y},*{x,z}}
-- 4. x*(y*z) -> (x*y)*z                *{x,*{y,z}} -> *{*{x,y},z}
-- 6. x*(y-z)) -> (x*y)-z               *{x,-{y,z}} -> -{*{x,y},z}
-- 7. (x-y)*z -> z*(x-y) -> (x*z)-y     *{-{x,y},z} -> -{*{x,z},y}
-- 8. (x+y)-z -> (x-z)+(y-z)            -{+{x,y},z} -> +{-{x,z},-{y,z}}
-- 9. (x+y)*z -> (x*z)+(y*z)            *{+{x,y},z} -> +{*{x,z},*{y,z}}
--
-- 10. +{x,y,z,...} -> +{+{x,y},z}...}}}}}
-- 11. *{x,y,z,...} -> *{*{x,y},z,...}}}}}
-- 12. -{x,y,z,...} =  -{-{x,y},z,...}}}}}
-- 
--
-- format actuel:
-- union{a1,a2,a3,...           -> a1+a2+a3+...
-- intersection{a1,a2,a3,...}   -> a1*a2*a3*...
-- difference{a1,a2,a3,...}     -> a1-a2-a3-...
--
-- x - union(y,z,...) -> x-y-z-...
-- x * union(y,z,...) -> x*y, x*z, x*...
--
-- x - intersection(y,z,...) -> x-y, x-z, x-...
-- x * intersection(y,z,...) -> x*y*z*...
-- x - difference(y,z,p) -> x- (y-z) - (y*p)

--
-- utilise seulement union/intersection/difference
-- tout le reste est une primitive dans ce cas-ci
--
-- 1. difference { x , union { y,z } } -> difference { difference { x , y } , z }     
--
-- a la fin on veut une list d'operation du genre
-- union { ... }
-- ou chaque element est un A [-|*] B [-|*] C ...
--

--
-- support --
--

function indent(level)
    for i=1,(level or 0) do io.write(". ") end
end

function combine(op,a,b)
    --print("combine ",op,a,b)
    if getmetatable(a)=="geometry" and getmetatable(b)=="geometry" then
        return op{a,b}
    else
        print(debug.traceback("Mauvaise operation geometrique entre "..tostring(a).." et "..tostring(b),3))
        os.exit()
    end
end


function readonlytable(table)
    return setmetatable({}, {
        __index = table,
        __newindex =    function(table, key, value) print("Attempt to modify read-only table") end,
        __metatable = "geometry",
        __len = function() return #table end,
        __tostring = function() return tostring(table) end,
        __add = function(a,b) return combine(union(),a,b) end,
        __mul = function(a,b) return combine(intersection(),a,b) end,
        __sub = function(a,b) return combine(difference(),a,b) end
    });
end

-- tout ce qui est un parametre avec une valeur autre que fonction
function printparam(t)
    s="{"
    for key, val in pairs(t) do
        if type(key)=="string" and type(val)~="function" then s=s..key.."="..tostring(val).."," end
    end
    return s:sub(1,-2).."}"
end

-- conserve seulement les parametres (cles) et enleve les elements
function params(t)
    local n = #t
    local newt={}
    for k,v in pairs(t) do
        if type(k) == "string" and type(v)~="function" then newt[k]=v end
    end
    return newt
end

-- ajoute a base tous les itemms sans cle de extra
-- flatten les table ordinaires
function merge(base,extra)
    local j=#base
    for i=1,#extra do
        --print("merge",i,#extra,getmetatable(extra[i]))
        local z=extra[i]
        if type(z)=="table" and getmetatable(z)=="geometry" then
            base[#base+1]=z
        else
            -- permet {sphere{}} ou {{{sphere{}}}}
            merge(base,extra[i])
        end
    end
end


--
-- vec{4,5,6}  est une simple table {4,5,6} avec un tostring specifique
--
function vec(a) return setmetatable(a,{
    __tostring = function (v) 
            local s="{"
            for i=1,#v do s=s..tostring(v[i])..(i==#v and "" or ",") end
            return s.."}"
        end
    })
end

--
-- mat{{1,2},{3,4}} est une simple table de table avec un tostring specifique
--
function mat(a) return setmetatable(a,{
    __tostring = function(v)
            local s="{"
            for i=1,#v do s=s..tostring(vec(v[i]))..(i==#v and "" or ",") end
            return s.."}"
    end
    })
end


--sphere{r=56}
--sphere{}{param=r,from=10,to=100,step=5}


--
-- classes generiques --
--

--
-- une primitve, sans children
--
-- primitve("sphere",{r=10,center=true}
--
function primitive(nom,arg)
    local self=arg -- normalement on verifie les argument et ajuste les defaut
    self.primitive=nom
    self.dump = function(level) indent(level) print(self) end
    self.infix = function(prev) print(prev..tostring(self)) end
    self.render = function()
        print("render primitve! "..self.primitive)
    end
    setmetatable(self,{
        __tostring = function() return self.primitive..printparam(self) end
    })
    return readonlytable(self)
end

--
-- un operateur, avec children
--
-- operator("intersection",{convexity=3}){ child1, child2, child3, ... }
--
-- this operator is a
--
function operator(nom,arg)
    local self=params(arg) -- on garde seulement les parametres... et on fait une table locale
    self.operator=nom
    self.dump = function(level)
        local s
        if self.operator=="union" then s="+"
        elseif self.operator=="difference" then s="-"
        elseif self.operator=="intersection" then s="*"
        else s=self.operator..printparam(self)
        end
        indent(level) print(s.." {")
        local up=(level or 0)+1
        for i=1,#self do self[i].dump(up) end
        indent(level) print("}")
    end
    self.infix = function(prev)
        local s
        if self.operator=="union" then s="+ "
        elseif self.operator=="difference" then s="- "
        elseif self.operator=="intersection" then s="* "
        else s=self.operator..printparam(self).." "
        end
        self[1].infix(prev or "+ ")
        for i=2,#self do self[i].infix(s) end
    end
    self.render = function()
        -- start by rendering all children
        for i=1,#self do self[i].render() end
        print("render operator: ",self.operator)
    end
    setmetatable(self,{
        __tostring = function()
            s=self.operator..printparam(self).." {"
            for i=1,#self do s=s..tostring(self[i])..(i==#self and "" or ",") end
            return s.."}"
        end
    })
    --return readonlytable(self)
    -- if the arg is not a table, then error
    return function (children) merge(self,children) return readonlytable(self) end
end

--
-- rendering
-- opencsg : binarize -> transforme {x,y,z,..} en binaires seulement
--
-- union/intersection/difference -> +{+{+{+{x,y},z,...
-- translate/rotate/scale{x,y,z} -> +{t{x},t{y},t{z}}
-- tout le reste : primitve
-- translate{union{...}}->union{translate{},translate{}...}
--



level=0

function binarize(s)
    --print("binarize level=",level)
    --print(readonlytable(s))
    level=level+1
    if s.operator=="union" or s.operator=="intersection" or s.operator=="difference" then
        if #s==0 then return null{} end
        if #s==1 then return binarize(s[1]) end
        local first=binarize(s[1])
        for i=2,#s do
            first=operator(s.operator,{}){first,binarize(s[i])}
        end
        level=level-1
        return first
    elseif s.operator=="translate" or s.operator=="rotate" or s.operator=="scale" then
        if #s==0 then return null{} end
        local ps=params(s)
        if #s==1 then return operator(s.operator,ps){binarize(s[1])} end
        local first=operator(s.operator,ps){binarize(s[1])}
        for i=2,#s do
            first=operator("union",{}){first,operator(s.operator,ps){binarize(s[i])}}
        end
        level=level-1
        return first
    else
        level=level-1
        return readonlytable(s)
    end
end

--
-- si on a un operateur, il aura max 2 children. (assume binarize deja applique)
--
-- 1. x-(y+z) -> (x-y)-z                -{x,+{y,z}} -> -{-{x,y},z}
-- 3. x-(y*z) -> (x-y)+(x-z)            -{x,*{y,z}} -> +{-{x,y},-{x,z}}
-- 5. x-(y-z) -> (x-y)+(x*z)            -{x,-{y,z}} -> +{-{x,y},*{x,z}}
-- 2. x*(y+z) -> (x*y)+(x*z)            *{x,+{y,z}} -> +{*{x,y},*{x,z}}
-- 4. x*(y*z) -> (x*y)*z                *{x,*{y,z}} -> *{*{x,y},z}
-- 6. x*(y-z)) -> (x*y)-z               *{x,-{y,z}} -> -{*{x,y},z}
-- 7. (x-y)*z -> z*(x-y) -> (x*z)-y     *{-{x,y},z} -> -{*{x,z},y}
-- 8. (x+y)-z -> (x-z)+(y-z)            -{+{x,y},z} -> +{-{x,z},-{y,z}}
-- 9. (x+y)*z -> (x*z)+(y*z)            *{+{x,y},z} -> +{*{x,z},*{y,z}}
--
function normalize(s)
    if #s==0 then return s end
    if #s==1 then return operator(s.operator,params(s)){normalize(s[1])} end
    if s.operator=="difference" then
        if s[2].operator=="union" and s[2][2] then
            -- 1. x-(y+z) -> (x-y)-z
            local x,y,z = normalize(s[1]),normalize(s[2][1]),normalize(s[2][2])
            return (x-y)-z
            --return difference{}{difference{}{s[1],s[2][1]},s[2][2]}
        elseif s[2].operator=="intersection" and s[2][2] then
            -- 3. x-(y*z) -> (x-y)+(x-z)
            local x,y,z = normalize(s[1]),normalize(s[2][1]),normalize(s[2][2])
            return (x-y)+(x-z)
            --return union{}{difference{}{s[1],s[2][1]},difference{}{s[1],s[2][2]}}
        elseif s[2].operator=="difference" and s[2][2] then
            -- 5. x-(y-z) -> (x-y)+(x*z)
            local x,y,z = normalize(s[1]),normalize(s[2][1]),normalize(s[2][2])
            return (x-y)+(x*z)
            --return union{}{difference{}{s[1],s[2][1]},intersection{}{s[1],s[2][2]}}
        elseif s[1].operator=="union" and s[1][2] then
            -- 8. (x+y)-z -> (x-z)+(y-z)
            local x,y,z = normalize(s[1][1]),normalize(s[1][2]),normalize(s[2])
            return (x-z)+(y-z)
            --return union{}{difference{}{s[1][1],s[2]},difference{}{s[1][2],s[2]}}
        end
    elseif s.operator=="intersection" then
        if s[2].operator=="union" and s[2][2] then
            -- 2. x*(y+z) -> (x*y)+(x*z)            *{x,+{y,z}} -> +{*{x,y},*{x,z}}
            local x,y,z = normalize(s[1]),normalize(s[2][1]),normalize(s[2][2])
            return (x*y)+(x*z)
            --return union{}{intersection{}{s[1],s[2][1]},intersection{}{s[1],s[2][2]}}
        elseif s[2].operator=="intersection" and s[2][2] then
            -- 4. x*(y*z) -> (x*y)*z                *{x,*{y,z}} -> *{*{x,y},z}
            local x,y,z = normalize(s[1]),normalize(s[2][1]),normalize(s[2][2])
            return (x*y)*z
            --return intersection{}{intersection{}{s[1],s[2][1]},s[2][2]}
        elseif s[2].operator=="difference" and s[2][2] then
            -- 6. x*(y-z)) -> (x*y)-z               *{x,-{y,z}} -> -{*{x,y},z}
            local x,y,z = normalize(s[1]),normalize(s[2][1]),normalize(s[2][2])
            return (x*y)-z
            --return difference{}{intersection{}{s[1],s[2][1]},s[2][2]}
        elseif s[1].operator=="difference" and s[1][2] then
            -- 7. (x-y)*z -> z*(x-y) -> (x*z)-y     *{-{x,y},z} -> -{*{x,z},y}
            local x,y,z = normalize(s[1][1]),normalize(s[1][2]),normalize(s[2])
            return (x*z)-y
            --return difference{}{intersection{}{s[1][1],s[2]},s[1][2]}
        elseif s[1].operator=="union" and s[1][2] then
            -- 9. (x+y)*z -> (x*z)+(y*z)            *{+{x,y},z} -> +{*{x,z},*{y,z}}
            local x,y,z = normalize(s[1][1]),normalize(s[1][2]),normalize(s[2])
            return (x*z)+(y*z)
            --return union{}{intersection{}{s[1][1],s[2]},intersection{}{s[1][2],s[2]}}
        end
    end
    return operator(s.operator,params(s)){normalize(s[1]),normalize(s[2])}
end


--
-- classes specifiques --
--

function union(a) return operator("union",{}) end
function intersection(a) return operator("intersection",{}) end
function difference(a) return operator("difference",{}) end

function rotate(a) return operator("rotate",{r=(a.r or vec{a.rx or 0,a.ry or 0,a.rz or 0})}) end
function translate(a) return operator("translate",{t=(a.t or vec{a.tx or 0,a.ty or 0,a.tz or 0})}) end
function scale(a) return operator("scale",{}) end
-- hull -> retourne une seule geometry
-- projection(a)
-- linear_extrude (2d)
-- rotate_extrude (2d)
-- resize
-- mirror
-- multmatrix
-- color
-- offset (2d)
-- minkowski
-- render


function sphere(a) return primitive("sphere",{r=a.r or 1}) end  -- d, fa, fs, fn
function cylinder(a) return primitive("cylinder",{h=a.h or 1,r1=a.r1 or a.r or 1,r2=a.r2 or a.r or 1,center=a.center or false}) end -- fa fs fn
function cube(a) return primitive("cube",{size=a.size or vec{1,1,1},center=false}) end
function polyhedron(a) return primitive("polyhedron",{points=a.points or mat{},faces=a.faces or mat{},convexity=a.convexity or 1}) end
function cone(a) return primitive("cone",{h=a.h or 1, r1=a.r1 or 0, r2=a.r2 or 1, center=a.center or false}) end
function null(a) return primitive("null",{}) end

-- square (2d)
-- circle (2d)
-- ellipse (2d)
-- regular polygon (2d)
-- polygon (2d)
-- import
-- text (2d)

--
-- for loop
-- On doit faire un for qui ramasse les items dans une liste 
--
function loop(a)
    local op=a.op or "union"
    local t={}
    for i=(a.from or 1),(a.to or 10),(a.step or 1) do table.insert(t,a.f(i)) end
    return operator(op,{}) (t)
end

--a=loop{op="intersection",from=1,to=10,f=function (i) return sphere{r=i} end}
--a.dump()






