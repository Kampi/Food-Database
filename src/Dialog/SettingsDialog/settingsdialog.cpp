#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget* parent) : QDialog(parent), _mUi(new Ui::SettingsDialog)
{
    _mUi->setupUi(this);
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    this->setWindowFlags(this->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);

    //_mUi->buttonBox_Close->button(QDialogButtonBox::Save)->setText(tr("Speichern"));
    //_mUi->buttonBox_Close->button(QDialogButtonBox::Discard)->setText(tr("Verwerfen"));
}

SettingsDialog::~SettingsDialog()
{
    delete _mUi;
}
