#include "ITestProduct.h"
#include "cppps/dl/IPlugin.h"
#include "cppps/dl/Export.h"
#include <iostream>

using namespace cppps;

class Plugin: public IPlugin
{
public:
  std::string getName() const override {return PLUGIN_NAME;}
  std::string getVersionString() const override {return PLUGIN_VERSION;}
  void prepare(const ICliPtr& /*cli*/, IApplication& /*app*/) override {};
  void submitProviders(const SubmitProvider& /*submitProvider*/) override {};
  void submitConsumers(const SubmitConsumer& submitConsumer) override
  {
    submitConsumer("product_b", [this](const Resource& resource){
      product = resource.as<ITestProductPtr>();
    });
  };
  void initialize() override
  {
    std::cout << "Acquired product value: " << product->getValue() << std::endl;
  }
  void start() override {}
  void stop() override {}
  void unload() override {}
private:
  ITestProductPtr product {nullptr};
};

CPPPS_EXPORT_PLUGIN(Plugin)
