--
-- test the demonstrate all primitives
--

nef=require("nef")


--[[
a=cube{}
b=sphere{n=32}
c=cylinder{n=32}

b:translate{x=1}
c:translate{y=1}


--d=a+b+c
--d:export{name="yo-d.stl"}

e=union{a,b,c}
e:export{name="yo-e.stl"}

f=intersection{a,b,c}
f:export{name="yo-f.stl"}

g=difference{a,b,c}
g:export{name="yo-g.stl"}

print(g:info())
g:regularize();
print(g:info())

g:export{name="yo-gg.stl"}
--]]

--[[
function main() {
    return union(
        difference(
            cube({size: 3, center: true}),
            sphere({r:2, center: true})
        ),
        intersection(
            sphere({r: 1.3, center: true}),
            cube({size: 2.1, center: true})
        )
    ).translate([0,0,1.5]).scale(10); }

--]]

function usb {
        translate([3.5,w/2-7/2,0]) cube([2,7,0.5]);
        translate(3.5,w/2,0){a,b,c}
}


function duplicate(a)
    return a+translate{tx=5,a}
end

function range(init, limit, step)
    step = step or 1
    return function(lim, value)
        value = value + step
        if lim * step >= value * step then
            return value
        end
    end, limit, init - step
end

--for i in range(3,10,0.1) do print(i) end


a,b,c,d =   cube{x=1.5,y=1.5,z=1.5},
            sphere{r=2,n=16},
            sphere{r=1.3,n=16},
            cube{x=1.05,y=1.05,z=1.05}
    
--e=(a-b)+(c*d)

e=duplicate{a}


e:export{name="yo-e.stl"}

