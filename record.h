#ifndef RECORD_H
#define RECORD_H

#include <QDialog>
#include <QDebug>
#include <QTableView>
#include <memory>
#include "libs/xSql/handle.h"

#pragma region CFG
  #define LISTSIZE 8
  #define OVERHAUL 20
#pragma endregion

namespace Ui {
  class RecordWindow;
}

class Record : public QDialog
{
  Q_OBJECT

public:
  explicit Record(QWidget* parent = nullptr);
  ~Record();

public slots:
  void showWindow();
  void priceListChanged(QTableView* pTbOrders, Handle* hQuery,
                        std::shared_ptr<double[]> priceList,
                        size_t m_sizeOfPriceList);

private slots:
  void on_name_comboBox_currentIndexChanged(int index);
  void on_type_repair_comboBox_currentIndexChanged(int index);
  void on_accept_btn_clicked();
  void on_status_comboBox_currentIndexChanged(int index);

private:
  Ui::RecordWindow* m_pUi;
  Handle* m_hQuery;
  struct priceListHandler;
  priceListHandler* m_pPriceListHandler;
  std::map<std::array<QString, LISTSIZE>,
           std::shared_ptr<double[]>> m_priceListStructure;
  std::map<std::array<QString, LISTSIZE>,
           std::shared_ptr<double[]>>::iterator m_map_it;
  QTableView* m_pTbOrders;
  double m_total;
  bool bIsOverhaul;
};

#endif // RECORD_H
