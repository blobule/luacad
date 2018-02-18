hole = function(h,w,d)
	return union{
		translate{x=w/2, y=d/2-0.1, z=h-w/2, rotate{x=90,cylinder{r=w/2,h=d}}}, 			
		cube{size=matrix{w,d,h-w/2}}
		}
end

body = cube{size=matrix{3,1,2}}
body = body-translate{x=1,hole(1.5,1,2)}

small_hole = hole(0.8,0.3,2)
body = body-translate{x=0.35,small_hole}-translate{x=2.35,small_hole}

top = cube{size=matrix{3.2,1.2,0.1}}
bottom = cube{size=matrix{1.2,1.2,0.1}}

body = body + translate{x=-0.1,y=-0.1,z=2,top}
body = body + translate{x=-0.1,y=-0.1, bottom} + translate{x=1.9,y=-0.1,bottom}
render(body)




