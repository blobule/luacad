print("---- matrix extra ----")


matrix=matrix or require("matrix")

-- {1,2,3}
function matrix.trans(v) return matrix{
    {1,0,0,v[1]},
    {0,1,0,v[2]},
    {0,0,1,v[3]},
    {0,0,0,1}}
end

function matrix.scale(v) return matrix{
    {v[1],0,0,0},
    {0,v[2],0,0},
    {0,0,v[3],0},
    {0,0,0,1}}
end

function matrix.rotx(a) return matrix{
    {1,0,0,0},
    {0,math.cos(a),-math.sin(a),0},
    {0,math.sin(a),math.cos(a),0},
    {0,0,0,1}}
end

function matrix.roty(a) return matrix{
    {math.cos(a),0,math.sin(a),0},
    {0,1,0,0},
    {-math.sin(a),0,math.cos(a),0},
    {0,0,0,1}}
end

function matrix.rotz(a) return matrix{
    {math.cos(a),-math.sin(a),0,0},
    {math.sin(a),math.cos(a),0,0},
    {0,0,1,0},
    {0,0,0,1}}
end

--
-- scale autour du point c
-- {1,1,1},{0,0,0}
--
function matrix.scaleAround(v,c)
    return matrix.trans(c)*matrix.scale(v)*matrix.trans({-c[1],-c[2],-c[3]})
end

--
-- rotate around a axis w
--
function matrix.rot(a,w)
    local x,y,z=w[1],w[2],w[3]
    local n2=x*x+y*y+z*z
    local n=math.sqrt(n2)
    local ca,sa=math.cos(a),math.sin(a)
    local x2,y2,z2=x*x,y*y,z*z
    print(x,y,z,n2)

    local m=matrix{
        {x2 + (y2 + z2)*ca, x*y - x*y*ca - z*n*sa, x*z - x*z*ca + y*n*sa,0},
        {x*y - x*y*ca + z*n*sa, y2 + (x2 + z2)*ca, y*z - y*z*ca - x*n*sa,0}, 
        {x*z - x*z*ca - y*n*sa, y*z - y*z*ca + x*n*sa, z2 + (x2 + y2)*ca,0},
        {0,0,0,n2}}
    return m/n2
end

--a=matrix{{1,2,3},{4,5,6},{7,8,9}}
--print(a)

--[[
a=matrix.rot(0.345,{0.3,-0.2,0.5})
print(a)
b=matrix.trans({1,2.0,3})
print(b)
c=matrix.scale({1,2.0,3})
print(c)

print(a*b*c)
print(c*b*a)

a=matrix{{1,2},{3,4}}
print(a[1][1])
--]]


--[[
a=matrix.rotx(math.pi/4)
b=matrix.roty(math.pi/4)
c=matrix.rotz(math.pi/4)
print(a)
print(b)
print(c)
print(a*b*c)
--]]



return matrix

