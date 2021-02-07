#include "version.h"

#include <cppps/Application.h>
#include <cppps/Logging.h>

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
  app.setPluginDirectories({app.getAppDirPath() + "/minex_plugins"});
  return app.exec(argc, argv);
}

