
--
-- CSG operations
--
-- available
--	3d	union (+)
--	3d	intersection (*)
--	3d	difference (-)
--
-- todo
--	...
--


a=difference{
	sphere{n=16},
	cube{}
}

-- same as above
b= sphere{n=32} - cube{}


--a=loop{op=union{},from=1,to=10,f=function (i) return rotate{z=i*10}{translate{x=6}{sphere{2}}} end}


display(b)



















