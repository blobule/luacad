--
-- volume
--
-- return volume and center of mass
--

a=sphere{}-translate{x=1,sphere{}}


vol,center=volume(a)

print("volume is ",vol)
print("center is ",center[1],center[2],center[3])

bbox=bbox(a)
print("bbox ",bbox.x1,bbox.x2,bbox.y1,bbox.y2,bbox.z1,bbox.z2)

render(a)

















