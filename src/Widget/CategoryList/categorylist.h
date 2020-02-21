#ifndef CATEGORYLIST_H
#define CATEGORYLIST_H

#include <QDir>
#include <QWidget>
#include <QInputDialog>

namespace Ui
{
    class CategoryList;
}

class CategoryList : public QWidget
{
    Q_OBJECT

    signals:
        void itemRemoved(const int index);
        void itemAdded(const QString item);

    public:
        explicit CategoryList(QWidget* parent = nullptr);
        ~CategoryList();


        void addItems(const QStringList &labels);

    private slots:
        void on_pushButton_AddRecipeCategory_clicked();
        void on_pushButton_RemoveRecipeCategory_clicked();

    private:
        Ui::CategoryList* _mUi;
};

#endif // CATEGORYLIST_H
