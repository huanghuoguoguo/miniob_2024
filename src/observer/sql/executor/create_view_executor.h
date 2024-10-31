//
// Created by glwuy on 24-10-29.
//

#ifndef CREATE_VIEW_EXECUTOR_H
#define CREATE_VIEW_EXECUTOR_H
#include <event/session_event.h>
#include <event/sql_event.h>
#include <session/session.h>
#include <storage/db/db.h>

#include "common/value.h"
#include "sql/stmt/create_view_stmt.h"


class CreateViewStmt;
class Session;
class SQLStageEvent;
/**
 * @brief 创建视图的执行器
 * @ingroup Executor
 */
class CreateViewExecutor
{
public:
    CreateViewExecutor() = default;
    virtual ~CreateViewExecutor() = default;

    RC execute(SQLStageEvent* sql_event)
    {
        RC rc = RC::SUCCESS;
        Stmt* stmt = sql_event->stmt();
        Session* session = sql_event->session_event()->session();
        string sql = sql_event->sql();
        ASSERT(stmt->type() == StmtType::CREATE_VIEW,
               "create view executor can not run this command: %d", static_cast<int>(stmt->type()));
        CreateViewStmt* create_view_stmt = static_cast<CreateViewStmt*>(stmt);
        rc = session->get_current_db()->create_view(create_view_stmt->view_name().c_str(),
                                                    create_view_stmt->select_stmt(), sql,
                                                    create_view_stmt->query_expressions());
        if (RC::SUCCESS != rc)
        {
            LOG_WARN("failed to create view %s, rc=%s", create_view_stmt->view_name().c_str(), strrc(rc));
        }
        return rc;
    }
};


#endif //CREATE_VIEW_EXECUTOR_H
