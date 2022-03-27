#include "IProduct.h"
#include <cppps/dl/IPlugin.h>
#include <cppps/dl/Export.h>
#include <cppps/logging/Logging.h>

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
  void submitConsumers(const SubmitConsumer& submitConsumer) override
  {
    submitConsumer("shared_logger", [](const Resource& resource){
      auto logger = resource.as<cppps::LoggerPtr>();
      cppps::importLogger(logger);
    });
  };
  void initialize() override
  {
    LOG(INFO) << "Creating product";
    product = std::make_shared<Product>();
  }
  void start() override {}
  void stop() override {}
  void unload() override {}
private:
  IProductPtr product {nullptr};
};

CPPPS_EXPORT_PLUGIN(Plugin)

