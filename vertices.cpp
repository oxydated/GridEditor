#include <GL/glew.h>
#include <GL/glxew.h>
#include <vector>

using namespace std;

GLfloat vertexDelta[] = { -0.02f,0.0f,0.0f,
                       0.0f,0.02f,0.0f,
                       0.0f,0.0f,0.0f,
                       0.02f,0.0f,0.0f };
GLfloat colours[] = { 1.0f, 0.0f, 0.0f,
                      0.0f, 1.0f, 0.0f,
                      0.0f, 0.0f, 1.0f,
                      1.0f, 0.0f, 1.0f };

std::vector<GLfloat> vertices(9000, 0.0);

std::vector<unsigned short> indices(2000);

unsigned short numTriangles = 0;
unsigned short oldNumTriangles = 0;

unsigned int vertexArrayObjID[2];
unsigned int vertexBufferObjID[4];
unsigned int indexBufferObjID = 0;

void insertNewDot(float x, float y);

void init(void) {

    glGenVertexArrays(2, &vertexArrayObjID[0]);

    glBindVertexArray(vertexArrayObjID[0]);
    glGenBuffers(3, vertexBufferObjID);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

//    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID[1]);
//    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), colours, GL_STATIC_DRAW);
//    glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);
//    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBufferObjID[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned short), indices.data(), GL_DYNAMIC_DRAW);

    glBindVertexArray(0);
}

void displayVertices() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(vertexArrayObjID[0]);

    if(numTriangles > oldNumTriangles){
        unsigned short offsetIndex = oldNumTriangles * 3 * sizeof(unsigned short);
        unsigned short offsetVertex = oldNumTriangles * 9 * sizeof(GLfloat);

        unsigned short sizeIndex = 3 * sizeof(unsigned short);
        unsigned short sizeVertex = 9 * sizeof(GLfloat);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBufferObjID[2]);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offsetIndex, sizeIndex, (const void*)(indices.data() + oldNumTriangles * 3));

        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID[0]);
        glBufferSubData(GL_ARRAY_BUFFER, offsetVertex, sizeVertex, (const void*)(vertices.data()+oldNumTriangles * 9));

        oldNumTriangles++;
    }

    glVertexAttrib3f((GLuint)1, 1.0, 1.0, 1.0); // set constant color attribute
    glDrawElements(GL_TRIANGLES, numTriangles * 3, GL_UNSIGNED_SHORT, 0);

    glBindVertexArray(0);
}

void insertNewDot(float x, float y){
    unsigned short newIndex = numTriangles;
    numTriangles ++;
    indices[newIndex*3] = newIndex*3;
    indices[newIndex*3+1] = newIndex*3+1;
    indices[newIndex*3+2] = newIndex*3+2;

    vertices[newIndex*9 + 0] =  -0.02f + x; vertices[newIndex*9 + 1] = y; vertices[newIndex*9 + 2]  = 0.0f;

    vertices[newIndex*9 + 3] = x; vertices[newIndex*9 + 4] = 0.02f + y; vertices[newIndex*9 + 5] = 0.0f;

    vertices[newIndex*9 + 6] = x; vertices[newIndex*9 + 7] = y; vertices[newIndex*9 + 8] = 0.0f;

}
