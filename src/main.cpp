#include "Helpers.h"
#include <GLFW/glfw3.h>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <chrono>
#include <iostream>
using namespace std;
using namespace Eigen;

// VertexBufferObject Definition: vertices, properties
VertexBufferObject VBO;
VertexBufferObject VBO_C;

// Matrix Definition: MatrixXf M(rows,cols)
Eigen::MatrixXf V(2,3);
Eigen::MatrixXf Vertex(2,3); // each triangle has the size of 6 = 2 * 3 vertices
Eigen::MatrixXf C(3,3); // property: color
Eigen::Matrix4f view(4,4); // contains the view transformation
Eigen::Matrix4f trs(4,4); // contains the view transformation

// Global Variable: Triangle Insertion Mode
int clicks = 0;
int insertion = 1;

// Global Variable: Triangle Translation Mode
int selectTri = 0;
Vector2f beginning(0, 0);
Vector2f ending(0, 0);

// Global Variable: Triangle Deletion Mode
int deleteTri = 0;
int deletion = 0;

// Mode Control
struct modeFlags{
    bool modeI;
    bool modeO;
    bool modeP;
    bool modeH;
    bool modeJ;
    bool modeK;
    bool modeL;
    bool modeC;
    bool modeW;
    bool modeA;
    bool modeS;
    bool modeD;
    bool modePlus;
    bool modeMinus;
    bool keyFraming;
};
modeFlags mode = {false, false, false, false, false, false, false,
                  false, false, false, false, false, false, false, false};

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    // Get the position of the mouse in the window
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    // Get the size of the window
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    // Convert screen position to world coordinates
    // NOTE: y axis is flipped in glfw
    double xworld = ((xpos/double(width))*2)-1;
    double yworld = (((height-1-ypos)/double(height))*2)-1;

    // Keep track of the mouse clicks

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        printf("mouse enter: (%f, %f)\n", xworld, yworld);

        if(mode.modeI == true){
            // Update the info of triangle vertices (previous info kept)
            Vertex.conservativeResize(2, clicks + 1);
            Vertex.col(clicks) << xworld, yworld;
            clicks++;
        }// end of Triangle Insertion Mode

        if(mode.modeO == true){
            float p_x = ((xpos / float(width)) * 2) - 1.0;
            float p_y = (((height - 1 - ypos) / float(height)) * 2) - 1.0;
            beginning << p_x, p_y;
            Eigen::Vector2f cur(p_x, p_y);

            // Loop on each triangle inserted: i is the index of triangle
            for (unsigned i = 0; i < Vertex.cols() / 3; i++) {
                // Get the position of triangle vertices
                Vector2f a, b, c;
                a << Vertex.col(i*3);
                b << Vertex.col(i*3 + 1);
                c << Vertex.col(i*3 + 2);
                float a_x = a(0), a_y = a(1), b_x = b(0), b_y = b(1), c_x = c(0), c_y = c(1);
                float numerator_abc = a_x * (b_y - c_y) + b_x * (c_y - a_y) + c_x * (a_y - b_y);
                float area_abc = abs(numerator_abc) / 2.0;
                float numerator_pbc = p_x * (b_y - c_y) + b_x * (c_y - p_y) + c_x * (p_y - b_y);
                float area_pbc = abs(numerator_pbc) / 2.0;
                float numerator_apc = a_x * (p_y - c_y) + p_x * (c_y - a_y) + c_x * (a_y - p_y);
                float area_apc = abs(numerator_apc) / 2.0;
                float numerator_abp = a_x * (b_y - p_y) + b_x * (p_y - a_y) + p_x * (a_y - b_y);
                float area_abp = abs(numerator_abp) / 2.0;
                float alpha = area_pbc / area_abc;
                float beta = area_apc / area_abc;
                float gamma = area_abp / area_abc;

                // Figure out whether p is inside the triangle constructed by points a, b, c
                if ((alpha >= 0 && alpha <= 1) && (beta >= 0 && beta <= 1) && (gamma >= 0 && gamma <= 1)) {
                    selectTri = i;
                }
            }// end of inside/outside judgement on cursor
            std::cout << "Triangle No." << selectTri+1 << " Selected! " << std::endl;
        }// end of Triangle Translation Mode

        if(mode.modeP == true){
            float p_x = ((xpos / float(width)) * 2) - 1.0;
            float p_y = (((height - 1 - ypos) / float(height)) * 2) - 1.0;
            Eigen::Vector2f cur(p_x, p_y);

            // Loop on each triangle inserted: i is the index of triangle
            for (unsigned i = 0; i < Vertex.cols() / 3; i++) {
                // Get the position of triangle vertices
                Vector2f a, b, c;
                a << Vertex.col(i*3);
                b << Vertex.col(i*3 + 1);
                c << Vertex.col(i*3 + 2);
                float a_x = a(0), a_y = a(1), b_x = b(0), b_y = b(1), c_x = c(0), c_y = c(1);
                float numerator_abc = a_x * (b_y - c_y) + b_x * (c_y - a_y) + c_x * (a_y - b_y);
                float area_abc = abs(numerator_abc) / 2.0;
                float numerator_pbc = p_x * (b_y - c_y) + b_x * (c_y - p_y) + c_x * (p_y - b_y);
                float area_pbc = abs(numerator_pbc) / 2.0;
                float numerator_apc = a_x * (p_y - c_y) + p_x * (c_y - a_y) + c_x * (a_y - p_y);
                float area_apc = abs(numerator_apc) / 2.0;
                float numerator_abp = a_x * (b_y - p_y) + b_x * (p_y - a_y) + p_x * (a_y - b_y);
                float area_abp = abs(numerator_abp) / 2.0;
                float alpha = area_pbc / area_abc;
                float beta = area_apc / area_abc;
                float gamma = area_abp / area_abc;

                // Figure out whether p is inside the triangle constructed by points a, b, c
                if ((alpha >= 0 && alpha <= 1) && (beta >= 0 && beta <= 1) && (gamma >= 0 && gamma <= 1)) {
                    deleteTri = i;
                    deletion++;
                }
            }// end of inside/outside judgement on cursor
            std::cout << "Triangle No." << deleteTri+1 << " Deleted! " << std::endl;

            // Delete the group of selected columns
            unsigned int numRows = Vertex.rows();
            unsigned int numCols = Vertex.cols();
            if( 3*deleteTri < numCols ){
                for(unsigned i = 0; i < 3; i++){
                    Vertex.block(0,3*deleteTri,numRows,numCols-3*deleteTri) = Vertex.block(0,3*deleteTri+1,numRows,numCols-3*deleteTri);
                }
            }
            Vertex.conservativeResize(numRows,numCols);
        }// end of Triangle Deletion Mode
    }// end of mouse PRESSED

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE){
        printf("mouse leave: (%f, %f)\n", xworld, yworld);
        if(mode.modeO == true){
            ending << xworld, yworld;
            printf("mouse direction: (%f, %f)\n", ending(0)-beginning(0), ending(1)-beginning(1));
            int index = selectTri*3;
            Vertex.col(index) << Vertex.col(index) + ending - beginning;
            Vertex.col(index + 1) << Vertex.col(index + 1) + ending - beginning;
            Vertex.col(index + 2) << Vertex.col(index + 2) + ending - beginning;
            VBO.update(Vertex);
        }
    }// end of mouse RELEASED

    // Upload the vertex set to the GPU
    VBO.update(Vertex);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // Key Options
    switch (key){
        case  GLFW_KEY_1:
            // Update the position of the first vertex
            V.col(0) << -0.5,  0.5;
            break;
        case GLFW_KEY_2:
            V.col(0) << 0,  0.5;
            break;
        case  GLFW_KEY_3:
            V.col(0) << 0.5,  0.5;
            break;
        case  GLFW_KEY_I:
            // Triangle Insertion
            std::cout << "Triangle Insertion Mode: Enabled\n" << std::endl;
            mode = {true, false, false, false, false, false, false, false, false, false, false, false, false, false, false};
            glfwSetTime (10.0);
            break;
        case GLFW_KEY_O:
            // Triangle Translation
            std::cout << "Triangle Translation Mode: Enabled\n" << std::endl;
            mode = {false, true, false, false, false, false, false, false, false, false, false, false, false, false, false};
            glfwSetTime (10.0);
            break;
        case  GLFW_KEY_P:
            // Triangle Deletion
            std::cout << "Triangle Deletion Mode: Enabled\n" << std::endl;
            mode = {false, false, true, false, false, false, false, false, false, false, false, false, false, false, false};
            glfwSetTime (10.0);
            break;
        case  GLFW_KEY_H:
            // Clockwise rotation
            V.col(0) << -0.5,  0.5;
            break;
        case GLFW_KEY_J:
            // Counter-clockwise rotation
            V.col(0) << 0,  0.5;
            break;
        case  GLFW_KEY_K:
            // Scale up
            V.col(0) << 0.5,  0.5;
            break;
        case  GLFW_KEY_L:
            // Scale down
            V.col(0) << -0.5,  0.5;
            break;
        case GLFW_KEY_C:
            // Coloring
            V.col(0) << 0,  0.5;
            break;
        case  GLFW_KEY_MINUS:
            // Zoom in
            V.col(0) << -0.5,  0.5;
            break;
        case GLFW_KEY_KP_ADD:
            // Zoom out
            V.col(0) << 0,  0.5;
            break;
        case  GLFW_KEY_W:
            // Translate down
            V.col(0) << 0.5,  0.5;
            break;
        case  GLFW_KEY_A:
            // Translate right
            V.col(0) << -0.5,  0.5;
            break;
        case GLFW_KEY_S:
            // Translate up
            V.col(0) << 0,  0.5;
            break;
        case  GLFW_KEY_D:
            // Translate left
            V.col(0) << 0.5,  0.5;
            break;
        case  GLFW_KEY_F:
            // Keyframing
            V.col(0) << 0.5,  0.5;
            break;
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, 1);
        default:
            break;
    }
}

int main(void) {
    GLFWwindow* window;

    // Initialize the library
    if (!glfwInit()){
        return -1;
    }

    // Activate supersampling
    glfwWindowHint(GLFW_SAMPLES, 8);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

    // On apple we have to load a core profile with forward compatibility
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(921, 921, "CG Assignment-2 (dz1120)", NULL, NULL);
    if (!window){
        glfwTerminate();
        return -1;
    }
    // Make the window's context current
    glfwMakeContextCurrent(window);

    #ifndef __APPLE__
      glewExperimental = true;
      GLenum err = glewInit();
      if(GLEW_OK != err){
        /* Problem: glewInit failed, something is seriously wrong. */
       fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
      }
      glGetError();
      fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
    #endif

    int major, minor, rev;
    major = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR);
    minor = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR);
    rev = glfwGetWindowAttrib(window, GLFW_CONTEXT_REVISION);
    printf("OpenGL version recieved: %d.%d.%d\n", major, minor, rev);
    printf("Supported OpenGL is %s\n", (const char*)glGetString(GL_VERSION));
    printf("Supported GLSL is %s\n", (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));

    // Initialize the VAO
    VertexArrayObject VAO;
    VAO.init();
    VAO.bind();

    // Initialize the VBO with the initial vertices data
    VBO.init();
    V.resize(2,3);
    V << 0,  0.5, -0.5, 0.5, -0.5, -0.5;
    VBO.update(V);

    // Initialize the second VBO for the property color
    VBO_C.init();
    C.resize(3,3);
    C << 1,  0, 0,
         0,  1, 0,
         0,  0, 1;
    VBO_C.update(C);

    // Initialize the OpenGL Program
    Program program;
    const GLchar* vertex_shader =
            "#version 150 core\n"
                    "in vec2 position;"
                    "in vec3 color;"
                    "uniform mat4 view;"
                    "uniform mat4 Translation;"
                    "out vec3 f_color;"
                    "void main()"
                    "{"
                    "    gl_Position = view * Translation * vec4(position, 0.0, 1.0);"
                    "    f_color = color;"
                    "}";
    const GLchar* fragment_shader =
            "#version 150 core\n"
                    "in vec3 f_color;"
                    "out vec4 outColor;"
                    "uniform vec3 triangleColor;"
                    "void main()"
                    "{"
                    "    outColor = vec4(triangleColor, 1.0);"
                    "}";

    // Compile the two shaders and upload the binary to the GPU
    program.init(vertex_shader,fragment_shader,"outColor");
    program.bind();

    // Inputs of vertex shader: vertices and any other properties
    program.bindVertexAttribArray("position",VBO);
    program.bindVertexAttribArray("color",VBO_C);

    // Save the current time --- it will be used to dynamically change the triangle color
    auto t_start = std::chrono::high_resolution_clock::now();

    // Register the keyboard and mouse callback
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    // Update viewport
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Loop until closing the current window
    while (!glfwWindowShouldClose(window)){
        VAO.bind();
        program.bind();

        // Set the uniform value of color depending on the time difference
        auto t_now = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();
        glUniform3f(program.uniform("triangleColor"), 0.0f, (float)(sin(time * 4.0f) + 1.0f) / 2.0f, (float)(sin(time * 4.0f) + 1.0f) / 2.0f);

        // Adjust the view as window size changing
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        float aspect_ratio = float(height)/float(width);
        view << aspect_ratio,0, 0, 0,
                0,           1, 0, 0,
                0,           0, 1, 0,
                0,           0, 0, 1;
        glUniformMatrix4fv(program.uniform("view"), 1, GL_FALSE, view.data());

        // Initialize the Translation Matrix if the triangle is selected
        trs << 1, 0, 0, 0,
               0, 1, 0, 0,
               0, 0, 1, 0,
               0, 0, 0, 1;
        glUniformMatrix4fv(program.uniform("Translation"), 1, GL_FALSE, trs.data());

        // Clear the FrameBuffer
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//        glClear(GL_COLOR_BUFFER_BIT);

        // Enable blending test
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Enable Depth Test: (side effect) disable the highlight effects
//        glEnable(GL_DEPTH_TEST);

        if(clicks == 0){
            // Draw an initial triangle
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }else if (clicks > 0){
            glDrawArrays(GL_TRIANGLES, 0, clicks);
        }

        // Drawing Preview: the starting point, each segment, possible triangle
        if(mode.modeI == true){
            if(clicks % 3 == 1){
                glUniform3f(program.uniform("triangleColor"), 1.0f, 1.0f, 1.0f);
                glDrawArrays(GL_POINTS, clicks - 1, 1);

                // Get the position of the mouse in the window to generate preview
                double xpos, ypos;
                glfwGetCursorPos(window, &xpos, &ypos);
                int w, h;
                glfwGetWindowSize(window, &w, &h);
                double xworld = ((xpos/double(width))*2)-1;
                double yworld = (((height-1-ypos)/double(height))*2)-1;
                Eigen::Vector2f cur(xworld, yworld);

                // Preview a line segment
                if(!Vertex.col(clicks).isApprox(cur)){
                    Vertex.conservativeResize(2, clicks + 1);
                    Vertex.col(clicks) << xworld, yworld;
                    VBO.update(Vertex);
                    glUniform3f(program.uniform("triangleColor"), 1.0f, 1.0f, 1.0f);
                    glDrawArrays(GL_LINE_STRIP, clicks - 1, 2);
                }
            }

            // The second click: draw a line segment, then preview a triangle
            if(clicks % 3 == 2){
                glUniform3f(program.uniform("triangleColor"), 1.0f, 1.0f, 1.0f);
                glDrawArrays(GL_LINE_STRIP, clicks - 1, 2);
                glDrawArrays(GL_LINE_STRIP, clicks - 2, 2);

                // Get the position of the mouse in the window to generate preview
                double xpos, ypos;
                glfwGetCursorPos(window, &xpos, &ypos);
                int w, h;
                glfwGetWindowSize(window, &w, &h);
                double xworld = ((xpos/double(width))*2)-1;
                double yworld = (((height-1-ypos)/double(height))*2)-1;
                Eigen::Vector2f cur(xworld, yworld);

                // Preview a triangle
                if(!Vertex.col(clicks).isApprox(cur)){
                    Vertex.conservativeResize(2, clicks + 1);
                    Vertex.col(clicks) << xworld, yworld;
                    VBO.update(Vertex);
                    glDrawArrays(GL_LINE_LOOP, clicks - 2, 3);
                }
            }
            
            // The third click: draw a triangle
            if(clicks != 0 && clicks % 3 == 0){
                glDrawArrays(GL_TRIANGLES, clicks - 3, 3);
                glUniform3f(program.uniform("triangleColor"), 0.0f, (float)(sin(time * 4.0f) + 1.0f) / 2.0f, (float)(sin(time * 4.0f) + 1.0f) / 2.0f);
                glDrawArrays(GL_LINE_LOOP, clicks - 3, 3);
                // Figure out the total number of inserted triangles
                insertion = clicks / 3;
            }
            glDrawArrays(GL_TRIANGLES, 0, clicks);
        }// end of Triangle Insertion Mode

        // Using Barycentric Interpolation
        if(mode.modeO == true) {
            glDrawArrays(GL_TRIANGLES, 0, Vertex.cols());

            // Highlight the selected triangle
            int index = selectTri*3;
            glUniform3f(program.uniform("triangleColor"), 1.0f, 1.0f, 0.0f);
            glDrawArrays(GL_TRIANGLES, index, 3);

            // Debugging info
            printf("the beginning of movement: (%f, %f)\n", beginning(0), beginning(1));
            printf("the end of movement: (%f, %f)\n", ending(0), ending(1));
            printf("translation direction: (%f, %f)\n", ending(0)-beginning(0), ending(1)-beginning(1));

        }// end of Triangle Translation Mode

        if(mode.modeP == true) {
            glDrawArrays(GL_TRIANGLES, 0, Vertex.cols());
        }// end of Triangle Deletion Mode

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    std::cout << "Triangle Inserted: " << insertion << std::endl;
    std::cout << "Triangle Deleted: " << deletion << std::endl;
    std::cout << "Current # of Triangles:" << insertion - deletion << std::endl;

    // Deallocate opengl memory
    program.free();
    VAO.free();
    VBO.free();
    glfwTerminate();

    return 0;
}