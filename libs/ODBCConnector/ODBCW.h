#ifndef ODBCW_H
#define ODBCW_H


#include <QtSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QMessageBox>
#include <QDebug>
#include <QHostInfo>
#include <QApplication>
#include <QLabel>
#include "PCSM_msg_box.h"
#include "boost/noncopyable.hpp"


/* ------------------------------------------------------------------
 * Simple ODBC Wrapper, based on Singleton pattern
 * ------------------------------------------------------------------
 */

class CODBCW : public boost::noncopyable {
public:
  static CODBCW& getInstance(QString host_qstr, QLabel* status_l, std::size_t* isOpen);
  QString databaseName() const;
  void close();
  bool getStatus();
private:
  CODBCW(QString host_qstr, QLabel* status_l, std::size_t* isOpen);
  ~CODBCW() = default;

  QSqlDatabase db;
  bool status;
};

#endif // ODBCW_H_H
