#ifndef RHIO_FILESYSTEM_HPP
#define RHIO_FILESYSTEM_HPP

#include <vector>
#include <string>

namespace RhIO {

/**
 * List and return all forders name in
 * given directory path
 * Throw std::runtime_error is given path 
 * does not exist
 */
std::vector<std::string> listDirectories(const std::string& path);

/**
 * List and return all regular file name in
 * given directory path
 * Throw std::runtime_error is given path 
 * does not exist
 */
std::vector<std::string> listFiles(const std::string& path);

/**
 * Create a new folder inside given directory path
 * Throw std::runtime_error is given path 
 * does not exist
 */
void createDirectory(const std::string& path, 
    const std::string& name);

}

#endif

