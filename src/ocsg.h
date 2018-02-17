#ifndef OCSG_H
#define OCSG_H

//
// support opencsg
// with nef understanding
//
//

// en premier!
#include <GL/glew.h>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_GLX
#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3native.h>



#include <opencsg.h>
#include <displaylistPrimitive.h>

typedef std::vector<OpenCSG::Primitive*> t_primitives;
typedef std::vector<t_primitives*> t_products;

class ocsg {
    private:
        GLFWwindow* window;
        int width,height;
        int shifted; // shift active?
        int leftButton; // 1=left clic active
        double lastX,lastY; // used when moving left-clic

        // un seul pour l'instant
        //t_primitives primitives;
        t_products products;

        int nbRender;

        // orientation de la camera
        double rotx,roty,rotz;
        // distFactor
        double distFactor;

        // temporaire
        //GLuint vertex_buffer, vertex_shader, fragment_shader, program;
        //GLint mvp_location, vpos_location, vcol_location;

        GLuint program; // shaderinfo 0
        GLuint uniColor1; // uniforma color1

        void enable_opencsg_shaders(void);
        void setupCamera(double dist,bool perspective,double fov,double rx,double ry,double rz);
        static void drawTriangle(float *p0,float *p1,float *p2, bool mirrored);

        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void mouse_callback( GLFWwindow *window,  double x,double y);
        static void error_callback(int error, const char* description);
        static void resize_callback( GLFWwindow *window,  GLsizei width, GLsizei height);
        static void mouse_button_callback( GLFWwindow *window, int button, int action, int mods);
        static void scroll_callback( GLFWwindow *window,  double dx,double dy);
        


         
    public:
        ocsg() {
            rotx=roty=rotz=0.0; distFactor=1.0;
        };

        int init(void);
        void uninit(void);
        void clear(void); // efface tous les produits (donc toutes les primitives), reset product to 0
        int draw(char op,float *vertices,int *facets,int nbV,int nbF); // op:'-' or '*'
        void newProduct(); // start of a new product
        int render(void);


};




#endif
