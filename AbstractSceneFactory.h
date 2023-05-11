#pragma once
#include "IScene.h"
#include <string>

//�V�[���H��
class AbstractSceneFactory
{
public:
	//���z�f�X�g���N�^
	virtual ~AbstractSceneFactory() = default;
	//�V�[������
	virtual IScene* CreateScene(const std::string& sceneName) = 0;
};

