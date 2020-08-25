#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QMap<QString, QStringList> Categories, QWidget* parent) : QDialog(parent),
                                                                                         _mUi(new Ui::SettingsDialog),
                                                                                         _mNewCategories(Categories)
{
    this->_mUi->setupUi(this);
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    this->setWindowFlags(this->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);

    this->_mMapping.insert("Recipes", "Rezepte");
    this->_mMapping.insert("Units", "Einheiten");
    this->_mMapping.insert("Ingredients", "Zutaten");

    for(auto Category: Categories.keys())
    {
        CategoryList* TabWidget = new CategoryList(this);
        TabWidget->addItems(Categories.value(Category, QStringList("")));

        connect(TabWidget, &CategoryList::itemAdded, this, &SettingsDialog::ItemAdded);
        connect(TabWidget, &CategoryList::itemRemoved, this, &SettingsDialog::ItemRemoved);

        this->_mUi->tabWidget_Settings->addTab(TabWidget, this->_mMapping.value(Category));
    }

    this->_mUi->buttonBox_Close->button(QDialogButtonBox::Save)->setText(tr("Speichern"));
    this->_mUi->buttonBox_Close->button(QDialogButtonBox::Discard)->setText(tr("Verwerfen"));
}

SettingsDialog::~SettingsDialog()
{
    delete this->_mUi;
}

QMap<QString, QStringList> SettingsDialog::categories() const
{
    return this->_mNewCategories;
}

void SettingsDialog::on_buttonBox_Close_clicked(QAbstractButton* button)
{
    if(button == this->_mUi->buttonBox_Close->button(QDialogButtonBox::Save))
    {
        this->accept();
    }
    else if(button == this->_mUi->buttonBox_Close->button(QDialogButtonBox::Discard))
    {
        this->reject();
    }
}

void SettingsDialog::ItemAdded(QString item)
{
    QString CurrentTab = this->_mMapping.key(this->_mUi->tabWidget_Settings->tabText(this->_mUi->tabWidget_Settings->currentIndex()));
    QStringList List = this->_mNewCategories.value(CurrentTab, QStringList(""));
    List.append(item);
    this->_mNewCategories.insert(CurrentTab, List);
}

void SettingsDialog::ItemRemoved(int index)
{
    QString CurrentTab = this->_mMapping.key(this->_mUi->tabWidget_Settings->tabText(this->_mUi->tabWidget_Settings->currentIndex()));
    QStringList List = this->_mNewCategories.value(CurrentTab, QStringList(""));
    List.removeAt(index);
    this->_mNewCategories.insert(CurrentTab, List);
}
