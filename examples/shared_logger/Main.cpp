#include "cppps-shared-logger/Version.h"

#include <cppps/dl/Application.h>
//#include <cppps/logging/Logging.h>

// Run with --help argument to see the help page

int main(int argc, char *argv[])
{
  const cppps::AppInfo info {
    cppps::logex::APP_NAME,
    cppps::logex::APP_DESCRIPTION,
    cppps::logex::APP_NAME
        + std::string(" ")
        + cppps::logex::getVersionString()
  };

  cppps::Application app(info);
  app.setPluginDirectories({app.getAppDirPath() + "/" + cppps::logex::PLUGIN_DIR_NAME});
  return app.exec(argc, argv);
}

