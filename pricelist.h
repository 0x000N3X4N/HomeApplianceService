#ifndef PRICELISTWINDOW_H
#define PRICELISTWINDOW_H


#include <QDialog>
#include <QtSql>
#include <QMenu>
#include "component.h"
#include "PCSM_base_types.h"
#include "libs/ODBCConnector/query_controller.h"
#include "comp_add.h"
#include "comp_deleter.h"
#include "comp_type_add.h"
#include "comp_type_deleter.h"
#include "PCSM_msg_box.h"


namespace Ui {
  class PriceListWindow;
}


class CPriceList : public QWidget, public PCOM::CCompsTraits {
  Q_OBJECT

public:
  explicit CPriceList(QWidget *parent = nullptr);
  CComp_TyDeleter* getCompTypeDel() const;
  CCompDeleter* getCompDel() const;
  ~CPriceList();

public slots:
  void setTablePriceList(QSortFilterProxyModel* hCompFModel,
                         QSortFilterProxyModel* hCompTypeFModel);

signals:
  void showCompAdd(QTableView* hTbPriceList);
  void showCompTypeAdd(QTableView* hTbCompType);
  void showCompDeleter(QTableView* hTbPriceList);
  void showCompTypeDeleter(QTableView* hTbCompType, QTableView* hTbComps);

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
  CCompDeleter* m_pCompDeleter;
};


#endif // PRICELISTWINDOW_H
