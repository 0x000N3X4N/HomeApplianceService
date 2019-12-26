#include "ODBCW.h"


CODBCW::CODBCW(QString host_qstr, QLabel* status_l, std::size_t* isOpen) {
  *isOpen = 0;

  try {
    QString connection_qstr;
    db = QSqlDatabase::addDatabase("QODBC");

    QString db_path = QDir::toNativeSeparators(QDir::currentPath()) + "\\DB\\PCSMDB.mdf";
    qDebug() << db_path;
    QString connStr = "DRIVER={SQL Server};SERVER=%1;AttachDbFilename=%2;Database=PCSMDB;Trusted_Connection=Yes;";
    db.setDatabaseName(connStr.arg(host_qstr).arg(db_path));

    status_l->setStyleSheet("color: rgb(255, 255, 0);");
    status_l->setText("connecting...");
    //TODO: multithreading
    status = db.open();

    if (!status) {
      status_l->setStyleSheet("color: rgb(255, 0, 0);");
      CMsgBox::show(QMessageBox::Critical, nullptr, "Connection error!", db.lastError().text());
      exit(-1);
    } else {
      *isOpen = 1;
      status_l->setStyleSheet("color: rgb(0, 255, 0);");
      status_l->setText("Success!");
      qDebug() << "Successfully connected to database!";
    }
  }
  catch(...) {
    CMsgBox::show(QMessageBox::Critical, nullptr, "Error!", "CODBCW::CODBCW : Unexpected error!");
  }
}

CODBCW& CODBCW::getInstance(QString host_qstr, QLabel* status_l, std::size_t* isOpen) {
  static CODBCW instance(host_qstr, status_l, isOpen);
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
