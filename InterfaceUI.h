#pragma once
#include <TEasing.h>
#include <Vector2.h>

enum class UIMove
{
	START,
	END,
};

class InterfaceUI
{
public:
	/// <summary>
	/// �ړ�����ʒu�̎n�_�ƏI�_���w��
	/// </summary>
	/// <param name="start">�n�_</param>
	/// <param name="end">�I�_</param>
	void InitPos(const Vector2& start, const Vector2& end);

	/// <summary>
	/// �g�k����傫���̍ő�ƍŏ����w��
	/// </summary>
	/// <param name="start"></param>
	/// <param name="end"></param>
	void InitScale(const Vector2& min, const Vector2& max);

	virtual void Update() = 0;
	virtual void Draw() = 0;

	/// <summary>
	/// UI�𓮂����n�߂�
	/// </summary>
	/// <param name="move">���苓��or�߂苓�����w��</param>
	void Move(UIMove move);

	/// <summary>
	/// UI���g�k����
	/// </summary>
	/// <param name="move">�傫���Ȃ鋓��or�������Ȃ鋓�����w��</param>
	void Scaling(UIMove move);

	/// <summary>
	/// �ʒu���擾
	/// </summary>
	/// <returns></returns>
	Vector2 GetPos();

	/// <summary>
	/// �I�����Ă��邩���擾
	/// </summary>
	/// <param name="move">�m�F���������̋������w��</param>
	/// <returns></returns>
	bool GetMoveEnd(UIMove move_);

	bool GetScaleEnd(UIMove scale_);

protected:
	/// <summary>
	/// �C���^�[�t�F�[�X���Ŏw�肵�Ă��鏔�X�𓮂���
	/// Update�̍Ō�ŌĂԂ̂�z��
	/// </summary>
	void InterFaceUpdate();

	Vector2 pos = {-1000,-1000};
	Vector2 scale = {1.0f,1.0f};
	
	Vector2 startPos{-1000,-1000};
	Vector2 endPos{ -1000,-1000 };

	Vector2 saveStartPos{ -1000,-1000 };
	Vector2 saveEndPos{ -1000,-1000 };

	Vector2 minScale{ -1000,-1000 };
	Vector2 maxScale{ -1000,-1000 };

	TEasing::easeTimer moveTimer = 0.5f;
	TEasing::easeTimer scalingTimer = 0.5f;

	UIMove move = UIMove::START;
	UIMove scaling = UIMove::START;
};

