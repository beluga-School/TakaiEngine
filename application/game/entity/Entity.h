#pragma once
#include "Obj.h"
#include "Box.h"
#include "Tag.h"
#include <vector>

//3d�`�悳���S�ẴI�u�W�F�N�g
class Entity : public Obj3d
{
public:
	bool IsDead() const {
		return isDead;
	}

	/// <summary>
	/// �w�肵���^�O�����邩�m�F����
	/// </summary>
	/// <param name="check">�m�F�������^�O</param>
	/// <returns>��������true �Ȃ��Ȃ�false</returns>
	bool CheckTag(TagTable check);

	/// <summary>
	/// �w�肵���^�O��t����(�d�����Ă����ꍇ�͕t���Ȃ�)
	/// </summary>
	/// <param name="check">�t�������^�O</param>
	/// <returns>�^�O�̃Z�b�g�ɐ�����true �d������������false</returns>
	bool SetTag(TagTable check);
	
	/// <summary>
	/// �w�肵���^�O���폜����
	/// </summary>
	/// <param name="check">�폜�������^�O</param>
	/// <returns>�^�O�̍폜�ɐ�����true �^�O��������Ȃ��ꍇfalse</returns>
	bool DeleteTag(TagTable check);

	void Register();

	//Entity�͕K���l�p�̓����蔻�������
	Box box;

	Entity()
	{
		SetTag(TagTable::Box);

		box.cubecol.parentEntity = this;
	}

	/// <summary>
	/// �����蔻��ɕt�^���ꂽ�A�I�u�W�F�N�g���ƂɈ�ӂɒ�`����Ă���ID��Ԃ�
	/// </summary>
	/// <returns></returns>
	uint32_t GetID()const;

protected:
	bool isDead = false;

private:
	//��������Ƃ��Ɏg���^�O
	std::vector<TagTable> taglist;
};