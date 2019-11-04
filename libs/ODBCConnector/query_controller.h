#ifndef HANDLE_H
#define HANDLE_H


#include <QSqlDatabase>
#include "ODBCW.h"


class CQueryController {
public:
  CQueryController(CODBCW& other);
  ~CQueryController();
  bool executeSqlQuery(const QString& query);
  QVariant parse_value(const QString& name);
  QVariant parse_value(int i);
  int size() const;
  bool next();
  bool isSelect() const;
  void free(CODBCW& db);
  void clear();
  QSqlQuery getQuery();
private:
  QSqlQuery mysqlquery;
};

#endif // HANDLE_H
