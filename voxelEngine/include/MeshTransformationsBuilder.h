#pragma once

#include <glad/glad.h>
#include <string>
#include <Vertex.h>
#include <Texture.h>
#include <vector>
#include <Shader.h>
#include <BasicCamera.h>
#include <MeshTransformations.h>

class MeshTransformationsBuilder
{
private:
	glm::vec3 _scaleFactor = glm::vec3(1.0f);
	float _rotationAngle = glm::radians(0.0f);
	glm::vec3 _rotationAxis = glm::vec3(1.0f);
	glm::vec3 _position = glm::vec3(1.0f);



public:
    // Create a transformation builder. Defaults to not applying any transformations
    MeshTransformationsBuilder();

    // Scale by a given factor in all directions. This will overwrite any other scale operations applied
    MeshTransformationsBuilder& scaleBy(float scaleFactor);
    // Scale x, y, z independently by given factor. This will overwrite any other scale operations applied
    MeshTransformationsBuilder& scaleBy(glm::vec3 scaleFactor);
    // Rotate around the X-axis. This will overwrite any other rotation operations applied.
    MeshTransformationsBuilder& rotateAroundXAxis(float rotationAngle);
    // Rotate around the X-axis. This will overwrite any other rotation operations applied.
    MeshTransformationsBuilder& rotateAroundYAxis(float rotationAngle);
    // Rotate around the X-axis. This will overwrite any other rotation operations applied.
    MeshTransformationsBuilder& rotateAroundZAxis(float rotationAngle);
    // Rotate around the the given axis. This will overwrite any other rotation operations applied.
    MeshTransformationsBuilder& rotateAroundAxis(float rotationAngle, glm::vec3 rotationAxis);
    // Rotate around the the given axis. This will overwrite any other rotation operations applied.
    MeshTransformationsBuilder& translateTo(glm::vec3 position);

    MeshTransformations build();
};