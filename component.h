#ifndef COMPONENT_H
#define COMPONENT_H


#include <QObject>
#include <QString>
#include <vector>
#include <memory>


template <typename NameT = QString, typename IdT = size_t>
struct comp;

class CCompsTraits;

typedef std::vector<std::shared_ptr<comp<>>> compsT;


template <typename NameT, typename IdT>
struct comp : public QObject {
  IdT       get_id()     { return id;   }
  NameT     get_name()   { return name; }
protected:
  NameT name;
  IdT id;
};


class CCompsTraits {

};


class CComponents : public comp<> {
public:
  CComponents();

  compsT* get()              { return &comps;  }
  void    set(compsT _comps) { comps = _comps; }

protected:
  static compsT comps;
};


#endif // COMPONENT_H
