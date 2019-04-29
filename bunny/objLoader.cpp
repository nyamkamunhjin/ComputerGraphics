#include <iostream>
#include <string>
#include <vector>
#include <glm/vec3.hpp>
#include <fstream>

bool objLoader(const char *filePath, std::vector<glm::vec3> &out_vertices) {

    // create temp vector array for vertices;
    std::vector<unsigned int> vertexIndices;
    std::vector<glm::vec3> temp_vertices;

    // read obj file
    FILE *file = fopen(filePath, "r");
    if(file == NULL) {
        printf("Cannot open file!\n");
        return false;
    }

    // start putting infos to temp vector array
    while(1) {
        char lineHeader[128];

        int res = fscanf(file, "%s", lineHeader);
        if(res == EOF) break; // End Of File. Quit the loop.

        if(strcmp(lineHeader, "v") == 0) {
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            temp_vertices.push_back(vertex);
        } else if(strcmp(lineHeader, "f") == 0) {
            unsigned int vertexIndex[3];
            int matches = fscanf(file, "%d %d %d", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2]);

            if(matches != 3) {
                printf("File can't be read");
                return false;
            }
            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
        }
    }

    // for each vertex of each triangle
    for(unsigned int i = 0; i < vertexIndices.size(); i++) {
        unsigned int vertexIndex = vertexIndices[i];
        glm::vec3 vertex = temp_vertices[vertexIndex - 1];
        out_vertices.push_back(vertex);
    }
}