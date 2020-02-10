#include "editrecipedialog.h"
#include "ui_editrecipedialog.h"

EditRecipeDialog::EditRecipeDialog(Recipe& OldRecipe, QWidget* parent) : QDialog(parent),
                                                                         _mUi(new Ui::EditRecipeDialog),
                                                                         _mIngredientsModel(new IngredientModel(_mNewIngredients)),
                                                                         _mOldRecipe(OldRecipe),
                                                                         _mNewRecipe(OldRecipe)
{
    _mUi->setupUi(this);
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    this->setWindowFlags(this->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);

    // Update the UI with the old values
    _mUi->lineEdit_Name->setText(_mOldRecipe.Name());
    _mUi->lineEdit_Note->setText(_mOldRecipe.Note());
    _mUi->lineEdit_Link->setText(_mOldRecipe.Link());
    _mUi->spinBox_Persons->setValue(int(_mOldRecipe.Persons()));
    _mUi->spinBox_CookingTime->setValue(int(_mOldRecipe.CookingTime()));
    _mUi->lineEdit_Timer1->setText(_mOldRecipe.Timer1Name());
    _mUi->spinBox_Timer1->setValue(int(_mOldRecipe.Timer1Value()));
    _mUi->lineEdit_Timer2->setText(_mOldRecipe.Timer2Name());
    _mUi->spinBox_Timer2->setValue(int(_mOldRecipe.Timer2Value()));
    _mUi->plainTextEdit_Description->setPlainText(_mOldRecipe.Description());
    _mUi->lineEdit_Category->setText(_mOldRecipe.Category());

    if(_mOldRecipe.Timer1Value() == 0)
    {
        _mUi->lineEdit_Timer1->setEnabled(false);
    }

    if(_mOldRecipe.Timer2Value() == 0)
    {
        _mUi->lineEdit_Timer2->setEnabled(false);
    }

    // Get the ingredients from the recipe and update the table view
    _mNewIngredients = _mOldRecipe.Ingredients();
    _mUi->tableView_Ingredients->setModel(_mIngredientsModel);
    _mUi->tableView_Ingredients->horizontalHeader()->setStretchLastSection(true);
    _mUi->tableView_Ingredients->horizontalHeader()->setSectionsClickable(false);

    _mUi->buttonBox->button(QDialogButtonBox::Save)->setText(tr("Speichern"));
    _mUi->buttonBox->button(QDialogButtonBox::Discard)->setText(tr("Verwerfen"));

    _mUi->spinBox_CookingTime->setSuffix(" " + tr("Minuten"));
}

EditRecipeDialog::~EditRecipeDialog()
{
    delete _mIngredientsModel;
    delete _mUi;
}

void EditRecipeDialog::on_buttonBox_clicked(QAbstractButton* button)
{
    if(button == _mUi->buttonBox->button(QDialogButtonBox::Save))
    {
        // Overwrite the old values
        _mNewRecipe.setIngredients(_mNewIngredients);
        _mOldRecipe = _mNewRecipe;

        this->accept();
    }
    else if(button == _mUi->buttonBox->button(QDialogButtonBox::Discard))
    {
        this->reject();
    }
}

void EditRecipeDialog::on_lineEdit_Name_textChanged(const QString& arg1)
{
    _mNewRecipe.setName(arg1);
}

void EditRecipeDialog::on_lineEdit_Category_textChanged(const QString& arg1)
{
    _mNewRecipe.setCategory(arg1);
}

void EditRecipeDialog::on_lineEdit_Note_textEdited(const QString& arg1)
{
    _mNewRecipe.setNote(arg1);
}

void EditRecipeDialog::on_lineEdit_Link_textEdited(const QString& arg1)
{
    _mNewRecipe.setLink(arg1);
}

void EditRecipeDialog::on_spinBox_Persons_valueChanged(int arg1)
{
    _mNewRecipe.setPersons(uint(arg1));
}

void EditRecipeDialog::on_spinBox_CookingTime_valueChanged(int arg1)
{
    _mNewRecipe.setCookingTime(uint(arg1));
}

void EditRecipeDialog::on_spinBox_Timer1_valueChanged(int arg1)
{
    _mNewRecipe.setTimer1Value(uint(arg1));

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

void EditRecipeDialog::on_lineEdit_Timer1_textEdited(const QString& arg1)
{
    _mNewRecipe.setTimer1Name(arg1);
}

void EditRecipeDialog::on_spinBox_Timer2_valueChanged(int arg1)
{
    _mNewRecipe.setTimer2Value(uint(arg1));

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

void EditRecipeDialog::on_lineEdit_Timer2_textEdited(const QString& arg1)
{
    _mNewRecipe.setTimer2Name(arg1);
}

void EditRecipeDialog::on_plainTextEdit_Description_textChanged()
{
    _mNewRecipe.setDescription(_mUi->plainTextEdit_Description->toPlainText());
}

void EditRecipeDialog::on_pushButton_AddIngredients_clicked()
{
    _mNewIngredients.push_back(Ingredient("Name", "Note", 0, "Unit", 0, "Section"));
    _mIngredientsModel->insertRow(_mIngredientsModel->rowCount(QModelIndex()));
    _mIngredientsModel->layoutChanged();
    _mUi->tableView_Ingredients->scrollToBottom();
}

void EditRecipeDialog::on_pushButton_RemoveIngredients_clicked()
{
    QItemSelectionModel* select = _mUi->tableView_Ingredients->selectionModel();

    if(select->hasSelection() & !select->selectedRows().empty())
    {
        foreach(auto index, select->selectedRows())
        {
            _mIngredientsModel->removeRows(index.row(), 1);
        }
    }
}