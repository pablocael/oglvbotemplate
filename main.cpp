#include "mainwindow.h"

#include <iostream>

#include <QApplication>
#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>

class Widget : public QOpenGLWidget
{
protected:
    void initializeGL()
    {
        glClearColor(0, 0, 0, 1.0f);

        // build background quad
        _program = new QOpenGLShaderProgram();
//        _program->addShaderFromSourceFile(QOpenGLShader::Vertex, "/Users/pabloelias/Documents/Development/SimpleOGL/simple.vert");
//        _program->addShaderFromSourceFile(QOpenGLShader::Fragment, "/Users/pabloelias/Documents/Development/SimpleOGL/simple.frag");

//        _program->link();
//        _program->bind();

//        _vertexAttr = _program->attributeLocation("vertex");
//        //_textureUniform = _program->uniformLocation("tex");

//        std::cout << ">>>>>>. vertexAttr: " << _vertexAttr << std::endl;

//        std::cout << "SHADER LOG: " << _program->log().toStdString() << std::endl;

        _program->addShaderFromSourceCode(QOpenGLShader::Vertex,
            "attribute highp vec4 vertex;\n"
            "attribute highp vec2 texCoord;\n"
            "varying highp vec2 tex;\n"
            "void main(void)\n"
            "{\n"
            "   tex = texCoord;\n"
            "   gl_Position = vertex;\n"
            "}");
        _program->addShaderFromSourceCode(QOpenGLShader::Fragment,

            "varying highp vec2 tex;\n"
            "void main(void)\n"
            "{\n"
            "   gl_FragColor = vec4(tex.xy,0,1);\n"
            "}");
        _program->link();
        _program->bind();

        vertexLocation = _program->attributeLocation("vertex");
        texCoord = _program->attributeLocation("texCoord");


        static GLfloat const triangleVertices[] = {
            -1, -1, 0, 0, 0,
            1, -1, 0.0f, 1, 0,
            0.0, 1, 0, 0.5, 1
        };
        _vbo = new QOpenGLBuffer();
        _vbo->create();
        _vbo->bind();
        int bufferSize = sizeof(triangleVertices);
        _vbo->allocate(bufferSize);
        _vbo->write(0, triangleVertices, bufferSize);
        _vbo->release();
    }
    void resizeGL(int width, int height) {
        glViewport(0,0, width, height);

    }
    void paintGL() {
        // Draw the scene:
         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

         QColor color(0, 255, 0, 255);

         QMatrix4x4 pmvMatrix;
         pmvMatrix.ortho(rect());

         _vbo->bind();

         _program->enableAttributeArray(vertexLocation);
        _program->enableAttributeArray(texCoord);
         _program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, 5 * sizeof(GLfloat));
         _program->setAttributeBuffer(texCoord, GL_FLOAT, 3 * sizeof(GLfloat), 2, 5 * sizeof(GLfloat));
         glDrawArrays(GL_TRIANGLES, 0, 3);

         _program->disableAttributeArray(vertexLocation);

         _vbo->release();
    }

private:
    int vertexLocation;
    int texCoord;
    QOpenGLBuffer* _vbo;
    QOpenGLShaderProgram* _program;
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    QSurfaceFormat format;
//    format.setSamples(16);
//    format.setVersion(3, 2);
//    format.setProfile(QSurfaceFormat::CoreProfile);
 //   QSurfaceFormat::setDefaultFormat(format);

    MainWindow w;
    w.setCentralWidget(new Widget());
    w.show();


    return a.exec();
}
