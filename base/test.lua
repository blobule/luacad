--
-- luacad
--

require("cad")


x=sphere{}
y=cube{}
z=cylinder{}

a=polyhedron{points=mat{{0,0,0},{0,0,1},{0,1,0},{0,1,1},{1,0,0},{1,0,1},{1,1,0},{1,1,1}},faces=mat{{0,1,2},{3,4,5}}}
b=null{}


print(x)
print(y)
print(z)
print(a)
print(b)

--x.render()
--r1=x-(y+z)
r1=difference{}{
    sphere{},
    cube{},
    cylinder{}
}
r1.render()

print(r1)
r1.dump()

--[[
r1=difference{}{ x, union{}{ y,z }}
r1=x-(y+z)

r3=difference{}{ x, intersection{}{ y,z }}
r3=x-(y*z)

r5=difference{}{ x, difference{}{ y,z }}
r5a=x-(y-z)

r2=intersection{}{ x, union{}{ y,z }}
r2=x*(y+z)

r4=intersection{}{ x, intersection{}{ y,z }}
r4=x*(y*z)

r6=intersection{}{ x, difference{}{ y,z }}
r6=x*(y-z)

r7=intersection{}{ difference{}{x,y}, z }
r7=(x-y)*z

r8=difference{}{ union{}{x,y}, z }
r8=(x+y)-z

r9=intersection{}{ union{}{x,y}, z }
r9=(x+y)*z

r=r1+r2*r3-r4+r5-r6*r7-r8*r9
r=r1

r.dump()
c=binarize(r)
c.dump()
print("-------------------------------")
c=normalize(c)
c.dump()
print("-------------------------------")
c.infix()


--]]




