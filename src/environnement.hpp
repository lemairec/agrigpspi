#ifndef ENVIRONNEMENT_H
#define ENVIRONNEMENT_H

#include <string>

// the configured options and settings for Tutorial
const std::string  ProjectName             = "gps_agri";
const std::string  MachineName             = "Darwin-16.1.0;x86_64;Xcode";

//const unsigned int ProjectVersionMajor     = ;
//const unsigned int ProjectVersionMinor     = ;
//const unsigned int ProjectVersionPatch     = ;

const std::string  ProjectVersion = "..";

const std::string  ProjectCompleteName = "gps_agri ..@PROJECT_VERSION_PATCH";

const std::string  ProjectSourceDir = "/Users/lemairec/fablab/agrigpspi";
const std::string  ProjectSourceBin = "/Users/lemairec/fablab/agrigpspi/build";

#define LOG_FILE "/Users/lemairec/fablab/agrigpspi/build/log.txt"
#define GPS_LOG_FILE "/Users/lemairec/fablab/agrigpspi/build/gps.ubx"

#endif //ENVIRONNEMENT_H
