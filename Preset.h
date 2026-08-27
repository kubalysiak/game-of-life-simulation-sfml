#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <cstdint>
#include <filesystem>
#include <iostream>

struct Preset {
    std::string name;
    int width = 0;
    int height = 0;
    std::vector<uint8_t> data;
};

class PresetManager {
public:
 
    static bool savePreset(const Preset& preset, const std::string& folderPath) {
        std::string filepath = folderPath + "/" + preset.name + ".txt";
        std::ofstream file(filepath);
        if (!file.is_open()) return false;

        file << preset.width << " " << preset.height << "\n";
        for (int y = 0; y < preset.height; ++y) {
            for (int x = 0; x < preset.width; ++x) {
                file << (preset.data[static_cast<size_t>(y) * preset.width + x] ? '1' : '0');
            }
            file << "\n";
        }
        return true;
    }

  
    static Preset loadPreset(const std::string& filepath) {
        Preset p;
        std::ifstream file(filepath);
        if (!file.is_open()) return p;

        std::filesystem::path pathObj(filepath);
        p.name = pathObj.stem().string();

        file >> p.width >> p.height;
        p.data.resize(static_cast<size_t>(p.width) * p.height, 0);

        std::string line;
        int y = 0;
        while (file >> line && y < p.height) {
            for (int x = 0; x < p.width && x < static_cast<int>(line.length()); ++x) {
                p.data[static_cast<size_t>(y) * p.width + x] = (line[x] == '1' ? 1 : 0);
            }
            y++;
        }
        return p;
    }
};