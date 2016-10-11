#ifndef _FIGGEOM_H__
#define _FIGGEOM_H__

#include <iostream>
#include <string>

#include <cstdio>

struct Vertex {
    double x;
    double y;
};

struct Color {
    char r;
    char g;
    char b;
};

class FigGeom {
    private:
        static const char* base_name;
        static int instances;
        int vertices_number;
        Vertex *vertices_positions;
        Color color;
        char *name;
    public:
        FigGeom(int vertices_number);
        FigGeom(int vertices_number, float *x_pos, float *y_pos);
        FigGeom(const FigGeom &figgeom);
        ~FigGeom();

        void Affiche();
        double Perimetre();
        friend std::ostream& operator<<(std::ostream &stream, const FigGeom &fig);
        friend void operator>>(const FigGeom &fig, FILE *file);
};

#endif
