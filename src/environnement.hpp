#ifndef ENVIRONNEMENT_H
#define ENVIRONNEMENT_H

#include <string>

// the configured options and settings for Tutorial
const std::string  ProjectName             = "agri_gps_pi";
const std::string  MachineName             = "Linux-3.16.0-4-amd64;x86_64;Unix Makefiles";

//const unsigned int ProjectVersionMajor     = ;
//const unsigned int ProjectVersionMinor     = ;
//const unsigned int ProjectVersionPatch     = ;

const std::string  ProjectVersion = "..";

const std::string  ProjectCompleteName = "agri_gps_pi ..@PROJECT_VERSION_PATCH";

const std::string  ProjectSourceDir = "/home/lemairec/workspace/agrigpspi";
const std::string  ProjectSourceBin = "/home/lemairec/workspace/agrigpspi/build";

#define LOG_FILE "/home/lemairec/workspace/agrigpspi/build/log.txt"
#define GPS_LOG_FILE "/home/lemairec/workspace/agrigpspi/build/gps.ubx"
#define GPS_TEST_FILE "/home/lemairec/workspace/agrigpspi/gps.ubx"

#endif //ENVIRONNEMENT_H
