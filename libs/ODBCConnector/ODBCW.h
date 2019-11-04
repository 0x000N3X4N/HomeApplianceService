#ifndef ODBCW_H
#define ODBCW_H


#include <QtSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QDebug>


/* ------------------------------------------------------------------
 * Simple ODBC Wrapper, based on Singleton pattern
 * ------------------------------------------------------------------
 */

class CODBCW {
public:
  static CODBCW& getInstance();
  QString databaseName() const;
  void close();
  bool getStatus();
private:
  CODBCW();
  ~CODBCW() = default;
  CODBCW(const CODBCW& instance) = delete;

  QSqlDatabase db;
  bool status;
};

#endif // ODBCW_H_H
