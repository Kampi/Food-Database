#include "recipesmodel.h"

RecipesModel::RecipesModel(QList<Recipe>& Recipes, QObject* parent) : QAbstractTableModel(parent), _mData(Recipes)
{
}

int RecipesModel::rowCount(const QModelIndex&) const
{
    return _mData.size();
}

int RecipesModel::columnCount(const QModelIndex&) const
{
    return 4;
}

QVariant RecipesModel::data(const QModelIndex& index, int role) const
{
    if(role == Qt::DisplayRole)
    {
        switch(index.column())
        {
            case RECIPESMODEL_TABLE_NAME:
            {
                return QString("%1").arg(_mData.at(index.row()).Name());
            }
            case RECIPESMODEL_TABLE_PERSONS:
            {
                return QString("%1").arg(_mData.at(index.row()).Persons());
            }
            case RECIPESMODEL_TABLE_COOKINGTIME:
            {
                return QString("%1 " + tr("Minuten")).arg(_mData.at(index.row()).CookingTime());
            }
            case RECIPESMODEL_TABLE_CATEGORY:
            {
                return QString("%1").arg(_mData.at(index.row()).Category());
            }
            default:
            {
                return QVariant();
            }
        }
    }

    return QVariant();
}

bool RecipesModel::setData(const QModelIndex& index, const QVariant&, int role)
{
    if(role == Qt::EditRole)
    {
        QLocale locale = QLocale().system();
    }

    emit QAbstractItemModel::dataChanged(index, index);

    return true;
}

QVariant RecipesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal)
    {
        if(role == Qt::DisplayRole)
        {
            switch(section)
            {
                case RECIPESMODEL_TABLE_NAME:
                {
                    return tr("Name");
                }
                case RECIPESMODEL_TABLE_PERSONS:
                {
                    return tr("Personen");
                }
                case RECIPESMODEL_TABLE_COOKINGTIME:
                {
                    return tr("Zubereitungszeit");
                }
                case RECIPESMODEL_TABLE_CATEGORY:
                {
                    return tr("Kategorie");
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
    }
    else if(orientation == Qt::Vertical)
    {
        if(role == Qt::DisplayRole)
        {
            return section + 1;
        }
    }

    return QVariant();
}

Qt::ItemFlags RecipesModel::flags(const QModelIndex& index) const
{
    Qt::ItemFlags Flags = QAbstractTableModel::flags(index);

    return Flags;
}

bool RecipesModel::removeRows(int position, int rows, const QModelIndex& parent)
{
    beginRemoveRows(parent, position, position + rows - 1);

    for(int row = 0; row < rows; ++row)
    {
        _mData.removeAt(position);
    }

    endRemoveRows();

    return true;
}

void RecipesModel::clear(void)
{
    beginResetModel();
    endResetModel();
}
