#ifndef XSQLCONNECTOR_H
#define XSQLCONNECTOR_H

#include <QtSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QDebug>

/* ------------------------------------------------------------------
 * Simple MySQL Connector/C Wrapper
 * ------------------------------------------------------------------
 */

class XSqlDatabase {
public:
  static XSqlDatabase& getInstance();
  QString databaseName() const;
  void close();
  bool getStatus();
private:
  XSqlDatabase();
  ~XSqlDatabase() = default;
  XSqlDatabase(const XSqlDatabase& instance) = delete;

  QSqlDatabase db;
  bool status;
};

#endif // XSQLCONNECTOR_H
