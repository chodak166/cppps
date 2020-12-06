#ifndef IPLUGIN_H
#define IPLUGIN_H

#include <string>
#include <memory>
#include <functional>

namespace cppps {

class ICli;
using ICliPtr = std::shared_ptr<ICli>;

class IPlugin;
using IPluginPtr = std::shared_ptr<IPlugin>;
using IPluginUPtr = std::unique_ptr<IPlugin>;
using PluginDeleter = std::function<void(cppps::IPlugin*)>;
using IPluginDPtr = std::unique_ptr<IPlugin, PluginDeleter>;

class Providers;
class Consumers;

class IPlugin
{
public:
  virtual ~IPlugin() = default;
  virtual std::string getName() const = 0;
  virtual std::string getVersionString() const = 0;
  // virtual bool isCritical() const = 0; //TODO
  virtual void prepare(const ICliPtr& app) = 0;
  virtual void submitProviders(Providers& providers) = 0;
  virtual void submitConsumers(Consumers& providers) = 0;
  virtual void initialize() = 0;
  virtual void start() = 0;
  virtual void stop() = 0;
  virtual void unload() = 0;
};

}



#endif // IPLUGIN_H
