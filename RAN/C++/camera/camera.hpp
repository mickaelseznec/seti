#include <iostream>
#include <string>

class Camera {
    protected:
    std::string name;

    public:
    Camera(std::string name) {
        std::cout << "Building camera " << name << std::endl;
        this->name = name;
    }
    virtual void init() = 0;
    void transmission() {
        init();
        std::cout << "Transmission with " << name << std::endl;
    }

    virtual ~Camera() {};
};

class CameraRGB: public Camera {
    public:
        CameraRGB(std::string name): Camera(name){};
        void init() {
            std::cout << "In RGB init of " << name << std::endl;
        }
};

class CameraBW: public Camera {
    public:
        CameraBW(std::string name): Camera(name){};
        void init() {
            std::cout << "In BW init of " << name << std::endl;
        }
};
