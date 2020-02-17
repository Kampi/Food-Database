#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QPushButton>

namespace Ui
{
    class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit SettingsDialog(QWidget* parent = nullptr);
        ~SettingsDialog();

    private:
        Ui::SettingsDialog* _mUi;
};

#endif // SETTINGSDIALOG_H
