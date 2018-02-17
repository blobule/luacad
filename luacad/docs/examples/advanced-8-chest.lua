half_cyl = function(r,h)
	fcyl = cylinder{r=r,h=h} - cylinder{r=0.8*r,h=0.8*h}
	hcyl = fcyl - translate{y=-r,z=-h/2,cube{size=matrix{r,2*r,h}}}
	return hcyl
end

open_box = function(sx,sy,sz)
	bx = cube{size=matrix{sx,sy,sz},center=true}
	bx = bx - translate{z=0.2*sz, cube{size=matrix{sx*0.8,sy*0.8,sz*0.8},center=true}}
	return bx
end

generate_content=function(n)
	a = loop{op="union", from=1, to=n, f=function(i)
		return translate{x=math.random(),y=math.random(), z=math.random(),
			rotate{x=math.random()/3,y=math.random()/3,z=math.random()/3,
				cylinder{r=0.15,h=0.06}
			}
		} 
	end}
	return a
end

top = translate{z=1.85,y=-1.5, rotate{y=90,x=120, half_cyl(1,3)}}
bx = open_box(3,2,2)
content = translate{z=-1.8,resize{x=2.4,y=1.6,z=1.8,generate_content(20)}}

--render(content)
render(bx + top + content)






