--a vase using spheres and cylinders mainly with the difference operator

body = scale{z=1.5, sphere{n=16}}
body = body + translate{z=1.5,cylinder{r=0.5,h=1.8}}

neg = translate{x=1.32,z=1.81,scale{z=1.1,sphere{n=16}}}
n =16
for i=1,n,1 do body = body - rotate{z=360/n*i,neg} end

--flat base
body = body - translate{z=-2.1, cube{size=matrix{2,2,2},center=true}}

render(body)



