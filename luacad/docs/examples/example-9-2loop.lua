

a=loop{op=union{},from=1,to=10,f=function (i)
	return
	loop{op=union{},from=-5,to=5,f=function (j)
		print(i,j)
		return rotate{z=i*10,x=j*10}{translate{x=20}{sphere{2}}}
	end}
end}


--[[
b=scale{5}{cube{}}

c=b-a
c.dump()

render(c)

--]]

render(a)















