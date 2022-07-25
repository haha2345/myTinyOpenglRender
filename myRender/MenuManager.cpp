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
		// std::string name = SceneManager::instance()->getSceneName();
		// ImGui::Text(name.c_str());
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
		importModel("Model/nanosuit/nanosuit.obj");
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

		// ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

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

		if(selectedObj_->getObjectType()==ObjectType::oMesh)
		{
			auto meshObject = dynamic_cast<MeshObject*>(selectedObj_.get());
			ImGui::Text("Material");
			{
				//Name
				if (ImGui::Button("Change"))
				{
				}

				if (meshObject==nullptr)
				{
					ImGui::SameLine();
					ImGui::Text("Missing Material");
				}
				else
				{
					ImGui::SameLine();
					char matNameChar[128];
					strcpy_s(matNameChar, "material");
					ImGui::InputText("##MatName", matNameChar, IM_ARRAYSIZE(matNameChar));

					//Color
					glm::vec4 matColor(1.0f, 1.0f, 1.0f, 1.0f);
					float matColorArray[4] = { matColor.x, matColor.y, matColor.z, matColor.w };
					ImGui::ColorEdit4("Color", matColorArray);
					//
					//Diffuse
					ImTextureID texID=(ImTextureID)TextureManager::instance()->getNullTex();
					
					
					ImGui::PushID(1);
					ImGui::ImageButton(texID, ImVec2(32.0f, 32.0f));
						// InitFileBrowser("Resources\\Textures", ".tga", &MenuManager::ImportDiffuse);
					ImGui::PopID();
					//
					ImGui::SameLine();
					ImGui::BeginGroup();
					ImGui::Text("Albedo");
					if (ImGui::Button("Clear##D"))
						// mat->RemoveDiffuse();
					ImGui::SameLine();
					ImGui::Text("diffuse");
					ImGui::EndGroup();
					
					ImGui::Spacing();
					
					texID= (ImTextureID)TextureManager::instance()->loadDefaultN();
					
					ImGui::PushID(2);
					ImGui::ImageButton(texID, ImVec2(32.0f, 32.0f));
						// InitFileBrowser("Resources\\Textures", ".tga", &MenuManager::ImportNormal);
					ImGui::PopID();
					
					ImGui::SameLine();
					ImGui::BeginGroup();
					ImGui::Text("Normal");
					if (ImGui::Button("Clear##N"))
						// mat->RemoveNormal();
					ImGui::SameLine();
					ImGui::Text("normal");
					ImGui::EndGroup();
					
					ImGui::Spacing();
					
					//Specular
					texID = (ImTextureID)TextureManager::instance()->loadDefaultS();
					
					
					ImGui::PushID(3);
					ImGui::ImageButton(texID, ImVec2(32.0f, 32.0f));
						// InitFileBrowser("Resources\\Textures", ".tga", &MenuManager::ImportSpecular);
					ImGui::PopID();
					
					ImGui::SameLine();
					ImGui::BeginGroup();
					ImGui::Text("Specular(r:Roughness g:Metallic)");
					ImGui::Button("Clear##S");
						// mat->RemoveSpecular();
					ImGui::SameLine();
					// ImGui::Text(mat->GetSpecularPath().c_str());
					ImGui::EndGroup();
					
					ImGui::Spacing();
				}
			}
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
	io.ConfigFlags |= ImGuiWindowFlags_MenuBar;

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

	drawMainMenuBar();

	if (showSceneEditorDialog_)
		drawSceneEditorDialog();

	if (showObjectEditorDialog_)
		drawObjEditorDialog();
}

std::shared_ptr<Object> MenuManager::getSelectedObj() const
{
	return selectedObj_;
}
