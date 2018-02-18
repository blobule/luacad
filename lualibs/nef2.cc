
#include "nef2.h"
#include  "cgalutils.h"


//// constructeurs

nef2::nef2() { }
nef2::nef2(nef2 &other) { N=other.N; }

void nef2::square(double x,double y,double z) {
    N=CGAL_Nef_polyhedron2();
}


/*
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


*/
 
 

