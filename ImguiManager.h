#pragma once
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx12.h"
#include "Vector2.h"

class GUI
{
public:
	GUI(const char* name);
	
	void Begin(Vector2 pos, Vector2 size);
	void End();
private:
	const char* name;
	bool setPosFlag = false;
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