#include "cppps-minimal/Version.h"

#include <cppps/dl/Application.h>

// Run with --help argument to see the help page

int main(int argc, char *argv[])
{
  const cppps::AppInfo info {
    cppps::minex::APP_NAME,
    cppps::minex::APP_DESCRIPTION,
    cppps::minex::APP_NAME
        + std::string(" ")
        + cppps::minex::getVersionString()
  };

  cppps::Application app(info);
  app.setPluginDirectories({app.getAppDirPath() + "/" + cppps::minex::PLUGIN_DIR_NAME});
  return app.exec(argc, argv);
}

