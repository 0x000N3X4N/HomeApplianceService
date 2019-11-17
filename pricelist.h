#ifndef PRICELISTWINDOW_H
#define PRICELISTWINDOW_H


#include <QDialog>
#include <QtSql>
#include <QMenu>
#include "comp_add.h"
#include "service_deleter.h"
#include "comp_type_add.h"


namespace Ui {
  class PriceListWindow;
}

class CPriceList : public QWidget {
  Q_OBJECT

public:
  explicit CPriceList(QWidget *parent = nullptr);
  ~CPriceList();

public slots:
  void setTablePriceList(QSortFilterProxyModel* );

signals:
  void showCompAdd(QTableView* hTbPriceList);
  void showServiceDeleter(QTableView* hTbPriceList);

  //TODO: something universal method to switch between widgets, like:
  //template<T>
  //void switchTo(T*)

private slots:
  void on_add_price_btn_clicked();
  void on_delete_btn_clicked();
  void on_up_btn_clicked();
  void on_down_btn_clicked();
  void on_left_btn_clicked();
  void on_right_btn_clicked();

private:
  Ui::PriceListWindow* m_pUi;
  QMenu* m_menu_ptr;
  CCompAdd* m_comp_add_ptr;
  CComp_TyAdd* m_CompTyAdd_ptr;
  CServiceDeleter* m_pServiceDeleter;
};


#endif // PRICELISTWINDOW_H
