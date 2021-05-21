#include "recipesproxy.h"

RecipesProxy::RecipesProxy(QObject* parent) : QSortFilterProxyModel(parent)
{
}

bool RecipesProxy::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const
{
    QModelIndex Index;

    // Display everything when no name is set
    if(!this->_mName.length())
    {
        return true;
    }

    Index = this->sourceModel()->index(sourceRow, 1, sourceParent);

    // Check if the name of the recipe contains the substring
    return this->sourceModel()->data(Index).toString().contains(this->_mName, Qt::CaseInsensitive);
}

QVariant RecipesProxy::headerData(int section, Qt::Orientation orientation, int role) const
{
    return this->sourceModel()->headerData(section, orientation, role);
}

void RecipesProxy::setName(QString Name)
{
    this->_mName = Name;
    this->invalidateFilter();
}
