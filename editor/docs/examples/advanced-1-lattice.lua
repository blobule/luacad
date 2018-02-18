--[[

$fn = 20;
n = 2; // joints per 10mm
sx = 20;
sy = 30;
sz = 10;
th = 1;


lattice();
module lattice()
{
 
  for(x=[0:10/n:sx], y=[0:10/n:sy], z=[0:10/n:sz] )
    translate([sx/2-x,sy/2-y,sz/2-z])
    sphere(th);
 
  for(x=[0:10/n:sx], z=[0:10/n:sz] ) // y bars
    translate([sx/2-x,0,sz/2-z])
    rotate([90, 0])
    cylinder(r=th/2, h=sy, center = true);

  for(y=[0:10/n:sy], z=[0:10/n:sz] ) // y bars
    translate([0, sy/2-y,sz/2-z])
    rotate([0, 90])
    cylinder(r=th/2, h=sx, center = true);

  for(x=[0:10/n:sx], y=[0:10/n:sy] ) // z bars
    translate([sx/2-x, sy/2-y])
    cylinder(r=th/2, h=sz, center = true);
} 

--]]

n = 2; -- joints per 10mm
sx = 20;
sy = 30;
sz = 10;
th = 1;

a={}
for x=0,sx,n/2 do
   for y=0,sy,10/n do
	for z=0,sz,10/n do		
		print(x,y,z)
		table.insert(a,translate{x=x,y=y,z=z}{sphere{}})
	end
   end
end
b=union{}(a)
print(b)
render(b)

--a=loop{op=union{},from=1,to=10,f=function (i) return rotate{z=i*10}{translate{x=6}{sphere{2}}} end}
--b=scale{5}{cube{}}
--c=b-a
--c.dump()
--render(c)






