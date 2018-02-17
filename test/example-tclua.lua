

a=nef.cube{}

b=nef.sphere{r=1.5,n=32}
print(b)
b:transform{1,0,0,0.5, 0,1,0,0, 0,0,1,0}

c=nef.cylinder{n=32}
c:transform{1,0,0,1.5, 0,1,0,0, 0,0,1,0}


geomprods={
    {"+",a},
    {"-",b},
    {"+",c},
    {"*",b}
}

geom=geomprods





