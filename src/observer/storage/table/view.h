//
// Created by glwuy on 24-10-29.
//

#ifndef VIEW_H
#define VIEW_H
#include "table.h"

/**
* 视图可以被当作table直接使用，但是对view的查询，增加，删除，改变，都会影响到原始表。
* 基于单个表的视图：
*   当视图只基于一个表时，通常可以执行增删改查操作（INSERT、DELETE、UPDATE），前提是视图的查询满足某些条件。
*   例如，如果视图中没有聚合函数、GROUP BY、DISTINCT 等复杂操作，那么通常可以对其进行更新。
* 基于多个表的视图：
*   当视图基于多个表时，通常不支持增删改查操作。这是因为 MySQL 无法确定如何将数据更改应用到多个表上。
*   例如，涉及联合（JOIN）、子查询或聚合的视图通常被认为是只读的。
* 例外情况
*   有些情况下，即使是基于单个表的视图，也可能因为特定的查询条件（如涉及计算字段）而无法进行更新。
*/
class View : Table {
  // 重写父类的增删改查，并且校验是否能够增删改查。

  // 真实表会打开一个table，存储表的元数据，view也要进行持久化。
private:

  // 持有真实表的指针。
  std::set<Table*> tables;
  // select_stmt* 原始创建的select。每次打开这个view，是否类似于一个table_scan?打开一个物理计划？
  SelectStmt* selectStmt;
  BinderContext* binderContext;
};



#endif //VIEW_H
