

--
-- retourne un engrenage
--
-- en xy, centre en (0,0) 
-- 
-- to mesh, pitch_circle should touch
-- same mm_per_tooth and pressur_angle
--
-- pitch_angle : 14.5, 20, 25, ou meme 28 
--

function vector(a)	return matrix{a}^'T' end


-- these 4 functions are used by gear
function gear_polar(r,theta) return vector{math.sin(math.rad(theta)), math.cos(math.rad(theta))}*r end

-- convert gear_polar to cartesian coordinates
function gear_iang(r1,r2) return math.deg(math.sqrt((r2/r1)*(r2/r1) - 1) - math.acos(r1/r2)) end

-- unwind a string this many degrees to go from radius r1 to radius r2
function gear_q7(f,r,b,r2,t,s) return gear_q6(b,s,t,(1-f)*math.max(b,r)+f*r2) end

-- radius a fraction f up the curved side of the tooth
function gear_q6(b,s,t,d) return gear_polar(d,s*(gear_iang(b,d)+t)) end



--
--    mm_per_tooth    = 3,    //this is the "circular pitch", the circumference of the pitch circle divided by the number of teeth
--    number_of_teeth = 11,   //total number of teeth around the entire perimeter
--    thickness       = 6,    //thickness of gear in mm
--    hole_diameter   = 3,    //diameter of the hole in the center, in mm
--    twist           = 0,    //teeth rotate this many degrees from bottom of gear to top.  360 makes the gear a screw with each thread going around once
--    teeth_to_hide   = 0,    //number of teeth to delete to make this only a fraction of a circle
--    pressure_angle  = 28,   //Controls how straight or bulged the tooth sides are. In degrees.
--    clearance       = 0.0,  //gap between top of a tooth on one gear and bottom of valley on a meshing gear (in millimeters)
--    backlash        = 0.0   //gap between two meshing teeth, in the direction along the circumference of the pitch circle

function gear(args)
	print("gear")
	-- default values
	args.mm_per_tooth = args.mm_per_tooth or 3
	args.number_of_teeth = args.number_of_teeth or 11
	args.thickness = args.thickness or 6
	args.hole_diameter = args.hole_diameter or 3
	args.twist = args.twist or 0
	args.teeth_to_hide = args.teeth_to_hide or 0
	args.pressure_angle = args.pressure_angle or 28
	args.clearance = args.clearance or 0.0
	args.backlash = args.backlash or 0.0

	local p = args.mm_per_tooth*args.number_of_teeth/math.pi/2.0
	local c = p+args.mm_per_tooth/math.pi-args.clearance
	local b = p*math.cos(math.rad(args.pressure_angle))
	local r = p-(c-p)-args.clearance
	local t = args.mm_per_tooth/2.0-args.backlash/2
	local k = -gear_iang(b,p) - math.deg(t/2/p)

	print("got",k)
--[[

          difference()
  {
               for (i = [0:number_of_teeth-teeth_to_hide-1] )
                   rotate([0,0,i*360/number_of_teeth])
                       linear_extrude(height = thickness, center = true, convexity = 10, twist = twist)
                           polygon(
                               points=[
                                   [0, -hole_diameter/10],
                                   gear_polar(r, -181/number_of_teeth),
                                   gear_polar(r, r<b ? k : -180/number_of_teeth),
                                   gear_q7(0/5,r,b,c,k, 1),gear_q7(1/5,r,b,c,k, 1),gear_q7(2/5,r,b,c,k, 1),gear_q7(3/5,r,b,c,k, 1),gear_q7(4/5,r,b,c,k, 1),gear_q7(5/5,r,b,c,k, 1),
                                   gear_q7(5/5,r,b,c,k,-1),gear_q7(4/5,r,b,c,k,-1),gear_q7(3/5,r,b,c,k,-1),gear_q7(2/5,r,b,c,k,-1),gear_q7(1/5,r,b,c,k,-1),gear_q7(0/5,r,b,c,k,-1),
                                   gear_polar(r, r<b ? -k : 180/number_of_teeth),
                                   gear_polar(r, 181/number_of_teeth)
                               ],
                                paths=[[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16]]
                           );
               cylinder(h=2*thickness+1, r=hole_diameter/2, center=true, $fn=20);
  }
 }




--]]




	print("mm_per_tooth = ",args.mm_per_tooth)
	print("press = ",args.pressure_angle)
end


a=gear{t_a=123,z=345,mm_per_tooth=5}
print(a)

--[[
p=poly(sphere{n=32})

print("V",#p.points)
print("F",#p.faces)

print(p.points[1],p.points[2],p.points[3])

for i=1,#p.points do p.points[i]=p.points[i]*(1+math.random()*0.1) end 

--]]


--[[
j=1
for i=1,#p.points,3 do print(j,p.points[i],p.points[i+1],p.points[i+2]) j=j+1 end 

j=1
for i=1,#p.faces,3 do print(j,p.faces[i],p.faces[i+1],p.faces[i+2]) j=j+1 end 
--]]

--b=polyhedron{points=p.points,faces=p.faces}


--render(b-sphere{r=1.05,n=32,triangles=1})






















