#include "pricelist.h"
#include "ui_pricelistwindow.h"

PriceList::PriceList(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::PriceListWindow)
{
  ui->setupUi(this);
  ui->tb_price_list->horizontalHeader()
                   ->setSectionResizeMode(QHeaderView::Stretch);
  setWindowTitle("PriceList");
}

PriceList::~PriceList() { delete ui; }

void PriceList::setTablePriceList(QSortFilterProxyModel* model) {
  ui->tb_price_list->setModel(model);
  ui->tb_price_list->setSortingEnabled(true);
}

void PriceList::showWindow() { this->show(); }
