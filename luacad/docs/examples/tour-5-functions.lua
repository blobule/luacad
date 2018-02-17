

--
-- module
--
--
--
--

function g(x)
	return sphere{r=2}+translate{x=x,sphere{r=2}}
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

--duplicates arg[2], separes their centers by args.sep units in x
function dup(args)
	local s=args.sep or 1
	return union{
		translate{x=-s/2,args[1]},
		translate{x=s/2,args[1]}
	}
end
a=dup{sep=1.2,cube{}}
render(a)

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
print("doc",dup)







