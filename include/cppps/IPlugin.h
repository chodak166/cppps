#ifndef IPLUGIN_H
#define IPLUGIN_H

#include <string>
#include <memory>
#include <functional>

namespace cppps {

class IApplication;
using IApplicationPtr = std::shared_ptr<IApplication>;

class IPlugin;
using IPluginPtr = std::shared_ptr<IPlugin>;
using IPluginUPtr = std::unique_ptr<IPlugin>;
using IPluginDeleter = std::function<void(cppps::IPlugin*)>;
using IPluginDPtr = std::unique_ptr<IPlugin, IPluginDeleter>;

class IPlugin
{
public:
  virtual ~IPlugin() = default;
  virtual std::string getName() const = 0;
  virtual std::string getVersionString() const = 0;
  virtual void prepare(const IApplicationPtr& app) = 0;
};

}



#endif // IPLUGIN_H
