
//
// support opencsg
// with nef understanding
//
//

#include <ocsg.h>



#include "linmath.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

void ocsg::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    ocsg *me=(ocsg *)glfwGetWindowUserPointer(window);
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, 1);
    std::cout << "KEY "<<key<<" code="<<scancode<<"  action="<<action<<" mods="<<mods<<"\n";
    if( key==GLFW_KEY_LEFT_SHIFT || key==GLFW_KEY_RIGHT_SHIFT ) {
        me->shifted=(action==GLFW_PRESS);
    }
}

void ocsg::mouse_callback( GLFWwindow *window,  double x,double y) {
    ocsg *me=(ocsg *)glfwGetWindowUserPointer(window);
    //glfwGetWindowSize(window, &width, &height);

    // ajuste la rotation
    if( me->shifted ) {
        me->rotz=(double)x/me->width*360.0;
        me->roty=((double)y/me->height-0.5)*2.0*90.0;
        std::cout << "mouse "<<x<<","<<y<<" : "<<me->roty<<","<<me->rotz<<"\n";
    }

    if( me->leftButton ) {
        if( me->lastX>=0 && me->lastY>=0 ) {
            me->rotz+=(x-me->lastX);
            me->roty+=(y-me->lastY);
            if( me->roty<-90 ) me->roty=-90.0;
            if( me->roty>90 ) me->roty=90.0;
            if( me->rotz<0 ) me->rotz+=360.0;
            if( me->rotz>=360.0 ) me->rotz-=360.0;
        }
        me->lastX=x;
        me->lastY=y;
    }
}

void ocsg::scroll_callback( GLFWwindow *window,  double dx,double dy) {
    ocsg *me=(ocsg *)glfwGetWindowUserPointer(window);

    if( dy<0 ) me->distFactor*=1.1;
    else if( dy>0 ) me->distFactor/=1.1;

    std::cout << "scroll "<<dx<<","<<dy<<"\n";
}

void ocsg::mouse_button_callback( GLFWwindow *window, int button, int action, int mods) {
    ocsg *me=(ocsg *)glfwGetWindowUserPointer(window);
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        std::cout << "Bouton droit!\n";
    }else if (button == GLFW_MOUSE_BUTTON_LEFT ) {
        std::cout << "Bouton gauche! " << action<<"\n";
        me->leftButton=(action==GLFW_PRESS);
        me->lastX=me->lastY=-1.0;
        /*
        if( action==GLFW_PRESS ) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_HAND_CURSOR);
        }else{
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        */
        /*
GLLFW_ARROW_CURSOR
GLFW_IBEAM_CURSOR
GLFW_CROSSHAIR_CURSOR
GLFW_HAND_CURSOR
GLFW_HRESIZE_CURSOR
GLFW_VRESIZE_CURSOR
*/
    }else if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {
        std::cout << "Bouton milieu!\n";
    }
}

void ocsg::resize_callback( GLFWwindow *window,  GLsizei width, GLsizei height) {
    ocsg *me=(ocsg *)glfwGetWindowUserPointer(window);
    std::cout << "size is "<<width<<","<<height<<"\n";
    me->width=width;
    me->height=height;
    glViewport(0, 0, width, height);
}


void ocsg::error_callback(int error, const char* description)
{
        fprintf(stderr, "Error: %s\n", description);
}




///////////////////////////////////////////////////////////////

void ocsg::enable_opencsg_shaders()
{
    /* Uniforms: 1 color - face color
    Outputs: shading
    */
    const char *vs_source =
      "varying float shading;\n"
      "void main() {\n"
      "  vec4 p0 = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
      "  gl_Position = p0;\n"
      "  vec3 normal, lightDir;\n"
      "  normal = normalize(gl_NormalMatrix * gl_Normal);\n"
      "  lightDir = normalize(vec3(gl_LightSource[0].position));\n"
      "  shading = 0.2 + abs(dot(normal, lightDir));\n"
      "}\n";
    /* Inputs: shading */
    const char *fs_source =
      "uniform vec4 color1;\n"
      "varying float shading;\n"
      "void main() {\n"
      "  gl_FragColor = vec4(color1.r * shading, color1.g * shading, color1.b * shading, color1.a);\n"
      "}\n";

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    GLenum err1 = glGetError();
    if (err1 != GL_NO_ERROR) { fprintf(stderr, "1 OpenGL Error: %s\n", gluErrorString(err1)); }
    glShaderSource(vs, 1, (const GLchar**)&vs_source, NULL);
    glCompileShader(vs);

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, (const GLchar**)&fs_source, NULL);
    glCompileShader(fs);

    program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    uniColor1 = glGetUniformLocation(program, "color1");

    GLenum err = glGetError();
    if (err != GL_NO_ERROR) { fprintf(stderr, "OpenGL Error: %s\n", gluErrorString(err)); }

    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
      int loglen;
      char logbuffer[1000];
      glGetProgramInfoLog(program, sizeof(logbuffer), &loglen, logbuffer);
      fprintf(stderr, "OpenGL Program Linker Error:\n%.*s", loglen, logbuffer);
    } else {
      int loglen;
      char logbuffer[1000];
      glGetProgramInfoLog(program, sizeof(logbuffer), &loglen, logbuffer);
      if (loglen > 0) {
        fprintf(stderr, "OpenGL Program Link OK:\n%.*s", loglen, logbuffer);
      }
      glValidateProgram(program);
      glGetProgramInfoLog(program, sizeof(logbuffer), &loglen, logbuffer);
      if (loglen > 0) {
        fprintf(stderr, "OpenGL Program Validation results:\n%.*s", loglen, logbuffer);
      }
    }
}


///////////////////////////////////////////////////////////////


int ocsg::init(void)
{
    std::cout << "init\n";

    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) return(-1);


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return(-1);
    }

    //glfwSetWindowPos(window,offx,offy);

    glfwSetWindowUserPointer(window,this); // pour les callback

    int wid=glfwGetX11Window(window);
    std::cout << "wid is "<<wid<<"\n";

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window,mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetWindowSizeCallback(window,resize_callback);
    glfwSetScrollCallback(window, scroll_callback);


    glfwMakeContextCurrent(window);

    // get initial window size
    glfwGetWindowSize(window, &width, &height);
    std::cout << "initial size is "<<width<<","<<height<<"\n";

    // MUST be called after MakeContextCurrent
    glewExperimental=GL_TRUE;
    if( glewInit()!=GLEW_OK ) {
         printf("No glew!\n");
         return(-1);
    }
    glfwSwapInterval(1);

    int OpenGLVersion[2];
    glGetIntegerv(GL_MAJOR_VERSION, &OpenGLVersion[0]);
    glGetIntegerv(GL_MINOR_VERSION, &OpenGLVersion[1]);
    printf("OpenGL major version = %d\n",OpenGLVersion[0]);
    printf("OpenGL minor version = %d\n",OpenGLVersion[1]);




    glEnable(GL_DEPTH_TEST);
    glDepthRange(-100000.0, +100000.0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light_position0[] = {-1.0, -1.0, +1.0, 0.0};
    GLfloat light_position1[] = {+1.0, +1.0, -1.0, 0.0};

    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);

    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    // The following line is reported to fix issue #71
    //glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 64);
    glEnable(GL_COLOR_MATERIAL);

    enable_opencsg_shaders();


    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

    // Setup the view of the CSG shape
    //glMatrixMode(GL_PROJECTION);
    //gluPerspective(40.0, 1.0, 1.0, 10.0);
    //glMatrixMode(GL_MODELVIEW);

    glfwSetTime(0.0);
    nbRender=0;

    return wid;
}


void ocsg::uninit(void)
{
    if( window!=NULL) glfwDestroyWindow(window);
    window=NULL;
    glfwTerminate();
}

// pour l'instant:
// 0 : GIMBAL (zoomValue, pespective, fov, rotxyz

void ocsg::setupCamera(double dist,bool perspective,double fov,double rx,double ry,double rz)
{
    float aspectratio = width / (float) height;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // gimbal
    if( perspective ) {
        gluPerspective(fov, aspectratio, 0.1*dist, 100*dist);
    }else{
        double h = dist * tan(fov/2*M_PI/180);
        glOrtho(-h*aspectratio, h*aspectratio, -h, h, -100*dist, +100*dist);
    }
    gluLookAt(0.0, -dist, 0.0,
                0.0, 0.0, 0.0,
                0.0, 0.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotated(rz, 0.0, 0.0, 1.0);
    glRotated(ry, 0.0, 1.0, 0.0);
    glRotated(rx, 1.0, 0.0, 0.0);
}



void ocsg::clear(void)
{
    //std::cout << "clear\n";

    for (t_products::const_iterator i = products.begin(); i != products.end(); ++i) {
        t_primitives* primitives=(*i);
        for (t_primitives::const_iterator j = primitives->begin(); j != primitives->end(); ++j) {
            OpenCSG::DisplayListPrimitive* p = static_cast<OpenCSG::DisplayListPrimitive*>(*j);
            glDeleteLists(1, p->getDisplayListId());
            delete p;
        }
        delete primitives;
    }
    products.clear();
}

void ocsg::newProduct(void)
{
    //std::cout << "end\n";
    // add a new primitive to the products
    products.push_back(new t_primitives());
}



void ocsg::drawTriangle(float *p0,float *p1,float *p2, bool mirrored)
{
    double ax = p1[0] - p0[0], bx = p1[0] - p2[0];
    double ay = p1[1] - p0[1], by = p1[1] - p2[1];
    double az = p1[2] - p0[2], bz = p1[2] - p2[2];
    double nx = ay*bz - az*by;
    double ny = az*bx - ax*bz;
    double nz = ax*by - ay*bx;
    double nl = sqrt(nx*nx + ny*ny + nz*nz);
    glNormal3d(nx / nl, ny / nl, nz / nl);

    glVertex3d(p0[0], p0[1], p0[2]);
    if (!mirrored) glVertex3d(p1[0], p1[1], p1[2]);
    glVertex3d(p2[0], p2[1], p2[2]);
    if (mirrored) glVertex3d(p1[0], p1[1], p1[2]);
}



int ocsg::draw(char op,float *vertices,int *facets,int nbV,int nbF)
{
    //std::cout << "draw op="<<op<<"\n";

    glfwMakeContextCurrent(window);
    //int id=drawBlub();
    
    GLuint id = glGenLists(1);
    glNewList(id, GL_COMPILE);

#if 0
    // activate and specify pointer to vertex array
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glDrawElements(GL_TRIANGLES, nbF*3, GL_UNSIGNED_INT, facets);
    //glDisableClientState(GL_VERTEX_ARRAY);

    float *normals;
    normals=(float *)malloc(nbV*3*sizeof(float));
    for(int i=0;i<nbV;i++) {
        normals[i*3+0]=(random()%1000-500.0)/500.0;
        normals[i*3+1]=(random()%1000-500.0)/500.0;
        normals[i*3+2]=(random()%1000-500.0)/500.0;
    }

    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT,0,normals);
    //glDisableClientState(GL_NORMAL_ARRAY);
#endif

    glBegin(GL_TRIANGLES);
    for(int i=0;i<nbF;i++) {
        drawTriangle(
                vertices+(facets[i*3+0])*3,
                vertices+(facets[i*3+1])*3,
                vertices+(facets[i*3+2])*3,
                false);
    }
    glEnd();

    glEndList();

    //free(normals);

    OpenCSG::Operation oper;
    if( op=='*' ) oper=OpenCSG::Intersection;
    else if( op=='-' ) oper=OpenCSG::Subtraction;
    else oper=OpenCSG::Intersection;

    // if there are no products, make one
    //if( products.size()==0 ) endProduct();
    if( products.size()==0 ) {
        std::cout << "did you forget to call newProduct before first draw?? -> fixed\n";
        newProduct();
    }

    t_primitives *primitives=products.back();
    primitives->push_back(new OpenCSG::DisplayListPrimitive(id, oper, 2)); // , convexity

    return 0;
}

//float rot=0.0f;

int ocsg::render(void)
{
    glfwMakeContextCurrent(window);
    //std::cout << "render\n";
    if( glfwWindowShouldClose(window) ) return(1); // done!

        //float ratio;
        //int width, height;
        //mat4x4 m, p, mvp;
        //glfwGetFramebufferSize(window, &width, &height);
        //ratio = width / (float) height;
        //glViewport(0, 0, width, height);
        //printf("viewport is (%d,%d)\n",width,height);

        /*
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-2.0, 2.0, 2.0, -2.0, 0.0, 1000.0);
        */


    setupCamera(7.0*distFactor,false,60.0,rotx,roty,rotz);

    //rot+=0.5;

    
    //glDisable(GL_DEPTH_TEST);
    //glEnable(GL_BLEND);
    //glClearColor( 0.0, 1.0, 0.0, 1.0 );
    

    glUseProgram(0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    for (t_products::const_iterator i = products.begin(); i != products.end(); ++i) {
        OpenCSG::render(*(*i));
    }
    //printf("render %d primitives\n",(int)primitives.size());

    glUseProgram(program);

    //glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_EQUAL);
    //glEnable(GL_LIGHTING);
    //glEnable(GL_COLOR_MATERIAL);
    //glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glEnable( GL_BLEND );
    //
    //glDisable(GL_CULL_FACE);
    //glShadeModel(GL_SMOOTH);
    //glFrontFace(GL_CCW);
    //
    //glCullFace(GL_BACK);
    //glDisable(GL_CULL_FACE);
    //
    //glEnable(GL_DEPTH_TEST);
    //glEnable(GL_NORMALIZE);
    //glEnable(GL_AUTO_NORMAL);

    float colors[] = {
        1,0,0,1,
        0,1,0,1,
        0,0,1,1,
        1,1,0,1,
        0,1,1,1,
        1,0,1,1,
        1,1,1,1,
    };

    int k=0;
    for (t_products::const_iterator i = products.begin(); i != products.end(); ++i) {
        t_primitives *primitives=(*i);
        for (t_primitives::const_iterator j = primitives->begin(); j != primitives->end(); ++j) {
            float *c=colors+(k%7)*4;
            glUniform4f(uniColor1,c[0],c[1],c[2],c[3]);
            (*j)->render();
            k++;
        }
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
    //glfwWaitEvents();

    nbRender++;
    if( (nbRender+1)%1000==0 ) {
        std::cout << "( fps is "<<nbRender/glfwGetTime() <<")\n";
    }

    return(0);
}






