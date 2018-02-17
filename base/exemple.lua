



sphere{r=8}

module bidule() {
    hull {
        cube(10)
        sphere(20)
        translate(30) children()
    }
}


function bidule(p)
    hull{
        sphere{r=p.f},
        cube{5,6,7}
        g=5
    }
end

hull{
    sphere{r=5},
    cube{5,6,7},
    g=5
}

bidule{f=56,rad=89,toto=20}

end


