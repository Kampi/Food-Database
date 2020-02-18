#include "comboboxdelegate.h"

ComboBoxDelegate::ComboBoxDelegate(QStringList  Categories, QObject* parent) : QStyledItemDelegate(parent),
                                                                               _mCategories(Categories)
{
}

QWidget* ComboBoxDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QComboBox* ComboBox = new QComboBox(parent);
    ComboBox->addItems(_mCategories);

    return ComboBox;
}

void ComboBoxDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    QComboBox* ComboBox = qobject_cast<QComboBox*>(editor);
    const QString CurrentText = index.data(Qt::EditRole).toString();
    const int Index = ComboBox->findText(CurrentText);
    if(Index >= 0)
    {
       ComboBox->setCurrentIndex(Index);
    }

   // QStyledItemDelegate::setEditorData(editor, index);
}

void ComboBoxDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    QComboBox* ComboBox = qobject_cast<QComboBox*>(editor);
    model->setData(index, ComboBox->currentText(), Qt::EditRole);
}

void ComboBoxDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyledItemDelegate::updateEditorGeometry(editor, option, index);
}

void ComboBoxDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyledItemDelegate::paint(painter, option, index);
}
