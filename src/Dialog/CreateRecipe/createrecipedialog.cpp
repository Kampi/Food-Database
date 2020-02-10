#include "createrecipedialog.h"
#include "ui_createrecipedialog.h"

CreateRecipeDialog::CreateRecipeDialog(QList<Recipe>& Recipes, QWidget* parent) :   QDialog(parent),
                                                                                    _mUi(new Ui::CreateRecipe),
                                                                                    _mIngredientsModel(new IngredientModel(_mIngredientsList)),
                                                                                    _mRecipes(Recipes)
{
    _mUi->setupUi(this);
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    this->setWindowFlags(this->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);

    _mUi->tableView_Ingredients->setModel(_mIngredientsModel);
    _mUi->tableView_Ingredients->horizontalHeader()->setStretchLastSection(true);
    _mUi->tableView_Ingredients->horizontalHeader()->setSectionsClickable(false);
    //_mUi->tableView_Ingredients->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    _mUi->buttonBox->button(QDialogButtonBox::Save)->setText(tr("Speichern"));
    _mUi->buttonBox->button(QDialogButtonBox::Discard)->setText(tr("Verwerfen"));

    _mUi->spinBox_CookingTime->setSuffix(" " + tr("Minuten"));

    // Fill the table with some dummy values
    for(uint i = 0; i < 3; i++)
    {
        this->_addIngredient(QString("Ingredient %1").arg(i), "Empty note", i, "Some unit", 1.25 * i, "Some section");
    }
}

CreateRecipeDialog::~CreateRecipeDialog()
{
    delete _mIngredientsModel;
    delete _mUi;
}

void CreateRecipeDialog::on_pushButton_AddIngredients_clicked()
{
    this->_addIngredient();
}

void CreateRecipeDialog::on_pushButton_RemoveIngredients_clicked()
{
    QItemSelectionModel* select = _mUi->tableView_Ingredients->selectionModel();

    if(select->hasSelection() & !select->selectedRows().empty())
    {
        foreach(auto index, select->selectedRows())
        {
            _mIngredientsModel->removeRows(select->currentIndex().row(), 1);
        }
    }
}

void CreateRecipeDialog::on_buttonBox_clicked(QAbstractButton* button)
{
    if(button == _mUi->buttonBox->button(QDialogButtonBox::Save))
    {
        _mRecipes.push_back(Recipe(
                                _mUi->lineEdit_Name->text(),
                                _mUi->lineEdit_Note->text(),
                                _mUi->lineEdit_Link->text(),
                                _mUi->plainTextEdit_Description->toPlainText(),
                                _mUi->lineEdit_Timer1->text(),
                                _mUi->lineEdit_Timer2->text(),
                                _mUi->lineEdit_Category->text(),
                                uint(_mUi->spinBox_Persons->value()),
                                uint(_mUi->spinBox_CookingTime->value()),
                                uint(_mUi->spinBox_Timer1->value()),
                                uint(_mUi->spinBox_Timer2->value()),
                                _mIngredientsList
                                ));

        this->accept();
    }
    else if(button == _mUi->buttonBox->button(QDialogButtonBox::Discard))
    {
        this->reject();
    }
}

void CreateRecipeDialog::_addIngredient(QString Name, QString Note, uint Amount, QString Unit, double Price, QString Section)
{
    _mIngredientsList.push_back(Ingredient(Name, Note, Amount, Unit, Price, Section));
    _mIngredientsModel->insertRow(_mIngredientsModel->rowCount(QModelIndex()));
    _mIngredientsModel->layoutChanged();
    _mUi->tableView_Ingredients->scrollToBottom();
}

void CreateRecipeDialog::on_spinBox_Timer1_valueChanged(int arg1)
{
    if(arg1 > 0)
    {
        _mUi->lineEdit_Timer1->setEnabled(true);
    }
    else if(arg1 == 0)
    {
        _mUi->lineEdit_Timer1->setEnabled(false);
        _mUi->lineEdit_Timer1->clear();
    }
}

void CreateRecipeDialog::on_spinBox_Timer2_valueChanged(int arg1)
{
    if(arg1 > 0)
    {
        _mUi->lineEdit_Timer2->setEnabled(true);
    }
    else if(arg1 == 0)
    {
        _mUi->lineEdit_Timer2->setEnabled(false);
        _mUi->lineEdit_Timer2->clear();
    }
}
