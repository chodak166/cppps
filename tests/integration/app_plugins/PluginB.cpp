#include "ITestProduct.h"
#include <cppps/IPlugin.h>
#include <boost/dll/alias.hpp>

using namespace cppps;

class ProductB: public ITestProduct
{
public:
  std::string getValue() const override
  {
    return "product_b_test_value";
  }
};

using ProductBPtr = std::shared_ptr<ProductB>;

class Plugin: public IPlugin
{
public:
  std::string getName() const override {return PLUGIN_NAME;}
  std::string getVersionString() const override {return PLUGIN_VERSION;}
  void prepare(const ICliPtr& /*app*/) override {};
  void submitProviders(const SubmitProvider& submitProvider) override
  {
    submitProvider("product_b", [this](){
      return std::static_pointer_cast<ITestProduct>(product);
    });
  };
  void submitConsumers(const SubmitConsumer& /*submitConsumer*/) override {}
  void initialize() override
  {
    product = std::make_shared<ProductB>();
  }
  void start() override {}
  void stop() override {}
  void unload() override {}

private:
  ProductBPtr product {nullptr};
};


cppps::IPluginUPtr makePlugin()
{
  return std::make_unique<Plugin>();
}

BOOST_DLL_ALIAS(makePlugin, make_plugin)
