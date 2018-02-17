--
-- test the demonstrate all primitives
--
-- un nef peut etre remplace par une table de nef
-- et on va traverser recursivement pour obtenir le resultat
--
-- union{a,b,{c,d},e} === union{a,b,c,d,e}
-- union{{{{a,b}}}} == union{a,b}
-- difference{{a,b},{c,d},e} == difference{a,b,c,d,e}
--
-- translate{} preserve la structure :  translate{x=1,y=2,z=4}{v{3,4,5},a,b,{c,d}} -> {a,b,{c,d}}
--

nef=require("nef")


a=cube{}
b0=sphere{n=32}
c=cylinder{n=32}


--b:translate{x=1}
b0:translate{x=1}
b=translate{x=1,b0}
c=c:translate{y=1}

e=hull{a,b,c}
e:export{name="yo-e.stl"}

--translate{x=1}rotate{y=90}{a,b,c}



--[[

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

--[[
e= union{
    difference{
        cube{x=1.5,y=1.5,z=1.5},
        sphere{r=2,n=16}
    },
    intersection{
        sphere{r=1.3,n=16},
        cube{x=1.05,y=1.05,z=1.05}
    }
   }


function t(a) {
}

e:export{name="yo-e.stl"}
--]]
--
--

