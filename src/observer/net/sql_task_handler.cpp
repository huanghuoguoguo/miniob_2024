/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by Wangyunlai on 2024/01/10.
//

#include "net/sql_task_handler.h"
#include "net/communicator.h"
#include "event/session_event.h"
#include "event/sql_event.h"
#include "session/session.h"

RC SqlTaskHandler::handle_event(Communicator *communicator)
{
  SessionEvent *event = nullptr;
  RC rc = communicator->read_event(event);
  if (OB_FAIL(rc)) {
    return rc;
  }

  if (nullptr == event) {
    return RC::SUCCESS;
  }

  session_stage_.handle_request2(event); //将指定会话设置到线程变量里，设置当前正在处理的请求。原理为设置指针。

  SQLStageEvent sql_event(event, event->query()); // 创建一个新的查询对象，包括查询信息

  rc = handle_sql(&sql_event);
  if (OB_FAIL(rc)) {
    LOG_TRACE("failed to handle sql. rc=%s", strrc(rc));
    event->sql_result()->set_return_code(rc);
  }

  bool need_disconnect = false;

  rc = communicator->write_result(event, need_disconnect);
  LOG_INFO("write result return %s", strrc(rc));
  if (OB_FAIL(rc)) {
    LOG_TRACE("failed to write result. rc=%s", strrc(rc));
    event->sql_result()->set_return_code(rc);
  }
  event->session()->set_current_request(nullptr);
  Session::set_current_session(nullptr);

  delete event;

  if (need_disconnect) {
    return RC::INTERNAL;
  }
  return RC::SUCCESS;
}

RC SqlTaskHandler::handle_sql(SQLStageEvent *sql_event)
{
  // 查询缓存阶段 先初始化为空状态rc
  RC rc = query_cache_stage_.handle_request(sql_event);
  if (OB_FAIL(rc)) {
    LOG_TRACE("failed to do query cache. rc=%s", strrc(rc));
    return rc;
  }

  rc = parse_stage_.handle_request(sql_event);  // parse_stage_  先进入解析阶段，词法分析--语法分析
  if (OB_FAIL(rc)) {
    LOG_TRACE("failed to do parse. rc=%s", strrc(rc));
    return rc;
  }

  rc = resolve_stage_.handle_request(sql_event);  // 语法树解析成数据库内部能识别的数据结构-stmt
  if (OB_FAIL(rc)) {
    LOG_TRACE("failed to do resolve. rc=%s", strrc(rc));
    return rc;
  }

  rc = optimize_stage_.handle_request(sql_event);  // 查询优化阶段--逻辑计划--物理计划
  if (rc != RC::UNIMPLEMENTED && rc != RC::SUCCESS) {
    LOG_TRACE("failed to do optimize. rc=%s", strrc(rc));
    return rc;
  }

  rc = execute_stage_.handle_request(sql_event);  // 计划执行阶段
  if (OB_FAIL(rc)) {
    LOG_TRACE("failed to do execute. rc=%s", strrc(rc));
    return rc;
  }

  return rc;
}