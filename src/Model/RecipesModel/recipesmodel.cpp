#include "recipesmodel.h"

RecipesModel::RecipesModel(QObject* parent) : QAbstractTableModel(parent)
{
}

int RecipesModel::rowCount(const QModelIndex&) const
{
    return this->_mData.size();
}

int RecipesModel::columnCount(const QModelIndex&) const
{
    return 4;
}

QVariant RecipesModel::data(const QModelIndex& index, int role) const
{
    if((!index.isValid()) || ((index.row() >= this->_mData.size()) || (index.row() < 0)))
    {
        return QVariant();
    }

    if(role == Qt::DisplayRole)
    {
        switch(index.column())
        {
            case RECIPESMODEL_TABLE_NAME:
            {
                return QString("%1").arg(this->_mData.at(index.row()).Name());
            }
            case RECIPESMODEL_TABLE_PERSONS:
            {
                return QString("%1").arg(this->_mData.at(index.row()).Persons());
            }
            case RECIPESMODEL_TABLE_COOKINGTIME:
            {
                return QString("%1 " + tr("Minuten")).arg(this->_mData.at(index.row()).CookingTime());
            }
            case RECIPESMODEL_TABLE_CATEGORY:
            {
                return QString("%1").arg(this->_mData.at(index.row()).Category());
            }
            default:
            {
                return QVariant();
            }
        }
    }
    else if(role == Qt::TextAlignmentRole)
    {
        if(index.column() == RECIPESMODEL_TABLE_NAME)
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
        return QVariant::fromValue<Recipe>(this->_mData.at(index.row()));
    }

    return QVariant();
}

bool RecipesModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if((!index.isValid()) || (index.row() < 0) || (index.row() >= this->_mData.size()))
    {
        return false;
    }

    if(role == Qt::EditRole)
    {
        this->_mData.replace(index.row(), qvariant_cast<Recipe>(value));
    }

    return true;
}

QVariant RecipesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
    {
        return QVariant();
    }

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

bool RecipesModel::insertRows(int position, int rows, const QModelIndex& parent)
{
    beginInsertRows(parent, position, position + rows - 1);

    for(int row = 0; row < rows; row++)
    {
        this->_mData.insert(position, Recipe());
    }

    endInsertRows();
    return true;
}

bool RecipesModel::removeRows(int position, int rows, const QModelIndex& parent)
{
    beginRemoveRows(parent, position, position + rows - 1);

    for(int row = 0; row < rows; ++row)
    {
        this->_mData.removeAt(position);
    }

    endRemoveRows();

    return true;
}

void RecipesModel::clear(void)
{
    beginResetModel();
    endResetModel();
}
