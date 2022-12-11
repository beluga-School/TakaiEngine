#pragma once
#include <list>
#include <string>

class Scene
{
public:
	/// <summary>
	/// シーンクラス
	/// </summary>
	/// <param name="sceneStr">表示するシーンの名前</param>
	/// <param name="sceneNo">シーンの番号</param>
	Scene(std::string sceneStr);

	std::string str;
	//int sceneNo = Error;
};

class SceneManager
{
public:
	Scene *nowScene = nullptr;
	
	std::list<Scene> sceneList;

	void LoadScene(Scene scene);
	void ChangeScene(std::string sceneStr);
	//void ChangeScene(int sceneNo);

	static SceneManager *GetInstance();

private:
	SceneManager(){};
	~SceneManager(){};

	SceneManager(const SceneManager& a) = delete;
	SceneManager& operator=(const SceneManager&) = delete;
};

