#ifndef NUMBERSONLYDELEGATE_H
#define NUMBERSONLYDELEGATE_H

#include <QLineEdit>
#include <QStyledItemDelegate>

class NumbersOnlyDelegate  : public QStyledItemDelegate
{
    Q_OBJECT

    public:
        NumbersOnlyDelegate(QValidator* Validator, QObject* parent = nullptr);
        QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
        void setEditorData(QWidget* editor, const QModelIndex& index) const override;
        void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;
        void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
        void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

    private:
        QValidator* _mValidator;
};

#endif // NUMBERSONLYDELEGATE_H
