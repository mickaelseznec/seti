#include "figgeom.h"

#include <iostream>
#include <string>

#include <cstdio>
#include <cmath>
#include <cstring>

int FigGeom::instances = 0;

FigGeom::FigGeom(int vertices_number) {
    ++instances;
    this->vertices_number = vertices_number;
    vertices_positions = new Vertex[vertices_number];

    for (int i = 0; i < vertices_number; i++) {
        vertices_positions[i].x = cos((2*M_PI*i)/vertices_number);
        vertices_positions[i].y = sin((2*M_PI*i)/vertices_number);
    }
    name = new char[sizeof("Figure ") + 3 + 1];
    sprintf(name, "Figure %d", instances);
}

FigGeom::FigGeom(int vertices_number, float *x_pos, float *y_pos) {
    ++instances;
    this->vertices_number = vertices_number;
    vertices_positions = new Vertex[vertices_number];

    for (int i = 0; i < vertices_number; i++) {
        vertices_positions[i].x = x_pos[i];
        vertices_positions[i].y = y_pos[i];
    }
    name = new char[sizeof("Figure ") + 3 + 1];
    sprintf(name, "Figure %d", instances);
}

FigGeom::~FigGeom() {
    delete[] vertices_positions;
    delete[] name;
}

FigGeom::FigGeom(const FigGeom &figgeom) {
    ++instances;
    vertices_number = figgeom.vertices_number;
    color = figgeom.color;
    vertices_positions = new Vertex[vertices_number];
    memcpy(vertices_positions, figgeom.vertices_positions, sizeof(Vertex) * vertices_number);
    name = new char[sizeof("Figure ") + 3 + 1];
    sprintf(name, "Figure %d", instances);
}

std::ostream& operator<<(std::ostream &stream, const FigGeom &fig) {
    stream << fig.name << std::endl;
    for (int i = 0; i < fig.vertices_number; i++) {
        stream << "Vertex " << i << " at x=" << fig.vertices_positions[i].x << " y=" << fig.vertices_positions[i].y << std::endl;
    }
    return stream;
}

void operator>>(const FigGeom &fig, FILE *file) {
    fprintf(file, "%s\n", fig.name);
    for (int i = 0; i < fig.vertices_number; i++) {
        fprintf(file, "Vertex %d at x=%f y=%f\n", i, fig.vertices_positions[i].x, fig.vertices_positions[i].y);
    }
}

void operator>>(const FigGeom &fig, const char* path) {
    FILE* file = fopen(path, "a");
    fig >> file;
    fclose(file);
}

void FigGeom::Affiche() {
    std::cout << *this << std::endl;
}

double dist(const Vertex &a, const Vertex &b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

double FigGeom::Perimetre() {
    double perimetre = 0;
    for (int i = 0; i < vertices_number - 1; i++) {
        perimetre += dist(vertices_positions[i], vertices_positions[i + 1]);
    }
    perimetre += dist(vertices_positions[0], vertices_positions[vertices_number - 1]);
    return perimetre;
}

int main() {
    FigGeom square(4);
    FigGeom square2 = square;
    square.Affiche();
    FigGeom vertex(1);
    std::cout << square.Perimetre() << std::endl;
    std::cout << vertex.Perimetre() << std::endl;
    square2 >> "test.txt";
    return 0;
}
