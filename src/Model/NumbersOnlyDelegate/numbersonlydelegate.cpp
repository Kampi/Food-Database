#include "numbersonlydelegate.h"

NumbersOnlyDelegate::NumbersOnlyDelegate(QValidator* Validator, QObject* parent) : QStyledItemDelegate(parent),
                                                                                   _mValidator(Validator)
{
}

QWidget* NumbersOnlyDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QWidget* Editor = QStyledItemDelegate::createEditor(parent, option, index);
    QLineEdit* LineEdit = qobject_cast<QLineEdit*>(Editor);
    if(LineEdit)
    {
        LineEdit->setValidator(_mValidator);
    }

    return Editor;
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
