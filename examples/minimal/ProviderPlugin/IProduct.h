#ifndef IPRODUCT_H
#define IPRODUCT_H

#include <string>
#include <memory>

class IProduct
{
public:
  virtual ~IProduct() = default;
  virtual int getValue() = 0;
  virtual int increaseValue() = 0;
};

using IProductPtr = std::shared_ptr<IProduct>;
using IProductWPtr = std::weak_ptr<IProduct>;

#endif // IPRODUCT_H
