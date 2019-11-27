#ifndef ODBCW_H
#define ODBCW_H


#include <QtSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QMessageBox>
#include <QDebug>
#include "boost/noncopyable.hpp"


/* ------------------------------------------------------------------
 * Simple ODBC Wrapper, based on Singleton pattern
 * ------------------------------------------------------------------
 */

class CODBCW : public boost::noncopyable {
public:
  static CODBCW& getInstance();
  QString databaseName() const;
  void close();
  bool getStatus();
private:
  CODBCW();
  ~CODBCW() = default;

  QSqlDatabase db;
  bool status;
};

#endif // ODBCW_H_H
