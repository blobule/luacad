

a=sphere{r=1,n=64,triangles=1}


b=cube{size=matrix{1,2,3},center=true}


c=cylinder{h=1,r=1,center=false}
c2=cylinder{h=4,r1=3,d2=0,n=32,center=false}

cone1=cone{r=4,h=5}
coneT=cone1-translate{z=-1,cone1}


e=import{name="/usr/local/share/luacad/docs/data/triceratops.stl"}

--renderize(coneT,true)

--render(e)
--render(coneT+e)

display(e+coneT)

--export{name="uncone.stl",coneT}


















