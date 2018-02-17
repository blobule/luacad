--
-- volume
--
-- return volume and center of mass
--

a=sphere{}-translate{x=1,sphere{}}


vol,center=volume(a)

sur = surface(a)

print("volume is ",vol)
print("center is ",center[1],center[2],center[3])
print("area of the surface is ", sur)

bbx=bbox(a)
print("bbox ",bbx.x1,bbx.x2,bbx.y1,bbx.y2,bbx.z1,bbx.z2)

render(a)



















