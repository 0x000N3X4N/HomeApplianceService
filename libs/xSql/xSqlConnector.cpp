#include "xSqlConnector.h"

XSqlDatabase::XSqlDatabase() {
  db = QSqlDatabase::addDatabase("QMYSQL");
  db.setHostName("localhost");
  db.setUserName("nativeuser");
  db.setPassword("new_password");
  db.setDatabaseName("home_appliance_service");
  status = db.open();
  if (!status) {
    qDebug() << db.lastError();
    qFatal( "Failed to connect." );
  } else
    qDebug() << "Successfully connected to database!";
}

XSqlDatabase& XSqlDatabase::getInstance() {
  static XSqlDatabase instance;
  return instance;
}

void XSqlDatabase::close() {
  db.close();
}

QString XSqlDatabase::databaseName() const {
  return db.databaseName();
}

bool XSqlDatabase::getStatus() {
  return this->status;
}
