#ifndef RAY_MARCH_H
#define RAY_MARCH_H

#include <string>
#include <nlohmann/json.hpp>

#include "Shader.h"
#include "Scene.h"

class Raymarch : public Scene {
    public:
        Raymarch();
        void setRootDir(const std::string& rootDir);
        void setSize(int width, int height);
        void setNearPlane(float nearPlane);
        void setFieldOfView(float vfieldOfView);
        void setCameraMatrix(glm::mat4 cameraMatrix);
        bool initScene() override;
        void renderScene() override;
        void updateScene(float t) override;
        void destroyScene() override;
        void resizeScene(int width, int height) override;

        nlohmann::json globalSceneParams;
    
    private:
        Shader shader;
        unsigned int VAO, VBO, EBO;
        std::string vertexShaderSrc;
        std::string fragmentShaderSrc;
        unsigned int SCR_WIDTH;
        unsigned int SCR_HEIGHT;
        float nearPlane;
        float vfieldOfView;

        std::string m_rootDir;
        GLuint rgbVolumeTexture;
        GLuint alphaVolumeTexture;
        GLuint featureVolumeTexture;
        GLuint atlasIndexTexture;
        GLuint weightsTexZero;
	    GLuint weightsTexOne;
	    GLuint weightsTexTwo;

        unsigned int indices[6] = { 0, 2, 1, 2, 3, 1 };
        std::vector<float> vertices;

        bool loadScene(const std::string& sceneFile, int width, int height);
        void createRayMarchMaterial();
        void createPlaneBufferGeometry(float width, float height, float z = 0.0f);

        static GLuint createDummyNetworkWeightTexture();
        static GLuint createNetworkWeightTexture(nlohmann::json networkWeights);
        static GLuint createFloatTextureFromData(int width, int height, float* data);
        std::string createViewDependenceFunctions() const;

        static std::string digits(int d, int w);
        static std::string replaceAll(std::string str, const std::string& from, const std::string& to);

};


#endif