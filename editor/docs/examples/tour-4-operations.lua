
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

a=union{
	sphere{},
	cube{}
}
a2=sphere{} + cube{}

b=intersection{
	sphere{},
	cube{}
}
b=sphere{}*cube{}

c=difference{
	sphere{},
	cube{}
}
c2= sphere{} - cube{}

-- note that you can have more than 2 nef polyhedron in an operation if you use operation{...}
-- example:
d = intersection{sphere{},cone{},cube{}}
e = difference{sphere{},cone{},cube{}}



render(d)





















