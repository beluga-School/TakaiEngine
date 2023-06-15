#pragma once
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx12.h"
#include "Vector2.h"
#include <string>

class GUI
{
public:
	GUI(const std::string& name);
	
	void Begin(const Vector2& pos,const Vector2 &size);
	void End();
private:
	std::string mName;
	bool mSetPosFlag = false;
};

class ImguiManager
{
public:

	/// <summary>
	/// imgui�̏�����
	/// </summary>
	void Initialize();

	void PreUpdate();

	/// <summary>
	/// imgui�̕`��
	/// </summary>
	void Draw();

	void PreDraw();

	/// <summary>
	/// imgui�̏I������
	/// </summary>
	void Finalize();

	/// <summary>
	/// �C���X�^���X���擾
	/// </summary>
	/// <returns></returns>
	static ImguiManager* GetInstance();

private:
	ImguiManager(){};
	~ImguiManager() {};
	ImguiManager(const ImguiManager& a) = delete;
	ImguiManager& operator=(const ImguiManager&) = delete;
};