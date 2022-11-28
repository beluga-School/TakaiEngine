#pragma once
#include <list>
#include <string>

class Scene
{
public:
	Scene(std::string sceneStr);

	std::string str;
};

class SceneManager
{
public:
	Scene *nowScene = nullptr;
	
	std::list<Scene> sceneList;

	void LoadScene(Scene scene);
	void ChangeScene(std::string sceneStr);
};

