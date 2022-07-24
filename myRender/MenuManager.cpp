//
// Filename: MenuManager.cpp
// Created by W. Mysterio on 2022-07-24 02:00:04.
// Description:
// Mail: woden3702@gmail.com
//
#include "MenuManager.h"


MenuManager* MenuManager::ins_ = nullptr;
MenuManager::MenuManager()
{
}

void MenuManager::drawMainMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		ImGui::Text(("<" + SceneManager::instance()->getSceneName() + ">").c_str());
		if (ImGui::BeginMenu("Scene"))
		{
			drawMainMenuBar_Scene();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Editor"))
		{
			drawMainMenuBar_Editor();
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void MenuManager::drawMainMenuBar_Scene()
{
	if (ImGui::MenuItem("Import Model"))
	{

	}
	if (ImGui::MenuItem("Add a Cube"))
	{
		auto cube = new MeshObject();
		std::string name = "cube";
		cube->setName(name);
		cube->setMeshData(MeshManager::instance()->getBuildInBox());
		cube->setTexture(TextureManager::instance()->loadDefaultD());
		SceneManager::instance()->addObject(std::shared_ptr<Object>(cube));
	}
	if (ImGui::MenuItem("Add a Sphere"))
	{
		auto sphere = new MeshObject();
		std::string name = "sphere";
		sphere->setName(name);
		sphere->setMeshData(MeshManager::instance()->getBuildInBox());
		sphere->setTexture(TextureManager::instance()->loadDefaultD());
		SceneManager::instance()->addObject(std::shared_ptr<Object>(sphere));
	}
	if (ImGui::MenuItem("Reload all shaders"))
	{
	}
}

void MenuManager::drawMainMenuBar_Editor()
{
	if (ImGui::MenuItem("Scene Editor"))
	{
		showSceneEditorDialog_ = true;
	}
	if (ImGui::MenuItem("Object Editor"))
	{
		showObjectEditorDialog_ = true;
	}
}

void MenuManager::drawSceneEditorDialog()
{
	ImGui::SetNextWindowSize(ImVec2(250, 800), ImGuiCond_FirstUseEver);
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse;

	if (ImGui::Begin("Scene Editor", &showSceneEditorDialog_, windowFlags))
	{
		//if (ImGui::Selectable("Main Camera##Main", selectedObj_ == CameraManager::instance()->getCurCamera()))
			// selectedObj_ = CameraManager::instance()->getCurCamera();

		//if (ImGui::Selectable("Main Light##Main", selectedObj == LightManager::Instance()->GetLight(WeatherSystem::Instance()->GetSunLightID())))
			//selectedObj = LightManager::Instance()->GetLight(WeatherSystem::Instance()->GetSunLightID());

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

		for (auto& object : SceneManager::instance()->getObjects())
		{
			ImGui::Selectable(object->getName().c_str());
			if (ImGui::IsItemClicked())
			{
				selectedObj_ = object;
				showObjectEditorDialog_ = true;
			}
		}
	}
	ImGui::End();
}

void MenuManager::drawObjEditorDialog()
{
	ImGui::SetNextWindowSize(ImVec2(300, 400), ImGuiCond_FirstUseEver);
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse;

	if (ImGui::Begin("Object Editor", &showObjectEditorDialog_, windowFlags))
	{
		std::string objName = "";
		if (selectedObj_ != nullptr)
			objName = selectedObj_->getName();

		char objNameChar[32];
		strcpy_s(objNameChar, objName.c_str());
		ImGui::InputText("Object Name", objNameChar, IM_ARRAYSIZE(objNameChar));

		if (selectedObj_ != nullptr)
			selectedObj_->setName(std::string(objNameChar));

		ImGui::Text("Transform");
		{
			float pos[3] = { 0.0f, 0.0f, 0.0f };
			float rot[3] = { 0.0f, 0.0f, 0.0f };
			float scale[3] = { 0.0f, 0.0f, 0.0f };

			if (selectedObj_ != nullptr)
			{
				glm::vec3 objPos = selectedObj_->getPosition();
				glm::vec3 objRot = selectedObj_->getRotation();
				glm::vec3 objScale = selectedObj_->getScale();

				pos[0] = objPos.x;
				pos[1] = objPos.y;
				pos[2] = objPos.z;

				rot[0] = objRot.x;
				rot[1] = objRot.y;
				rot[2] = objRot.z;

				scale[0] = objScale.x;
				scale[1] = objScale.y;
				scale[2] = objScale.z;
			}

			ImGui::DragFloat3("Position", pos, 0.01f, -FLT_MAX, +FLT_MAX, "%.6f");
			ImGui::DragFloat3("Rotation", rot, 0.1f, -FLT_MAX, +FLT_MAX, "%.6f");
			ImGui::DragFloat3("Scale", scale, 0.1f, -FLT_MAX, +FLT_MAX, "%.6f");

			if (selectedObj_ != nullptr)
			{
				selectedObj_->setPosition(glm::vec3(pos[0], pos[1], pos[2]));
				selectedObj_->setRotation(glm::vec3(rot[0], rot[1], rot[2]));
				selectedObj_->setScale(glm::vec3(scale[0], scale[1], scale[2]));
			}
		}

		if (selectedObj_ == nullptr)
		{
			ImGui::End();
			return;
		}
	}
	ImGui::End();

}

void MenuManager::importModel(std::string path)
{
	FileImportManager::instance()->loadFile(path);
}

MenuManager::~MenuManager()
{
}

MenuManager* MenuManager::instance()
{
	if (ins_ == nullptr)
		ins_ = new MenuManager();
	return ins_;
}

void MenuManager::initMenu()
{
	showMenu_ = true;
	IMGUI_CHECKVERSION();
	guiContext_ = ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(WindowManager::instance()->getWindow(), true);
	ImGui_ImplOpenGL3_Init();

	showSceneEditorDialog_ = true;
	showObjectEditorDialog_ = true;

	selectedObj_ = nullptr;


	ImGuiIO& io = ImGui::GetIO();
	// io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGuiStyle& style = ImGui::GetStyle();
	style.FrameBorderSize = 2.0f;
	style.WindowBorderSize = 2.0f;
	style.PopupBorderSize = 2.0f;
}

void MenuManager::renderMenu()
{
	if (!showMenu_)
		return;

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void MenuManager::updateMenu()
{
	if (!showMenu_)
		return;

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

	// drawMainMenuBar();

	if (showSceneEditorDialog_)
		drawSceneEditorDialog();

	if (showObjectEditorDialog_)
		drawObjEditorDialog();
}

std::shared_ptr<Object> MenuManager::getSelectedObj() const
{
	return selectedObj_;
}
