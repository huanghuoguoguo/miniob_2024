//
// Created by glwuy on 25-4-13.
//

#ifndef OPERATOR_ITERATOR_H
#define OPERATOR_ITERATOR_H



#include <functional>
#include "common/rc.h"
#include "physical_operator.h"
class PhysicalOperator;

class OperatorIterator {
public:
  static RC iterate_child_oper(PhysicalOperator* expr, std::function<RC(PhysicalOperator*)> callback);
};




#endif //OPERATOR_ITERATOR_H
