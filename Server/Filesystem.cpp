#include <stdexcept>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "Filesystem.hpp"
#include "IONode.hpp"

namespace RhIO {

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

    int result = mkdir(newPath.c_str(), 0777);
    if (result != 0) {
        throw std::runtime_error(
            "RhIO unable to create directory in: " + path);
    }
}

}

