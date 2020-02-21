#include "ingredientmodel.h"

IngredientModel::IngredientModel(QList<Ingredient>& Ingredients, QObject* parent) : QAbstractTableModel(parent),
                                                                                    _mData(Ingredients)
{
}

int IngredientModel::rowCount(const QModelIndex&) const
{
    return _mData.size();
}

int IngredientModel::columnCount(const QModelIndex&) const
{
    return 6;
}

QVariant IngredientModel::data(const QModelIndex& index, int role) const
{
    if((role == Qt::DisplayRole) || (role == Qt::EditRole))
    {
        QLocale locale = QLocale().system();

        switch(index.column())
        {
            case INGREDIENTSMODEL_TABLE_NAME:
            {
                return QString("%1").arg(_mData.at(index.row()).Name());
            }
            case INGREDIENTSMODEL_TABLE_AMOUNT:
            {
                return QString("%1").arg(locale.toString(_mData.at(index.row()).Amount()));
            }
            case INGREDIENTSMODEL_TABLE_UNIT:
            {
                return QString("%1").arg(_mData.at(index.row()).Unit());
            }
            case INGREDIENTSMODEL_TABLE_PRICE:
            {
                return QString("%1 " + tr("€")).arg(locale.toString(_mData.at(index.row()).Price()));
            }
            case INGREDIENTSMODEL_TABLE_SECTION:
            {
                return QString("%1").arg(_mData.at(index.row()).Section());
            }
            case INGREDIENTSMODEL_TABLE_NOTE:
            {
                return QString("%1").arg(_mData.at(index.row()).Note());
            }
            default:
            {
                return QString("");
            }
        }
    }

    return QVariant();
}

bool IngredientModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    bool Result = true;

    if(role == Qt::EditRole)
    {
        QLocale locale = QLocale().system();

        switch(index.column())
        {
            case INGREDIENTSMODEL_TABLE_NAME:
            {
                _mData[index.row()].setName(value.toString());
                Result = true;
                break;
            }
            case INGREDIENTSMODEL_TABLE_AMOUNT:
            {
                _mData[index.row()].setAmount(locale.toDouble(value.toString()));
                Result = true;
                break;
            }
            case INGREDIENTSMODEL_TABLE_UNIT:
            {
                _mData[index.row()].setUnit(value.toString());
                Result = true;
                break;
            }
            case INGREDIENTSMODEL_TABLE_PRICE:
            {
                _mData[index.row()].setPrice(locale.toDouble(value.toString()));
                Result = true;
                break;
            }
            case INGREDIENTSMODEL_TABLE_SECTION:
            {
                _mData[index.row()].setSection(value.toString());
                Result = true;
                break;
            }
            case INGREDIENTSMODEL_TABLE_NOTE:
            {
                _mData[index.row()].setNote(value.toString());
                Result = true;
                break;
            }
            default:
            {
                Result = false;
                break;
            }
        }
    }

    emit QAbstractItemModel::dataChanged(index, index);

    return Result;
}

QVariant IngredientModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal)
    {
        if(role == Qt::DisplayRole)
        {
            switch(section)
            {
                case INGREDIENTSMODEL_TABLE_NAME:
                {
                    return tr("Name");
                }
                case INGREDIENTSMODEL_TABLE_AMOUNT:
                {
                    return tr("Menge");
                }
                case INGREDIENTSMODEL_TABLE_UNIT:
                {
                    return tr("Einheit");
                }
                case INGREDIENTSMODEL_TABLE_PRICE:
                {
                    return tr("Preis");
                }
                case INGREDIENTSMODEL_TABLE_SECTION:
                {
                    return tr("Kategorie");
                }
                case INGREDIENTSMODEL_TABLE_NOTE:
                {
                    return tr("Notizen");
                }
                default:
                {
                    return QVariant();
                }
            }
        }/*
        else if(role == Qt::SizeHintRole)
        {
            if(section == INGREDIENTSMODEL_TABLE_AMOUNT)
            {
                QSize size(20, 50);
                return QVariant(size);
            }
            else
            {
                QSize size(100, 50);
                return QVariant(size);
            }
        }*/
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

Qt::ItemFlags IngredientModel::flags(const QModelIndex& index) const
{
    Qt::ItemFlags Flags = Qt::ItemIsEditable | QAbstractTableModel::flags(index);

    return Flags;
}

bool IngredientModel::removeRows(int position, int rows, const QModelIndex& parent)
{
    beginRemoveRows(parent, position, position + rows - 1);

    for(int row = 0; row < rows; ++row)
    {
        _mData.removeAt(position);
    }

    endRemoveRows();

    return true;
}
