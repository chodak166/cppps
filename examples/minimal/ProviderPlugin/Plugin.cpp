#include "IProduct.h"
#include "cppps/IPlugin.h"
#include <boost/dll/alias.hpp>
#include <iostream>

using namespace cppps;

class Product: public IProduct
{
public:
  int getValue() override
  {
    return value;
  }

  int increaseValue() override
  {
    return ++value;
  }

private:
  int value {0};
};

class Plugin: public IPlugin
{
public:
  std::string getName() const override {return PLUGIN_NAME;}
  std::string getVersionString() const override {return PLUGIN_VERSION;}
  void prepare(const ICliPtr& /*cli*/, IApplication& /*app*/) override {};
  void submitProviders(const SubmitProvider& submitProvider) override
  {
    submitProvider("product", [this](){return product;});
  };
  void submitConsumers(const SubmitConsumer& /*submitConsumer*/) override {};
  void initialize() override
  {
    product = std::make_shared<Product>();
  }
  void start() override {}
  void stop() override {}
  void unload() override {}
private:
  IProductPtr product {nullptr};
};


cppps::IPluginUPtr makePlugin()
{
  return std::make_unique<Plugin>();
}

BOOST_DLL_ALIAS(makePlugin, make_plugin)
