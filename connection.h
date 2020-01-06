#ifndef CONNECTION_H
#define CONNECTION_H


#include <QDialog>
#include "mainw.h"
#include "libs/ODBCConnector/query_controller.h"


namespace Ui {
  class ConnectionWindow;
}

class CConnection : public QDialog {
  Q_OBJECT

public:
  explicit CConnection(QWidget *parent = nullptr);
  ~CConnection();

private slots:
  void on_accept_btn_clicked();

signals:
  void showMainWindow();

private:
  Ui::ConnectionWindow* m_pUi;
  CQueryController* m_hQuery;
  MainWindow* m_hMain;
};

#endif // CONNECTION_H
