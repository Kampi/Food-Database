#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) :   QMainWindow(parent),
                                            _mUi(new Ui::MainWindow),
                                            _mDatabaseState(new QLabel(this)),
                                            _mRecipesModel(new RecipesModel(this)),
                                            _mRecipesProxy(new RecipesProxy(this)),
                                            _mIsEdited(false)
{
    this->_mUi->setupUi(this);

    this->setWindowFlags(this->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
    this->statusBar()->setSizeGripEnabled(false);
    if(this->_loadSettings())
    {
        this->_updateStatusbar();
        this->_setLanguageMenu();

        this->_mRecipesProxy->setSourceModel(this->_mRecipesModel);

        this->_mUi->tableView_Recipes->setModel(this->_mRecipesProxy);
        this->_mUi->tableView_Recipes->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        this->_mUi->tableView_Recipes->setAlternatingRowColors(true);
        this->_mUi->tableView_Recipes->setSortingEnabled(true);
        this->_mUi->tableView_Recipes->sortByColumn(RecipesModel::RECIPESMODEL_TABLE_NAME, Qt::AscendingOrder);

        this->_mUi->statusBar->addPermanentWidget(this->_mDatabaseState);

        // Create some dummy recipes
        QList<Ingredient> Ingredients({Ingredient(tr("Erste"), tr("Notiz"), 1, "kg", 1.55, this->_mCategories.value("Ingredients", QStringList("")).at(0)), Ingredient(tr("Zweite"), tr("Mehr"), 1, "l", 1.0, this->_mCategories.value("Ingredients", QStringList("")).at(1))});
        this->_mUi->tableView_Recipes->model()->insertRows(0, 2);
        this->_mUi->tableView_Recipes->model()->setData(this->_mUi->tableView_Recipes->model()->index(0, 0),
                                                        QVariant::fromValue<>(Recipe(tr("Erstes Rezept"), tr("Notiz"), tr("Link zu Rezept 1"), tr("Kurze Beschreibung für Rezept 1."), tr("Beschreibung erste Stoppuhr."), tr("Beschreibung zweite Stoppuhr."), this->_mCategories.value("Recipes", QStringList("")).at(0), 1, 30, 10, 20, Ingredients)));
        this->_mUi->tableView_Recipes->model()->setData(this->_mUi->tableView_Recipes->model()->index(1, 0),
                                                        QVariant::fromValue<>(Recipe(tr("Zweites Rezept"), tr("Notiz"), ("Link zu Rezept 2"), tr("Kurze Beschreibung für Rezept 2."), tr("Beschreibung erste Stoppuhr."), tr("Beschreibung zweite Stoppuhr."), this->_mCategories.value("Recipes", QStringList("")).at(1), 1, 10, 30, 40, Ingredients)));

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

        // Save the new recipe
        int Rows = this->_mUi->tableView_Recipes->model()->rowCount();
        this->_mUi->tableView_Recipes->model()->insertRows(Rows, 1);
        this->_mUi->tableView_Recipes->model()->setData(this->_mUi->tableView_Recipes->model()->index(Rows, 0), QVariant::fromValue<Recipe>(Dialog->recipe()));

        // Scroll the table view to the button
        this->_mUi->tableView_Recipes->scrollToBottom();

        // Data has changed
        this->_mIsEdited = true;
    }
}

void MainWindow::EditRecipe_finished(int result)
{
    if(result == 1)
    {
        RecipeDialog* Dialog = qobject_cast<RecipeDialog*>(sender());

        // Save the modified recipe
        this->_mUi->tableView_Recipes->model()->setData(this->_mUi->tableView_Recipes->currentIndex(), QVariant::fromValue<Recipe>(Dialog->recipe()));

        // Data has changed
        this->_mIsEdited = true;
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
    this->_openDatabase();
}

void MainWindow::on_action_CloseDatabase_triggered(void)
{
    this->_closeDatabase();
}

void MainWindow::on_action_WriteDatabase_triggered(void)
{
    this->_writeRecipesToDatabase();
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
        this->_mUi->tableView_Recipes->model()->removeRows(select->selectedRows().first().row(), select->selectedRows().count());

        // Data has changed
        this->_mIsEdited = true;
    }
    else
    {
        QMessageBox::information(this, tr("Hinweis"), tr("Bitte mindenstens ein Rezept auswählen!"), QMessageBox::Ok);
    }
}

void MainWindow::_editRecipe(QModelIndex Index)
{
    RecipeDialog* Dialog = new RecipeDialog(this->_mCategories, qvariant_cast<Recipe>(this->_mUi->tableView_Recipes->model()->data(Index, Qt::UserRole)), this);
    Dialog->setWindowTitle(tr("Rezept bearbeiten"));
    connect(Dialog, &QDialog::finished, this, &MainWindow::EditRecipe_finished);
    Dialog->show();
}

void MainWindow::_createDatabase(void)
{
    QString Path = QFileDialog::getSaveFileName(this, tr("Datenbank anlegen"), QDir::homePath(), ("SQLite (*.sqlite3)"));
    if(!Path.isEmpty())
    {
        if(this->_mDatabase.Create(Path))
        {
            this->_mIsEdited = true;
            this->_mUi->action_CloseDatabase->setEnabled(true);
            this->_mUi->action_OpenDatabase->setEnabled(false);
            this->_mUi->statusBar->showMessage(Path);
        }
        else
            {
            #ifdef QT_DEBUG
                qDebug() << "[ERROR] Unable to create database: " << this->_mDatabase.GetLastError();
            #endif

            QMessageBox::critical(this, tr("Fehler"), tr("Datenbank kann nicht erzeugt werden!"), QMessageBox::Ok);
        }

        this->_updateStatusbar();
    }
}

void MainWindow::_openDatabase(void)
{
    QString Path = QFileDialog::getOpenFileName(this, tr("Datenbank öffnen"), QDir::homePath(), ("SQLite (*.sqlite3)"));
    if(Path.length() > 0)
    {
        if(this->_mDatabase.Open(Path))
        {
            this->_mUi->action_CloseDatabase->setEnabled(true);
            this->_mUi->action_OpenDatabase->setEnabled(false);
            this->_mUi->statusBar->showMessage(Path);
            this->_readRecipesFromDatabase();
        }
        else
        {
            #ifdef QT_DEBUG
                qDebug() << "[ERROR] Unable to open database: " << this->_mDatabase.GetLastError();
            #endif

            QMessageBox::critical(this, tr("Fehler"), tr("Datenbank kann nicht geöffnet werden!"), QMessageBox::Ok);
        }

        this->_updateStatusbar();
    }
}

void MainWindow::_closeDatabase(void)
{
    if(this->_mDatabase.IsOpen())
    {
        if(this->_mIsEdited)
        {
            QMessageBox MessageBox(QMessageBox::Question, tr("Datenbank speichern?"), tr("Soll die Datenbank gespeichert werden?"), QMessageBox::Yes | QMessageBox::No, this);
            MessageBox.setButtonText(QMessageBox::Yes, tr("Ja"));
            MessageBox.setButtonText(QMessageBox::No, tr("Nein"));
            if(MessageBox.exec() == QMessageBox::Yes)
            {
                this->_writeRecipesToDatabase();
            }
        }

        this->_mDatabase.Close();

        this->_mUi->action_CloseDatabase->setEnabled(false);
        this->_mUi->action_OpenDatabase->setEnabled(true);
    }

    this->_updateStatusbar();
}

void MainWindow::_readRecipesFromDatabase(void)
{
    if(this->_mDatabase.IsOpen())
    {
        // Clear the table view
        this->_mUi->tableView_Recipes->model()->removeRows(0, this->_mUi->tableView_Recipes->model()->rowCount());

        // Get the data from the database
        QList<Recipe> Recipes;
        if(this->_mDatabase.Read(Recipes))
        {
            // Add the recipes to the model
            this->_mUi->tableView_Recipes->model()->insertRows(0, Recipes.size());
            for(int i = 0x00; i < Recipes.size(); i++)
            {
                this->_mUi->tableView_Recipes->model()->setData(this->_mUi->tableView_Recipes->model()->index(i, 0), QVariant::fromValue<Recipe>(Recipes.at(i)));
            }
        }
        else
        {
            QMessageBox::critical(this, tr("Fehler"), tr("Datenbank kann nicht gelesen werden!"), QMessageBox::Ok);
        }
    }
}

void MainWindow::_writeRecipesToDatabase(void)
{
    if(this->_mDatabase.IsOpen())
    {
        // Get the data from the model
        QList<Recipe> Recipes;
        for(int i = 0x00; i < this->_mUi->tableView_Recipes->model()->rowCount(); i++)
        {
            Recipes.push_back(qvariant_cast<Recipe>(this->_mUi->tableView_Recipes->model()->data(this->_mUi->tableView_Recipes->model()->index(i, 0), Qt::UserRole)));
        }

        // Write the recipes into the database
        if(!this->_mDatabase.Write(Recipes))
        {
            QMessageBox::critical(this, tr("Fehler"), tr("Datenbank kann nicht geschrieben werden!"), QMessageBox::Ok);
        }
        else
        {
            this->_mIsEdited = false;
        }
    }
}

void MainWindow::_updateStatusbar(void)
{
    QString FileName;

    if(this->_mDatabase.IsOpen())
    {
        FileName = ":/Icons/Ressources/Icons/icons8-connected-24.png";
    }
    else
    {
        FileName = ":/Icons/Ressources/Icons/icons8-disconnected-24.png";
    }

    QPixmap Pixmap;
    Pixmap.load(FileName);
    this->_mDatabaseState->setPixmap(Pixmap);
}

void MainWindow::_exportRecipe(void)
{
    if(this->_mUi->tableView_Recipes->selectionModel()->hasSelection() & !this->_mUi->tableView_Recipes->selectionModel()->selectedRows().empty())
    {
        QString File = QFileDialog::getSaveFileName(this, tr("Speicherort wählen"), QDir::homePath(), ("JSON (*.json)"));
        if(File.length() > 0)
        {
            QList<Recipe> SelectedRecipes;

            QModelIndexList Selection = this->_mUi->tableView_Recipes->selectionModel()->selectedRows();
            for(int i = 0x00; i < Selection.count(); i++)
            {
                SelectedRecipes.push_back(qvariant_cast<Recipe>(this->_mUi->tableView_Recipes->model()->data(Selection.at(i), Qt::UserRole)));
            }

            if(!this->_mRecipeExport.Export(File, SelectedRecipes))
            {
                #ifdef QT_DEBUG
                    qDebug() << "[ERROR] Unable to write JSON database!";
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
