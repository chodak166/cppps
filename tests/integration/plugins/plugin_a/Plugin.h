#ifndef PLUGIN_H
#define PLUGIN_H

#include <cppps/IPlugin.h>

class Plugin: public cppps::IPlugin
{
public:
  Plugin();
  ~Plugin();

  // IPlugin interface
  std::string getName() const override {return "PLUGIN A";}
  std::string getVersionString() const override {return "0.0.1";}
  std::string getLocation() const override;
  void load() override;

};

#endif // PLUGIN_H
