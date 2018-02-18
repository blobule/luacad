
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
	sphere{},
	cube{}
}

-- same as above
b= sphere{} - cube{}


-- a=loop{op=union{},from=1,to=10,f=function (i) return rotate{z=i*10}{translate{x=6}{sphere{2}}} end}


render(b)

















