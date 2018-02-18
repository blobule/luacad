

a=loop{op="union",from=1,to=10,f=function (i)
	print(i)
	return rotate{z=i*10,translate{x=6,sphere{r=2,n=16}}}
	end}

a.dump()

b=scale{s=5,cube{}}

c=b-a
c.dump()

render(c)





















