#include "numbersonlydelegate.h"

NumbersOnlyDelegate::NumbersOnlyDelegate(QValidator* Validator, QObject* parent) : QStyledItemDelegate(parent),
                                                                                   _mValidator(Validator)
{

}

QWidget* NumbersOnlyDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QWidget* editor = QStyledItemDelegate::createEditor(parent, option, index);
    QLineEdit* lineEditEditor = qobject_cast<QLineEdit*>(editor);
    if(lineEditEditor)
    {
        lineEditEditor->setValidator(_mValidator);
    }

     return editor;
}

void NumbersOnlyDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    QStyledItemDelegate::setEditorData(editor, index);
}

void NumbersOnlyDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    QStyledItemDelegate::setModelData(editor, model, index);
}

void NumbersOnlyDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyledItemDelegate::updateEditorGeometry(editor, option, index);
}

void NumbersOnlyDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyledItemDelegate::paint(painter, option, index);
}
