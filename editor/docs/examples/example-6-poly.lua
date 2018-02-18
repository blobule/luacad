
--
-- retourne { points={x1,y1,z1,x2,y2,z2,...},faces={a1,b1,c1,a2,b2,c2,...}} }
--

p=poly(sphere{n=16,triangles=1})

-- donne 3*nb de points (ce sont des points 3d x,y,z)
print("V",#p.points)
-- donne 3*nb de faces (ce sont des triangles)
print("F",#p.faces)


for i=1,#p.points do p.points[i]=p.points[i]*(1+math.random()*0.2) end 



--[[
j=1
for i=1,#p.points,3 do print(j,p.points[i],p.points[i+1],p.points[i+2]) j=j+1 end 

j=1
for i=1,#p.faces,3 do print(j,p.faces[i],p.faces[i+1],p.faces[i+2]) j=j+1 end 
--]]

b=polyhedron{points=p.points,faces=p.faces}


render(b-sphere{r=1.05,n=32,triangles=1})
























