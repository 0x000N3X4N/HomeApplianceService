#include "handle.h"

Handle::Handle(XSqlDatabase& other) {
  QSqlQuery tmp(reinterpret_cast<QSqlDatabase&>(other));
  mysqlquery = tmp;
}

Handle::~Handle() { }

bool Handle::executeSqlQuery(const QString& query) {
  return mysqlquery.exec(query);
}

QVariant Handle::parse_value(const QString& name) { return mysqlquery.value(name); }

QVariant Handle::parse_value(int i)               { return mysqlquery.value(i);    }

int Handle::size() const                          { return mysqlquery.size();      }

bool Handle::next()                               { return mysqlquery.next();      }

bool Handle::isSelect() const                     { return mysqlquery.isSelect();  }

QSqlQuery Handle::getQuery()                      { return mysqlquery;             }

void Handle::free(XSqlDatabase& db) {
  db.close();
  QSqlDatabase::removeDatabase(db.databaseName());
}

void Handle::clear() {
  mysqlquery.clear();
}

