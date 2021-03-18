#include "directory_manager.hpp"
#include <iostream>
#include <fstream>
#include <string>

#include <environnement.hpp>
using namespace std;

DirectoryManager & DirectoryManager::Instance(){
    static DirectoryManager gf;
    return gf;
}

void DirectoryManager::readFile(){
    string line;
    ifstream myfile ("path.txt");
    int i =0;
    if (myfile.is_open())
    {
      while ( getline (myfile,line) )
      {
          if(i==0){
              m_bin_dir = line;
          }
          if(i==1){
              m_source_dir = line;
          }
          ++i;
      }
      myfile.close();
    }
}

DirectoryManager::DirectoryManager(){
    readFile();
    if(m_source_dir.empty()){
        ofstream myfile ("path.txt");
        if (myfile.is_open())
        {
            myfile << ProjectSourceBin2;
            myfile << std::endl;
            myfile << ProjectSourceDir2;
            myfile.close();
        }
        readFile();
    }
}

const std::string & DirectoryManager::getBinDirectory(){
    return m_bin_dir;
}

const std::string & DirectoryManager::getSourceDirectory(){
    return m_source_dir;
}


DirectoryManager::~DirectoryManager(){
    
}
