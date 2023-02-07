#pragma once

namespace Util
{
	//ウィンドウサイズ
	const float window_width = 1280;
	const float window_height = 720;

	template <class T>
	T Clamp(T t,T min, T max)
	{
		if (t < min) { return min; };
		if (t > max) { return max; };
		return t;
	}

    struct lerpVariables
    {
        //現在の経過時間
        float elapsedTime = 0.0f;
        //何秒かけて移動するか
        float maxTime = 1.0f;

        //呼び出したらイージングが起動
        void Start();

        //経過時間を更新
        void Update();

        //現在が最大秒数のうちどの割合かを(0.0f～1.0f)の値で返す
        float GetTimeRate();

    private:
        bool run = false;
    };
};

