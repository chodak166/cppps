#ifndef IPLUGIN_H
#define IPLUGIN_H

#include "cppps/Resource.h"
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

//class ResourceProvider;
//class ResourceConsumer;

using SubmitProvider = std::function<void(std::string_view, const ResourceProvider&)>;
using SubmitConsumer = std::function<void(std::string_view, const ResourceConsumer&)>;

//class IProviderSet;
//class IConsumerSet;

class IPlugin
{
public:
  virtual ~IPlugin() = default;
  virtual std::string getName() const = 0;
  virtual std::string getVersionString() const = 0;
  // virtual bool isCritical() const = 0; //TODO
  virtual void prepare(const ICliPtr& app) = 0;
  virtual void submitProviders(const SubmitProvider& submitProvider) = 0;
  virtual void submitConsumers(const SubmitConsumer& submitConsumer) = 0;
  virtual void initialize() = 0;
  virtual void start() = 0;
  virtual void stop() = 0;
  virtual void unload() = 0;
};

//class IProviderSet
//{
//public:
//  virtual ~IProviderSet() = default;
//  virtual void add()
//};

}



#endif // IPLUGIN_H
