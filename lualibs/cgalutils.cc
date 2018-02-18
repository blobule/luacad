// this file is split into many separate cgalutils* files
// in order to workaround gcc 4.9.1 crashing on systems with only 2GB of RAM


#include "cgalutils.h"
//#include "polyset.h"
//#include "printutils.h"
//#include "Polygon2d.h"
//#include "polyset-utils.h"
//#include "grid.h"
//#include "node.h"

//#include "cgal.h"

//#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
//#include <CGAL/normal_vector_newell_3.h>
//#include <CGAL/Handle_hash_function.h>

//#include <CGAL/config.h> 
//#include <CGAL/version.h> 

#include <string.h>

#include <CGAL/Polyhedron_incremental_builder_3.h>
typedef CGAL_Polyhedron::HalfedgeDS HalfedgeDS;

#include <iostream>


//#include "svg.h"
#include "Reindexer.h"
//#include "hash.h"
#include "GeometryUtils.h"

#include <map>
#include <queue>




// A modifier creating a triangle with the incremental builder.
template <class HDS>
class Build_triangle : public CGAL::Modifier_base<HDS> {
public:
     std::vector<IndexedTriangle> allTriangles;
     Reindexer<Vector3f> allVertices;

     Build_triangle( std::vector<IndexedTriangle> &allTriangles, Reindexer<Vector3f> &allVertices) {
        this->allTriangles=allTriangles;
        this->allVertices=allVertices;
     }
     void operator()( HDS& hds) {
         const Vector3f *verts = allVertices.getArray();
         CGAL::Polyhedron_incremental_builder_3<HDS> B( hds, true);

         B.begin_surface( allVertices.size(), allTriangles.size(), allTriangles.size()*2);

         for(int i=0;i<allVertices.size();i++) {
            B.add_vertex( typename HDS::Vertex::Point(verts[i][0],verts[i][1],verts[i][2]) );
            //std::cout << "**** vertex "<<i<<"  "<<verts[i][0]<<" "<<verts[i][1]<<" "<<verts[i][2]<<"\n";
         }

         for(const auto &t : allTriangles) {
            //std::cout << "**** face with "<<t[0]<<" "<<t[1]<<" "<<t[2]<<"\n";

            //std::cout << "facet " << t[0]<<" "<<t[1]<<" "<<t[2]<<"\n";
            B.begin_facet();
            B.add_vertex_to_facet( t[0]);
            B.add_vertex_to_facet( t[1]);
            B.add_vertex_to_facet( t[2]);
            B.end_facet();
		 }
         B.end_surface();
     }
};


//
// pour creer a partir de vertice et facets
//
template <class HDS>
class Build_facets : public CGAL::Modifier_base<HDS> {
public:
     const std::vector<int> facets;
     const std::vector<Vector3f> vertices;

     Build_facets( const std::vector<int> &facets, const std::vector<Vector3f> &vertices) : facets(facets), vertices(vertices) { }

     void operator()( HDS& hds) {
         //const Vector3f *verts = allVertices.getArray();
         CGAL::Polyhedron_incremental_builder_3<HDS> B( hds, true);

         // compte les facettes (n v1 v2 ... vn ...)
         int nb=0;
         for(int i=0;i<facets.size();i+=facets[i]+1) nb++;

         //std::cout << "nb vertex="<<vertices.size()<<" nbface="<<nb<<"\n";

         B.begin_surface( vertices.size()*10, nb*10, nb*2*10);

         int i=0;
         for(const auto &t : vertices) {
            B.add_vertex( typename HDS::Vertex::Point(t[0],t[1],t[2]) );
            //std::cout << "** vertex "<<i<<"  "<<t[0]<<" "<<t[1]<<" "<<t[2]<<"\n";
            i++;
         }

         for(int i=0;i<facets.size();i+=facets[i]+1) {
            //std::cout <<"facets @ "<<i<<"\n";
            B.begin_facet();
            //std::cout << "face \n";
            for(int j=1;j<=facets[i];j++) {
                    //std::cout  << facets[i+j]<< " \n";
                    B.add_vertex_to_facet( facets[i+j] );
            }
            B.end_facet();
            //std::cout <<"face done\n";
		 }
         B.end_surface();
     }
};






namespace CGALUtils {


     bool createPolyhedronFromVerticeAndFacets(const std::vector<Vector3f> &vertices,const std::vector<int> &facets,CGAL_Polyhedron &P) {
         //std::cout << "create from vertex data\n";

         /*
         for(int i=0;i<facets.size();i+=facets[i]+1) {
             int n=facets[i];
             std::cout << "face "<<i<<" n="<<n<<" : ";
             for(int j=1;j<=n;j++) {
                std::cout << " " << facets[i+j];
             }
             std::cout << "\n";
         }

         for(const auto &t : vertices) {
             std::cout << "vertex "<<t[0]<<","<<t[1]<<","<<t[2]<<"\n";
         }
         */

        Build_facets<HalfedgeDS> poly(facets,vertices);
        P.delegate( poly );
     }






    /****
	CGAL_Iso_cuboid_3 boundingBox(const CGAL_Nef_polyhedron3 &N)
	{
		CGAL_Iso_cuboid_3 result(0,0,0,0,0,0);
		CGAL_Nef_polyhedron3::Vertex_const_iterator vi;
		std::vector<CGAL_Nef_polyhedron3::Point_3> points;
		// can be optimized by rewriting bounding_box to accept vertices
		CGAL_forall_vertices(vi, N)
		points.push_back(vi->point());
		if (points.size()) result = CGAL::bounding_box(points.begin(), points.end());
		return result;
	}

	namespace {

		// lexicographic comparison
		bool operator < (Vector3d const& a, Vector3d const& b) {
			for (int i = 0; i < 3; i++) {
				if (a[i] < b[i]) return true;
				else if (a[i] == b[i]) continue;
				return false;
			}
			return false;
		}
	}

	struct VecPairCompare {
		bool operator ()(std::pair<Vector3d, Vector3d> const& a,
						 std::pair<Vector3d, Vector3d> const& b) const {
			return a.first < b.first || (!(b.first < a.first) && a.second < b.second);
		}
	};
    ****/


/*
	Create a PolySet from a Nef Polyhedron 3. return false on success, 
	true on failure. The trick to this is that Nef Polyhedron3 faces have 
	'holes' in them. . . while PolySet (and many other 3d polyhedron 
	formats) do not allow for holes in their faces. The function documents 
	the method used to deal with this
*/

    union swap {
        int i;
        float f;
        unsigned char b[4];
    };


	bool createSTLFromNefPolyhedron3(const CGAL_Nef_polyhedron3 &N,std::ostream &output,int binary)
	{
        //std::cout << "YO stl!\n";
		// 1. Build Indexed PolyMesh
		// 2. Validate mesh (manifoldness)
		// 3. Triangulate each face
		//    -> IndexedTriangleMesh
		// 4. Validate mesh (manifoldness)
		// 5. Create PolySet

		bool err = false;

		// 1. Build Indexed PolyMesh
		Reindexer<Vector3f> allVertices;
		std::vector<std::vector<IndexedFace>> polygons;

		CGAL_Nef_polyhedron3::Halffacet_const_iterator hfaceti;
		CGAL_forall_halffacets(hfaceti, N) {
			CGAL::Plane_3<CGAL_Kernel3> plane(hfaceti->plane());
			// Since we're downscaling to float, vertices might merge during this conversion.
			// To avoid passing equal vertices to the tessellator, we remove consecutively identical
			// vertices.
			polygons.push_back(std::vector<IndexedFace>());
			std::vector<IndexedFace> &faces = polygons.back();
			// the 0-mark-volume is the 'empty' volume of space. skip it.
			if (!hfaceti->incident_volume()->mark()) {
				CGAL_Nef_polyhedron3::Halffacet_cycle_const_iterator cyclei;
				CGAL_forall_facet_cycles_of(cyclei, hfaceti) {
					CGAL_Nef_polyhedron3::SHalfedge_around_facet_const_circulator c1(cyclei);
					CGAL_Nef_polyhedron3::SHalfedge_around_facet_const_circulator c2(c1);
					faces.push_back(IndexedFace());
					IndexedFace &currface = faces.back();
					CGAL_For_all(c1, c2) {
						CGAL_Point_3 p = c1->source()->center_vertex()->point();
						// Create vertex indices and remove consecutive duplicate vertices
						int idx = allVertices.lookup(vector_convert<Vector3f>(p));
                        // SEB
						if (currface.empty() || idx != currface.back()) currface.push_back(idx);
					}
					if (!currface.empty() && currface.front() == currface.back()) currface.pop_back();
					if (currface.size() < 3) faces.pop_back(); // Cull empty triangles
				}
			}
			if (faces.empty()) polygons.pop_back(); // Cull empty faces
		}

		// 2. Validate mesh (manifoldness)
		int unconnected = GeometryUtils::findUnconnectedEdges(polygons);
		if (unconnected > 0) {
            std::cout<<"Error: Non-manifold mesh encountered: %d unconnected edges"<< unconnected<<std::endl;
		}
		// 3. Triangulate each face
		const Vector3f *verts = allVertices.getArray();
		std::vector<IndexedTriangle> allTriangles;
		for(const auto &faces : polygons) {

			// CGAL::Vector_3<CGAL_Kernel3> nvec = plane.orthogonal_vector();
			// K::Vector_3 normal(CGAL::to_double(nvec.x()), CGAL::to_double(nvec.y()), CGAL::to_double(nvec.z()));
			std::vector<IndexedTriangle> triangles;
			bool err = GeometryUtils::tessellatePolygonWithHoles(verts, faces, triangles, NULL);
			if (!err) {
				for(const auto &t : triangles) {
					assert(t[0] >= 0 && t[0] < (int)allVertices.size());
					assert(t[1] >= 0 && t[1] < (int)allVertices.size());
					assert(t[2] >= 0 && t[2] < (int)allVertices.size());
					allTriangles.push_back(t);
				}
			}
		}

		// 4. Validate mesh (manifoldness)
		int unconnected2 = GeometryUtils::findUnconnectedEdges(allTriangles);
		if (unconnected2 > 0) {
            std::cout<<"Error: Non-manifold triangle mesh created: "<<unconnected2<<" unconnected edges"<<std::endl;
		}


        // OUTPUT!
        if( binary==0 ) {
            setlocale(LC_NUMERIC, "C"); // Ensure radix is . (not ,) in output
            output << "solid LuaCAD_Model\n";


            for(const auto &t : allTriangles) {
                Vector3f normal = (verts[t[1]] - verts[t[0]]).cross(verts[t[2]] - verts[t[0]]);
                normal.normalize();

                output << "facet normal ";
                if (is_finite(normal) && !is_nan(normal)) {
                    output << normal[0] << " " << normal[1] << " " << normal[2] << "\n";
                } else {
                    output << "0 0 0\n";
                }

                output << " outer loop\n";
                output << "  vertex "<<verts[t[0]][0]<<" "<<verts[t[0]][1]<<" "<<verts[t[0]][2]<<"\n";
                output << "  vertex "<<verts[t[1]][0]<<" "<<verts[t[1]][1]<<" "<<verts[t[1]][2]<<"\n";
                output << "  vertex "<<verts[t[2]][0]<<" "<<verts[t[2]][1]<<" "<<verts[t[2]][2]<<"\n";
                output << " endloop\n";
                output << "endfacet\n";
            }

            output << "endsolid LuaCAD_Model\n";
            setlocale(LC_NUMERIC, "");      // Set default locale
        }else{
            // binaire!!!
            //f.write( static_cast <char*> ( &(pts.count) ), sizeof( unsigned ) );
            unsigned char buf[80];
            swap s;
            for(int i=0;i<80;i++) buf[i]=' ';
            buf[0]='V';
            buf[1]='C';
            buf[2]='G';
            
            buf[10]='C';
            buf[11]='O';
            buf[12]='L';
            buf[13]='O';
            buf[14]='R';
            buf[15]='=';
            buf[16]=255; // r
            buf[17]=64;  // g
            buf[18]=64;  // b
            buf[19]=128;  // a
           
            output.write( (char *)buf,80);
            int sz=allTriangles.size();
            s.i=sz;
            //std::cout << "size is "<<sz<<"\n";
            output.write( (const char *)s.b,4);

            for(const auto &t : allTriangles) {
                Vector3f normal = (verts[t[1]] - verts[t[0]]).cross(verts[t[2]] - verts[t[0]]);
                normal.normalize();

                float x=normal[0];
                float y=normal[1];
                float z=normal[2];

                if (!is_finite(normal) || is_nan(normal)) {
                    x=y=z=0.0;
                }
                s.f=x; output.write( (const char *)s.b, 4 );
                s.f=y; output.write( (const char *)s.b, 4 );
                s.f=z; output.write( (const char *)s.b, 4 );

                for(int i=0;i<3;i++) {
                    for(int j=0;j<3;j++) {
                        s.f=verts[t[i]][j];
                        output.write( (const char *)s.b, 4 );
                    }
                }
                unsigned short color=0x8000|((((31<<5)|31)<<5)|0);
                color=0xf0f0;
                output.write( (const char *)&color, 2 );
            }

        }


		return err;
	}



bool createNefPolyhedron3FromSTL(std::istream &input,CGAL_Nef_polyhedron3 &N)
{
    // lire un stl
    unsigned char buf[80];

    // check if binary
    input.read((char *)buf,5);
    buf[5]=0;

    std::vector<Vector3f> vertices;
    std::vector<int> facets;
    int *idx;

    float norm[3];
    Vector3f pos;

    if( strcmp("solid",(const char *)buf)==0 ) {
        //std::cout << "ASCII\n";

        std::string line;
        std::getline(input, line);
        //std::cout << "got "<<line<<"\n";
        int nf=0;
        while (!input.eof()) {
            std::getline(input, line);
            //std::cout << "next is "<<line<<"\n";
            if( sscanf(line.c_str()," facet normal %f %f %f ",norm+0,norm+1,norm+2) != 3 ) continue;
            //std::cout << "facet " << norm[0]<<","<<norm[1]<<","<<norm[2]<<"\n";
            // we have a facet
            std::getline(input, line); sscanf(line.c_str()," outer loop ");
            for(int j=0;j<3;j++) {
                std::getline(input, line);
                sscanf(line.c_str()," vertex %f %f %f ",&pos[0],&pos[1],&pos[2]);
                //std::cout << "line is "<<line<<"\n";
                //std::cout << "vertex " << pos[0]<<","<<pos[1]<<","<<pos[2]<<"\n";
                vertices.push_back(pos);
            }
            std::getline(input, line); sscanf(line.c_str()," endloop ");
            facets.push_back(nf*3+0);
            facets.push_back(nf*3+1);
            facets.push_back(nf*3+2);
            nf++;
        }
        idx=(int *)malloc(nf*3*sizeof(int));
        for(int i=0;i<nf*3;i++) { idx[i]=i; }

    }else{
        //std::cout << "BIN\n";

        input.read((char *)buf+5,80-5);
        swap s;
        input.read((char *)&s,4);
        int sz=s.i;
        //std::cout << "input size is "<<sz<<" facets\n";

        idx=(int *)malloc(sz*3*sizeof(int));

        for(int i=0;i<sz;i++) {
            input.read((char *)&s,4);norm[0]=s.f;
            input.read((char *)&s,4);norm[1]=s.f;
            input.read((char *)&s,4);norm[2]=s.f;

            //std::cout << "norm "<<norm[0]<<","<<norm[1]<<","<<norm[2]<<"\n";

            for(int j=0;j<3;j++) {
                input.read((char *)&s,4);pos[0]=s.f;
                input.read((char *)&s,4);pos[1]=s.f;
                input.read((char *)&s,4);pos[2]=s.f;
                vertices.push_back(pos);
                //std::cout << "vertex "<<pos[0]<<","<<pos[1]<<","<<pos[2]<<"\n";
            }
            facets.push_back(i*3+0);
            facets.push_back(i*3+1);
            facets.push_back(i*3+2);

            idx[i*3+0]=i*3+0;
            idx[i*3+1]=i*3+1;
            idx[i*3+2]=i*3+2;

            //std::cout << "facet "<<i*3+0<<","<<i*3+1<<","<<i*3+2<<"\n";

            input.read((char *)buf,2);
        }
    }

    //std::cout << "vertices sz "<<vertices.size()<<"\n";
    //std::cout << "facets sz "<<facets.size()<<"\n";

    /*
    for(int i=0;i<vertices.size();i++) {
        std::cout << "@ vertex "
            <<vertices[i][0]
            <<vertices[i][1]
            <<vertices[i][2]<<"\n";
    }
    for(int i=0;i<facets.size();i++) {
        std::cout << "@ facet "<<facets[i]<<"\n";
    }
    */

    // simplification des distances, version simpliste
    int nb=0;
    for(int i=0;i<vertices.size();i++) {
        for(int j=0;j<i;j++) {
            Vector3f d2;
            d2=vertices[i]-vertices[j];
            double dist=d2.dot(d2);
            //if( dist<1.0 ) std::cout << "dist "<<i<<" "<<j<<" " << dist<<"\n";
            if( dist<0.00000001 ) {
                //std::cout << "merge "<<i<<" and "<<j<<"\n";
                idx[i]=j;
                nb++;
                break;
            }
        }
    }
    //std::cout << "flushed "<<nb<<" donc il reste " << vertices.size()-nb<<"\n";

    int k=0;
    int *idxFinal;
    idxFinal=(int *)malloc(vertices.size()*sizeof(int));

    for(int i=0;i<vertices.size();i++) {
        if( idx[i]==i ) {
            idxFinal[i]=k++;
        }else{
            idxFinal[i]=idxFinal[idx[i]];
        }
    }

    //for(int i=0;i<vertices.size();i++) std::cout << "idx["<<i<<"] is "<<idx[i]<<" is "<<idxFinal[i]<<"\n";

    std::vector<Vector3f> verticesFinal;
    std::vector<int> facetsFinal;


    for(int i=0;i<vertices.size();i++) if( idx[i]==i ) verticesFinal.push_back(vertices[i]);
    for(int i=0;i<facets.size();i++) {
        if( i%3==0 ) facetsFinal.push_back(3);
        facetsFinal.push_back(idxFinal[facets[i]]);
    }

    //std::cout << "vertices final sz "<<verticesFinal.size()<<"\n";

    CGAL_Polyhedron P;
    createPolyhedronFromVerticeAndFacets(verticesFinal,facetsFinal,P);
    N=CGAL_Nef_polyhedron3(P);

    free(idx);
    free(idxFinal);

    return false;
}



/*
	Create a PolySet from a Nef Polyhedron 3. return false on success, 
	true on failure. The trick to this is that Nef Polyhedron3 faces have 
	'holes' in them. . . while PolySet (and many other 3d polyhedron 
	formats) do not allow for holes in their faces. The function documents 
	the method used to deal with this
*/
#if 1
	bool createPolyhedronFromNefPolyhedron3(const CGAL_Nef_polyhedron3 &N,CGAL_Polyhedron &ph)//, PolySet &ps)
	{
		// 1. Build Indexed PolyMesh
		// 2. Validate mesh (manifoldness)
		// 3. Triangulate each face
		//    -> IndexedTriangleMesh
		// 4. Validate mesh (manifoldness)
		// 5. Create PolySet

		bool err = false;

		// 1. Build Indexed PolyMesh
		Reindexer<Vector3f> allVertices;
		std::vector<std::vector<IndexedFace>> polygons;

		CGAL_Nef_polyhedron3::Halffacet_const_iterator hfaceti;
		CGAL_forall_halffacets(hfaceti, N) {
			CGAL::Plane_3<CGAL_Kernel3> plane(hfaceti->plane());
			// Since we're downscaling to float, vertices might merge during this conversion.
			// To avoid passing equal vertices to the tessellator, we remove consecutively identical
			// vertices.
			polygons.push_back(std::vector<IndexedFace>());
			std::vector<IndexedFace> &faces = polygons.back();
			// the 0-mark-volume is the 'empty' volume of space. skip it.
			if (!hfaceti->incident_volume()->mark()) {
				CGAL_Nef_polyhedron3::Halffacet_cycle_const_iterator cyclei;
				CGAL_forall_facet_cycles_of(cyclei, hfaceti) {
					CGAL_Nef_polyhedron3::SHalfedge_around_facet_const_circulator c1(cyclei);
					CGAL_Nef_polyhedron3::SHalfedge_around_facet_const_circulator c2(c1);
					faces.push_back(IndexedFace());
					IndexedFace &currface = faces.back();
					CGAL_For_all(c1, c2) {
						CGAL_Point_3 p = c1->source()->center_vertex()->point();
						// Create vertex indices and remove consecutive duplicate vertices
						int idx = allVertices.lookup(vector_convert<Vector3f>(p));
                        // SEB
						if (currface.empty() || idx != currface.back()) currface.push_back(idx);
					}
					if (!currface.empty() && currface.front() == currface.back()) currface.pop_back();
					if (currface.size() < 3) faces.pop_back(); // Cull empty triangles
				}
			}
			if (faces.empty()) polygons.pop_back(); // Cull empty faces
		}

		// 2. Validate mesh (manifoldness)
		int unconnected = GeometryUtils::findUnconnectedEdges(polygons);
		if (unconnected > 0) {
            std::cout<<"Error: Non-manifold mesh encountered: %d unconnected edges"<< unconnected<<std::endl;
		}
		// 3. Triangulate each face
		const Vector3f *verts = allVertices.getArray();
		std::vector<IndexedTriangle> allTriangles;
		for(const auto &faces : polygons) {
#if 0 // For debugging
			std::cerr << "---\n";
			for(const auto &poly : faces) {
				for(auto i : poly) {
					std::cerr << i << " ";
				}
				std::cerr << "\n";
			}
#if 0 // debug
			std::cerr.precision(20);
			for(const auto &poly : faces) {
				for(auto i : poly) {
					std::cerr << verts[i][0] << "," << verts[i][1] << "," << verts[i][2] << "\n";
				}
				std::cerr << "\n";
			}
#endif // debug
			std::cerr << "-\n";
#endif // debug
#if 0 // For debugging
		std::cerr.precision(20);
		for (size_t i=0;i<allVertices.size();i++) {
			std::cerr << verts[i][0] << ", " << verts[i][1] << ", " << verts[i][2] << "\n";
		}		
#endif // debug

			/* at this stage, we have a sequence of polygons. the first
				 is the "outside edge' or 'body' or 'border', and the rest of the
				 polygons are 'holes' within the first. there are several
				 options here to get rid of the holes. we choose to go ahead
				 and let the tessellater deal with the holes, and then
				 just output the resulting 3d triangles*/

			// We cannot trust the plane from Nef polyhedron to be correct.
			// Passing an incorrect normal vector can cause a crash in the constrained delaunay triangulator
			// See http://cgal-discuss.949826.n4.nabble.com/Nef3-Wrong-normal-vector-reported-causes-triangulator-crash-tt4660282.html
			// CGAL::Vector_3<CGAL_Kernel3> nvec = plane.orthogonal_vector();
			// K::Vector_3 normal(CGAL::to_double(nvec.x()), CGAL::to_double(nvec.y()), CGAL::to_double(nvec.z()));
			std::vector<IndexedTriangle> triangles;
			bool err = GeometryUtils::tessellatePolygonWithHoles(verts, faces, triangles, NULL);
			if (!err) {
				for(const auto &t : triangles) {
					assert(t[0] >= 0 && t[0] < (int)allVertices.size());
					assert(t[1] >= 0 && t[1] < (int)allVertices.size());
					assert(t[2] >= 0 && t[2] < (int)allVertices.size());
					allTriangles.push_back(t);
				}
			}
		}

#if 0 // For debugging
		for(const auto &t : allTriangles) {
			std::cerr << t[0] << " " << t[1] << " " << t[2] << "\n";
		}
#endif // debug
		// 4. Validate mesh (manifoldness)
		int unconnected2 = GeometryUtils::findUnconnectedEdges(allTriangles);
		if (unconnected2 > 0) {
            std::cout<<"Error: Non-manifold triangle mesh created: "<<unconnected2<<" unconnected edges"<<std::endl;
		}


        // incremental builder of polyhedron
        // :
        Build_triangle<HalfedgeDS> triangle(allTriangles,allVertices);
        ph.delegate( triangle);


		return err;
	}
#endif // createPolySetFromNefPolyhedron3



/*
	Create a PolySet from a Nef Polyhedron 3, but simply keep the vertices and facets in lists compatible with opengl TRIANGLE

    This is EXACTLY the same function as createPolyhedronFromNePolyhedron3, except the last 3 lines
*/
	bool createVerticesAndFacetsFromNefPolyhedron3(const CGAL_Nef_polyhedron3 &N,float *&vertices,int *&facets,int &nbV,int &nbF)
	{
		bool err = false;

		// 1. Build Indexed PolyMesh
		Reindexer<Vector3f> allVertices;
		std::vector<std::vector<IndexedFace>> polygons;

		CGAL_Nef_polyhedron3::Halffacet_const_iterator hfaceti;
		CGAL_forall_halffacets(hfaceti, N) {
			CGAL::Plane_3<CGAL_Kernel3> plane(hfaceti->plane());
			// Since we're downscaling to float, vertices might merge during this conversion.
			// To avoid passing equal vertices to the tessellator, we remove consecutively identical
			// vertices.
			polygons.push_back(std::vector<IndexedFace>());
			std::vector<IndexedFace> &faces = polygons.back();
			// the 0-mark-volume is the 'empty' volume of space. skip it.
			if (!hfaceti->incident_volume()->mark()) {
				CGAL_Nef_polyhedron3::Halffacet_cycle_const_iterator cyclei;
				CGAL_forall_facet_cycles_of(cyclei, hfaceti) {
					CGAL_Nef_polyhedron3::SHalfedge_around_facet_const_circulator c1(cyclei);
					CGAL_Nef_polyhedron3::SHalfedge_around_facet_const_circulator c2(c1);
					faces.push_back(IndexedFace());
					IndexedFace &currface = faces.back();
					CGAL_For_all(c1, c2) {
						CGAL_Point_3 p = c1->source()->center_vertex()->point();
						// Create vertex indices and remove consecutive duplicate vertices
						int idx = allVertices.lookup(vector_convert<Vector3f>(p));
                        // SEB
						if (currface.empty() || idx != currface.back()) currface.push_back(idx);
					}
					if (!currface.empty() && currface.front() == currface.back()) currface.pop_back();
					if (currface.size() < 3) faces.pop_back(); // Cull empty triangles
				}
			}
			if (faces.empty()) polygons.pop_back(); // Cull empty faces
		}

		// 2. Validate mesh (manifoldness)
		int unconnected = GeometryUtils::findUnconnectedEdges(polygons);
		if (unconnected > 0) {
            std::cout<<"Error: Non-manifold mesh encountered: %d unconnected edges"<< unconnected<<std::endl;
		}
		// 3. Triangulate each face
		const Vector3f *verts = allVertices.getArray();
		std::vector<IndexedTriangle> allTriangles;
		for(const auto &faces : polygons) {

			std::vector<IndexedTriangle> triangles;
			bool err = GeometryUtils::tessellatePolygonWithHoles(verts, faces, triangles, NULL);
			if (!err) {
				for(const auto &t : triangles) {
					assert(t[0] >= 0 && t[0] < (int)allVertices.size());
					assert(t[1] >= 0 && t[1] < (int)allVertices.size());
					assert(t[2] >= 0 && t[2] < (int)allVertices.size());
					allTriangles.push_back(t);
				}
			}
		}

		// 4. Validate mesh (manifoldness)
		int unconnected2 = GeometryUtils::findUnconnectedEdges(allTriangles);
		if (unconnected2 > 0) {
            std::cout<<"Error: Non-manifold triangle mesh created: "<<unconnected2<<" unconnected edges"<<std::endl;
		}

        vertices=(float *)malloc(allVertices.size()*3*sizeof(float));
        facets=(int *)malloc(allTriangles.size()*3*sizeof(int));

        for(int i=0;i<allVertices.size();i++) {
            vertices[i*3+0]=verts[i][0];
            vertices[i*3+1]=verts[i][1];
            vertices[i*3+2]=verts[i][2];
        }

        int i=0;
        for(const auto &t : allTriangles) {
              //std::cout << "**** face with "<<t[0]<<" "<<t[1]<<" "<<t[2]<<"\n";
              facets[i++]=t[0];
              facets[i++]=t[1];
              facets[i++]=t[2];
        }

        nbV=allVertices.size();
        nbF=allTriangles.size();

        // incremental builder of polyhedron
        // :
        //Build_triangle<HalfedgeDS> triangle(allTriangles,allVertices);
        //ph.delegate( triangle);

		return err;
	}




static void updateCenterOfMass( CGAL_Polyhedron::Point_3 const& p1,
                                CGAL_Polyhedron::Point_3 const& p2,
                                CGAL_Polyhedron::Point_3 const& p3,
                                CGAL_Polyhedron::Point_3 const& ps,
                                NT3 &volumeTotal,
                                NT3 centerOfMass[3]) {
        //std::cout << "p1 ("<<p1.x()<<","<<p1.y()<<","<<p1.z()<<")\n";
        //std::cout << "p2 ("<<p2.x()<<","<<p2.y()<<","<<p2.z()<<")\n";
        //std::cout << "p3 ("<<p3.x()<<","<<p3.y()<<","<<p3.z()<<")\n";
        //std::cout << "ps ("<<ps.x()<<","<<ps.y()<<","<<ps.z()<<")\n";
        NT3 vol=abs(CGAL::volume(ps,p1,p2,p3));
        NT3 centroid[3];
        centroid[0]=(p1[0]+p2[0]+p3[0]+ps[0])/4;
        centroid[1]=(p1[1]+p2[1]+p3[1]+ps[1])/4;
        centroid[2]=(p1[2]+p2[2]+p3[2]+ps[2])/4;
        volumeTotal+=vol;
        centerOfMass[0]+=centroid[0]*vol;
        centerOfMass[1]+=centroid[1]*vol;
        centerOfMass[2]+=centroid[2]*vol;
        //std::cout << "volume="<<to_double(vol)<<" centroid="<<to_double(centroid[0])<<","<<to_double(centroid[1])<<","<<to_double(centroid[2])<<"\n";
        //std::cout<<"center of mass is ("<<to_double(centerOfMass[0])<<","<<to_double(centerOfMass[1])<<","<<to_double(centerOfMass[2])<<")\n";
}



void computeVolume(const CGAL_Nef_polyhedron3 &N,NT3 &volumeTotal,NT3 centerOfMass[3]) {
    // attention, il faut etre sur que N->p3()->is_simple(), sinon ca fonctionne pas fort
    //std::cout << "volumen of nef!"<<std::endl;
    CGAL::Timer t;
    std::vector<CGAL_Polyhedron> parts;
    t.start();
    CGAL_Nef_polyhedron3 decomp=N;
    CGAL::convex_decomposition_3(decomp);
    std::cout <<"nb volumes = "<<decomp.number_of_volumes()<<"\n";
    // the first volume is the outer volume, which ignored in the decomposition
    CGAL_Nef_polyhedron3::Volume_const_iterator ci = decomp.volumes_begin();
    for(; ci != decomp.volumes_end(); ++ci) {
        //std::cout <<"one part...\n";
        if(ci->mark()) {
            //std::cout<<" converting to poly.\n";
            CGAL_Polyhedron poly;
            decomp.convert_inner_shell_to_polyhedron(ci->shells_begin(), poly);
            parts.push_back(poly);
        }
    }
    t.stop();
    std::cout << "Center of mass: decomposed into "<<parts.size()<< " convex parts\n";
    std::cout << "Center of mass: decomposition took "<<t.time()<<" sec\n";

    volumeTotal=0;
    centerOfMass[0]=0;
    centerOfMass[1]=0;
    centerOfMass[2]=0;
    // on affiche chaque partie...
    for(int i=0;i<(int)parts.size();i++) {
        //std::cout<<"part "<<i<<" : facets = "<<parts[i].size_of_facets()<<", vertex="<<parts[i].size_of_vertices()<<"\n";
        //
        // on trouve le centre des points (xs,ys,zs)
        //
        CGAL_Polyhedron::Vertex_const_iterator vi = parts[i].vertices_begin();
        NT3 xs=0,ys=0,zs=0;
        int nb=0;
        for(;vi!=parts[i].vertices_end();vi++) {
            CGAL_Polyhedron::Point_3 const& p = vi->point();
            xs+=p[0];
            ys+=p[1];
            zs+=p[2];
            nb++;
            //std::cout << "vertice ("<<x<<","<<y<<","<<z<<")\n";
        }
        CGAL_Polyhedron::Point_3 ps(xs/nb,ys/nb,zs/nb);
        //std::cout << "center is ("<<ps[0]<<","<<ps[1]<<","<<ps[2]<<")\n";
        //
        // on visite chaque facette...
        // on la triangule
        CGAL_Polyhedron::Facet_const_iterator fi = parts[i].facets_begin();
        for(;fi!=parts[i].facets_end();fi++) {
            //std::cout << "facet "<<fi->is_triangle()<<" , nbedge="<<fi->facet_degree()<<"\n";
            if( fi->is_triangle() ) {
                // tetrahedre avec le centre... et voila!
                CGAL_Polyhedron::Facet::Halfedge_around_facet_const_circulator he,end;
                end = he = fi->facet_begin();
                //CGAL_Polyhedron P;
                CGAL_Polyhedron::Point_3 const& p1 = he->vertex()->point();he++;
                CGAL_Polyhedron::Point_3 const& p2 = he->vertex()->point();he++;
                CGAL_Polyhedron::Point_3 const& p3 = he->vertex()->point();
                updateCenterOfMass(p1,p2,p3,ps,volumeTotal,centerOfMass);
            }else{
                // on decoupe!
                // trouve le centre de la facette
                CGAL_Polyhedron::Facet::Halfedge_around_facet_const_circulator he,end;
                NT3 xc=0,yc=0,zc=0;
                end = he = fi->facet_begin();
                CGAL_For_all(he,end) {
                    CGAL_Polyhedron::Point_3 const& p = he->vertex()->point();
                    xc+=p[0];yc+=p[1];zc+=p[2];
                    //std::cout << "facet halfedge ("<<to_double(p[0])<<","<<to_double(p[1])<<","<<to_double(p[2])<<")\n";
                }
                xc/=(int)fi->facet_degree();
                yc/=(int)fi->facet_degree();
                zc/=(int)fi->facet_degree();
                CGAL_Polyhedron::Point_3 pc(xc,yc,zc);
                //std::cout << "facet center ("<<to_double(xc)<<","<<to_double(yc)<<","<<to_double(zc)<<")\n";
                // on fait le tour de la facette, en ajoutant les tetra (p1,p2,ps,pc)
                end = he = fi->facet_begin();
                CGAL_For_all(he,end) {
                    CGAL_Polyhedron::Point_3 const& p1 = he->vertex()->point();
                    CGAL_Polyhedron::Point_3 const& p2 = he->next()->vertex()->point();
                    //std::cout << "facet halfedge p1 ("<<to_double(p1[0])<<","<<to_double(p1[1])<<","<<to_double(p1[2])<<")\n";
                    //std::cout << "facet halfedge p2 ("<<to_double(p2[0])<<","<<to_double(p2[1])<<","<<to_double(p2[2])<<")\n";
                    updateCenterOfMass(p1,p2,pc,ps,volumeTotal,centerOfMass);
                }
            }
        }
    }

    // finalise le centre de masse
    if( volumeTotal>0 ) {
        centerOfMass[0]/=volumeTotal;
        centerOfMass[1]/=volumeTotal;
        centerOfMass[2]/=volumeTotal;
    }
    //std::cout<<"Volume Total = "<<to_double(volumeTotal)<<"\n";
    //std::cout<<"center of mass is ("<<to_double(centerOfMass[0])<<","<<to_double(centerOfMass[1])<<","<<to_double(centerOfMass[2])<<")\n";

    /*
    CGAL_Iso_cuboid_3 bb=boundingBox(N);
    std::cout << bb<<"\n";
    double xmin=CGAL::to_double(bb.xmin());
    double xmax=CGAL::to_double(bb.xmax());
    std::cout << xmin << "," << xmax<<"\n";
    */
}




CGAL_Iso_cuboid_3 boundingBox(const CGAL_Nef_polyhedron3 &N) {
    CGAL_Iso_cuboid_3 result(0,0,0,0,0,0);
    CGAL_Nef_polyhedron3::Vertex_const_iterator vi;
    std::vector<CGAL_Nef_polyhedron3::Point_3> points;
    // can be optimized by rewriting bounding_box to accept vertices
    CGAL_forall_vertices(vi, N) points.push_back(vi->point());
    if (points.size()) result = CGAL::bounding_box(points.begin(), points.end());
    return result;
}




void extractPoints(std::vector<CGAL_Point_3> &points,const CGAL_Nef_polyhedron3 &N) {

    int nb=0;
    for( CGAL_Nef_polyhedron3::Vertex_const_iterator i = N.vertices_begin();
            i != N.vertices_end(); ++i) {
                    points.push_back(vector_convert<CGAL_Point_3>(i->point()));
                    nb++;
    }
    //std::cout << "extracted "<<nb<<" points\n";
}


void hull(std::vector<CGAL_Point_3> &points,CGAL_Nef_polyhedron3 &N) {
    // Apply hull
    //std::cout<< "applying hull nbpoints="<<points.size()<<"\n";
    //bool success = false;
    if( points.size() >= 4 ) {
        //try {
            CGAL_Polyhedron r;
            CGAL::convex_hull_3(points.begin(), points.end(), r);
            //std::cout<<"After hull vertices: "<< r.size_of_vertices() << "\n";
            //std::cout<<"After hull facets: "<< r.size_of_facets() << "\n";
            //std::cout<<"After hull closed: "<< r.is_closed() << "\n";
            //std::cout<<"After hull valid: "<< r.is_valid()<<"\n";
            N=CGAL_Nef_polyhedron3(r);
            //success = !createPolySetFromPolyhedron(r, result);
        //} catch (const CGAL::Failure_exception &e) {
        //    std::cout<<"ERROR: CGAL error in applyHull(): "<< e.what()<<"\n";
        //}
    }
}







}; // namespace CGALUtils









