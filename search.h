#ifndef SEARCH_H
#define SEARCH_H


#include <QWidget>
#include <QStack>


namespace Ui {
  class SearchWindow;
}

enum QT {
  NotFound,
  Param,
  Wildcard
};

namespace QueryParamsKeywords {
  constexpr auto PRICE = "price";
  constexpr auto YEAR = "year";
}

namespace QPK = QueryParamsKeywords;

struct Params {
  QString price;
  QString year;
};

struct QueryLexer {
  static std::tuple<std::size_t, std::size_t, std::size_t> exec(QString& query) {

  }
};

struct QueryTyper {
  QString extractBrackets(QString& in) {
      QString backets_str;

      for (auto it = in.begin(); it != in.end(); it++)
          if (*it == '[' || *it == ']') backets_str.push_back(*it);

      return backets_str;
  }

  bool isBalanced(QString& brackets_str) {
      QStack<QChar> s;
      QChar c;

      for (auto it = brackets_str.begin(); it != brackets_str.end(); it++)
      {
          if (*it == '[')
              s.push(*it);
          else if (s.empty()) return false;

          if (*it == ']') {
              c = s.top();

              s.pop();
              continue;
          }
      }

      return s.empty();
  }

  QT extract(QString& query) {

  }
};


class CSearch : public QWidget {
  Q_OBJECT

public:
  explicit CSearch(QWidget *parent = nullptr);
  ~CSearch();

private slots:
  void on_search_btn_clicked();

private:
  Ui::SearchWindow* m_pUi;
};


#endif // SEARCH_H
