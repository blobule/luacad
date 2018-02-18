#ifndef CGALUTILS_H
#define CGALUTILS_H

#include "cgal.h"

#include "linalg.h"

//#include "polyset.h"
//#include "CGAL_Nef_polyhedron.h"
//#include "enums.h"

//#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
/*
typedef CGAL::Epick K;
typedef CGAL::Point_3<K> Vertex3K;
typedef std::vector<Vertex3K> PolygonK;
typedef std::vector<PolygonK> PolyholeK;
*/


namespace /* anonymous */ {
        template<typename Result, typename V>
        Result vector_convert(V const& v) {
                return Result(CGAL::to_double(v[0]),CGAL::to_double(v[1]),CGAL::to_double(v[2]));
       	}
}

namespace CGALUtils {
	bool createSTLFromNefPolyhedron3(const CGAL_Nef_polyhedron3 &N,std::ostream &output,int binary);
	bool createPolyhedronFromNefPolyhedron3(const CGAL_Nef_polyhedron3 &N,CGAL_Polyhedron &ph);
	bool createNefPolyhedron3FromSTL(std::istream &input,CGAL_Nef_polyhedron3 &N);

    bool createVerticesAndFacetsFromNefPolyhedron3(const CGAL_Nef_polyhedron3 &N,float *&vertices,int *&facets,int &nbV,int &nbF);

    // indexes are of the form:  n v1 v2 .. vn 3 v1 v2 v3 4 v1 v2 v3 v4 ...
	bool createPolyhedronFromVerticeAndFacets(const std::vector<Vector3f> &vertices,const std::vector<int> &facets,CGAL_Polyhedron &P);

    void computeVolume(const CGAL_Nef_polyhedron3 &N,NT3 &volumeTotal,NT3 centerOfMass[3]);

    CGAL_Iso_cuboid_3 boundingBox(const CGAL_Nef_polyhedron3 &N);

    void extractPoints(std::vector<CGAL_Point_3> &points,const CGAL_Nef_polyhedron3 &N);
    void hull(std::vector<CGAL_Point_3> &points,CGAL_Nef_polyhedron3 &N);

};

#endif

