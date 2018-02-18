

--
-- module
--
--
--
--

function g(x)
	return sphere{3}+translate{x=x,sphere{3}}
end
--render(g(3))


--
-- duplicate the children with separation in X
--
--[[
duplicate=
	setmetatable({},{
	__call = function(t,args) print("allo",args.x) end,
	__tostring = function() return "documentation de la fonction duplicate" end
})
duplicate{x=456}
print(duplicate)
--]]

--
-- args are args.x args.<argument name>
-- and children are args[1], args[2], #args for numb of children
--
function dup(args)
	print("Nb of children is ",#args)
	local s=args.sep or 1
	return union{
		translate{x=-s/2,args[1]},
		translate{x=s/2,args[1]}
	}
end
a=dup{sep=3,sphere{}}
--render(a)

--
-- plus complet avec parametres
--

function documente(f,params,d)
	return setmetatable({},{
		__call = function(t,...) return f(...) end,
		__tostring = function() return d end
	})
end

dup=documente(dup,{x=1},"ceci est dup")
a=dup{sep=3,sphere{}}
print("doc",dup)
render(a)




