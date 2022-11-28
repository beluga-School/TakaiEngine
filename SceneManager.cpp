#include "SceneManager.h"

void SceneManager::LoadScene(Scene scene)
{
	sceneList.push_back(scene);
}

void SceneManager::ChangeScene(std::string sceneStr)
{
	for (Scene& scene : sceneList)
	{
		if (scene.str == sceneStr)
		{
			nowScene = &scene;
			break;
		}
	}
}

Scene::Scene(std::string str)
{
	this->str = str;
}