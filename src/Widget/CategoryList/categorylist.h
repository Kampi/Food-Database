#ifndef CATEGORYLIST_H
#define CATEGORYLIST_H

#include <QDir>
#include <QWidget>
#include <QInputDialog>

#ifdef QT_DEBUG
    #include <QDebug>
#endif

namespace Ui
{
    class CategoryList;
}

class CategoryList : public QWidget
{
    Q_OBJECT

    signals:
        void itemRemoved(const int Index);
        void itemAdded(const QString Item);

    public:
        explicit CategoryList(QWidget* parent = nullptr);
        ~CategoryList();

        void addItems(const QStringList& Labels);

    private slots:
        void on_pushButton_AddRecipeCategory_clicked(void);
        void on_pushButton_RemoveRecipeCategory_clicked(void);

    private:
        Ui::CategoryList* _mUi;
};

#endif // CATEGORYLIST_H
