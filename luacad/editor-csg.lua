--
--

print("editor-csg");

ocsg=require("opencsg")
nef=require("nef")

-- toutes les commandes sont ici
luacad=require("luacad")

wid=ocsg.init()
print("lua wid is ",wid)

--
--
-- gp est une liste dont les elements sont
-- {op="+"|"*"|"-",nef=[le nef],hash=[le hash]}
--
--
function drawDisplayProds(gp)
    ocsg.clear()
    local op
    -- remplace le premier '+' par '*'
    for i,v in ipairs(gp) do
        if v.op=="+" then
            print(">>new product")
            ocsg.newProduct()
            op="*"
        else
            op=v.op
        end
        print(">> draw ",op," h=",v.hash, "nef=",v.nef)
        ocsg.draw(op,v.nef)
    end
end


renderer = coroutine.create(
    function()
		::loop::
		k=coroutine.yield()
        if k then
            print("Got new geometry!!!")
            --local t=os.clock()
            drawDisplayProds(k)
            --print("draw ",math.floor((os.clock()-t)*1000),"ms")
        end
        --local t=os.clock()
		ocsg.render()
        --`print("render ",math.floor((os.clock()-t)*1000),"ms")
        --print(i)
		goto loop
    end
)


return wid

--for i = 1, 1000000 do coroutine.resume(renderer,i) end


