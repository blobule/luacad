

#include "cgal.h"



 
class nef {
    private:
        CGAL_Nef_polyhedron3 N;

    public:
        nef(void);
        nef(nef &other); // duplicate

        // shapes (3D)
        void cube(double x,double y,double z,int center);
        void sphere(double r,int nba,int nbb,int triangles);
        void cylinder(double r1,double r2,double h,int nb,int triangles);
        void polyhedron(float *vertices,int *facets,int nbV,int nbF);

        // 2D
        void square(double x,double y,double z);


        // IO
        void save(const char *name);
        void load(const char *name);
        void myexport(const char *name,int binary);
        void myimport(const char *name);

        // transformations
        //void translate(double tx,double ty,double tz);
        //void rotate(double rx,double ry,double rz);
        //void scale(double sx,double sy,double sz);
        void transform(double v[12]);

        // unary operators
        void myunion(nef &a);
        void intersection(nef &a);
        void difference(nef &a);

        // dual operators
        void myunion(nef &a,nef &b);
        void intersection(nef &a,nef &b);
        void difference(nef &a,nef &b);

        // information
        void bbox(double box[6]); // [xmin,xmax,ymin,ymax,zmin,zmax]
        void volume(double &volume,double centerOfMass[3]);
        void info(int num,int &valid,int &simple,int &volumes);

        // to and from polyhedron

        // other
        void regularize();

        void extractPoints( std::vector<CGAL_Point_3> &points );

        void hull(std::vector<nef *> obj);

        void renderGL(float *&vertices,int *&facets,int &nbV,int &nbF);

};
 
 
//
// userdata contenant un nef
//
typedef struct {
    nef *n;
    int num;
} lnef_userdata_t;

