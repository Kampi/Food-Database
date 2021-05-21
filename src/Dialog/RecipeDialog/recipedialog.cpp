#include "recipedialog.h"
#include "ui_recipedialog.h"

RecipeDialog::RecipeDialog(int ID, QMap<QString, QStringList> Categories, Recipe CurRecipe, QWidget* parent) : QDialog(parent),
                                                                                                               _mUi(new Ui::RecipeDialog),
                                                                                                               _mID(ID),
                                                                                                               _mIngredientsModel(new IngredientModel(this)),
                                                                                                               _mSectionsComboBox(new ComboBoxDelegate(Categories.value("Ingredients", QStringList("")), this)),
                                                                                                               _mUnitComboBox(new ComboBoxDelegate(Categories.value("Units", QStringList("")), this)),
                                                                                                               _mNumbersOnlyDelegate(new NumbersOnlyDelegate(new QDoubleValidator(this)))
{
    this->_mUi->setupUi(this);

    // Fill the UI elements
    this->_mUi->lineEdit_Name->setText(CurRecipe.Name());
    this->_mUi->comboBox_Category->addItems(Categories.value("Recipes", QStringList("")));
    int Index = this->_mUi->comboBox_Category->findText(CurRecipe.Category());
    if(Index != -1)
    {
       this->_mUi->comboBox_Category->setCurrentIndex(Index);
    }

    this->_mUi->lineEdit_Note->setText(CurRecipe.Note());
    this->_mUi->lineEdit_Link->setText(CurRecipe.Link());
    this->_mUi->spinBox_Persons->setValue(uint(CurRecipe.Persons()));
    this->_mUi->spinBox_CookingTime->setValue(uint(CurRecipe.CookingTime()));
    this->_mUi->timeEdit_Timer1->setTime(QTime(0, 0, 0).addSecs(CurRecipe.Timer1Value()));
    if(!CurRecipe.Timer1Value())
    {
        this->_mUi->lineEdit_Timer1->setEnabled(false);
        this->_mUi->lineEdit_Timer1->clear();
    }
    else
    {
        this->_mUi->lineEdit_Timer1->setText(CurRecipe.Timer1Name());
    }

    this->_mUi->timeEdit_Timer2->setTime(QTime(0, 0, 0).addSecs(CurRecipe.Timer2Value()));
    if(!CurRecipe.Timer2Value())
    {
        this->_mUi->lineEdit_Timer2->setEnabled(false);
        this->_mUi->lineEdit_Timer2->clear();
    }
    else
    {
        this->_mUi->lineEdit_Timer2->setText(CurRecipe.Timer2Name());
    }

    this->_mUi->plainTextEdit_Description->setPlainText(CurRecipe.Description());

    // Get the ingredients from the recipe and update the table view
    this->_mUi->tableView_Ingredients->setModel(this->_mIngredientsModel);
    this->_mUi->tableView_Ingredients->model()->setData(this->_mUi->tableView_Ingredients->model()->index(0, 0), QVariant::fromValue(CurRecipe.Ingredients()), Qt::UserRole);
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
    this->_mRecipe = Recipe(CurRecipe);
    this->_mCategories = Categories;
}

RecipeDialog::RecipeDialog(QMap<QString, QStringList> Categories, Recipe CurRecipe, QWidget* parent) : RecipeDialog(0, Categories, CurRecipe, parent)
{
}

RecipeDialog::~RecipeDialog()
{
    delete this->_mUi;
}

Recipe RecipeDialog::recipe() const
{
    return this->_mRecipe;
}

uint RecipeDialog::id() const
{
    return this->_mID;
}

void RecipeDialog::on_pushButton_AddIngredients_clicked()
{
    QList<Ingredient> List;
    List.push_back(Ingredient(tr("Name"), tr("Notiz"), 0.0, this->_mCategories.value("Units", QStringList("")).at(0), 0.0, this->_mCategories.value("Ingredients", QStringList("")).at(0)));

    this->_mUi->tableView_Ingredients->model()->setData(this->_mUi->tableView_Ingredients->model()->index(this->_mUi->tableView_Ingredients->model()->rowCount(), 0), QVariant::fromValue(List), Qt::UserRole);
    this->_mUi->tableView_Ingredients->model()->layoutChanged();
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
            this->_mUi->tableView_Ingredients->model()->removeRows(select->currentIndex().row(), 1);
        }
    }
    else
    {
        QMessageBox::information(this, tr("Hinweis"), tr("Bitte mindestens eine Zutat auswählen!"), QMessageBox::Ok);
    }

    if(this->_mUi->tableView_Ingredients->model()->rowCount() == 0)
    {
        this->_mUi->pushButton_RemoveIngredients->setEnabled(false);
    }
    else
    {
        this->_mUi->pushButton_RemoveIngredients->setEnabled(true);
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
        this->_mRecipe.setIngredients(qvariant_cast<QList<Ingredient>>(this->_mUi->tableView_Ingredients->model()->data(this->_mUi->tableView_Ingredients->model()->index(0, 0), Qt::UserRole)));
        this->accept();
    }
    else if(button == this->_mUi->buttonBox_Close->button(QDialogButtonBox::Discard))
    {
        this->reject();
    }
}
