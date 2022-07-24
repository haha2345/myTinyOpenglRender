//
// Filename: MenuManager.h
// Created by W. Mysterio on 2022-07-24 02:00:04.
// Description:
// Mail: woden3702@gmail.com
//

#ifndef __MENUMANAGER_H__
#define __MENUMANAGER_H__
#include <memory>
#include <string>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include "imgui/imgui.h"
#include "WindowManager.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "FileImportManager.h"
#include "Object.h"

class MenuManager
{
private:
	MenuManager();

	static MenuManager* ins_;
	bool showMenu_;
	ImGuiContext* guiContext_;
	std::shared_ptr<Object> selectedObj_;
	std::string selectedObjName_;


	bool showSceneEditorDialog_;
	bool showObjectEditorDialog_;

	void drawMainMenuBar();
	void drawMainMenuBar_Scene();
	void drawMainMenuBar_Editor();

	void drawSceneEditorDialog();
	void drawObjEditorDialog();

	void importModel(std::string path);
	
public:
	~MenuManager();
	static MenuManager* instance();

	void initMenu();
	void renderMenu();
	void updateMenu();

	std::shared_ptr<Object> getSelectedObj() const;

};

#endif //__MENUMANAGER_H__


