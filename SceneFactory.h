#pragma once
#include "AbstractSceneFactory.h"

class SceneFactory : public AbstractSceneFactory
{
public:
	//���̃Q�[���p�̃V�[���𐶐�
	IScene* CreateScene(const std::string& sceneName)override;
};

