#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget* parent) : QDialog(parent), _mUi(new Ui::AboutDialog)
{
    _mUi->setupUi(this);
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    this->setWindowFlags(this->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);

    _mUi->textEdit_About->setText(QString("Food Database v%1.%2.%3 \n"
                                          "Author: Daniel Kampert \n"
                                          "Website: https://www.kampis-elektroecke.de"
                                          ).arg(VERSION_MAJOR).arg(VERSION_MINOR).arg(VERSION_BUILD));
}

AboutDialog::~AboutDialog()
{
    delete _mUi;
}
