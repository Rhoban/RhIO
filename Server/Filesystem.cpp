#include <stdexcept>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "Server/Filesystem.hpp"
#include "Server/IONode.hpp"

namespace RhIO {

bool isDirectory(const std::string& path)
{
     DIR* dir = opendir(path.c_str());
     if (dir == nullptr) {
         return false;
     } else {
         closedir(dir);
         return true;
     }
}

std::vector<std::string> listDirectories(const std::string& path)
{
     DIR* dir = opendir(path.c_str());
     if (dir == nullptr) {
         throw std::runtime_error(
            "RhIO unable to open path directory: " + path);
     }

     std::vector<std::string> list;
     struct dirent* dirList;
     while ((dirList = readdir(dir)) != nullptr) {
         if (dirList->d_type == DT_DIR) {
             list.push_back(std::string(dirList->d_name));
         }
     }

     closedir(dir);
     return list;
}

std::vector<std::string> listFiles(const std::string& path)
{
     DIR* dir = opendir(path.c_str());
     if (dir == nullptr) {
         throw std::runtime_error(
            "RhIO unable to open path directory: " + path);
     }

     std::vector<std::string> list;
     struct dirent* dirList;
     while ((dirList = readdir(dir)) != nullptr) {
         if (dirList->d_type == DT_REG) {
             list.push_back(std::string(dirList->d_name));
         }
     }

     closedir(dir);
     return list;
}

void createDirectory(const std::string& path, 
    const std::string& name)
{
    std::string newPath;
    if (
        name.length() > 0 && 
        name[0] != separator && 
        path.length() > 0 && 
        path[path.length()-1] != separator
    ) {
        newPath = std::string(path + separator + name);
    } else {
        newPath = std::string(path + name);
    }

    //Build path from deepest requested
    //folder to "root" requested folder
    std::vector<std::string> parts;
    size_t pos = newPath.length();
    while (true) {
        parts.push_back(newPath.substr(0, pos));
        pos = newPath.find_last_of("/", pos-1);
        if (pos == 0 || pos == std::string::npos) {
            break;
        }
    }

    //Find all non existing directories in path
    int index = 0;
    for (;index<(int)parts.size();index++) {
        if (isDirectory(parts[index])) {
            break;
        }
    }
    index--;
    if (index < 0) {
        return;
    }
    //Create all non existing directories
    for (;index>=0;index--) {
        int result = mkdir(parts[index].c_str(), 0777);
        if (result != 0) {
            throw std::runtime_error(
                "RhIO unable to create directory in: " + path);
        }
    }
}

}

