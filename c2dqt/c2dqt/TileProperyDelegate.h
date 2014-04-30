#ifndef _TILE_PROPERTYDELEGATE_H_
#define _TILE_PROPERTYDELEGATE_H_
#include <QItemDelegate>
#include <QModelIndex>
#include <QObject>
#include <QSize>
#include <QComboBox>


class TileProperyDelegate: public QItemDelegate
{
    Q_OBJECT
public:
    TileProperyDelegate();
    TileProperyDelegate(QObject *parent);
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, 
                const QModelIndex &index) const;
};


#endif // _TILE_PROPERTYDELEGATE_H_

