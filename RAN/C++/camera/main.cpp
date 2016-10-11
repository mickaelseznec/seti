#include "camera.hpp"

int main() {
    Camera **cams = new Camera*[3];
    cams[0] = new CameraRGB("Nikon");
    cams[1] = new CameraBW("Kodak");

    cams[0]->transmission();
    cams[1]->transmission();
    delete cams[0];
    delete cams[1];
    delete[] cams;
    return 0;
}
