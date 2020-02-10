#ifndef RECIPESMODEL_H
#define RECIPESMODEL_H

#include <QAbstractTableModel>

#include "recipe.h"

class RecipesModel : public QAbstractTableModel
{
    enum RecipesModel_Table
    {
        RECIPESMODEL_TABLE_NAME,
        RECIPESMODEL_TABLE_PERSONS,
        RECIPESMODEL_TABLE_COOKINGTIME,
        RECIPESMODEL_TABLE_CATEGORY
    };

    public:
        RecipesModel(QList<Recipe>& Recipes, QObject* parent = nullptr);

        int rowCount(const QModelIndex& parent = QModelIndex()) const override;
        int columnCount(const QModelIndex& parent = QModelIndex()) const override;

        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
        bool setData(const QModelIndex &index, const QVariant& value, int role = Qt::EditRole) override;

        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

        Qt::ItemFlags flags(const QModelIndex& index) const override;

        bool removeRows(int position, int rows, const QModelIndex& index = QModelIndex()) override;

        void clear(void);

    private:
        QList<Recipe>& _mData;
};

#endif // RECIPESMODEL_H
