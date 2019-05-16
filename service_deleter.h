#ifndef SERVICE_DELETER_H
#define SERVICE_DELETER_H

#include <QDialog>
#include <QTableView>
#include <QMessageBox>
#include "libs/xSql/handle.h"

namespace Ui {
  class ServiceDeleterWindow;
}

class CServiceDeleter : public QDialog
{
  Q_OBJECT

public:
  explicit CServiceDeleter(QWidget *parent = nullptr);
  ~CServiceDeleter();

public slots:
  void showWindow(QTableView* hTbPriceList);

private slots:
  void on_accept_deleter_btn_clicked();

private:
  Ui::ServiceDeleterWindow* m_pUi;
  Handle* m_hQuery;
  QTableView* m_hTbPriceList;
};

#endif // SERVICE_DELETER_H
