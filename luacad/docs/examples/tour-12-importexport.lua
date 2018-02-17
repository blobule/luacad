geo = import{name='docs/data/triceratops.stl'}
geo = geo - rotate{x=10,y=20,z=30,cube{size=matrix{4,0.2,4},center=true}}

export{name='triceCut.stl',binary=0,geo}

cut = import{name='triceCut.stl'}

render(cut)


