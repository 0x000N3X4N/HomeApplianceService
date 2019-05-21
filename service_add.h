#ifndef PRICEADD_H
#define PRICEADD_H

#include <QDialog>
#include <QTableView>
#include <QMessageBox>
#include "libs/xSql/handle.h"

namespace Ui {
  class ServiceAddWindow;
}

class CServiceAdd : public QDialog
{
  Q_OBJECT

public:
  explicit CServiceAdd(QWidget *parent = nullptr);
  ~CServiceAdd();

public slots:
  void showWindow(QTableView* hTbPriceList);

private slots:
  void on_add_service_btn_clicked();

private:
  Ui::ServiceAddWindow* m_pUi;
  Handle* m_hQuery;
  QTableView* m_hTbPriceList;
  std::array<QString, 20> m_listOfEquipment {
    "MSI GS65 STEALTH",
    "ASUS ROG ZEPHYRUS GX501",
    "MSI GT75 TITAN",
    "ASUS ROG G703GI",
    "RAZER BLADE",
    "DELL XPS 15",
    "ASUS ROG STRIX GL502",
    "ACER PREDATOR HELIOS 300",
    "DELL G3 15",
    "GIGABYTE AERO 15",
    "ALIENWARRE 17 R5",
    "Lenovo Legion Y520",
    "ASUS VivoBook Pro 15 ",
    "Lenovo IdeaPad 310",
    "Lenovo ThinkPad X1 Carbon",
    "Lenovo ThinkPad P1",
    "Lenovo ThinkPad T480",
    "Lenovo Yoga Chromebook C630",
    "Lenovo Legion Y740",
    "Lenovo Legion Y7000"
  };
};

#endif // PRICEADD_H
