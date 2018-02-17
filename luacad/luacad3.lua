
--
-- lua cad
--
print("===== luacad 3 =====")

nef=nef or require("nef")
cache=cache or require("cache")

--matrix=require("matrix")
matrix=matrix or require("matrix-extra")

require("table-extra")

-- pas de cache sur disque pour le moment
-- (usememcache,usediskcache)
cache.init(true,false)


--md5=require("md5")
--print(md5.sumhexa("blub"))

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

----------------------------------------------------
----------------------------------------------------
--
-- support 
--
----------------------------------------------------
----------------------------------------------------

--
-- flatten a table, removing only pure tables (without metatable)
-- only keep the non-keys
--
function flatten(arr)
	local result = {}
	local function flatten(arr)
		for _,v in ipairs(arr) do
			if type(v) == "table" and not getmetatable(v) then
				flatten(v)
			else
				result[#result+1]=v
			end
		end
	end
	flatten(arr)
	renurn result
end


--
-- return the key/values of an array
-- not the non-keys
-- assume a key is never numeric
-- put the keys into r, or return a new table if r is not provided
--
--
function keyValues(t,r)
	r = r or {}
	for k, v in pairs(t) do
		if type(k) ~= "number" then r[k]=v end
	end
	return r
end

--
--
-- Adds the key-values from take t to the flattened table s
--
--
function merge(params,children)
	return keyValues(params,flatten(children))
end




-- add the elements of t2 at the end of table t1
-- copy since tables are read only
--[[
function tablemerge(t1,t2)
    local t={}
    for k,v in pairs(t1) do t[k]=v end
    for _,v in ipairs(t2) do table.insert(t,v) end
    return t
end
--]]

--
-- merge la table t2 dans la table t1, seulement les elements array
--[[
function itablemerge(t1,t2)
    for _,v in ipairs(t2) do table.insert(t1,v) end
end
--]]

-- copy the keys of t1 to t2
--[[
function copykeys(keys,t1,t2)
    for _,v in ipairs(keys) do
        t2[v]=t1[v]
    end
    return t2
end
--]]


-- transforme une matrix 4x4 en vecteur de 12 elements
function mat4x4flat(m)
    return {
        m[1][1],m[1][2],m[1][3],m[1][4],
        m[2][1],m[2][2],m[2][3],m[2][4],
        m[3][1],m[3][2],m[3][3],m[3][4]}
end



function indent(level)
    for i=1,(level or 0) do io.write("  ") end
end

--[[
function combine(op,a,b)
    --print("combine ",op,a,b)
    if getmetatable(a)=="geometry" and getmetatable(b)=="geometry" then
        return operator(op,{},{a,b})
    else
        print(debug.traceback("Mauvaise operation geometrique entre "..tostring(a).." et "..tostring(b),3))
        os.exit()
    end
end
--]]


function readonlytable(table)
    return setmetatable({}, {
        __index = table,
        __newindex =    function(table, key, value) print("Attempt to modify read-only table (key="..key..",val="..tostring(value)) end,
        __metatable = "geometry",
        __len = function() return #table end,
        __ipairs = function() return ipairs(table) end,
        __pairs = function() return pairs(table) end,
        __tostring = function() return tostring(table) end,
        __add = function(a,b) return combine("union",a,b) end,
        __mul = function(a,b) return combine("intersection",a,b) end,
        __sub = function(a,b) return combine("difference",a,b) end
    });
end

-- tout ce qui est un parametre avec une valeur autre que fonction
-- saute aussi 'primitive' et 'operator" qui sont reserves
function printparam(t)
    local s=""
    for k,v in pairs(keyValues(t)) do
		-- on reuse seulement les cles qui commencent par "_"
		if type(k)=="string" and k:sub(1,1)~="_" and type(v)~="function" then
        	s=s..k.."="..tostring(v)..","
		end
    end
    return "{"..s:sub(1,-2).."}"
end

-- conserve seulement les parametres (cles) et enleve les elements
-- chaque operateur a une liste de parametres valides (keys)
function params(t,keys)
    --[[
    local n = #t
    local newt={}
    for k,v in pairs(t) do
        if type(k) == "string" and type(v)~="function" then newt[k]=v end
    end
    return newt
    --]]
    if not keys then print("parms with no keys!!!") error("yo") end
    local newt={}
    for k,v in ipairs(keys) do
        newt[v]=t[v]
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


--[[
--
-- vector{4,5,6}  est une simple table {4,5,6} avec un tostring specifique
--
function vector(a) return setmetatable(a,{
h    __tostring = function (v) 
            local s="{"
            for i=1,#v do s=s..tostring(v[i])..(i==#v and "" or ",") end
            return s.."}"
        end
    })
end

--
-- matrix{{1,2},{3,4}} est une simple table de table avec un tostring specifique
--
function matrix(a) return setmetatable(a,{
    __tostring = function(v)
            local s="{"
            for i=1,#v do s=s..tostring(vector(v[i]))..(i==#v and "" or ",") end
            return s.."}"
    end
    })
end
--]]


--sphere{r=56}{}
--sphere{}{param=r,from=10,to=100,step=5}


--
-- classes generiques --
--

--
-- une primitve, sans children
--
-- primitve("sphere",{'r',d'},{r=10,center=true})
-- keys are the official parameters (used to compute slug and print)
--
--
function primitiveOLD(nom,keys,args)
    local self=params(args,keys) -- normalement on verifie les argument et ajuste les defaut
    self.primitive=nom
    self.keys=keys
    -- render va retourner le nef, mais aussi le hash
    self.render=function()
        --print("render "..self.primitive.." always render")
        if not self.hash then print("MAKE HASH!");self.computehash() end
        --print("primitive hash "..self.hash)
        local n=cache.lookup(self.hash)
        if n then
            --print("primi found "..self.hash.." in cache")
        else
            --print("primi not found "..self.hash.." in cache, rendering")
            n=rendering[nom](self)
            cache.set(self.hash,n)
        end
        return n,self.hash
    end
    self.computehash = function() self.hash=cache.hash(self.primitive..printparam(self)) end
    self.dump = function(level) indent(level) print(self) end
    self.infix = function(op)
        --print("infix prim ... ",op,self.primitive)
        n,h=self.render()
        --print(op..tostring(self))
        return {{op=op,nef=n,hash=h}}
    end
    setmetatable(self,{
        __tostring = function() return self.primitive..printparam(self) end
    })
    return readonlytable(self)
end

--
-- un operateur, avec children
--
-- operator("intersection",{convexity=3},{child1, child2, child3,... })
-- operator("intersection",{convexity=3,child,child,child})
--
-- in the first form, the final operator keep keys from params and non keys from childrens
-- in the second form, the final operator keep param as is (keys and non keys)
--
function operator(nom,params,children)
    local self={}
    if children then
        -- keep key-values from params
        -- keep flattened non-keys from children
        self=merge(params,children)
    else
        -- keep key-values and non-keys from params
        self=flatten(params)
    end
    self.operator=nom
    self.render=function(tryonly)
        --print("render "..self.operator.." tryonly=",tryonly)
        if not self.hash then print("MAKE HASH!");self.computehash() end
        -- with tryonly true, check the cache but return nil if nothing is available
        -- with tryonly false or nil, render recursively everything.
        -- print("operator slug "..self.hash)
        local n=cache.lookup(self.hash)
        if not n then
            if tryonly then
                return nil
            end
            --print("oper not found "..self.hash.." in cache, rendering")
            -- get the children rendered
            local childs={}
            for i=1,#self do
                n=self[i].render(false)
                childs[#childs+1]=n
            end -- normalement seulement deux enfant, mais ca marche quand meme
            if not rendering[nom] then error("dont know how to render "..nom) end
            n=rendering[nom](self,childs)
            if not n then print("GOT a nil nef!!!") end
            cache.set(self.hash,n)
        end
        return n,self.hash
    end
    self.computehash = function()
        local s=self.operator..printparam(self).."{"
        for i=1,#self do self[i].computehash();s=s..self[i].hash.."," end
        self.hash=cache.hash(s.."}")
    end
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
        indent(up) print("}")
    end
    self.infix = function(op)
        local s
        local try=true
        if self.operator=="union" then s="+"
        elseif self.operator=="difference" then s="-"
        elseif self.operator=="intersection" then s="*"
        else
            -- we will ask for a full render
            try=false
        end

        n,h=self.render(try) -- try only for +/-/* , do not render if not in cache
        if n then
            --print("infix already in cache")
            --print(op..tostring(self).."  (in cache)")
            return {{op=op,nef=n,hash=h}}
        end
        -- we are +/-/* and not in cache, so we add to the product
        local display=self[1].infix(op)
        for i=2,#self do
            local ttt=self[i].infix(s)
            --print("infix merging display sz ",#display," with child ",i," len ",#ttt)
            display[#display+1]=ttt
            --print("infix result table is ",#display)
        end -- normalement seulement deux enfant, mais ca marche quand meme
        return flatten(display)
    end
    setmetatable(self,{
        __tostring = function()
            local s=self.operator..printparam(self).." {"
            for i=1,#self do s=s..tostring(self[i])..(i==#self and "" or ",") end
            return s.."}"
        end
    })
    return readonlytable(self)
end

--
-- rendering
-- binarize -> transforme {x,y,z,..} en binaires seulement
--
-- union/intersection/difference -> +{+{+{+{x,y},z,...
-- translate/rotate/scale{x,y,z} -> +{t{x},t{y},t{z}}
-- tout le reste : primitve
-- translate{union{a,b,c,d}}->union{translate{a},union{translate{b},union{translate{c},d}}}

--level=0

function binarize(s)
    print("binarize",s)
    print("   bin op=",s.operator," len=",#s)
 --   level=level+1
    if s.operator=="union" or s.operator=="intersection" or s.operator=="difference" then
        if #s==0 then return null{} end
        if #s==1 then return binarize(s[1]) end
        local first=binarize(s[1])
        -- a+b+c+... -> +(a,+(b,+(c,...)))
        for i=2,#s do
            first=operator(s.operator,s.keys,{first,binarize(s[i])})
        end
        --level=level-1
        return first
    elseif s.operator=="transform" then
        -- flatten
        print("binarizing a transform... len=",#s)
        if #s==0 then return null{} end
        if #s==1 then return operator(s.operator,s.keys,copykeys(s.keys,s,{binarize(s[1])})) end
        local first=operator(s.operator,s.keys,copykeys(s.keys,s,{binarize(s[1])}))
        for i=2,#s do
            first=operator("union",{},{first,operator(s.operator,s.keys,copykeys(s.keys,s,{binarize(s[i])}))})
        end
        --level=level-1
        return first
    else
        -- primitive ou operator qui fait rien (hull par exemple)
        --level=level-1
        return readonlytable(s)
    end
end

--
-- on doit executer binarize en premier
-- si on a un operateur, il aura max 2 children.
-- transform{m1}{transform{m2}{...}} -> transform{m1.m2}{...}
-- transform{m1}{nottransform{}{x}} -> nottransform{}{transform{m1}{x}}
-- transform{m1}{nottransform{}{x,y}} -> nottransform{}{transform{m1}{x},transform{m1}{y}}}
--
-- on assume:
-- transform : 1 enfant  (0 a tester)
-- union/difference/intersection : 1 ou 2 enfants 
-- autres operateur: >2 possible, on garde tout directement.
--
--
function normalize(s)
    print("NORM op=",s.operator," len=",#s)
    if #s==0 or #s>2 then return s end
    if #s==1 then
        if s.operator=="transform" and s[1] and s[1].operator=="transform" and s[1][1] then
            -- transform{m1}{transform{m2}{x}} -> transform{m1.m2}{x}
            -- on flush les autres parametres que m
            return normalize(operator(s[1].operator,s[1].keys,{m=s.m*s[1].m,s[1][1]}))
        end
        -- must be an operator. primitve are left alone
        if s.operator=="transform" and s[1] and s[1].operator and s[1].operator~="transform" then
            -- t{m}{!t{x}} ou t{m}{!t{x,y}}
            if s[1][1] and s[1][2] then
                -- t{m}{!t{x,y}} -> !t{t{m}{x},t{m}{y}}
                return normalize(operator(s[1].operator,s[1].keys,copykeys(s[1].keys,s[1],{
                    operator(s.operator,s.keys,copykeys(s.keys,s,{s[1][1]})),
                    operator(s.operator,s.keys,copykeys(s.keys,s,{s[1][2]}))
                })))
            elseif s[1][1] then
                -- t{m}{!t{x}} -> !t{t{m}{x}}
                return normalize(operator(s[1].operator,s[1].keys,copykeys(s[1].keys,s[1],{operator(s.operator,s.keys,s,{s[1][1]})})))
            else
                return normalize(s[1])
            end
        end
        return operator(s.operator,s.keys,copykeys(s.keys,s,{normalize(s[1])}))
    end
    -- deux operandes
    if s.operator=="difference" then
        if s[2].operator=="union" and s[2][2] then
            -- 1. x-(y+z) -> (x-y)-z
            local x,y,z = normalize(s[1]),normalize(s[2][1]),normalize(s[2][2])
            --return (x-y)-z
            return difference{difference{x,y},z}
        elseif s[2].operator=="intersection" and s[2][2] then
            -- 3. x-(y*z) -> (x-y)+(x-z)
            local x,y,z = normalize(s[1]),normalize(s[2][1]),normalize(s[2][2])
            --return (x-y)+(x-z)
            return union{difference{x,y},difference{x,z}}
        elseif s[2].operator=="difference" and s[2][2] then
            -- 5. x-(y-z) -> (x-y)+(x*z)
            local x,y,z = normalize(s[1]),normalize(s[2][1]),normalize(s[2][2])
            --return (x-y)+(x*z)
            return union{difference{x,y},intersection{x,z}}
        elseif s[1].operator=="union" and s[1][2] then
            -- 8. (x+y)-z -> (x-z)+(y-z)
            local x,y,z = normalize(s[1][1]),normalize(s[1][2]),normalize(s[2])
            --return (x-z)+(y-z)
            return union{difference{x,z},difference{y,z}}
        end
    elseif s.operator=="intersection" then
        if s[2].operator=="union" and s[2][2] then
            -- 2. x*(y+z) -> (x*y)+(x*z)            *{x,+{y,z}} -> +{*{x,y},*{x,z}}
            local x,y,z = normalize(s[1]),normalize(s[2][1]),normalize(s[2][2])
            --return (x*y)+(x*z)
            return union{intersection{x,y},intersection{x,z}}
        elseif s[2].operator=="intersection" and s[2][2] then
            -- 4. x*(y*z) -> (x*y)*z                *{x,*{y,z}} -> *{*{x,y},z}
            local x,y,z = normalize(s[1]),normalize(s[2][1]),normalize(s[2][2])
            --return (x*y)*z
            return intersection{intersection{x,y},z}
        elseif s[2].operator=="difference" and s[2][2] then
            -- 6. x*(y-z)) -> (x*y)-z               *{x,-{y,z}} -> -{*{x,y},z}
            local x,y,z = normalize(s[1]),normalize(s[2][1]),normalize(s[2][2])
            --return (x*y)-z
            return difference{intersection{x,y},z}
        elseif s[1].operator=="difference" and s[1][2] then
            -- 7. (x-y)*z -> z*(x-y) -> (x*z)-y     *{-{x,y},z} -> -{*{x,z},y}
            local x,y,z = normalize(s[1][1]),normalize(s[1][2]),normalize(s[2])
            --return (x*z)-y
            return difference{intersection{x,z},y}
        elseif s[1].operator=="union" and s[1][2] then
            -- 9. (x+y)*z -> (x*z)+(y*z)            *{+{x,y},z} -> +{*{x,z},*{y,z}}
            local x,y,z = normalize(s[1][1]),normalize(s[1][2]),normalize(s[2])
            --return (x*z)+(y*z)
            return union{intersection{x,z},intersection{y,z}}
        end
    end
    -- garde seulement les keys et normalize les deux operandes
    return operator(s.operator,s.keys,copykeys(s.keys,s,{normalize(s[1]),normalize(s[2])}))
end

--
-- applique normalize tant que ca ne change plus
-- pour l'instant, compare le tostring
--
--
function normalizeRepeat(s)
    i=1
    --print("***** normalize loop ",i);
    local n=normalize(s)
    local ns=tostring(n)
    repeat
        i=i+1
        --print("***** normalize loop ",i);
        local m=normalize(n)
        local ms=tostring(m)
        if ms==ns then return n end
        n=m
        ns=ms
    until false
end

--
-- render
-- 
-- assure de faire un binarize, puis un normalize
-- ensuite le render interne a l'objet, qui sera recursif.
--
-- Concept de cache...
-- on a un repertoire global des nef, acceder par un slug
-- on passe a travers l'arbre (apres binarize/normalize) et on genere les slugs recursivement
-- une fois qu'on a les slug, on peut faire le rendu...
-- On passe en mode infix, ou on parcours l'arbre: pour chaque noeud, si un nef existe avec ce slug, on faire un rendu direct tout de suite (et on arrete le infix recursif)
-- 
--

-- assume mode est "preview" ou "render"
function render(s)
    if mode=="preview" then
        print("***** PREVIEW *****")
        displayProducts=renderize(s,false)
    else
        print("***** RENDER *****")
        displayProducts=renderize(s,true)
    end
end

function renderize(s,fullrender)
    print("****** RENDERIZE fullrender=",fullrender,"*********")
    z=normalizeRepeat(binarize(s))
    print("------- normalized binarized --------")
    z.dump()
    z.computehash()
    print("-------  --------")
    ------
    -- si on veut faire un render complet, on peut simplement faire render tout de suite...
    -- render -> va ajouter dans la cache tout seul?
    if fullrender then z.render(false) end
    -- rempli une display list a afficher, compatible opencsg.
    -- si on ait un render complet, on devrait avoir seulement un item dans la liste, le nef complet
    -- on pourrait avoir une option "render=true" dans les operateurs pour obliger un rendu nef
    -- chaque item de la displaylist est {"+"|"-"|"*",nef}
    local displaylist=z.infix("*")

    -- va chercher les nef utiles
    for i,v in ipairs(displaylist) do
        displaylist[i].nef=cache.lookup(v.hash)
    end


    print("---- display list (len="..#displaylist..") ----")
    for i,v in ipairs(displaylist) do print(i.." : "..v.op.." "..v.hash,v.nef) end
    return displaylist
end



--
-- compuations on geometry
-- typically, we must render first
--
-- volume(a) return volume and center of mass
--
function volume(a)
    if getmetatable(a)~="geometry" then error("volume expects geometry") end
    local display=renderize(a,true)
    if #display ~= 1 then error("cant get rendering for volume comptation") end
    local vol,cx,cy,cz=display[1].nef:volume()
    print("vol got ",vol,cx,cy,cz)
    return vol,{cx,cy,cz}
end

function bbox(a)
    if getmetatable(a)~="geometry" then error("bbox expects geometry") end
    local display=renderize(a,true)
    if #display ~= 1 then error("cant get rendering for bbox comptation") end
    local x1,x2,y1,y2,z1,z2=display[1].nef:bbox()
    print("bbox got ",x1,x2,y1,y2,z1,z2)
    return x1,x2,y1,y2,z1,z2
end

-- on pourrait faire polyhedron(nef) -> {points=,faces=}
-- et polyhedonr({points,faces})
function poly(a)
    if getmetatable(a)~="geometry" then error("poly expects geometry") end
    local display=renderize(a,true)
    if #display ~= 1 then error("cant get rendering for poly comptation") end
    local p=display[1].nef:poly()
    print("got ",#p.points," points and ",#p.faces," faces")
    for k,v in pairs(p.points) do print("points key ",k," val ",v) end
    for k,v in pairs(p.faces) do print("faces key ",k," val ",v) end
    return p
end

--
-- classes specifiques --
--
function rad(deg) return deg*math.pi/180 end
function deg(rad) return rad/math.pi*180 end

function union(args) return operator("union",{},args) end
function intersection(args) return operator("intersection",{},args) end
function difference(args) return operator("difference",{},args) end
--function rotate(a) return operator("rotate",{r=(a.r or matrix{a.rx or 0,a.ry or 0,a.rz or 0})},{'r'}) end
function rotate(a)
    local m=matrix.rotx(rad(a.x or 0))*matrix.roty(rad(a.y or 0))*matrix.rotz(rad(a.z or 0))
    local args={m=m}
    return operator("transform",{'m'},tablemerge(args,a))
end
--function translate(a) return operator("translate",{t=(a.t or matrix{a.tx or 0,a.ty or 0,a.tz or 0})},{'t'}) end
function translate(a)
    local args={m=matrix.trans({a.x or 0,a.y or 0,a.z or 0})}
    return operator("transform",{'m'},tablemerge(args,a))
end
function scale(a)
    -- error("scale only accept scale{s=2} or scale{x=2,y=2,z=2}") end
    local m
    m=matrix.scale({a.x or a.s or 1,a.y or a.s or 1,a.z or a.s or 1})
    local args={m=m}
    return operator("transform",{'m'},tablemerge(args,a))
end

function hull(a) return operator("hull",{},a) end
--function scale(a) return operator("scale",{}) end
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




function sphere(a) return primitive("sphere",{'r','n','triangles'},{r=a[1] or a.r or 1,n=a.n or 8,triangles=a.triangles or 0}) end  -- r, d, fa, fs, fn
function cylinder(a) return primitive("cylinder",{'h','r'},{h=a.h or 1,r=a.r or 1,center=a.center or false}) end -- h,r,fa fs fn
function cube(a) return primitive("cube",{'size','center'},{size=a[1] or a.size or matrix{1,1,1},center=a.center or false}) end
function polyhedron(a) return primitive("polyhedron",{'points','faces','convexity'},{points=a.points or {},faces=a.faces or {},convexity=a.convexity or 1}) end
function cone(a) return primitive("cone",{'h','r1','r2'},{h=a.h or 1, r1=a.r1 or 0, r2=a.r2 or 1, center=a.center or false}) end

function import(a)
    if not a.name then error("import error: import{name='xyz.stl'}") end
    return primitive("import",{'name'},{name=a.name})
end
--function null(a) return primitive("null",{}) end
-- square (2d)
-- circle (2d)
-- ellipse (2d)
-- regular polygon (2d)
-- polygon (2d)
-- import
-- text (2d)

--
-- all nef rendering, always called with (self) the parameters
--

rendering={}

function rendering.sphere(params)
    print("rendering a sphere r="..params.r)
    return nef.sphere{r=params.r,n=params.n,triangles=params.triangles}
end
function rendering.cylinder(params)
    print("rendering a cylinder h="..params.h..",r="..params.r)
    return nef.cylinder{h=params.h,r=params.r,n=16,triangles=0}
end
function rendering.cube(params)
    print("rendering a cube size=",params.size,",center=",params.center)
    local n=nef.cube{x=params.size[1][1],y=params.size[2][1],z=params.size[3][1],center=params.center}
    print("got cube",n)
    return n
end
function rendering.polyhedron(params)
    print("rendering a polyhedron points=",#params.points,",faces=",#params.faces)
    local n=nef.polyhedron(params)
    return n
end
function rendering.import(params)
    print("rendering an import name="..params.name)
    return nef.import{name=params.name}
end

-- les parametres sont au cas ou ca serve...
--
function rendering.union(params,children)
    print("rendering a union")
    return nef.union(children)
end
function rendering.intersection(params,children)
    print("rendering an intersection")
    return nef.intersection(children)
end
function rendering.difference(params,children)
    print("rendering a difference")
    --local k=nef.union(self)
    return nef.difference(children)
end
function rendering.transform(params,children)
    print("rendering a transform nbchildren="..#children)
    local n=children[1]:dup()
    n:transform(mat4x4flat(params.m))
    return n
end
function rendering.hull(params,children)
    print("rendering hull, childs="..#children)
    return nef.hull(children)
end

--
-- for loop
-- On doit faire un for qui ramasse les items dans une liste 
--
function loop(a)
    local t={}
    for i=(a.from or 1),(a.to or 10),(a.step or 1) do t[#t+1]=a.f(i) end
    return operator(a.op or "union",{},t)
end

--a=loop{op="intersection",from=1,to=10,f=function (i) return sphere{r=i} end}
--a.dump()


--
--
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


--[[
x=sphere{}
y=cube{}
z=cylinder{}

--r0=difference{x,y}
--r0=x+y

--r1=difference{x, union{ y,z }}
r1=x-(y+z)


--r2=intersection{x, union{ y,z }}
r2=x*(y+z)

--r3=difference{x, intersection{ y,z }}
r3=x-(y*z)

--r4=intersection{x, intersection{ y,z }}
r4=x*(y*z)

--r5=difference{x, difference{ y,z }}
r5=x-(y-z)

--r6=intersection{ x, difference{ y,z }}
r6=x*(y-z)

--r7=intersection{ difference{x,y}, z }
r7=(x-y)*z

--r8=difference{ union{x,y}, z }
r8=(x+y)-z

--r9=intersection{ union{x,y}, z }
r9=(x+y)*z

r=r4
--+r2*r3-r4+r5-r6*r7-r8*r9
--r=r1


print("--------------- original --------------------")
r.dump()
c=binarize(r)
print("--------------- binarized --------------------")
c.dump()
print("--------------- binarized + normalized ----------------")
c=normalize(c)
c.dump()
print("----------------binarized + normalized, infix ---------------")
renderize(c,false)
print("-----------------------------------")

--]]

--[[
x1=sphere{1}
x2=sphere{2}
x3=sphere{3}
x4=sphere{4}
x5=sphere{5}
x6=sphere{6}
x7=sphere{7}
x8=sphere{8}

r=x1-(x2+x3+x4+x5+x6+x7+x8)
print("-- orig --")
r.dump()
print("-- bin --")
s=binarize(r)
s.dump()
print("-- norm1 --")
n=normalizeRepeat(s)
n.dump()
--]]

--
-- pour rendu
--
--[[
x=sphere{30}
y=cube{6}
z=difference{}{x,y,y}

z.dump()
--y.dump()

zb=binarize(z)

z.render()

z.dump()

zb.dump()

print("---")

print(tostring(z))
print(tostring(zb))
--]]

--
-- pour imprimer
--
-- [op] {x,y} -> x [op] y
-- + {x,y} -> [*] x | [*] y

--[[
a=translate{tx=10}{x,rotate{rx=34}{y,z}}
print("--------")
a.dump()
print("--------")
a.binarize().dump()
]]--

--[[
x=sphere{r=456}
y=cube{}
z=cylinder{}

a=union{
    x,
    intersection{y,z}
}
a.dump()

function dup(args)
    return
    union{
        translate{x=-1,table.unpack(args)}, -- pour children
        translate{x=1,args[1]}
    }
end

b=dup{x,y}
b.dump()
]]--





