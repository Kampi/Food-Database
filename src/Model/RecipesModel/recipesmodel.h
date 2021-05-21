#ifndef RECIPESMODEL_H
#define RECIPESMODEL_H

#include <QSqlRecord>
#include <QSqlTableModel>

#ifdef QT_DEBUG
    #include <QtDebug>
#endif

class RecipesModel : public QSqlTableModel
{
    Q_OBJECT

    public:
        RecipesModel(QObject* parent = nullptr, QSqlDatabase db = QSqlDatabase());

        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
        Qt::ItemFlags flags(const QModelIndex& index) const override;

    private:
};

Q_DECLARE_METATYPE(QSqlRecord)

#endif // RECIPESMODEL_H
