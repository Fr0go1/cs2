#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "global.hpp" // Include your global settings header
#include "ConfigSaver.hpp"

namespace MyConfigSaver {

    // Function to save the configuration to a file
    void SaveConfig(const std::string& filename) {
        std::ofstream configFile(filename);
        if (!configFile.is_open()) {
            std::cerr << "Error: Could not open the configuration file." << std::endl;
            return;
        }

        // Example: Save global settings to the file
        configFile << "aimbot " << global::aimbot << std::endl;
        configFile << "headbox " << global::headbox << std::endl;
        configFile << "Boxbox " << global::Boxbox << std::endl;
        configFile << "DrawBone " << global::DrawBone << std::endl;
        configFile << "ShowLosLine " << global::ShowLosLine << std::endl;
        configFile << "showHealthBar " << global::showHealthBar << std::endl;
        configFile << "BoxColor " << global::BoxColor.x << " " << global::BoxColor.y << " " << global::BoxColor.z << " " << global::BoxColor.w << std::endl;
        configFile << "BoxBoxColor " << global::BoxBoxColor.x << " " << global::BoxBoxColor.y << " " << global::BoxBoxColor.z << " " << global::BoxBoxColor.w << std::endl;
        configFile << "HeadColor " << global::HeadColor.x << " " << global::HeadColor.y << " " << global::HeadColor.z << " " << global::HeadColor.w << std::endl;
        configFile << "BoneColor " << global::BoneColor.x << " " << global::BoneColor.y << " " << global::BoneColor.z << " " << global::BoneColor.w << std::endl;
        configFile << "LOSColor " << global::LOSColor.x << " " << global::LOSColor.y << " " << global::LOSColor.z << " " << global::LOSColor.w << std::endl;
        configFile << "showWatermark " << global::showWatermark << std::endl;
        configFile << "watermarkX " << global::watermarkX << std::endl;
        configFile << "watermarkY " << global::watermarkY << std::endl;
        configFile << "fps " << global::fps << std::endl;
        configFile << "time " << global::time << std::endl;
        configFile << "cheat " << global::cheat << std::endl;

        configFile.close();
        std::cout << "Configuration saved to " << filename << std::endl;
    }

    // Function to load the configuration from a file
    void LoadConfig(const std::string& filename) {
        std::ifstream configFile(filename);
        if (!configFile.is_open()) {
            std::cerr << "Error: Could not open the configuration file." << std::endl;
            return;
        }

        std::string line;
        while (std::getline(configFile, line)) {
            std::istringstream iss(line);
            std::string key;
            if (iss >> key) {
                if (key == "aimbot") iss >> global::aimbot;
                else if (key == "headbox") iss >> global::headbox;
                else if (key == "Boxbox") iss >> global::Boxbox;
                else if (key == "DrawBone") iss >> global::DrawBone;
                else if (key == "ShowLosLine") iss >> global::ShowLosLine;
                else if (key == "showHealthBar") iss >> global::showHealthBar;
                else if (key == "BoxColor") iss >> global::BoxColor.x >> global::BoxColor.y >> global::BoxColor.z >> global::BoxColor.w;
                else if (key == "BoxBoxColor") iss >> global::BoxBoxColor.x >> global::BoxBoxColor.y >> global::BoxBoxColor.z >> global::BoxBoxColor.w;
                else if (key == "HeadColor") iss >> global::HeadColor.x >> global::HeadColor.y >> global::HeadColor.z >> global::HeadColor.w;
                else if (key == "BoneColor") iss >> global::BoneColor.x >> global::BoneColor.y >> global::BoneColor.z >> global::BoneColor.w;
                else if (key == "LOSColor") iss >> global::LOSColor.x >> global::LOSColor.y >> global::LOSColor.z >> global::LOSColor.w;
                else if (key == "showWatermark") iss >> global::showWatermark;
                else if (key == "watermarkX") iss >> global::watermarkX;
                else if (key == "watermarkY") iss >> global::watermarkY;
                else if (key == "fps") iss >> global::fps;
                else if (key == "time") iss >> global::time;
                else if (key == "cheat") iss >> global::cheat;
            }
        }

        configFile.close();
        std::cout << "Configuration loaded from " << filename << std::endl;
    }
} // namespace ConfigSaver
