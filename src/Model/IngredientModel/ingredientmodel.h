#ifndef INGREDIENTMODEL_H
#define INGREDIENTMODEL_H

#include <QSize>
#include <QAbstractTableModel>

#include "ingredient.h"

#ifdef QT_DEBUG
    #include <QtDebug>
#endif

class IngredientModel : public QAbstractTableModel
{
    Q_OBJECT

    public:
        enum IngredientsModel_Table
        {
            INGREDIENTSMODEL_TABLE_NAME,
            INGREDIENTSMODEL_TABLE_AMOUNT,
            INGREDIENTSMODEL_TABLE_UNIT,
            INGREDIENTSMODEL_TABLE_PRICE,
            INGREDIENTSMODEL_TABLE_SECTION,
            INGREDIENTSMODEL_TABLE_NOTE
        };

    public:
        IngredientModel(QObject* parent = nullptr);

        int rowCount(const QModelIndex& parent = QModelIndex()) const override;
        int columnCount(const QModelIndex& parent = QModelIndex()) const override;

        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
        bool setData(const QModelIndex &index, const QVariant& value, int role = Qt::EditRole) override;

        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

        Qt::ItemFlags flags(const QModelIndex& index) const override;

        bool removeRows(int position, int rows, const QModelIndex& index = QModelIndex()) override;

    private:
        QList<Ingredient> _mData;
};

#endif // INGREDIENTMODEL_H
