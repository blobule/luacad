

nef=require("nef")

a=cube{}
b=sphere{r=1.5,n=32}
b:translate{x=0.5}
c=cylinder{n=32}
c:translate{x=1.5}

draw={
    {op="+",geom=a},
    {op="-",geom=b},
    {op="+",geom=c},
    {op="*",geom=b}
}


print("clear")
for i, v in ipairs(draw) do
    if v.op=="+" then
        print("new product")
        op="*"
    else
        op=v.op
    end
    print("   op ",op," on ",v.geom)
end



