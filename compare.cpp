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
  auto prm_it = cmp_item_map.begin();
  auto prm_qstrlist = prm_it->second;

  for (auto it = cmp_item_map.begin(); it != cmp_item_map.end(); it++) {
    QVBoxLayout* V_layout = new QVBoxLayout();
    QFrame* fr = new QFrame();
    QLabel* title_l = new QLabel(it->first);
    title_l->setStyleSheet("QLabel { color : red; text-decoration: underline; font: 14pt Kristen ITC; }");
    V_layout->addWidget(title_l);


    for (auto i = 0; i < it->second.count(); ++i) {
      QLabel* spec_l = new QLabel(it->second[i]);

      if (it->second[i] != prm_qstrlist[i])
        spec_l->setStyleSheet("QLabel { background-color : red; color : yellow; }");

      V_layout->addWidget(spec_l);
      fr->setFrameStyle(QFrame::StyledPanel);
      fr->setLayout(V_layout);
      m_pUi->horizontalLayout->addWidget(fr);
    }
  }

  show();
}

void CCompare::clearWidgets(QLayout* layout) {
  if (!layout)
    return;

  while(auto item = layout->takeAt(0)) {
    delete item->widget();
    clearWidgets(item->layout());
  }
}

void CCompare::closeEvent(QCloseEvent* event) {
  clearWidgets(m_pUi->horizontalLayout);
}
