--
-- transformations
--
-- available
--	3d	rotate
--	3d	translate
--	3d	scale
--	3d	resize
--	3d	hull
-- todo
--	...
--

a=scale{s=2,sphere{}}
b=translate{x=1.5,sphere{}}
c=rotate{z=45,
	cube{size=matrix{2,2,2}}
	}

d=rotate{x=20,a,b}

e=d+cube{size=matrix{2,2,2}}

f=resize{x=2,y=3,z1=3,z2=4.5, e}

display(f)
--display(hull{e})










































