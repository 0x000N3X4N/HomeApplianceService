#ifndef PRICELISTWINDOW_H
#define PRICELISTWINDOW_H


#include <QDialog>
#include <QtSql>
#include <QMenu>
#include "PCSM_base_types.h"
#include "libs/ODBCConnector/query_controller.h"
#include "comp_add.h"
#include "comp_deleter.h"
#include "comp_type_add.h"
#include "comp_type_deleter.h"


namespace Ui {
  class PriceListWindow;
}


class CPriceList : public QWidget {
  Q_OBJECT

public:
  explicit CPriceList(QWidget *parent = nullptr);
  ~CPriceList();

public slots:
  void setTablePriceList(QSortFilterProxyModel* hCompFModel,
                         QSortFilterProxyModel* hCompTypeFModel);

signals:
  void showCompAdd(QTableView* hTbPriceList, component_item_Ty_T& _map_comp_Ty_items);
  void showCompTypeAdd(QTableView* hTbCompType);
  void showCompDeleter(QTableView* hTbPriceList, component_itemT& _map_comp_items);
  void showCompTypeDeleter(QTableView* hTbCompType);

  //TODO: something universal method to switch between widgets, like:
  //template<T>
  //void switchTo(T*)

private slots:
  void on_up_btn_clicked();
  void on_down_btn_clicked();
  void on_left_btn_clicked();
  void on_right_btn_clicked();

private:
  Ui::PriceListWindow* m_pUi;
  CQueryController* m_hQuery;
  QMenu* m_pCompAddMenu;
  QMenu* m_pCompDelMenu;
  CCompAdd* m_comp_add_ptr;
  CComp_TyAdd* m_CompTyAdd_ptr;
  CComp_TyDeleter* m_CompTyDel_ptr;
  CCompDeleter* m_pServiceDeleter;
  component_item_Ty_T m_map_comp_Ty_items;
  component_itemT m_map_comp_items;
};


#endif // PRICELISTWINDOW_H
