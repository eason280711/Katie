#pragma once
#include<vsg/all.h>
#include"TextEditor.h"
#include"ImGuiFileDialog.h"
#include<imgui.h>
#include"config_app.h"

namespace Katie {
    struct Params : public vsg::Inherit<vsg::Object, Params>
    {
        bool showGui = true; // you can toggle this with your own EventHandler and key
        bool showDemoWindow = false;
        bool showSecondWindow = false;
        float clearColor[3]{ 0.2f, 0.2f, 0.4f }; // Unfortunately, this doesn't change dynamically in vsg
        uint32_t counter = 0;
        float dist = 0.f;
        bool z_init = false;

        TextEditor editor;
        TextEditor::LanguageDefinition lang = TextEditor::LanguageDefinition::Python();

        char* fileToEdit;
        char* fileName;
        bool save = true;
    };

    class MyGuiComponent
    {
    public:
        MyGuiComponent(vsg::ref_ptr<Params> params) :
            _params(params)
        {
            ///////////////////////////////////////////////////////////////////////
            // TEXT EDITOR SAMPLE
            (*_params).editor.SetLanguageDefinition((*_params).lang);

            (*_params).fileToEdit = "";
            (*_params).fileName = "";
            (*_params).editor.SetPalette(TextEditor::GetKatiePalette());
            //	static const char* fileToEdit = "test.cpp";

            {
                std::ifstream t((*_params).fileToEdit);
                if (t.good())
                {
                    std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
                    (*_params).editor.SetText(str);
                }
            }

            (*_params).editor.SaveCallBack = [&]() {
                std::ofstream t((*_params).fileToEdit);
                auto content = (*_params).editor.GetText();
                if (t.good())
                    t << content;
                (*_params).save = true;
            };
        }

        // Example here taken from the Dear imgui comments (mostly)
        bool operator()()
        {
            bool visibleComponents = false;
            //editor
            auto cpos = (*_params).editor.GetCursorPosition();
            ImGui::Begin("Text Editor Demo", nullptr, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);
            ImGui::SetWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);

            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("File"))
                {
                    if (ImGui::MenuItem("Open File"))
                    {
                        ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".py", SCRIPT_ROOT);
                    }
                    if (ImGui::MenuItem("Save", "Ctrl-S"))
                    {
                        std::ofstream t((*_params).fileToEdit);
                        auto content = (*_params).editor.GetText();
                        if (t.good())
                            t << content;

                        (*_params).save = true;
                        /// save text....
                    }
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Edit"))
                {
                    bool ro = (*_params).editor.IsReadOnly();

                    if (ImGui::MenuItem("Undo", "ALT-Backspace", nullptr, !ro && (*_params).editor.CanUndo()))
                        (*_params).editor.Undo();
                    if (ImGui::MenuItem("Redo", "Ctrl-Y", nullptr, !ro && (*_params).editor.CanRedo()))
                        (*_params).editor.Redo();

                    ImGui::Separator();

                    if (ImGui::MenuItem("Copy", "Ctrl-C", nullptr, (*_params).editor.HasSelection()))
                        (*_params).editor.Copy();
                    if (ImGui::MenuItem("Cut", "Ctrl-X", nullptr, !ro && (*_params).editor.HasSelection()))
                        (*_params).editor.Cut();
                    if (ImGui::MenuItem("Delete", "Del", nullptr, !ro && (*_params).editor.HasSelection()))
                        (*_params).editor.Delete();
                    if (ImGui::MenuItem("Paste", "Ctrl-V", nullptr, !ro && ImGui::GetClipboardText() != nullptr))
                        (*_params).editor.Paste();

                    ImGui::Separator();

                    if (ImGui::MenuItem("Select all", nullptr, nullptr))
                        (*_params).editor.SetSelection(TextEditor::Coordinates(), TextEditor::Coordinates((*_params).editor.GetTotalLines(), 0));

                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }
            if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
            {
                // action if OK
                if (ImGuiFileDialog::Instance()->IsOk())
                {
                    std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
                    std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
                    auto selection = ImGuiFileDialog::Instance()->GetSelection();
                    // action
                    if (selection.size() > 0)
                    {
                        std::string str = ((*selection.begin()).second);
                        std::string Nme = ((*selection.begin()).first);
                        (*_params).fileToEdit = strdup(str.data());
                        (*_params).fileName = strdup(Nme.data());
                        ;                    std::ifstream t((*_params).fileToEdit);
                        if (t.good())
                        {
                            std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
                            (*_params).editor.SetText(str);
                        }
                    }
                }

                // close
                ImGuiFileDialog::Instance()->Close();
            }

            auto tmp = std::string((*_params).fileName) + ((!(*_params).save) ? "*" : "");
            auto path = std::string((*_params).fileToEdit);
            if ((*_params).editor.IsTextChanged())
                (*_params).save = false;
            ImGui::Text("%6d/%-6d %6d lines  | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, (*_params).editor.GetTotalLines(),
                (*_params).editor.GetLanguageDefinition().mName.c_str(),
                tmp.c_str());
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("%s", path.c_str());

            (*_params).editor.Render("TextEditor");
            ImGui::End();

            // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
            if (_params->showGui)
            {
                ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.

                ImGui::Text("Some useful message here.");                 // Display some text (you can use a format strings too)
                ImGui::Checkbox("Demo Window", &_params->showDemoWindow); // Edit bools storing our window open/close state
                ImGui::Checkbox("Another Window", &_params->showSecondWindow);
                ImGui::SliderFloat("float", &_params->dist, 0.0f, 1.0f);        // Edit 1 float using a slider from 0.0f to 1.0f
                ImGui::ColorEdit3("clear color", (float*)&_params->clearColor); // Edit 3 floats representing a color

                if (ImGui::Button("Button")) // Buttons return true when clicked (most widgets return true when edited/activated)
                    _params->counter++;

                ImGui::SameLine();
                ImGui::Text("counter = %d", _params->counter);

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::End();

                visibleComponents = true;
            }

            // 3. Show another simple window.
            if (_params->showSecondWindow)
            {
                ImGui::Begin("Another Window", &_params->showSecondWindow); // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
                ImGui::Text("Hello from another window!");
                if (ImGui::Button("Close Me"))
                    _params->showSecondWindow = false;
                ImGui::End();

                visibleComponents = true;
            }

            if (_params->showDemoWindow)
            {
                ImGui::ShowDemoWindow(&_params->showDemoWindow);

                visibleComponents = true;
            }
            return visibleComponents;
        }

    private:
        vsg::ref_ptr<Params> _params;
    };

    MyGuiComponent getGUI();
}