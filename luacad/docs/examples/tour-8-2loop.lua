

a=loop{op="union",from=1,to=10,f=function (i)
	print(i)
	return rotate{z=i*10,translate{x=6,sphere{r=2,n=16}}}
	end}

a.dump()

b=scale{s=5,cube{}}

c=b-a
c.dump()

-- exemple of a loop in a loop
cyl = translate{x=3.5, rotate{x=90, cylinder{r=0.3}}}
n=24

d = loop{op="union",from=1,to=3,f=function(j)
	return translate{z=j-2, 
		loop{op="union",from=1,to=n,f=function(i)
			return rotate{z=360/n*i,cyl} end}
		} 
	end
}
d.dump()

--render(d)
--render(d*c)

--another example of a loop in a loop, this time with i and j used int the inner loop
e=loop{op="union",from=1,to=10,f=function (i)
	return
	loop{op="union",from=-5,to=5,f=function (j)
		print(i,j)
		return rotate{z=i*10,x=j*10,translate{x=2, sphere{r=0.2}}}
	end}
end}
e.dump()
render(e)
























