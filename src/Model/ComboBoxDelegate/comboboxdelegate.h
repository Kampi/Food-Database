#ifndef COMBOBOXDELEGATE_H_
#define COMBOBOXDELEGATE_H_

#include <QComboBox>
#include <QStyledItemDelegate>

class ComboBoxDelegate : public QStyledItemDelegate
{
    public:
        ComboBoxDelegate(QObject* parent = nullptr);
        QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
        void setEditorData(QWidget* editor, const QModelIndex& index) const override;
        void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;
        void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
        void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};

#endif // COMBOBOXDELEGATE_H_
