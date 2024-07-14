#ifndef SCENE_H
#define SCENE_H

class Scene {
    public:
        virtual bool initScene() = 0;
        virtual void renderScene() = 0;
        virtual void updateScene(float t) = 0;
        virtual void destroyScene() = 0;
        virtual void resizeScene(int width, int height) = 0;
};

#endif