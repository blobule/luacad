--using intersection (*) and center=true as well as loops in this short example

X = cube{size=matrix{20,1,1},center=true}
Y = cube{size=matrix{1,20,1},center=true}
Z = cube{size=matrix{1,1,20},center=true}
long_cross = X+Y+Z

cub = cube{size=matrix{2,2,2},center=true}
cross = cub * long_cross
--cross = cross * translate{x=0.2,y=0.2,z=0.2,cross} * translate{x=-0.2,y=-0.2,z=-0.2, cross}

cross2 = loop{op="union", from=1, to=5, f=function(i) return translate{x=math.log(2*i),y=math.log(2*i),z=math.log(2*i), scale{x=0.75^i,y=0.75^i, z=0.75^i, cross}} end}


render(cross2)







