#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QMap<QString, QStringList> Categories, QWidget* parent) : QDialog(parent),
                                                                                         _mUi(new Ui::SettingsDialog),
                                                                                         _mNewCategories(Categories)
{
    _mUi->setupUi(this);
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    this->setWindowFlags(this->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);

    for(auto category: _mNewCategories.keys())
    {
        CategoryList* TabWidget = new CategoryList(this);
        TabWidget->addItems(_mNewCategories.value(category));

        connect(TabWidget, &CategoryList::itemAdded, this, &SettingsDialog::on_tab_ItemAdded);
        connect(TabWidget, &CategoryList::itemRemoved, this, &SettingsDialog::on_tab_ItemRemoved);

        _mUi->tabWidget_Settings->addTab(TabWidget, category);
    }

    _mUi->buttonBox_Close->button(QDialogButtonBox::Save)->setText(tr("Speichern"));
    _mUi->buttonBox_Close->button(QDialogButtonBox::Discard)->setText(tr("Verwerfen"));
}

SettingsDialog::~SettingsDialog()
{
    delete _mUi;
}

QMap<QString, QStringList> SettingsDialog::categories() const
{
    return _mNewCategories;
}

void SettingsDialog::on_buttonBox_Close_clicked(QAbstractButton* button)
{
    if(button == _mUi->buttonBox_Close->button(QDialogButtonBox::Save))
    {
        this->accept();
    }
    else if(button == _mUi->buttonBox_Close->button(QDialogButtonBox::Discard))
    {
        this->reject();
    }
}

void SettingsDialog::on_tab_ItemAdded(QString item)
{
    QString CurrentTab = _mUi->tabWidget_Settings->tabText(_mUi->tabWidget_Settings->currentIndex());
    QStringList List = _mNewCategories.value(CurrentTab);
    List.append(item);
    _mNewCategories.insert(CurrentTab, List);
}

void SettingsDialog::on_tab_ItemRemoved(int index)
{
    QString CurrentTab = _mUi->tabWidget_Settings->tabText(_mUi->tabWidget_Settings->currentIndex());
    QStringList List = _mNewCategories.value(CurrentTab);
    List.removeAt(index);
    _mNewCategories.insert(CurrentTab, List);
}
