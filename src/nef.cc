
#include "nef.h"
#include  "cgalutils.h"


//// constructeurs

nef::nef() { }
nef::nef(nef &other) { N=other.N; }
//nef::nef(CGAL_Nef_polyhedron &Ref) { N=Ref; }

// assume triangles
// mais on pourrait utiliser le format {<nb>,p1,p2,..pn,<nb>,p1,p2,...pn}
void nef::polyhedron(float *vertices,int *faces,int nbV,int nbF) {
    std::vector<Vector3f> verts;
    std::vector<int> facets; // n v1 v2 .. vn ...

    float *v;
    int i;
    for(i=0,v=vertices;i<nbV;i+=3,v+=3) {
        verts.push_back(Vector3f(v[0],v[1],v[2]));
    }
    int *f;
    for(i=0,f=faces;i<nbF;i+=3,f+=3) {
        facets.push_back(3);
        facets.push_back(f[0]-1); // pour offset array lua
        facets.push_back(f[1]-1);
        facets.push_back(f[2]-1);
    }

    CGAL_Polyhedron P;
    CGALUtils::createPolyhedronFromVerticeAndFacets(verts,facets,P);
    //std::cout << P;
    N=CGAL_Nef_polyhedron3(P);
}


void nef::cube(double x,double y,double z,int center) {
    std::vector<Vector3f> vertices;
    std::vector<int> facets; // n v1 v2 .. vn ...

    if( center ) {
        double X=x/2.0;
        double Y=y/2.0;
        double Z=z/2.0;
        vertices.push_back(Vector3f( -X, -Y,  Z));
        vertices.push_back(Vector3f( -X,  Y,  Z));
        vertices.push_back(Vector3f(  X,  Y,  Z));
        vertices.push_back(Vector3f(  X, -Y,  Z));
        vertices.push_back(Vector3f( -X, -Y, -Z));
        vertices.push_back(Vector3f( -X,  Y, -Z));
        vertices.push_back(Vector3f(  X,  Y, -Z));
        vertices.push_back(Vector3f(  X, -Y, -Z));
    }else{
        vertices.push_back(Vector3f(  0,  0,  z));
        vertices.push_back(Vector3f(  0,  y,  z));
        vertices.push_back(Vector3f(  x,  y,  z));
        vertices.push_back(Vector3f(  x,  0,  z));
        vertices.push_back(Vector3f(  0,  0,  0));
        vertices.push_back(Vector3f(  0,  y,  0));
        vertices.push_back(Vector3f(  x,  y,  0));
        vertices.push_back(Vector3f(  x,  0,  0));
    }
    int idx[] = {
        4,  3, 2, 1, 0,
        4,  0, 1, 5, 4,
        4,  6, 5, 1, 2,
        4,  3, 7, 6, 2,
        4,  4, 7, 3, 0,
        4,  4, 5, 6, 7,
       -1};
    
    for(int i=0;idx[i]>=0;i++) facets.push_back(idx[i]);

    CGAL_Polyhedron P;
    CGALUtils::createPolyhedronFromVerticeAndFacets(vertices,facets,P);
    //std::cout << P;
    N=CGAL_Nef_polyhedron3(P);
}


// triangle: 0=no (uuse quad), 1=yes
void nef::sphere(double r,int nba,int nbb,int triangles) {
    int a,b,c;
    double ga,gb;
    double x,y,z;
    double da;

    std::cout << "SPHERE nba="<<nba<<" nbb="<<nbb<<"\n";

    std::vector<Vector3f> vertices;
    std::vector<int> facets; // n v1 v2 .. vn ...

    int k=0;
    da=0.0;
    for(a=0;a<nba;a++) {
        ga=a*2.0*M_PI/nba;
        for(b=0;b<nbb;b++) {
            gb=((b+1)*M_PI/(nbb+1)-M_PI/2.0);
            if( triangles ) da=(b%2)*0.5*2.0*M_PI/nba;
            x=r*cos(ga+da)*cos(gb);
            y=r*sin(ga+da)*cos(gb);
            z=-r*sin(gb);
            vertices.push_back(Vector3f( x,y,z));
            k++;
        }
    }
    // top/bottom
    vertices.push_back(Vector3f( 0,0,r));
    vertices.push_back(Vector3f( 0,0,-r));

    int i=0;
    for(a=0;a<nba;a++) {
        for(b=0;b<nbb;b++) {
            if( b<nbb-1 ) {
                /*
            facets.push_back(4);
            facets.push_back(a*nbb+b);
            facets.push_back(((a+1)%nba)*nbb+b);
            facets.push_back(((a+1)%nba)*nbb+(b+1));
            facets.push_back(a*nbb+(b+1));
                */
                // triangule d'avance pour eviter les problemes
                if( b%2 || (triangles==0) ) {
                    facets.push_back(3);
                    facets.push_back(a*nbb+b);
                    facets.push_back(((a+1)%nba)*nbb+b);
                    facets.push_back(((a+1)%nba)*nbb+(b+1));
                    facets.push_back(3);
                    facets.push_back(((a+1)%nba)*nbb+(b+1));
                    facets.push_back(a*nbb+(b+1));
                    facets.push_back(a*nbb+b);
                }else{
                    facets.push_back(3);
                    facets.push_back(a*nbb+b);
                    facets.push_back(((a+1)%nba)*nbb+b);
                    facets.push_back(a*nbb+(b+1));
                    facets.push_back(3);
                    facets.push_back(((a+1)%nba)*nbb+b);
                    facets.push_back(((a+1)%nba)*nbb+(b+1));
                    facets.push_back(a*nbb+(b+1));
                }
            }
            i++;
        }
        // b=0 avec top
        facets.push_back(3);
        facets.push_back(k);
        facets.push_back(((a+1)%nba)*nbb+0);
        facets.push_back(a*nbb+0);
        // bottom
        facets.push_back(3);
        facets.push_back(k+1);
        facets.push_back(a*nbb+nbb-1);
        facets.push_back(((a+1)%nba)*nbb+nbb-1);
    }

    CGAL_Polyhedron P;
    CGALUtils::createPolyhedronFromVerticeAndFacets(vertices,facets,P);
    //std::cout << P;
    N=CGAL_Nef_polyhedron3(P);

}

void nef::cylinder(double r1,double r2,double h,int nb,int triangles) {
    int a,b,c;
    double ga,gb,da;
    double x,y,z;
    
    std::vector<Vector3f> vertices;
    std::vector<int> facets; // n v1 v2 .. vn ...

    if (r1 == 0.0 && r2 == 0.0) 
    {
        //retourner nef null
        // N = nef(); ne fonctionne pas
        return;
    }    

    else if (r1 == 0)
    {
        da=0.0;
        //index 0 a nb-1
        for(a=0;a<nb;a++) {
            ga = a*2.0*M_PI/nb;
            //if( triangles ) da = 0.5*2.0*M_PI/nb;
            x = r2*cos(ga);
            y = r2*sin(ga);
            z = -h/2.0;
            vertices.push_back(Vector3f(x,y,z));
        }
        // center bottom  index = nb
        vertices.push_back(Vector3f(0,0,-h/2.0));
        // center top index = nb+1
        vertices.push_back(Vector3f(0,0,h/2.0));
        for(a=0; a<nb; a++) {
            // side
            facets.push_back(3);
            facets.push_back(nb+1);
            facets.push_back((a+1)%nb);
            facets.push_back(a);
   
            // bottom
            facets.push_back(3);
            facets.push_back(a);
            facets.push_back((a+1)%nb);
            facets.push_back(nb);
        }
    }

    else if (r2 == 0)
    {
        da=0.0;
        //index 0 a nb-1
        for(a=0;a<nb;a++) {
            ga = a*2.0*M_PI/nb;
            //if( triangles ) da = 0.5*2.0*M_PI/nb;
            x = r1*cos(ga);
            y = r1*sin(ga);
            z = h/2.0;
            vertices.push_back(Vector3f(x,y,z));
        }
        // center bottom  index = nb
        vertices.push_back(Vector3f(0,0,-h/2.0));
        // center top index = nb+1
        vertices.push_back(Vector3f(0,0,h/2.0));
 
        for(a=0; a<nb; a++) {
            // side
            facets.push_back(3);
            facets.push_back(nb+1);
            facets.push_back((a+1)%nb);
            facets.push_back(a);
            
            // bottom
            facets.push_back(3);
            facets.push_back(a);
            facets.push_back((a+1)%nb);
            facets.push_back(nb);
        }
    //
    }
    
    else
    {
        da=0.0;
        for(a=0;a<nb;a++) {
            ga=a*2.0*M_PI/nb;
            if( triangles ) da=0.5*2.0*M_PI/nb;
            x=r2*cos(ga);
            y=r2*sin(ga);
            z=-h/2.0;
            vertices.push_back(Vector3f( x,y,z));
            x=r1*cos(ga-da);
            y=r1*sin(ga-da);
            z=h/2.0;
            vertices.push_back(Vector3f( x,y,z));
        }
        // center bottom (nb*2)
        vertices.push_back(Vector3f( 0,0,-h/2.0));
        // center top (nb*2+1)
        vertices.push_back(Vector3f( 0,0,h/2.0));

        for(a=0;a<nb;a++) {
            // side
            facets.push_back(3);
            facets.push_back(a*2+0);
            facets.push_back(a*2+1);
            facets.push_back(((a+1)%nb)*2+1);
            facets.push_back(3);
            facets.push_back(((a+1)%nb)*2+1);
            facets.push_back(((a+1)%nb)*2+0);
            facets.push_back(a*2+0);

            // bottom
            facets.push_back(3);
            facets.push_back(nb*2);
            facets.push_back(a*2+0);
            facets.push_back(((a+1)%nb)*2+0);

            // top
            facets.push_back(3);
            facets.push_back(nb*2+1);
            facets.push_back(((a+1)%nb)*2+1);
            facets.push_back(a*2+1);
        }
    }

    CGAL_Polyhedron P;
    CGALUtils::createPolyhedronFromVerticeAndFacets(vertices,facets,P);
    //std::cout << P;
    N=CGAL_Nef_polyhedron3(P);

}


// format brut du nef

void nef::save(const char *name) {
    std::cout << "SAVE "<<name<<"\n";

    std::ofstream myfile;
    myfile.open (name);
    myfile << N;
    myfile.close();
}

void nef::load(const char *name) {
    std::cout << "LOAD "<<name<<"\n";

    std::ifstream myfile;
    myfile.open(name);
    myfile >> N;
    myfile.close();
}

void nef::myexport(const char *name,int binary) {
    std::cout << "EXPORT "<<name<<" bin="<<binary<<"\n";

    std::ofstream myfile;
    myfile.open (name, std::ios::out | std::ios::binary);
    CGALUtils::createSTLFromNefPolyhedron3(N,myfile,binary);
    myfile.close();
}

void nef::myimport(const char *name) {
    std::cout << "IMPORT "<<name<<"\n";

    std::ifstream myfile;
    myfile.open (name, std::ios::in | std::ios::binary);
    CGALUtils::createNefPolyhedron3FromSTL(myfile,N);
    myfile.close();
}


void nef::info(int num,int &valid,int &simple,int &volumes) {
    valid=N.is_valid();
    simple=N.is_simple();
    volumes=N.number_of_volumes();
}


#if 0
void nef::translate(double tx,double ty,double tz) {
    std::cout << "translate "<<tx<<","<<ty<<","<<tz<<"\n";
    CGAL_Aff_transformation affine(CGAL::TRANSLATION, CGAL_Kernel3::Vector_3(tx,ty,tz));
    N.transform(affine);
}

void nef::rotate(double rx,double ry,double rz) {
    std::cout << "rotate "<<rx<<","<<ry<<","<<rz<<"\n";
    double crx=cos(rx*M_PI/180.0);
    double srx=sin(rx*M_PI/180.0);
    double cry=cos(ry*M_PI/180.0);
    double sry=sin(ry*M_PI/180.0);
    double crz=cos(rz*M_PI/180.0);
    double srz=sin(rz*M_PI/180.0);
    CGAL_Aff_transformation affineX(
         1.0, 0.0, 0.0, 0.0,
         0.0, crx, -srx, 0.0,
         0.0, srx, crx, 0.0 );
    CGAL_Aff_transformation affineY(
         cry, 0.0, sry, 0.0,
         0.0, 1.0, 0.0, 0.0,
         -sry,0.0, cry, 0.0);
    CGAL_Aff_transformation affineZ(
         crz, -srz, 0.0,0.0,
         srz, crz, 0.0, 0.0,
         0.0, 0.0, 1.0, 0.0);
    N.transform(affineX);
    N.transform(affineY);
    N.transform(affineZ);
}

// il manque scale... bof

#endif



// v[12] is a 3x4 affine transformation matrix
void nef::transform(double v[12]) {
    std::cout << "nef transform ";
    for(int i=0;i<12;i++) std::cout << v[i] << ",";
    std::cout << "\n";
    CGAL_Aff_transformation affine(
            v[0],v[1],v[2],v[3],
            v[4],v[5],v[6],v[7],
            v[8],v[9],v[10],v[11]);
    N.transform(affine);
}


void nef::myunion(nef &a) { N=N+a.N; }
void nef::difference(nef &a) { N=N-a.N; }
void nef::intersection(nef &a) { N=N*a.N; }

void nef::myunion(nef &a,nef &b) { N=a.N+b.N; }
void nef::difference(nef &a,nef &b) { N=a.N-b.N; }
void nef::intersection(nef &a,nef &b) { N=a.N*b.N; }



void nef::volume(double &volume,double centerOfMass[3]) {
    NT3 vol;
    NT3 center[3];
    CGALUtils::computeVolume(N,vol,center);
    volume=CGAL::to_double(vol);
    for(int i=0;i<3;i++) centerOfMass[i]=CGAL::to_double(center[i]);

    std::cout << "final volume="<<vol<<"="<<volume<<"\n";
    for(int i=0;i<3;i++)
        std::cout << "final center="<<center[i]<<"="<<centerOfMass[i]<<"\n";
}


void nef::bbox(double box[6]) {
    CGAL_Iso_cuboid_3 bb=CGALUtils::boundingBox(N);
    box[0]=CGAL::to_double(bb.xmin());
    box[1]=CGAL::to_double(bb.xmax());
    box[2]=CGAL::to_double(bb.ymin());
    box[3]=CGAL::to_double(bb.ymax());
    box[4]=CGAL::to_double(bb.zmin());
    box[5]=CGAL::to_double(bb.zmax());
}



void nef::regularize() {
    N.regularization();
}


void nef::extractPoints( std::vector<CGAL_Point_3> &points ) {
        CGALUtils::extractPoints(points,N);
}

// collect from obj (last is NULL)
void nef::hull(std::vector<nef *> obj) {
    std::cout << "nef hull -> sz="<<obj.size()<<"\n";
    std::vector<CGAL_Point_3> points;

    for(int i=0;i<obj.size();i++) {
        obj[i]->extractPoints(points);
    }
    std::cout << "hull has "<<points.size()<<" points\n";

    CGALUtils::hull(points,N);
}


//
// Dessine le nef sous la forme de triangles
//
void nef::renderGL(float *&vertices,int *&facets,int &nbV,int &nbF) {
    CGALUtils::createVerticesAndFacetsFromNefPolyhedron3(N,vertices,facets,nbV,nbF);
}


 
 

