#include "ODBCW.h"


CODBCW::CODBCW() {
  try {
    QString host_qstr = QHostInfo::localHostName(), connection_qstr;
    db = QSqlDatabase::addDatabase("QODBC");
    connection_qstr = QString("DRIVER={SQL Server};Server=%1;Database=PCSMDB;Uid=sa;Port=1433;Pwd=sa;")
                      .arg(host_qstr);

    db.setDatabaseName(connection_qstr);
    status = db.open();

    if (!status) {
      //TODO: debug info macro
      qDebug() << db.lastError();
      qFatal( "Failed to connect." );
    } else
      qDebug() << "Successfully connected to database!";
  }
  catch(...) {
    //TODO: error handling
//    QMessageBox::critical(this, "Error!", "CODBCW::CODBCW : Unexpected error!");
  }
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
