#include "../ProviderPlugin/IProduct.h"
#include "cppps/IPlugin.h"
#include "cppps/ICli.h"
#include "cppps/IApplication.h"
#include <boost/dll/alias.hpp>
#include <iostream>

using namespace cppps;

class Plugin: public IPlugin
{
public:
  std::string getName() const override {return PLUGIN_NAME;}
  std::string getVersionString() const override {return PLUGIN_VERSION;}
  void prepare(const ICliPtr& cli, IApplication& app) override
  {
    cli->addOption("-i,--increase", numOfIncreases, "Number of increases");

    app.setMainLoop([this](){
      for (int i = 0; i < numOfIncreases; ++i) {
        std::cout << "Increased product value: " << product->increaseValue() << std::endl;
      }
      std::cout << "Exiting" << std::endl;
      return 0;
    });
  };
  void submitProviders(const SubmitProvider& /*submitProvider*/) override {};
  void submitConsumers(const SubmitConsumer& submitConsumer) override
  {
    submitConsumer("product", [this](const Resource& resource){
      product = resource.as<IProductPtr>();
    });
  };
  void initialize() override
  {
    std::cout << "Initial product value: " << product->getValue() << std::endl;
  }
  void start() override {}
  void stop() override {}
  void unload() override {}
private:
  int numOfIncreases {1};
  IProductPtr product {nullptr};
};


cppps::IPluginUPtr makePlugin()
{
  return std::make_unique<Plugin>();
}

BOOST_DLL_ALIAS(makePlugin, make_plugin)
