#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QMap>
#include <QDialog>
#include <QSettings>
#include <QStringList>
#include <QPushButton>

#include "categorylist.h"

namespace Ui
{
    class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit SettingsDialog(QMap<QString, QStringList> Categories, QWidget* parent = nullptr);
        ~SettingsDialog();

        QMap<QString, QStringList> categories() const;

    private slots:
        void on_buttonBox_Close_clicked(QAbstractButton* button);
        void ItemAdded(QString item);
        void ItemRemoved(int index);

    private:
        Ui::SettingsDialog* _mUi;

        QMap<QString, QStringList> _mNewCategories;
        QMap<QString, QString> _mMapping;

        CategoryList* _mRecipeCategories;
};

#endif // SETTINGSDIALOG_H
