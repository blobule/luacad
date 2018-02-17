--
-- primitives
--
-- available:
-- 	3d	sphere
-- 	3d	cube
-- 	3d	cylinder
--	3d	polyhedron
--	3d	import
--
-- todo:
--	2d	cicrle
--	...
--

a=sphere{r=1,n=64,triangles=1}


b=cube{size=matrix{1,2,3},center=true}


c=cylinder{h=1,r=1,center=false}
c2=cylinder{h=4,r1=3,d2=0,n=32,center=false}

cone1=cone{r=4,h=5}
coneT=cone1-translate{z=-1,cone1}

p=poly(b)
for i=1,#p.points,3 do print(i,p.points[i],p.points[i+1],p.points[i+2])  end 
for i=1,#p.faces,3 do print(i,p.faces[i],p.faces[i+1],p.faces[i+2])  end 


d=polyhedron{points=p.points,faces=p.faces}



e=import{name="docs/data/triceratops.stl"}


render(coneT)


--export(coneT,"uncone.stl")










