#include <MeshTransformationsBuilder.h>

MeshTransformationsBuilder::MeshTransformationsBuilder() {

}

MeshTransformationsBuilder& MeshTransformationsBuilder::scaleBy(float scaleFactor) {
	_scaleFactor = glm::vec3(scaleFactor);
	return *this;
}

MeshTransformationsBuilder& MeshTransformationsBuilder::scaleBy(glm::vec3 scaleFactor) {
	_scaleFactor = scaleFactor;
	return *this;
}

MeshTransformationsBuilder& MeshTransformationsBuilder::rotateAroundXAxis(float rotationAngle) {
	_rotationAngle = rotationAngle;
	_rotationAxis = glm::vec3(1, 0, 0);
	return *this;
}


MeshTransformationsBuilder& MeshTransformationsBuilder::rotateAroundYAxis(float rotationAngle) {
	_rotationAngle = rotationAngle;
	_rotationAxis = glm::vec3(0, 1, 0);
	return *this;
}

MeshTransformationsBuilder& MeshTransformationsBuilder::rotateAroundZAxis(float rotationAngle) {
	_rotationAngle = rotationAngle;
	_rotationAxis = glm::vec3(0, 0, 1);
	return *this;
}

MeshTransformationsBuilder& MeshTransformationsBuilder::rotateAroundAxis(float rotationAngle, glm::vec3 rotationAxis) {
	_rotationAngle = rotationAngle;
	_rotationAxis = rotationAxis;
	return *this;
}


MeshTransformationsBuilder& MeshTransformationsBuilder::translateTo(glm::vec3 position) {
	_position = position;
	return *this;
}

MeshTransformations MeshTransformationsBuilder::build() {
	MeshTransformations meshTransformations = { _scaleFactor, _rotationAngle, _rotationAxis, _position };
	return meshTransformations;
}
