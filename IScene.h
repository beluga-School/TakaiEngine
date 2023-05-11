#pragma once

class SceneManager;
class IScene
{
public:
	virtual ~IScene() = default;

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void End() = 0;

	virtual void SetManager(SceneManager* SceneManager) {
		sceneManager = SceneManager;
	}

protected:
	//シーンマネージャ
	SceneManager* sceneManager = nullptr;
};

