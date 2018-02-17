--
-- test the demonstrate all primitives
--

nef=require("nef")

a=nef.cube{x=1,y=1,z=1}

--
--
-- transformations
--
b=a:dup()
a:transform({1,0,0,3,  0,1,0,0, 0,0,1,0})

print(a)
print(b)

c=a+b

c:export{name="blub.stl",binary=1}



