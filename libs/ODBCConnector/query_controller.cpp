#include "query_controller.h"

CQueryController::CQueryController(CODBCW& other) {
  QSqlQuery tmp(reinterpret_cast<QSqlDatabase&>(other));
  mysqlquery = tmp;
}

CQueryController::~CQueryController() { }

bool CQueryController::executeSqlQuery(const QString& query) {
  return mysqlquery.exec(query);
}

QVariant CQueryController::parse_value(const QString& name) { return mysqlquery.value(name); }

QVariant CQueryController::parse_value(int i)               { return mysqlquery.value(i);    }

int CQueryController::size() const                          { return mysqlquery.size();      }

bool CQueryController::next()                               { return mysqlquery.next();      }

bool CQueryController::isSelect() const                     { return mysqlquery.isSelect();  }

QSqlQuery CQueryController::getQuery()                      { return mysqlquery;             }

void CQueryController::free(CODBCW& db) {
  db.close();
  QSqlDatabase::removeDatabase(db.databaseName());
}

void CQueryController::clear() {
  mysqlquery.clear();
}

