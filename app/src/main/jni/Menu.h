void BeginDraw() {

     ImGuiIO &io = ImGui::GetIO();
     ImVec2 center = ImGui::GetMainViewport()->GetCenter();
     
     ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
     if (ImGui::Begin(OBFUSCATE("Chams Tester By ZEFF By tojik_proof_93 | APK | Unreal Engine | Unity"))) {
		g_window = ImGui::GetCurrentWindow();
        //ImGui::Text("%.1f FPS", io.Framerate);
		if (ImGui::BeginTabBar(OBFUSCATE("Tab"))) {
			if (ImGui::BeginTabItem(OBFUSCATE("Main"))) {
                #if defined(__aarch64__)
                ImGui::Text(OBFUSCATE("Bit: 64")); 
                #else
                ImGui::Text(OBFUSCATE("Bit: 32")); 
                #endif
                ImGui::Separator();
                ImGui::Text(OBFUSCATE("Telegram: @zeff_cheats @tojik_proof_93")); 
                ImGui::Separator();
                ImGui::Text(OBFUSCATE("Youtube: @zeff_source @saveeditoronlinecom")); 
                ImGui::Separator();
                ImGui::Text(OBFUSCATE("Discord: https://discord.gg/RSC4ZrVjS8")); 
                ImGui::Separator();
                ImGui::Text(OBFUSCATE("Made By ZEFF By tojik_proof_93"));
				ImGui::Separator();
				ImGui::Text(OBFUSCATE("Так же можно писать кириллицей в сурсе")); 
                ImGui::EndTabItem();
            }
			if (ImGui::BeginTabItem(OBFUSCATE("Visuals"))) {
				if (chamsint > 1) ImGui::Separator();
				ImGui::PushItemWidth(500);
				ImGui::Combo(OBFUSCATE("Current Chams"), &chamsint, Chams, IM_ARRAYSIZE(Chams));
				if (chamsint > 1) {
                    ImGui::PushItemWidth(45);
                    ImGui::ColorEdit3(OBFUSCATE("##Visible Color"), (float *) &visibleColor);
                    ImGui::SameLine();
                    ImGui::Checkbox(OBFUSCATE("RGB"), &enableRainbow);
				}
                if (chamsint == 6 || chamsint == 7) {
                    ImGui::ColorEdit3(OBFUSCATE("##In Wall Color"), (float *) &inWallColor);
                    ImGui::SameLine();
                    ImGui::Checkbox(OBFUSCATE("RGB"), &enableRainbowWall);
                }
                ImGui::PushItemWidth(0);
				if (chamsint > 1) ImGui::Separator();
				ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem(OBFUSCATE("Shaders"))) {
                for (std::string shader : shaders) {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 1));
                    if (mineShader == shader) ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 1, 0, 1));
                    ImGui::PushID(shader.c_str());
                    ImGui::Text("%s", shader.c_str());
                    ImGui::SameLine();
                    if (ImGui::Button(OBFUSCATE("Set"))) {
                        mineShader = shader;
                    }
                    ImGui::Separator();
				    ImGui::PopID();
                }
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 1));
                ImGui::EndTabItem();
            }
			ImGui::EndTabBar();
        }
    }
    if (enableRainbow) {
        visibleColor.x = redd/255;
        visibleColor.y = greenn/255;
        visibleColor.z = bluee/255;
    }
    if (enableRainbowWall) {
        inWallColor.x = redd/255;
        inWallColor.y = greenn/255;
        inWallColor.z = bluee/255;
    }
    performRGBChange();
    Patches();
}
