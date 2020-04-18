#include "categorylist.h"
#include "ui_categorylist.h"

CategoryList::CategoryList(QWidget* parent) : QWidget(parent),
                                              _mUi(new Ui::CategoryList)
{
    _mUi->setupUi(this);
}

CategoryList::~CategoryList()
{
    delete _mUi;
}

void CategoryList::addItems(const QStringList &labels)
{
    _mUi->listWidget_RecipeCategories->addItems(labels);
}

void CategoryList::on_pushButton_AddRecipeCategory_clicked()
{
    QInputDialog* Dialog = new QInputDialog(this, Qt::MSWindowsFixedSizeDialogHint | Qt::CustomizeWindowHint);
    Dialog->setOkButtonText(tr("Speichern"));
    Dialog->setCancelButtonText(tr("Verwerfen"));
    Dialog->setLabelText(tr("Kategorie hinzufügen:"));
    Dialog->setTextEchoMode(QLineEdit::Normal);
    int Status = Dialog->exec();

    QString Text = Dialog->textValue();
    if(Status && !Text.isEmpty())
    {
        _mUi->listWidget_RecipeCategories->addItem(Text);
        emit itemAdded(Text);
    }
}

void CategoryList::on_pushButton_RemoveRecipeCategory_clicked()
{
    int Index = _mUi->listWidget_RecipeCategories->currentRow();
    delete _mUi->listWidget_RecipeCategories->takeItem(Index);

    emit itemRemoved(Index);
}
