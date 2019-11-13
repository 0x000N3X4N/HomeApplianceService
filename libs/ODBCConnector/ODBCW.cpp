#include "ODBCW.h"

CODBCW::CODBCW() {
  db = QSqlDatabase::addDatabase("QODBC");
  db.setDatabaseName("DRIVER={SQL Server};Server=DESKTOP-6RR6GV2;Database=PCSMDB;Uid=sa;Port=1433;Pwd=sa;");
  status = db.open();
  if (!status) {
    qDebug() << db.lastError();
    qFatal( "Failed to connect." );
  } else
    qDebug() << "Successfully connected to database!";
}

CODBCW& CODBCW::getInstance() {
  static CODBCW instance;
  return instance;
}

void CODBCW::close() {
  db.close();
}

QString CODBCW::databaseName() const {
  return db.databaseName();
}

bool CODBCW::getStatus() {
  return this->status;
}
