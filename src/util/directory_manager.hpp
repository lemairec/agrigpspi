#ifndef DIRECTORY_MANAGER_H
#define DIRECTORY_MANAGER_H

#include <string>

class DirectoryManager {
    DirectoryManager();
    void readFile();
    
    std::string m_bin_dir;
    std::string m_source_dir;
public:
    static DirectoryManager & Instance();
    ~DirectoryManager();
    
    const std::string & getBinDirectory();
    const std::string & getSourceDirectory();
};

#endif //UTIL_H
