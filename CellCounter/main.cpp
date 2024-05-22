#include <filesystem>
#include <fstream>
#include <iostream>
#include <istream>
#include <map>
#include <vector>
#include <unordered_map>

namespace fs = std::filesystem;

using ValueType = double;

struct CellResult {
    ValueType value;
    std::pair<ValueType, ValueType> normal;
};

std::unordered_map<size_t, std::string> readPredefinedClasses(fs::path datasetPath)
{
    std::ifstream file(datasetPath / "predefined_classes.txt");
    if (!file.is_open())
        throw std::exception(); // TODO: change exception

    size_t classType;
    std::string name;
    std::string line;

    std::unordered_map<size_t, std::string> result;

    while (getline(file, line)) {
        std::stringstream ss;
        ss << line;
        ss >> classType;
        getline(ss, name);

        result[classType] = name;
    }

    return result;
}

std::map<std::string, CellResult> getSummary(fs::path datasetPath)
{
    auto predefinedClasses = readPredefinedClasses(datasetPath);
    std::map<std::string, CellResult> result;
    for (auto& predefClass : predefinedClasses)
        result[predefClass.second] = { 0, {0, 0} };

    size_t allCells = 0;

    for (auto path : fs::directory_iterator(datasetPath / "images_labels")) {
        std::ifstream file(path);

        std::string line;
        size_t classType;

        while (getline(file, line)) {
            std::stringstream ss;
            ss << line;
            ss >> classType;

            if (predefinedClasses.find(classType) == predefinedClasses.end())
                throw std::exception(); // TODO: add text to exception

            auto& res = result[predefinedClasses[classType]];
            res.value += 1;
            ++allCells;
        }
    }

    for (auto& predefClass : predefinedClasses)
        result[predefClass.second].value /= allCells / 100.;

    return result;
}

void printSummary(const std::map<std::string, CellResult>& summary)
{
    std::cout << std::setprecision(1) << std::fixed;
    for (auto& item : summary) {
        auto& name = item.first;
        auto& value = item.second.value;
        auto& normal = item.second.normal;
        std::cout << std::setw(30) << std::left << name << std::setw(10) << std::internal << value << std::setw(10) << normal.first << "-" << normal.second << std::endl;
    }
}

int main()
{
    auto summary = getSummary("C:/Users/doroshin/Documents/projects/cell-counter/build/data");
    printSummary(summary);

    return 0;
}