#ifndef RECIPESPROXY_H
#define RECIPESPROXY_H

#include <QSortFilterProxyModel>

#ifdef QT_DEBUG
    #include <QtDebug>
#endif

class RecipesProxy : public QSortFilterProxyModel
{
    Q_OBJECT

    public slots:
        void setName(QString Name);

    public:
        RecipesProxy(QObject* parent = nullptr);
        bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const;
        QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    private:
        QString _mName;
};

#endif // RECIPESPROXY_H
