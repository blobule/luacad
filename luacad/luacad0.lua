

--
-- luacad 0
--

print("=== luacad 0 ===")

-- pas de nef
-- pas de cache
-- pas de matrix

-- pas de read-only table

-- support

-- enleve les cles, garde les elements
function keepElements(t)
	return table.move(t,1,#t,1,{})
end

-- enleve les elements, garde les cles
function keepKeys(t)
	local r={}
	for k,v in pairs(t) do
		if type(k)~="number" then r[k]=v end
	end
	return r
end

-- get al keys, sorted
function getKeys(t)
	local r={}
	for k,v in pairs(t) do
		if type(k)~="number" then r[#r+1]=k end
	end
    table.sort(r)
	return r
end

-- une table avec les cles keys et les elements elems
-- robust
function make(keys,elems)
	return table.move(elems,1,#elems,1,keepKeys(keys))
end

function onlyKeys(t)
	return #t==0
end

function onlyElements(t)
	local n=0
	for k,v in pairs(t) do n=n+1 end
	return n==#t
end


-- print non reserved keys frist, then elements
-- keys are sorted
-- print only non reserved keys (not _*) 
-- set k to true to print keys
-- set e to true to print elements
function printAll(t,k,e)
	local s=""
	if k then
		for _,k in ipairs(getKeys(t)) do
			if not isReserved(k) then s=s..tostring(k).."="..tostring(t[k]).."," end
		end
	end
	if e then
		for k,v in pairs(t) do
			if type(k)=="number" then s=s..tostring(v).."," end
		end
	end
	return "{"..s:sub(1,-2).."}"
end



-- is the key reserved?
function isReserved(s)
	return s:sub(1,1)=="_"
end

-- dump a table
function dump(t)
	for k,v in pairs(t) do
		print("key ",k," val ",v)
	end
end

-- codes pour l'affichage
affiche={union="+",difference="-",intersection="*"}

function indent(level)
    for i=1,(level or 0) do io.write("  ") end
end


-- enleve les elements, garde les cles

--------
--
-- le graphe csg contient que des operateur, mais un operateur avec aucun enfant est une primitive
-- un operateur: un certain nombre de keys contenant les parametres,
--     et un certain nombre d'element array, qui sont les enfants.
--     les keys reserves debutent par _ et les autres sont utilisables par l'usager.
--
-- keys are only keys, no elements are kept
-- children are only elemens, no keys are kept
--
function operator(nom,keys,children)
	local self=make(keys,children)
	self._operator=nom
    self._dump=function(level)
		local s=affiche[self._operator] or self._operator
        indent(level) print(s..printAll(self,true,false).." {")
        local up=(level or 0)+1
        for i=1,#self do self[i]._dump(up) end
        indent(level or 0) print("}")
    end

	setmetatable(self,{
		__tostring = function()
            return self._operator..printAll(self,true,true)
        end
    })

	return self
end

-- certains operateurs sont "listables"  : op({a,b,c}) -> {op({a}),ope({b}),op({c})}
-- certains operateurs sont "composables" : rot(a,rot(b)) = rot(a+b)

-- listable:  op(p=1,a,b,c,d} -> {op{p=1,a},op{p=1,b},...}
-- binarizable:  op{a,b,c,d} -> op{a,op{b,c,d}} -> op{a,op{b,op{c,d}}}
--            color{red=200,a{...},b{...}} -> {color{red=200,a{...},color{red=200,b{...}}
-- compose:  color{red=200,color{red=100,a{...}}} -> color{red=_compose({red=200},{red=100}),a{...}}
-- distributive: T{m=1,+{a,b}} -> +{T{m=1,a},T[m=1,b}}
--               T{m=1,C{red=100,+{a,b}}} -> T{m=1,+{C{red=100,a},C{red=100,b}}} -> +{T{C{a}},T{C{b}}}

-- union: binarizable
-- +{a,b,c,d} -> +{a,+{b,c,d} -> +{a,+{b,+{c,d}}}
-- *{a,b,c,d} -> *{a,*{b,c,d} -> *{a,*{b,*{c,d}}}
-- -{a,b,c,d} -> -{a,+{b,c,d} -> -{a,*{b,*{c,d}}}

-- parametre operator -> T{m1,T{m2,...}}-> T{m1.m2,...}   (composition T(T( ou C(C( ...
--                    -> T{m1,notT
-- -> on doit definir qu'un operateur est une primitive...facile, c' est "drawable"

-- la propagation des operateurs s'arrete a un drawable.
-- on peut imaginer un drawable qui a des enfants, mais dans ce cas ...
-- align{h=-1,v=0,a,b}

-- les operateurs
-- +,-,* -> seuls operateurs a la fin, et le reste doit etre une "primitive"
--

-------------- binarize test ------------

--
-- binarize -> transforme [+|-|*]{x,y,z,..} en binaires seulement
--
-- T{+{+{a,b},c}} -> 
--
-- union/intersection/difference -> +{x,y,z,...} -> +{+{+{+{x,y},z,...
-- translate/rotate/scale{x,y,z} -> +{t{x},t{y},t{z}}
-- tout le reste : primitve
-- translate{union{a,b,c,d}}->union{translate{a},union{translate{b},union{translate{c},d}}}

--level=0

function binarize(s)
    print("binarize",s)
    print("   bin op=",s._operator," len=",#s)
    --   level=level+1
    if s._operator=="union" or s._operator=="intersection" or s._operator=="difference" then
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




---------------- operateurs -------------


function union(a) return operator("union",{},a) end
function intersection(a) return operator("intersection",{},a) end
function difference(a) return operator("difference",{},a) end

-- propagate: color{rgb=111,a,b,c} -> {a{rgb=111},b{rgb=111},c{rgb=111}}
-- prop{p1,prop{p2,a,b}} -> prop{p1,a{p2},b{p2}} -> {a{compose("prop",p1,p2)},b{p1,p2}}  
-- prop{p1,!prop{a,b}} -> !prop{prop{p1,a},prop{p1,b}}

-- color{p1,colorbyvolume{a}} -> ? 

--> type
--> union/intersection/difference : render
--> transformation / couleur : propagation de parametres et composition
--> primitive : render

function color(a) return operator("color",{rgb=a.rgb or "ffff80",_propagate=true},a) end

-- sphere{r=10,n=8,triangles=true}
function sphere(a) return operator("sphere",{r=a[1] or a.r or 1,n=a.n or 8,triangles=a.triangles or 0},{}) end
function cube(a) return operator("cube",{size=a[1] or a.size},{}) end


b=sphere{6}
a=sphere{r=10}
c=cube{34}
d=color{rgb="ff0000",union{a,b,c}}

--print(d)
--print("===")
--d._dump()

-- rendering process --
-- 1st - 

-- trois types de base: csg (+-*), transform (trans, rot, scale, move, resize, ...), color , primitive
-- T{+{a,b,c} -> +{T{a},T{b},T{c}} -> T est plus lourd que +
-- C + lourd que T :  C{T{a}} -> T{C{a}}
-- C{notC{a,b,c}} -> notC{C{a},C{b},C{c}}
-- C{c1,C{c2,a,b,c}} -> C{c1.c2,a,b,c}
-- T + lourd que csg
-- a la fin... csg ,,, T ,,, C {a}
-- primitive plus lourd que tout... pas de sous arbre. si on veut faire boundingsphere{a,b,c}
-- on peut mais tout s'arrete a la primitive et reprend dans ses params

-- simplify complex nodes to simple : move/resize -> transform, debug -> color
-- sink (move c lower, then T lower)
-- 
-- binarize all csg (il devrait etre propre, juste des sequences de +-* jusqua une primitive

T{m1,M{a,b}} -> T{m1,T{m2,a}} -> T{m1.m2,a}

-- rendering --
-- 1. propagate T



