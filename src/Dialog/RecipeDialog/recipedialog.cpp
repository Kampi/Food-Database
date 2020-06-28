#include "recipedialog.h"
#include "ui_recipedialog.h"

RecipeDialog::RecipeDialog(QMap<QString, QStringList> Categories, Recipe NewRecipe, QWidget* parent) : QDialog(parent),
                                                                                                       _mUi(new Ui::RecipeDialog),
                                                                                                       _mIngredientsModel(new IngredientModel(_mIngredients)),
                                                                                                       _mSectionsComboBox(new ComboBoxDelegate(Categories.value("Ingredients", QStringList("")), this)),
                                                                                                       _mUnitComboBox(new ComboBoxDelegate(Categories.value("Units", QStringList("")), this)),
                                                                                                       _mNumbersOnlyDelegate(new NumbersOnlyDelegate(new QDoubleValidator(this)))
{
    _mUi->setupUi(this);

    // Fill the UI elements
    _mUi->lineEdit_Name->setText(NewRecipe.Name());
    _mUi->comboBox_Category->addItems(Categories.value("Recipes", QStringList("")));
    int Index = _mUi->comboBox_Category->findText(NewRecipe.Category());
    if(Index != -1)
    {
       _mUi->comboBox_Category->setCurrentIndex(Index);
    }

    _mUi->lineEdit_Note->setText(NewRecipe.Note());
    _mUi->lineEdit_Link->setText(NewRecipe.Link());
    _mUi->spinBox_Persons->setValue(uint(NewRecipe.Persons()));
    _mUi->spinBox_CookingTime->setValue(uint(NewRecipe.CookingTime()));
    _mUi->lineEdit_Timer1->setText(NewRecipe.Timer1Name());
    _mUi->timeEdit_Timer1->setTime(QTime(0, 0, 0).addSecs(NewRecipe.Timer1Value()));
    _mUi->lineEdit_Timer2->setText(NewRecipe.Timer2Name());
    _mUi->timeEdit_Timer2->setTime(QTime(0, 0, 0).addSecs(NewRecipe.Timer2Value()));
    _mUi->plainTextEdit_Description->setPlainText(NewRecipe.Description());

    if(NewRecipe.Timer1Value() == 0)
    {
        _mUi->lineEdit_Timer1->setEnabled(false);
    }

    if(NewRecipe.Timer2Value() == 0)
    {
        _mUi->lineEdit_Timer2->setEnabled(false);
    }

    // Get the ingredients from the recipe and update the table view
    _mIngredients = NewRecipe.Ingredients();
    _mUi->tableView_Ingredients->setModel(_mIngredientsModel);
    _mUi->tableView_Ingredients->horizontalHeader()->setStretchLastSection(true);
    _mUi->tableView_Ingredients->horizontalHeader()->setSectionsClickable(false);
    _mUi->tableView_Ingredients->setItemDelegateForColumn(IngredientModel::INGREDIENTSMODEL_TABLE_SECTION, _mSectionsComboBox);
    _mUi->tableView_Ingredients->setItemDelegateForColumn(IngredientModel::INGREDIENTSMODEL_TABLE_UNIT, _mUnitComboBox);
    _mUi->tableView_Ingredients->setItemDelegateForColumn(IngredientModel::INGREDIENTSMODEL_TABLE_AMOUNT, _mNumbersOnlyDelegate);
    _mUi->tableView_Ingredients->setItemDelegateForColumn(IngredientModel::INGREDIENTSMODEL_TABLE_PRICE, _mNumbersOnlyDelegate);
    _mUi->tableView_Ingredients->setEditTriggers(QAbstractItemView::AllEditTriggers);

    // Add spinbox suffix
    _mUi->spinBox_CookingTime->setSuffix(" " + tr("Minuten"));

    _mUi->buttonBox_Close->button(QDialogButtonBox::Save)->setText(tr("Speichern"));
    _mUi->buttonBox_Close->button(QDialogButtonBox::Discard)->setText(tr("Verwerfen"));

    // Initialize the new recipe with the template
    _mRecipe = Recipe(NewRecipe);
    _mCategories = Categories;
}

RecipeDialog::~RecipeDialog()
{
    delete _mUi;
}

Recipe RecipeDialog::recipe() const
{
    return _mRecipe;
}

void RecipeDialog::on_pushButton_AddIngredients_clicked()
{
    _mIngredients.push_back(Ingredient(tr("Name"), tr("Notiz"), 0.0, _mCategories.value("Units", QStringList("")).at(0), 0.0, _mCategories.value("Ingredients", QStringList("")).at(0)));
    _mIngredientsModel->layoutChanged();
    _mUi->tableView_Ingredients->scrollToBottom();
}

void RecipeDialog::on_pushButton_RemoveIngredients_clicked()
{
    QItemSelectionModel* select = _mUi->tableView_Ingredients->selectionModel();

    if(select->hasSelection() & !select->selectedRows().empty())
    {
        foreach(auto index, select->selectedRows())
        {
            (void)index;
            _mIngredientsModel->removeRows(select->currentIndex().row(), 1);
        }
    }
    else
    {
        QMessageBox::information(this, tr("Hinweis"), tr("Bitte mindestens eine Zutat auswählen!"), QMessageBox::Ok);
    }
}

void RecipeDialog::on_lineEdit_Name_textChanged(const QString& arg1)
{
    _mRecipe.setName(arg1);
}

void RecipeDialog::on_comboBox_Category_currentTextChanged(const QString& arg1)
{
    _mRecipe.setCategory(arg1);
}

void RecipeDialog::on_lineEdit_Note_textEdited(const QString& arg1)
{
    _mRecipe.setNote(arg1);
}

void RecipeDialog::on_lineEdit_Link_textEdited(const QString& arg1)
{
    _mRecipe.setLink(arg1);
}

void RecipeDialog::on_spinBox_Persons_valueChanged(int arg1)
{
    _mRecipe.setPersons(uint(arg1));
}

void RecipeDialog::on_spinBox_CookingTime_valueChanged(int arg1)
{
    _mRecipe.setCookingTime(uint(arg1));
}

void RecipeDialog::on_lineEdit_Timer1_textEdited(const QString& arg1)
{
    _mRecipe.setTimer1Name(arg1);
}

void RecipeDialog::on_timeEdit_Timer1_userTimeChanged(const QTime& time)
{
    _mRecipe.setTimer2Value(uint(QTime(0, 0, 0).secsTo(time)));

    if(QTime(0, 0, 0).secsTo(time) > 0)
    {
        _mUi->lineEdit_Timer1->setEnabled(true);
    }
    else if(QTime(0, 0, 0).secsTo(time) == 0)
    {
        _mUi->lineEdit_Timer1->setEnabled(false);
        _mUi->lineEdit_Timer1->clear();
    }
}

void RecipeDialog::on_lineEdit_Timer2_textEdited(const QString& arg1)
{
    _mRecipe.setTimer2Name(arg1);
}

void RecipeDialog::on_timeEdit_Timer2_userTimeChanged(const QTime& time)
{
    _mRecipe.setTimer2Value(uint(QTime(0, 0, 0).secsTo(time)));

    if(QTime(0, 0, 0).secsTo(time) > 0)
    {
        _mUi->lineEdit_Timer2->setEnabled(true);
    }
    else if(QTime(0, 0, 0).secsTo(time) == 0)
    {
        _mUi->lineEdit_Timer2->setEnabled(false);
        _mUi->lineEdit_Timer2->clear();
    }
}

void RecipeDialog::on_plainTextEdit_Description_textChanged()
{
    _mRecipe.setDescription(_mUi->plainTextEdit_Description->toPlainText());
}

void RecipeDialog::on_buttonBox_Close_clicked(QAbstractButton* button)
{
    if(button == _mUi->buttonBox_Close->button(QDialogButtonBox::Save))
    {
        _mRecipe.setIngredients(_mIngredients);
        this->accept();
    }
    else if(button == _mUi->buttonBox_Close->button(QDialogButtonBox::Discard))
    {
        this->reject();
    }
}
