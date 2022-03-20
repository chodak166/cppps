// Copyright (c) 2020  Lukasz Chodyla
// Distributed under the MIT License.
// See accompanying file LICENSE.txt for the full license.

#ifndef PLUGINLOADER_H
#define PLUGINLOADER_H

#ifdef CPPPS_DL_USE_BOOST
#include "cppps/dl/BoostPluginLoader.h"
namespace cppps {
inline BoostPluginLoader getPluginLoader(){return BoostPluginLoader();}
}
#else

# ifdef _WIN32
#include "WindowsPluginLoader.h"
namespace cppps {
inline WindowsPluginLoader getPluginLoader(){return WindowsPluginLoader();}
}
# else
#include "DlopenPluginLoader.h"
namespace cppps {
inline DlopenPluginLoader getPluginLoader(){return DlopenPluginLoader();}
}
# endif
#endif

#endif // PLUGINLOADER_H
