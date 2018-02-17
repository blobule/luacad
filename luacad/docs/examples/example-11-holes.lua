
--
-- holes
--

function beigne()
	return
	cube{size=matrix{8,8,1},center=true}
	- sphere{r=2}
end

a=beigne()
b=translate{x=4,beigne()}

render(a+b)

-- en pratique on a
-- a=(A-Ha)
-- b=(B-Hb)
--
-- a+b = (A-Ha)+(B-Hb) = A-Ha+B-Hb
-- ce qu'on veut:
-- a+b = (A+B)-(Ha+Hb)
--
-- regle:
-- A-Ha = A+hole(Ha)
-- a+b = (A+hole(Ha))+(B+hole(Hb)) = A+hole(Ha)+B+hole(Hb)
-- = A+B + hole(ha)+hole(hb)
-- = A+B + hole(ha+hb)
-- = A+B - (ha+hb)
-- et voila!
--
-- aussi:
-- A+hole(Ha) ==> A+hole(Ha*A)
-- (on doit reduire Ha a son intersection avec A pour etre plus exact)

-- dans une liste d'item union{a,b,c,...,hole(x),hole(y),hole(z)}
-- -> difference{(a+b+c),(x+y+z)}
--



function beigne()
	return
	cube{size=matrix{8,8,1},center=true}
	,sphere{r=2}
end

a,ha=beigne()
k=translate{x=4,z=0,beigne()}
b,hb=table.unpack(normalize(binarize(k)))
print("=====")
b.dump()
print("=====")

-- il faut faire ceci aussi:  a-ha == a-(ha*a) 
render((a+b)-(ha*a+hb*b))
--render((a+b)-(ha+hb))


--[[
union{a,b,c,...,h(x),h(y),h(z)} = union{(a+b+c),h(x+y+z)}
a+h(x) == a-x

(a+h(x)) - (b+h(x))
--]]

--render((a-ha)*(b-hb))
--render((a-b)-(ha-hb))




























