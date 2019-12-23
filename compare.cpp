#include "compare.h"
#include "ui_compare.h"


CCompare::CCompare(QWidget *parent) :
  QWidget(parent),
  m_pUi(new Ui::CompareWindow)
{
  m_pUi->setupUi(this);
}

CCompare::~CCompare()
{
  delete m_pUi;
}

void CCompare::showWindow(std::map<QString, QStringList> cmp_item_map) {
  for (auto it = cmp_item_map.begin(); it != cmp_item_map.end(); it++) {
    QVBoxLayout* V_layout = new QVBoxLayout();
    QLabel* l = new QLabel("test");
    V_layout->addWidget(l);
    m_pUi->horizontalLayout->addLayout(V_layout);
  }

  show();
}
