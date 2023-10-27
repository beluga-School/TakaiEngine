#pragma once
#include "IScene.h"
#include <string>

/*! AbstractSceneFactory
	シーン工場(未実装)
*/
class AbstractSceneFactory
{
public:
	//仮想デストラクタ
	virtual ~AbstractSceneFactory() = default;
	//シーン生成
	virtual IScene* CreateScene(const std::string& sceneName) = 0;
};

