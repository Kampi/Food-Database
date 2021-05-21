#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) :   QMainWindow(parent),
                                            _mUi(new Ui::MainWindow),
                                            _mDatabaseState(new QLabel(this)),
                                            _mRecipesProxy(new RecipesProxy(this)),
                                            _mRecipesModel(new RecipesModel(this))
{
    this->_mUi->setupUi(this);
    this->_mUi->statusBar->addPermanentWidget(this->_mDatabaseState);

    if(this->_loadSettings())
    {
        this->_mDatabase = QSqlDatabase::addDatabase("QSQLITE");

        this->statusBar()->setSizeGripEnabled(false);
        this->setWindowFlags(this->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
        this->_updateDatabaseStatus();
        this->_setLanguageMenu();

        // Create the search window
        QLineEdit* SearchField = new QLineEdit(this);
        connect(SearchField, &QLineEdit::textChanged, this->_mRecipesProxy, &RecipesProxy::setName);

        // Modify the menu bar with the dynamic widgets
        foreach(QAction* action, this->menuBar()->actions())
        {
            // Skip seperators
            if(action->isSeparator())
            {
            }
            else if(action->menu())
            {
                // Add the line edit to the "Suchen" menu
                if(action->text() == tr("Suchen"))
                {
                    QWidgetAction* WidgetAction = new QWidgetAction(this);
                    WidgetAction->setDefaultWidget(SearchField);
                    action->menu()->addAction(WidgetAction);
                }
            }
            else
            {
            }
        }
    }
    else
    {
        exit(-1);
    }
}

MainWindow::~MainWindow()
{
    delete this->_mUi;
}

void MainWindow::CreateRecipe_finished(int result)
{
    if(result == 1)
    {
        RecipeDialog* Dialog = qobject_cast<RecipeDialog*>(sender());
        RecipesProxy* Proxy = dynamic_cast<RecipesProxy*>(this->_mUi->tableView_Recipes->model());
        QSqlTableModel* SqlModel = dynamic_cast<QSqlTableModel*>(Proxy->sourceModel());
        QSqlRecord Record = this->_RecipeToRecord(Dialog->recipe());

        if(SqlModel->insertRecord(-1, Record))
        {
            #ifdef QT_DEBUG
                qDebug() << "[INFO] Writing database!";
            #endif

            SqlModel->submitAll();
        }
        else
        {
            QMessageBox::information(this, tr("Fehler"), tr("Datenbank konnte nicht beschrieben werden werden!"), QMessageBox::Ok);

            this->_mDatabase.rollback();
        }
    }
}

void MainWindow::EditRecipe_finished(int result)
{
    if(result == 1)
    {
        RecipeDialog* Dialog = qobject_cast<RecipeDialog*>(sender());
        Recipe NewRecipe = Dialog->recipe();
        QSqlRecord Record = this->_RecipeToRecord(Dialog->recipe());
        RecipesProxy* Proxy = dynamic_cast<RecipesProxy*>(this->_mUi->tableView_Recipes->model());
        QSqlTableModel* SqlModel = dynamic_cast<QSqlTableModel*>(Proxy->sourceModel());

        // Get the index of the old recipe
        int Index = 0;
        for(int i = 0; i < SqlModel->rowCount(); i++)
        {
            QSqlRecord Record = SqlModel->record(i);
            if(Record.value("ID").toUInt() == Dialog->id())
            {
                Index = i;
                break;
            }
        }

        if(SqlModel->setRecord(Index, Record))
        {
            #ifdef QT_DEBUG
                qDebug() << "[INFO] Writing database!";
            #endif

            SqlModel->submitAll();
        }
        else
        {
            QMessageBox::information(this, tr("Fehler"), tr("Datenbank konnte nicht beschrieben werden werden!"), QMessageBox::Ok);

            this->_mDatabase.rollback();
        }
    }
}

void MainWindow::Settings_finished(int result)
{
    if(result == 1)
    {
        SettingsDialog* Dialog = qobject_cast<SettingsDialog*>(sender());
        this->_mCategories = Dialog->categories();
        this->_saveSettings();
    }
}

void MainWindow::on_tableView_Recipes_doubleClicked(const QModelIndex& index)
{
    this->_editRecipe(index);
}

void MainWindow::on_action_CreateDatabase_triggered(void)
{
    this->_createDatabase();
}

void MainWindow::on_action_OpenDatabase_triggered(void)
{
    QString Path = QFileDialog::getOpenFileName(this, tr("Datenbank öffnen"), QDir::homePath(), ("SQLite (*.sqlite3)"));
    this->_openDatabase(Path);
}

void MainWindow::on_action_CloseDatabase_triggered(void)
{
    this->_closeDatabase();
}

void MainWindow::on_action_NewRecipe_triggered(void)
{
    this->_createRecipe();
}

void MainWindow::on_action_RemoveRecipe_triggered(void)
{
    this->_removeRecipe();
}

void MainWindow::on_action_ExportRecipe_triggered(void)
{
    this->_exportRecipe();
}

void MainWindow::on_action_Settings_triggered(void)
{
    SettingsDialog* Dialog = new SettingsDialog(this->_mCategories, this);
    connect(Dialog, &QDialog::finished, this, &MainWindow::Settings_finished);
    Dialog->show();
}

void MainWindow::on_action_About_triggered(void)
{
    AboutDialog* Dialog = new AboutDialog(this);
    Dialog->show();
}

void MainWindow::on_action_AboutQt_triggered(void)
{
    QMessageBox::aboutQt(this, tr("Über Qt"));
}

void MainWindow::on_pushButton_NewRecipe_clicked(void)
{
    this->_createRecipe();
}

void MainWindow::on_pushButton_ExportRecipe_clicked(void)
{
    this->_exportRecipe();
}

void MainWindow::on_pushButton_RemoveRecipe_clicked(void)
{
    this->_removeRecipe();
}

void MainWindow::languageChange(bool)
{
    QAction* Action = qobject_cast<QAction*>(sender());
    Action->setChecked(true);
    this->_switchLanguage(Action->iconText());
}

void MainWindow::changeEvent(QEvent* event)
{
    if(event->type())
    {
        switch(event->type())
        {
            case QEvent::LanguageChange:
            {
                this->_mUi->retranslateUi(this);

                break;
            }
            case QEvent::LocaleChange:
            {
                QString Locale = QLocale::system().name();
                Locale.truncate(Locale.lastIndexOf('_'));
                this->_switchLanguage(Locale);

                break;
            }
            default:
            {
                break;
            }
        }
    }

    QMainWindow::changeEvent(event);
}

void MainWindow::closeEvent(QCloseEvent*)
{
    this->_closeDatabase();
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    if(event->key() == Qt::Key_Escape)
    {
        this->close();
    }
}

QSqlRecord MainWindow::_RecipeToRecord(Recipe Recipe)
{
    QSqlRecord Record;
    QJsonObject IngredientsRecord;
    QJsonObject IngredientsObject;

    foreach(auto Ingredient, Recipe.Ingredients())
    {
        IngredientsObject.insert("Note", Ingredient.Note());
        IngredientsObject.insert("Amount", Ingredient.Amount());
        IngredientsObject.insert("Unit", Ingredient.Unit());
        IngredientsObject.insert("Price", Ingredient.Price());
        IngredientsObject.insert("Section", Ingredient.Section());
        IngredientsRecord.insert(Ingredient.Name(), IngredientsObject);
    }
    QJsonDocument doc(IngredientsRecord);

    this->_mDatabase.transaction();

    RecipesProxy* Proxy = dynamic_cast<RecipesProxy*>(this->_mUi->tableView_Recipes->model());
    QSqlTableModel* SqlModel = dynamic_cast<QSqlTableModel*>(Proxy->sourceModel());

    Record = SqlModel->record();
    Record.remove(Record.indexOf("ID"));
    Record.setValue("Name", Recipe.Name());
    Record.setValue("Note", Recipe.Note());
    Record.setValue("Link", Recipe.Link());
    Record.setValue("Cooking_Description", Recipe.Description());
    Record.setValue("Persons", Recipe.Persons());
    Record.setValue("Cooking_Time", Recipe.CookingTime());
    Record.setValue("Category", Recipe.Category());
    Record.setValue("Timer1_Name", Recipe.Timer1Name());
    Record.setValue("Timer2_Name", Recipe.Timer2Name());
    Record.setValue("Timer1_Value", Recipe.Timer1Value());
    Record.setValue("Timer2_Value", Recipe.Timer2Value());
    Record.setValue("Ingredients", doc.toJson(QJsonDocument::Compact));

    return Record;
}

Recipe MainWindow::_RecordToRecipe(QSqlRecord Record)
{
    QList<Ingredient> Ingredients;

    QJsonObject IngredientsObject = QJsonDocument::fromJson(Record.field("Ingredients").value().toString().toUtf8()).object();
    for(auto it = IngredientsObject.begin(), end = IngredientsObject.end(); it != end; ++it)
    {
        QJsonObject Element = it.value().toObject();
        Ingredients.push_back(Ingredient(it.key(),
                                         Element.value("Note").toString(),
                                         Element.value("Amount").toDouble(),
                                         Element.value("Unit").toString(),
                                         Element.value("Price").toDouble(),
                                         Element.value("Section").toString()
                                         ));
    }

    return Recipe(Record.field("Name").value().toString(),
                  Record.field("Note").value().toString(),
                  Record.field("Link").value().toString(),
                  Record.field("Cooking_Description").value().toString(),
                  Record.field("Timer1_Name").value().toString(),
                  Record.field("Timer2_Name").value().toString(),
                  Record.field("Category").value().toString(),
                  Record.field("Persons").value().toInt(),
                  Record.field("Cooking_Time").value().toInt(),
                  Record.field("Timer1_Value").value().toInt(),
                  Record.field("Timer2_Value").value().toInt(),
                  Ingredients
                  );
}

void MainWindow::_saveSettings(void)
{
    QSettings Settings(QApplication::applicationDirPath() + "/Settings.ini", QSettings::IniFormat);
    Settings.setIniCodec(QTextCodec::codecForName("System"));
    Settings.setValue("Recipes/Categories", this->_mCategories.value("Recipes", QStringList("")));
    Settings.setValue("Ingredients/Categories", this->_mCategories.value("Ingredients", QStringList("")));
    Settings.setValue("Units/Categories", this->_mCategories.value("Units", QStringList("")));
}

bool MainWindow::_loadSettings(void)
{
    QString Path = QApplication::applicationDirPath() + "/Settings.ini";
    if(QFile(Path).exists())
    {
        QSettings Settings(Path, QSettings::IniFormat);
        Settings.setIniCodec(QTextCodec::codecForName("System"));

        if(Settings.allKeys().size() != 3)
        {
            QMessageBox::critical(this, tr("Fehler"), tr("Ungültige Einstellungen!"), QMessageBox::Ok);

            return false;
        }

        this->_addCategory(Settings.value("Ingredients/Categories").toStringList(), "Ingredients");
        this->_addCategory(Settings.value("Recipes/Categories").toStringList(), "Recipes");
        this->_addCategory(Settings.value("Units/Categories").toStringList(), "Units");

        foreach(auto Key, this->_mCategories.keys())
        {
            if(this->_mCategories.value(Key).size() == 0)
            {
                QMessageBox::critical(this, tr("Fehler"), tr("Ungültige Einstellungen!"), QMessageBox::Ok);

                return false;
            }
        }

        return true;
    }
    else
    {
        QMessageBox::critical(this, tr("Fehler"), tr("Einstellungen nicht gefunden!"), QMessageBox::Ok);

        return false;
    }
}

void MainWindow::_createRecipe(void)
{
    // Generate some dummy ingredients
    QList<Ingredient> Ingredients;
    for(uint i = 0; i < 3; i++)
    {
        Ingredients.push_back(Ingredient(QString(tr("Zutat")).append(QString(" %1").arg(i)), tr("Leere Notiz"), i, this->_mCategories.value("Units", QStringList("")).at(0), 1.25 * i, this->_mCategories.value("Ingredients", QStringList("")).at(0)));
    }

    // Generate a dummy recipe
    Recipe Template = Recipe(tr("Neues Rezept"), tr("Leere Notiz"), tr("Leerer Link"), tr("Leere Beschreibung"), tr("Stoppuhr 1"), tr("Stoppuhr 2"), this->_mCategories.value("Recipes", QStringList("")).at(0), 1, 10, 20, 30, Ingredients);

    RecipeDialog* Dialog = new RecipeDialog(this->_mCategories, Template, this);
    Dialog->setWindowTitle(tr("Neues Rezept"));
    connect(Dialog, &QDialog::finished, this, &MainWindow::CreateRecipe_finished);
    Dialog->show();
}

void MainWindow::_removeRecipe(void)
{
    QItemSelectionModel* select = this->_mUi->tableView_Recipes->selectionModel();

    if(select->hasSelection() & !select->selectedRows().empty())
    {
        // Remove the selected rows from the model
        RecipesProxy* Proxy = dynamic_cast<RecipesProxy*>(this->_mUi->tableView_Recipes->model());
        QSqlTableModel* SqlModel = dynamic_cast<QSqlTableModel*>(Proxy->sourceModel());
        SqlModel->removeRows(select->selectedRows().first().row(), select->selectedRows().count());
        SqlModel->select();
    }
    else
    {
        QMessageBox::information(this, tr("Hinweis"), tr("Bitte mindenstens ein Rezept auswählen!"), QMessageBox::Ok);
    }
}

void MainWindow::_editRecipe(QModelIndex Index)
{
    QSqlRecord RecipeRecord = qvariant_cast<QSqlRecord>(this->_mUi->tableView_Recipes->model()->data(Index, Qt::UserRole));

    RecipeDialog* Dialog = new RecipeDialog(RecipeRecord.value("ID").toUInt(), this->_mCategories, this->_RecordToRecipe(RecipeRecord), this);
    Dialog->setWindowTitle(tr("Rezept bearbeiten"));
    connect(Dialog, &QDialog::finished, this, &MainWindow::EditRecipe_finished);
    Dialog->show();
}

void MainWindow::_createDatabase(void)
{
    QString Path = QFileDialog::getSaveFileName(this, tr("Datenbank anlegen"), QDir::homePath(), ("SQLite (*.sqlite3)"));
    if(!Path.isEmpty())
    {
        #ifdef QT_DEBUG
            qDebug() << "[INFO] Create database at: " << Path << "!";
        #endif

        this->_mDatabase.setDatabaseName(Path);
        if(this->_mDatabase.open())
        {
            bool Result;
            QSqlQuery Query;

            Result = Query.exec("CREATE TABLE Recipes (ID INTEGER primary key, "
                                                      "Name VARCHAR(50), "
                                                      "Note VARCHAR(50), "
                                                      "Link MEDIUMTEXT, "
                                                      "Persons INTEGER, "
                                                      "Cooking_Description MEDIUMTEXT, "
                                                      "Category VARCHAR(50), "
                                                      "Cooking_Time INTEGER, "
                                                      "Timer1_Name VARCHAR(50), "
                                                      "Timer2_Name VARCHAR(50), "
                                                      "Timer1_Value INTEGER, "
                                                      "Timer2_Value INTEGER, "
                                                      "Ingredients MEDIUMTEXT)"
                                );

            if(!Result)
            {
                QMessageBox::information(this, tr("Fehler"), tr("Fehler beim Anlegen der Datenbank!"), QMessageBox::Ok);

                #ifdef QT_DEBUG
                    qDebug() << "[ERROR] Database error: " << this->_mDatabase.lastError() << "!";
                #endif
            }

            this->_openDatabase(Path);
        }
        else
        {
            QMessageBox::information(this, tr("Fehler"), tr("Datenbank konnte nicht angelegt werden!"), QMessageBox::Ok);

            #ifdef QT_DEBUG
                qDebug() << "[ERROR] Database error: " << this->_mDatabase.lastError() << "!";
            #endif
        }
    }
}

void MainWindow::_closeDatabase(void)
{
    if(this->_mDatabase.isOpen())
    {
        this->_mDatabase.commit();
        this->_mDatabase.close();
    }

    this->_updateDatabaseStatus();
}

void MainWindow::_exportRecipe(void)
{
    if(this->_mUi->tableView_Recipes->selectionModel()->hasSelection() & !this->_mUi->tableView_Recipes->selectionModel()->selectedRows().empty())
    {
        QString File = QFileDialog::getSaveFileName(this, tr("Speicherort wählen"), QDir::homePath(), ("JSON (*.json)"));
        if(File.length() > 0)
        {
            QList<QSqlRecord> SelectedRecipes;

            QModelIndexList Selection = this->_mUi->tableView_Recipes->selectionModel()->selectedRows();
            for(int i = 0x00; i < Selection.count(); i++)
            {
                SelectedRecipes.push_back(qvariant_cast<QSqlRecord>(this->_mUi->tableView_Recipes->model()->data(Selection.at(i), Qt::UserRole)));
            }

            if(!this->_mRecipeExport.Export(File, SelectedRecipes))
            {
                #ifdef QT_DEBUG
                    qDebug() << "[ERROR] Unable to write JSON!";
                #endif

                QMessageBox::critical(this, tr("Fehler"), tr("Export fehlgeschlagen!"), QMessageBox::Ok);
            }

            QMessageBox::information(this, tr("Hinweis"), tr("Rezepte erfolgreich exportiert!"), QMessageBox::Ok);
        }
    }
    else
    {
        QMessageBox::information(this, tr("Hinweis"), tr("Bitte mindenstens ein Rezept auswählen!"), QMessageBox::Ok);
    }
}

void MainWindow::_setLanguageMenu(void)
{
    QActionGroup* actionGroup = new QActionGroup(this);
    actionGroup->addAction(this->_mUi->action_German);
    actionGroup->addAction(this->_mUi->action_EnglishUS);
    actionGroup->setExclusive(true);

    connect(this->_mUi->action_German, &QAction::triggered, this, &MainWindow::languageChange);
    connect(this->_mUi->action_EnglishUS, &QAction::triggered, this, &MainWindow::languageChange);

    // Use german as default language
    emit this->_mUi->action_German->triggered(true);
}

void MainWindow::_updateDatabaseStatus(void)
{
    QString FileName;

    if(this->_mDatabase.isOpen())
    {
        this->_mUi->action_CreateDatabase->setEnabled(false);
        this->_mUi->action_OpenDatabase->setEnabled(false);
        this->_mUi->action_CloseDatabase->setEnabled(true);
        this->_mUi->pushButton_NewRecipe->setEnabled(true);
        this->_mUi->pushButton_RemoveRecipe->setEnabled(true);
        this->_mUi->pushButton_ExportRecipe->setEnabled(true);

        #ifdef QT_DEBUG
            qDebug() << "[INFO] Database open!";
        #endif

        FileName = ":/Icons/Ressources/Icons/icons8-connected-24.png";
    }
    else
    {
        this->_mUi->action_CreateDatabase->setEnabled(true);
        this->_mUi->action_OpenDatabase->setEnabled(true);
        this->_mUi->action_CloseDatabase->setEnabled(false);
        this->_mUi->pushButton_NewRecipe->setEnabled(false);
        this->_mUi->pushButton_RemoveRecipe->setEnabled(false);
        this->_mUi->pushButton_ExportRecipe->setEnabled(false);

        #ifdef QT_DEBUG
            qDebug() << "[INFO] Database closed!";
        #endif

        FileName = ":/Icons/Ressources/Icons/icons8-disconnected-24.png";
    }

    QPixmap Pixmap;
    Pixmap.load(FileName);
    this->_mDatabaseState->setPixmap(Pixmap);
}

void MainWindow::_openDatabase(QString Path)
{
    if(!Path.isEmpty())
    {
        #ifdef QT_DEBUG
            qDebug() << "[INFO] Open database at: " << Path;
        #endif

        this->_mDatabase.setDatabaseName(Path);

        if(this->_mDatabase.open())
        {
            // Read the recipes from the database
            this->_mRecipesModel = new RecipesModel(this, this->_mDatabase);
            this->_mRecipesModel->setTable("Recipes");
            this->_mRecipesModel->select();

            this->_mRecipesProxy->setSourceModel(this->_mRecipesModel);

            this->_mUi->tableView_Recipes->setModel(this->_mRecipesProxy);
            this->_mUi->tableView_Recipes->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
            this->_mUi->tableView_Recipes->setAlternatingRowColors(true);
            this->_mUi->tableView_Recipes->setSortingEnabled(true);
            this->_mUi->tableView_Recipes->sortByColumn(1, Qt::AscendingOrder);
            this->_mUi->tableView_Recipes->hideColumn(0);
            this->_mUi->tableView_Recipes->hideColumn(2);
            this->_mUi->tableView_Recipes->hideColumn(3);
            this->_mUi->tableView_Recipes->hideColumn(5);
            for(int i = 8; i < this->_mRecipesProxy->columnCount(); i++)
            {
                this->_mUi->tableView_Recipes->hideColumn(i);
            }
        }
        else
        {
            QMessageBox::information(this, tr("Fehler"), tr("Datenbank konnte nicht geöffnet werden!"), QMessageBox::Ok);

            #ifdef QT_DEBUG
                qDebug() << "[ERROR] Database error: " << this->_mDatabase.lastError() << "!";
            #endif
        }

        this->_updateDatabaseStatus();
    }
}

void MainWindow::_switchLanguage(QString Language)
{
    QLocale::setDefault(QLocale(Language));

    qApp->removeTranslator(&this->_mTranslator);

    if(this->_mTranslator.load(QString(QApplication::applicationDirPath().append(QString("/Languages/FoodDatabase_%1.qm").arg(Language)))))
    {
        qApp->installTranslator(&this->_mTranslator);
    }
}

void MainWindow::_addCategory(QStringList Settings, QString Name)
{
    Settings.sort();
    this->_mCategories.insert(Name, Settings);
}
