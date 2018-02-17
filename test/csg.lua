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


ocsg=require("opencsg")


nef=require("nef")

--[[
a=cube{}

b=sphere{r=1.5,n=32}
b:translate{x=0.5}

c=cylinder{n=32}
c:translate{x=1.5}


geomprods={
    {"+",a},
    {"-",b},
    {"+",c},
    {"*",b}
}
--]]

wid=ocsg.init()
print("lua wid is ",wid)

function drawGeomProds(gp)
    ocsg.clear()
    for i,v in ipairs(gp) do
        if v[1]=="+" then
            ocsg.newProduct()
            op="*"
        else
            op=v[1]
        end
        ocsg.draw(op,v[2])
    end
end


renderer = coroutine.create(
    function()
        --drawGeomProds(geomprods)
        --[[
		ocsg.clear()
		ocsg.newProduct()
		ocsg.draw("*",a)
		ocsg.draw("-",b)
		ocsg.newProduct()
		ocsg.draw("*",c)
		ocsg.draw("*",b)
        --]]
		::loop::
		k=coroutine.yield()
        if k then
            print("Got new geometry!!!")
            drawGeomProds(k)
        end
		ocsg.render()
        --print(i)
		goto loop
    end
)

return wid

--for i = 1, 1000000 do coroutine.resume(renderer,i) end

