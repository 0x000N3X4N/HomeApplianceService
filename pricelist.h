#ifndef PRICELISTWINDOW_H
#define PRICELISTWINDOW_H


#include <QDialog>
#include <QtSql>
#include "service_add.h"
#include "service_deleter.h"


namespace Ui {
  class PriceListWindow;
}

class CPriceList : public QDialog {
  Q_OBJECT

public:
  explicit CPriceList(QWidget *parent = nullptr);
  ~CPriceList();

public slots:
  void setTablePriceList(QSortFilterProxyModel* );

signals:
  void showServiceAdd(QTableView* hTbPriceList);
  void showServiceDeleter(QTableView* hTbPriceList);

private slots:
  void on_add_price_btn_clicked();
  void on_delete_btn_clicked();

private:
  Ui::PriceListWindow* m_pUi;
  CServiceAdd* m_pServiceAdd;
  CServiceDeleter* m_pServiceDeleter;
};


#endif // PRICELISTWINDOW_H
