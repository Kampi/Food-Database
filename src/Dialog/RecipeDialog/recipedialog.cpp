#include "recipedialog.h"
#include "ui_recipedialog.h"

RecipeDialog::RecipeDialog(QMap<QString, QStringList> Categories, Recipe NewRecipe, QWidget* parent) : QDialog(parent),
                                                                                                       _mUi(new Ui::RecipeDialog),
                                                                                                       _mIngredientsModel(new IngredientModel(_mIngredients)),
                                                                                                       _mSectionsComboBox(new ComboBoxDelegate(Categories.value("Ingredients", QStringList("")), this)),
                                                                                                       _mUnitComboBox(new ComboBoxDelegate(Categories.value("Units", QStringList("")), this)),
                                                                                                       _mNumbersOnlyDelegate(new NumbersOnlyDelegate(new QDoubleValidator(this)))
{
    this->_mUi->setupUi(this);

    // Fill the UI elements
    this->_mUi->lineEdit_Name->setText(NewRecipe.Name());
    this->_mUi->comboBox_Category->addItems(Categories.value("Recipes", QStringList("")));
    int Index = this->_mUi->comboBox_Category->findText(NewRecipe.Category());
    if(Index != -1)
    {
       this->_mUi->comboBox_Category->setCurrentIndex(Index);
    }

    this->_mUi->lineEdit_Note->setText(NewRecipe.Note());
    this->_mUi->lineEdit_Link->setText(NewRecipe.Link());
    this->_mUi->spinBox_Persons->setValue(uint(NewRecipe.Persons()));
    this->_mUi->spinBox_CookingTime->setValue(uint(NewRecipe.CookingTime()));
    this->_mUi->timeEdit_Timer1->setTime(QTime(0, 0, 0).addSecs(NewRecipe.Timer1Value()));
    if(!NewRecipe.Timer1Value())
    {
        this->_mUi->lineEdit_Timer1->setEnabled(false);
        this->_mUi->lineEdit_Timer1->clear();
    }
    else
    {
        this->_mUi->lineEdit_Timer1->setText(NewRecipe.Timer1Name());
    }

    this->_mUi->timeEdit_Timer2->setTime(QTime(0, 0, 0).addSecs(NewRecipe.Timer2Value()));
    if(!NewRecipe.Timer2Value())
    {
        this->_mUi->lineEdit_Timer2->setEnabled(false);
        this->_mUi->lineEdit_Timer2->clear();
    }
    else
    {
        this->_mUi->lineEdit_Timer2->setText(NewRecipe.Timer2Name());
    }

    this->_mUi->plainTextEdit_Description->setPlainText(NewRecipe.Description());

    // Get the ingredients from the recipe and update the table view
    this->_mIngredients = NewRecipe.Ingredients();
    this->_mUi->tableView_Ingredients->setModel(this->_mIngredientsModel);
    this->_mUi->tableView_Ingredients->horizontalHeader()->setStretchLastSection(true);
    this->_mUi->tableView_Ingredients->horizontalHeader()->setSectionsClickable(false);
    this->_mUi->tableView_Ingredients->setItemDelegateForColumn(IngredientModel::INGREDIENTSMODEL_TABLE_SECTION, this->_mSectionsComboBox);
    this->_mUi->tableView_Ingredients->setItemDelegateForColumn(IngredientModel::INGREDIENTSMODEL_TABLE_UNIT, this->_mUnitComboBox);
    this->_mUi->tableView_Ingredients->setItemDelegateForColumn(IngredientModel::INGREDIENTSMODEL_TABLE_AMOUNT, this->_mNumbersOnlyDelegate);
    this->_mUi->tableView_Ingredients->setItemDelegateForColumn(IngredientModel::INGREDIENTSMODEL_TABLE_PRICE, this->_mNumbersOnlyDelegate);
    this->_mUi->tableView_Ingredients->setEditTriggers(QAbstractItemView::AllEditTriggers);

    // Add spinbox suffix
    this->_mUi->spinBox_CookingTime->setSuffix(" " + tr("Minuten"));

    this->_mUi->buttonBox_Close->button(QDialogButtonBox::Save)->setText(tr("Speichern"));
    this->_mUi->buttonBox_Close->button(QDialogButtonBox::Discard)->setText(tr("Verwerfen"));

    // Initialize the new recipe with the template
    this->_mRecipe = Recipe(NewRecipe);
    this->_mCategories = Categories;
}

RecipeDialog::~RecipeDialog()
{
    delete this->_mUi;
}

Recipe RecipeDialog::recipe() const
{
    return this->_mRecipe;
}

void RecipeDialog::on_pushButton_AddIngredients_clicked()
{
    this->_mIngredients.push_back(Ingredient(tr("Name"), tr("Notiz"), 0.0, this->_mCategories.value("Units", QStringList("")).at(0), 0.0, this->_mCategories.value("Ingredients", QStringList("")).at(0)));
    this->_mIngredientsModel->layoutChanged();
    this->_mUi->tableView_Ingredients->scrollToBottom();
}

void RecipeDialog::on_pushButton_RemoveIngredients_clicked()
{
    QItemSelectionModel* select = this->_mUi->tableView_Ingredients->selectionModel();

    if(select->hasSelection() & !select->selectedRows().empty())
    {
        foreach(auto index, select->selectedRows())
        {
            (void)index;
            this->_mIngredientsModel->removeRows(select->currentIndex().row(), 1);
        }
    }
    else
    {
        QMessageBox::information(this, tr("Hinweis"), tr("Bitte mindestens eine Zutat auswählen!"), QMessageBox::Ok);
    }
}

void RecipeDialog::on_lineEdit_Name_textChanged(const QString& arg1)
{
    this->_mRecipe.setName(arg1);
}

void RecipeDialog::on_comboBox_Category_currentTextChanged(const QString& arg1)
{
    this->_mRecipe.setCategory(arg1);
}

void RecipeDialog::on_lineEdit_Note_textEdited(const QString& arg1)
{
    this->_mRecipe.setNote(arg1);
}

void RecipeDialog::on_lineEdit_Link_textEdited(const QString& arg1)
{
    this->_mRecipe.setLink(arg1);
}

void RecipeDialog::on_spinBox_Persons_valueChanged(int arg1)
{
    this->_mRecipe.setPersons(uint(arg1));
}

void RecipeDialog::on_spinBox_CookingTime_valueChanged(int arg1)
{
    this->_mRecipe.setCookingTime(uint(arg1));
}

void RecipeDialog::on_lineEdit_Timer1_textEdited(const QString& arg1)
{
    this->_mRecipe.setTimer1Name(arg1);
}

void RecipeDialog::on_timeEdit_Timer1_userTimeChanged(const QTime& time)
{
    this->_mRecipe.setTimer1Value(uint(QTime(0, 0, 0).secsTo(time)));

    if(QTime(0, 0, 0).secsTo(time) > 0)
    {
        this->_mUi->lineEdit_Timer1->setEnabled(true);
    }
    else if(QTime(0, 0, 0).secsTo(time) == 0)
    {
        this->_mUi->lineEdit_Timer1->setEnabled(false);
        this->_mUi->lineEdit_Timer1->clear();
    }
}

void RecipeDialog::on_lineEdit_Timer2_textEdited(const QString& arg1)
{
    this->_mRecipe.setTimer2Name(arg1);
}

void RecipeDialog::on_timeEdit_Timer2_userTimeChanged(const QTime& time)
{
    this->_mRecipe.setTimer2Value(uint(QTime(0, 0, 0).secsTo(time)));

    if(QTime(0, 0, 0).secsTo(time) > 0)
    {
        this->_mUi->lineEdit_Timer2->setEnabled(true);
    }
    else if(QTime(0, 0, 0).secsTo(time) == 0)
    {
        this->_mUi->lineEdit_Timer2->setEnabled(false);
        this->_mUi->lineEdit_Timer2->clear();
    }
}

void RecipeDialog::on_plainTextEdit_Description_textChanged()
{
    this->_mRecipe.setDescription(this->_mUi->plainTextEdit_Description->toPlainText());
}

void RecipeDialog::on_buttonBox_Close_clicked(QAbstractButton* button)
{
    if(button == this->_mUi->buttonBox_Close->button(QDialogButtonBox::Save))
    {
        this->_mRecipe.setIngredients(this->_mIngredients);
        this->accept();
    }
    else if(button == this->_mUi->buttonBox_Close->button(QDialogButtonBox::Discard))
    {
        this->reject();
    }
}
