#include<vsg/all.h>
#include"TextEditor.h"
#include"ImGuiFileDialog.h"
#include<imgui.h>
#include"config_app.h"
#include"GUI.h"

namespace Katie {
    MyGuiComponent Katie::getGUI()
    {
        auto params = Params::create();
        return MyGuiComponent(params);
    }
}