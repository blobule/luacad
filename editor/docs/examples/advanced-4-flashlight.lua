--using mainly cylinder

top_full = cylinder{r=0.5, h=0.5}
top_hole = cylinder{r=0.4, h=0.3}
top = top_full - translate{z=-0.25, top_hole}

angled_part = cylinder{r1=0.3,r2=0.5,h=0.4}
body = cylinder{r=0.3,h=2}

half_loop = difference{
	cylinder{r=0.27,h=0.1}, cylinder{r=0.2,h=0.1}
}

glass = sphere{n=64} - translate{x=-15,y=-15, z=-0.9, cube{size=matrix{30,30,30}}}

fl = 	top 
	+ translate{z=0.45, angled_part} 
	+ translate{z=1.65, body} 
	+ translate{z=2.65, rotate{x=90, half_loop}} 
	+ translate{z=0.8,glass}

render(scale{x=1.5,y=1.5,z=1.5,fl})

