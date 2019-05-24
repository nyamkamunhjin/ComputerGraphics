#include <GL/glut.h>
#include "GL/freeglut.h"    
#include <iostream>
#include <fstream>


// GLuint loadBMP(const char *imagepath) {
//     unsigned char header[54]; // Each BMP file begins by a 54-bytes header
//     unsigned int dataPos;     // Position in the file where the actual data begins
//     unsigned int width, height;
//     unsigned int imageSize;   // = width*height*3
//     // Actual RGB data
//     unsigned char *data;

//     FILE *file = fopen(imagepath, "rb");

//     if(!file) {
//         printf("Image not found.\n");
//         return 0;
//     }

//     if(fread(header, 1, 54, file) != 54) {
//         printf("Not a correct BMP file\n");
//         return 0;
//     }

//     if(header[0] != 'B' || header[1] != 'M') {
//         printf("Not a correct BMP file BM\n ");
//         return 0;
//     }

//     // Read ints from the byte array
//     dataPos    = *(int*)&(header[0x0A]);
//     imageSize  = *(int*)&(header[0x22]);
//     width      = *(int*)&(header[0x12]);
//     height     = *(int*)&(header[0x16]);

//     // Some BMP files are misformatted, guess missing information
//     if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
//     if (dataPos==0)      dataPos=54; // The BMP header is done that way

//     // Create a buffer
//     data = new unsigned char[imageSize];

//     // Read the actual data from the file into the buffer
//     fread(data, 1, imageSize, file);

//     //Everything is in memory now, the file can be closed
//     fclose(file);

//     // Create one OpenGL texture
//     GLuint textureID;
//     glGenTextures(1, &textureID);

//     // "Bind" the newly created texture : all future texture functions will modify this texture
//     glBindTexture(GL_TEXTURE_2D, textureID);

//     // Give the image to OpenGL
//     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

//     // Nice trilinear filtering.
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//     glGenerateMipmap(GL_TEXTURE_2D);

//     // Return the ID of the texture we just created
//     return textureID;
// }

GLuint loadBmp(const char *filename) {

    GLuint texture;

    int width, height;

    unsigned char *data;

    FILE * file;

    file = fopen(filename, "rb");

    if ( file == NULL ) return 0;
    width = 1024;
    height = 512;
    data = (unsigned char *)malloc(width * height * 3);
    //int size = fseek(file,);
    fread(data, width * height * 3, 1, file);
    fclose(file);

    for(int i = 0; i < width * height; i++) {
    int index = i * 3;
    unsigned char B, R;
    B = data[index];
    R = data[index + 2];

    data[index] = R;
    data[index + 2] = B;

    }


    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);


    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height,GL_RGB, GL_UNSIGNED_BYTE, data);
    free(data);

    return texture;
}