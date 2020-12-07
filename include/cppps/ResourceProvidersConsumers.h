// Copyright (c) 2020  Lukasz Chodyla
// Distributed under the Boost Software License v1.0.
// See accompanying file LICENSE.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt for the full license.

#ifndef RESOURCEPROVIDERSCONSUMERS_H
#define RESOURCEPROVIDERSCONSUMERS_H

#include "cppps/Resource.h"

namespace cppps {

using ResourceProvider = std::function<Resource()>;
using ResourceConsumer = std::function<void(const Resource&)>;

}

#endif // RESOURCEPROVIDERSCONSUMERS_H
