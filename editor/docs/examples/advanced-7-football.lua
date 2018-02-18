--half sine hull
n=90
space = 0.09
--[[
a = loop{op="union",from=0, to=n,f=function(i)
	return rotate{z=90, translate{z=i/8 , cylinder{h=0.05, r= 0.05 + math.sin(math.pi/n*(i))}}} 
	end}
render(hull{a})
]]--

sinus = {}
for i=1,n,1 do
	sinus[i] = translate{z=i*space, cylinder{h=0.05,r=0.05 + math.cos(math.pi*2/n*(i-1))
}}
end

seg_sin = {}
for i = 1,n-1,1 do
	seg_sin[i] = hull{sinus[i]+sinus[i+1]}
end

full_sin = loop{op="union", from=1, to=n-1, f=function(i)
	return seg_sin[i] end}

full_sin = translate{z=-n*space/2, full_sin}

render(full_sin)




