// Copyright (c) 2021  Lukasz Chodyla
// Distributed under the Boost Software License v1.0.
// See accompanying file LICENSE.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt for the full license.

#ifndef EXPORT_H
#define EXPORT_H

#ifdef CPPPS_DL_USE_BOOST
# include <boost/dll/alias.hpp>
#endif

#if __GNUC__ >= 4
#  if defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(__CYGWIN__)
#    define CPPPS_SYMBOL_EXPORT __attribute__((__dllexport__))
#  else
#    define CPPPS_SYMBOL_EXPORT __attribute__((__visibility__("default")))
#  endif
#else
#  define CPPPS_SYMBOL_EXPORT __declspec(dllexport)
#endif

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(__CYGWIN__)
#define CPPPS_DL_EXPORT_FN(FN_NAME, FN_ALIAS)                               \
  extern "C" {CPPPS_SYMBOL_EXPORT void* FN_ALIAS() {                        \
    return reinterpret_cast<void*>(reinterpret_cast<intptr_t>(&FN_NAME));}} \
  /**/
#else
#define CPPPS_DL_EXPORT_FN(FN_NAME, FN_ALIAS)                         \
  extern "C" {CPPPS_SYMBOL_EXPORT void* FN_ALIAS                      \
    = reinterpret_cast<void*>(reinterpret_cast<intptr_t>(&FN_NAME));} \
  /**/
#endif

#ifndef CPPPS_DL_USE_BOOST
#define CPPPS_EXPORT_PLUGIN(CLASS_NAME)                         \
  cppps::IPluginUPtr _makePlugin() {                            \
    return std::unique_ptr<cppps::IPlugin>(new CLASS_NAME());}  \
  CPPPS_DL_EXPORT_FN(_makePlugin, make_plugin)                  \
  /**/
#else
#define CPPPS_EXPORT_PLUGIN(CLASS_NAME)                         \
  cppps::IPluginUPtr _makePlugin() {                            \
    return std::unique_ptr<cppps::IPlugin>(new CLASS_NAME());}  \
  BOOST_DLL_ALIAS(_makePlugin, make_plugin)                     \
  /**/
#endif

#endif // EXPORT_H
