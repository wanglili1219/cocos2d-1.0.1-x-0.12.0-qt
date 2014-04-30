#include "TileProperyDelegate.h"

TileProperyDelegate::TileProperyDelegate()
{

}

TileProperyDelegate::TileProperyDelegate(QObject *parent)
{

}

QWidget *TileProperyDelegate::createEditor(QWidget *parent,
                                       const QStyleOptionViewItem &,
                                       const QModelIndex & mindex) const
{
	if (mindex.row() == 2 && mindex.column() == 1){
        QComboBox *cbo = new QComboBox(parent);
        cbo->addItem(" 1 ");
        cbo->addItem(" 2 ");
        cbo->addItem(" 3 ");
        cbo->installEventFilter(const_cast<TileProperyDelegate*>(this));
        return cbo;
	}

	return 0;
}

void TileProperyDelegate::setEditorData(QWidget *editor,const QModelIndex &index) const
{
    QComboBox *cbo = static_cast<QComboBox*>(editor);
    cbo->setCurrentIndex(0);
}

void TileProperyDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    QString tmpstr;
    QComboBox *cbo = static_cast<QComboBox*>(editor);
    tmpstr.setNum(cbo->currentIndex());
    model->setData(index, tmpstr);
}
void TileProperyDelegate::updateEditorGeometry(QWidget *editor,
                                           const QStyleOptionViewItem &option, const QModelIndex &) const
{
    editor->setGeometry(option.rect);
}


