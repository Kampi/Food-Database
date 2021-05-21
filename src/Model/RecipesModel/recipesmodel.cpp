#include "recipesmodel.h"

RecipesModel::RecipesModel(QObject* parent, QSqlDatabase db) : QSqlTableModel(parent, db)
{
}

QVariant RecipesModel::data(const QModelIndex& index, int role) const
{
    if((!index.isValid()) || ((index.row() >= QSqlTableModel::rowCount()) || (index.row() < 0)))
    {
        return QVariant();
    }

    if(role == Qt::TextAlignmentRole)
    {
        if(index.column() == 1)
        {
            return QVariant(Qt::AlignLeft | Qt::AlignVCenter);
        }
        else
        {
            return Qt::AlignCenter;
        }
    }
    else if(role == Qt::UserRole)
    {
        return QVariant::fromValue(QSqlTableModel::record(index.row()));
    }

    return QSqlTableModel::data(index, role);
}

QVariant RecipesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
    {
        return QVariant();
    }

    if(orientation == Qt::Horizontal)
    {
        switch(section)
        {
            case 1:
            {
                return tr("Name");
            }
            case 4:
            {
                return tr("Personen");
            }
            case 6:
            {
                return tr("Kategorie");
            }
            case 7:
            {
                return tr("Zubereitungszeit");
            }
            default:
            {
                return QVariant();
            }
        }
    }
    else if(orientation == Qt::Vertical)
    {
        return section + 1;
    }

    return QVariant();
}

Qt::ItemFlags RecipesModel::flags(const QModelIndex& index) const
{
    Qt::ItemFlags Flags = QSqlTableModel::flags(index);

    return Flags;
}
