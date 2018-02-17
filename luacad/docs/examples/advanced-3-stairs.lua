--simple stairs with parameters

--n=number of steps, h=heigth of steps, d=depth of steps, w=width of steps
stair = function(n,h,d,w)
	steps =  
	loop{op="union",from=1,to=n, 
		f = function(i) 
			return translate{x=i*d - d ,cube{size=matrix{d,w,i*h}}} 
		end
	}

	angle = math.atan(h/d)*180/math.pi
	side = translate{x=-1, cube{size=matrix{(n+1)*d,0.5,h*n+1.5}}}
	side = side - rotate{y=-angle, translate{x=-1,z=h,cube{size = matrix{2*n*d+10, 0.5, 2*n*h+10}}}}

	body = steps + translate{y=-0.5, side} + translate{y=w, side}
	--body = steps + translate{y=w, side}
	return body
end

render(stair(5, 0.5, 1, 4))

