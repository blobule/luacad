

n=require("nef")

--a=n.import{name="Vise_Handle.stl"}
--a:export{name="yo.stl"}


a=cylinder{r=30,h=50,n=256}


b=sphere{r=20}

z=a-cylinder{r=20,h=100,n=128}

c=cylinder{r=5,h=100,n=64}
c:translate{x=30}

for i=0,359,30 do
    c:rotate{z=30}
    z=z-c
end


z:export{name="yo.stl",binary=1}

