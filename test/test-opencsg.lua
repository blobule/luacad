--
-- test de opencsg
--
-- 
-- opencsg
--
-- opencsg.init() -> on pourrait avoird es windows 0,1,2,3,... ou avec le titre de la window
-- opencsg.clear() -> arease window
-- opencsg.draw(operation,obj) -> ajoute une primitve a dessiner, object could be nef. op is Union/Inter/Diff.
-- opencsg.render() -> render toutes les primitives en mode CSG, et ensuite en mode couleur
--
-- en partique, on a des listes de primitives.
-- a chaque operation "Union", on fabrique une nouvelle liste de primitives.
--    (un Union devient "new primitve list", add as intersection)
--
--
--
--
--

ocsg=require("opencsg")

nef=require("nef")

a=cube{}

b=sphere{r=1.5,n=32}
b:translate{x=0.5}

c=cylinder{n=32}
c:translate{x=1.5}

--c=b-a
--c:export{name="yo.stl"}



renderer = coroutine.create(
    function()
		ocsg.init()
		::loop::
		ocsg.clear()
		ocsg.draw("*",a)
		ocsg.draw("-",b)
		ocsg.endProduct()
		ocsg.draw("*",c)
		ocsg.draw("*",b)
		ocsg.render()
		coroutine.yield(co1)
		goto loop
    end
)

renderer2 = coroutine.create(
    function()
		ocsg.init()
		ocsg.clear()
      --[[
		ocsg.draw("*",a)
		ocsg.draw("-",b)
		ocsg.endProduct()
		ocsg.draw("*",c)
		ocsg.draw("*",b)
        --]]
		::loop::
		ocsg.render()
		i=coroutine.yield(co1)
        print(i)
        if i==200 then
		    ocsg.draw("*",a)
        elseif  i== 400 then
		    ocsg.draw("-",b)
		    ocsg.endProduct()
        elseif  i== 600 then
		    ocsg.draw("*",c)
        elseif  i== 800 then
		    ocsg.draw("*",b)
        end
		goto loop
    end
)

for i = 1, 1000000 do
    coroutine.resume(renderer2,i)
end





--ocsg.clear()
--ocsg.draw()
--ocsg.render()

--c=b-a


--ocsg.draw("*",c)





--ocsg.uninit()


