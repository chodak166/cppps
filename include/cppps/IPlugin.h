#ifndef IPLUGIN_H
#define IPLUGIN_H

#include <string>
#include <memory>

namespace cppps {

class IPlugin;
using IPluginPtr = std::shared_ptr<IPlugin>;
using IPluginUPtr = std::unique_ptr<IPlugin>;

class IPlugin
{
public:
  virtual ~IPlugin() = default;
  virtual std::string getName() const = 0;
  virtual std::string getVersionString() const = 0;
  virtual std::string getLocation() const = 0;
  virtual void load() = 0;
};

}



#endif // IPLUGIN_H
