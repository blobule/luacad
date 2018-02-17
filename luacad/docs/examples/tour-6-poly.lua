
-- poly(a)
-- returns all the points and faces of a in the form:
-- { points={x1,y1,z1,x2,y2,z2,...},faces={a1,b1,c1,a2,b2,c2,...}} }
--
-- note that there are only triangle faces (with multiple triangles on a same plane,
-- we obtain more complex polygons.

p=poly(sphere{n=16,triangles=1})

-- returns 3*nb of points (in this order: x,y,z coordinates of the point)
print("V",#p.points)
-- and also returns 3*nb of faces (faces are triangles and need 3 points)
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


























