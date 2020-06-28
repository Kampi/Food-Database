#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QFile>
#include <QDialog>
#include <QFileInfo>
#include <QTextStream>
#include <QPushButton>
#include <QDesktopServices>

namespace Ui
{
    class AboutDialog;
}

class AboutDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit AboutDialog(QWidget* parent = nullptr);
        ~AboutDialog();

    private:
        Ui::AboutDialog* _mUi;
};

#endif // ABOUTDIALOG_H
