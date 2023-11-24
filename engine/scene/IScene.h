#pragma once
#include <string>

//前方宣言
class SceneManager;

/*! IScene
	シーンのインターフェース
*/
class IScene
{
public:
	virtual ~IScene() = default;

	virtual void LoadResource() = 0;
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void End() = 0;

	//シーンマネージャーをセット
	virtual void SetManager(SceneManager& SceneManager_) {
		mSceneManager = &SceneManager_;
	}

	std::string oldSceneID = "";
	std::string sceneID = "";
protected:
	//シーンマネージャ
	SceneManager* mSceneManager = nullptr;

};

