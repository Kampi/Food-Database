#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) :   QMainWindow(parent),
                                            _mUi(new Ui::MainWindow),
                                            _mDatabaseState(new QLabel(this)),
                                            _mRecipesModel(new RecipesModel(_mRecipes, this)),
                                            _mIsEdited(false)
{
    _mUi->setupUi(this);

    this->setWindowFlags(this->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
    this->statusBar()->setSizeGripEnabled(false);
    if(this->_loadSettings())
    {
        this->_updateStatusbar();
        this->_setLanguageMenu();

        _mUi->tableView_Recipes->setModel(_mRecipesModel);
        _mUi->tableView_Recipes->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        _mUi->tableView_Recipes->horizontalHeader()->setSectionsClickable(false);

        _mUi->statusbar->addPermanentWidget(&_mStatusLabel);
        _mUi->statusbar->layout()->setContentsMargins(10,0,0,0);
        _mUi->statusbar->addPermanentWidget(&_mDatabaseState);
        _mUi->statusbar->setStyleSheet("margin-right: 10px;");

        // Create some dummy recipes
        QList<Ingredient> Ingredients({Ingredient(tr("Erste"), tr("Notiz"), 1, "kg", 1.55, _mCategories.value("Ingredients", QStringList("")).at(0)), Ingredient(tr("Zweite"), tr("Mehr"), 1, "l", 1.0, _mCategories.value("Ingredients", QStringList("")).at(1))});
        _mRecipes.push_back(Recipe(tr("Erstes Rezept"), tr("Notiz"), tr("Link zu Rezept 1"), tr("Kurze Beschreibung für Rezept 1."), tr("Beschreibung erste Stoppuhr."), tr("Beschreibung zweite Stoppuhr."), _mCategories.value("Recipes", QStringList("")).at(0), 1, 30, 10, 20, Ingredients));
        _mRecipes.push_back(Recipe(tr("Zweites Rezept"), tr("Notiz"), ("Link zu Rezept 2"), tr("Kurze Beschreibung für Rezept 2."), tr("Beschreibung erste Stoppuhr."), tr("Beschreibung zweite Stoppuhr."), _mCategories.value("Recipes", QStringList("")).at(1), 1, 10, 30, 40, Ingredients));
        _mRecipesModel->layoutChanged();
        _mUi->tableView_Recipes->scrollToBottom();
    }
    else
    {
        exit(-1);
    }
}

MainWindow::~MainWindow()
{
    delete _mRecipesModel;
    delete _mUi;
}

void MainWindow::CreateRecipe_finished(int result)
{
    if(result == 1)
    {
        RecipeDialog* Dialog = qobject_cast<RecipeDialog*>(sender());
        _mRecipes.push_back(Dialog->recipe());
        _mRecipesModel->insertRow(_mRecipesModel->rowCount(QModelIndex()));
        _mRecipesModel->layoutChanged();
        _mUi->tableView_Recipes->scrollToBottom();
        _mIsEdited = true;
    }
}

void MainWindow::EditRecipe_finished(int result)
{
    if(result == 1)
    {
        RecipeDialog* Dialog = qobject_cast<RecipeDialog*>(sender());
        _mRecipes.replace(_mUi->tableView_Recipes->currentIndex().row(), Dialog->recipe());
        _mIsEdited = true;
    }
}

void MainWindow::Settings_finished(int result)
{
    if(result == 1)
    {
        SettingsDialog* Dialog = qobject_cast<SettingsDialog*>(sender());
        _mCategories = Dialog->categories();
        this->_saveSettings();
    }
}

void MainWindow::on_tableView_Recipes_doubleClicked(const QModelIndex& index)
{
    this->_editRecipe(index.row());
}

void MainWindow::on_action_CreateDatabase_triggered()
{
    this->_createDatabase();
}

void MainWindow::on_action_OpenDatabase_triggered()
{
    this->_openDatabase();
}

void MainWindow::on_action_CloseDatabase_triggered()
{
    this->_closeDatabase();
}

void MainWindow::on_action_WriteDatabase_triggered()
{
    this->_writeRecipesToDatabase();
}

void MainWindow::on_action_NewRecipe_triggered()
{
    this->_createRecipe();
}

void MainWindow::on_action_RemoveRecipe_triggered()
{
    this->_removeRecipe();
}

void MainWindow::on_action_ExportRecipe_triggered()
{
    this->_exportRecipe();
}

void MainWindow::on_action_Settings_triggered()
{
    SettingsDialog* Dialog = new SettingsDialog(_mCategories, this);
    connect(Dialog, &QDialog::finished, this, &MainWindow::Settings_finished);
    Dialog->show();
}

void MainWindow::on_action_About_triggered()
{
    AboutDialog* Dialog = new AboutDialog(this);
    Dialog->show();
}

void MainWindow::on_action_AboutQt_triggered()
{
    QMessageBox::aboutQt(this, tr("Über Qt"));
}

void MainWindow::on_pushButton_NewRecipe_clicked()
{
    this->_createRecipe();
}

void MainWindow::on_pushButton_ExportRecipe_clicked()
{
    this->_exportRecipe();
}

void MainWindow::on_pushButton_RemoveRecipe_clicked()
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
                _mUi->retranslateUi(this);
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
    Settings.setValue("Recipes/Categories", _mCategories.value("Recipes", QStringList("")));
    Settings.setValue("Ingredients/Categories", _mCategories.value("Ingredients", QStringList("")));
    Settings.setValue("Units/Categories", _mCategories.value("Units", QStringList("")));
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

        foreach(auto Key, _mCategories.keys())
        {
            if(_mCategories.value(Key).size() == 0)
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
        Ingredients.push_back(Ingredient(QString(tr("Zutat")).append(QString(" %1").arg(i)), tr("Leere Notiz"), i, _mCategories.value("Units", QStringList("")).at(0), 1.25 * i, _mCategories.value("Ingredients", QStringList("")).at(0)));
    }

    // Generate a dummy recipe
    Recipe Template = Recipe(tr("Neues Rezept"), tr("Leere Notiz"), tr("Leerer Link"), tr("Leere Beschreibung"), tr("Stoppuhr 1"), tr("Stoppuhr 2"), _mCategories.value("Recipes", QStringList("")).at(0), 1, 10, 20, 30, Ingredients);

    RecipeDialog* Dialog = new RecipeDialog(_mCategories, Template, this);
    Dialog->setWindowTitle(tr("Neues Rezept"));
    connect(Dialog, &QDialog::finished, this, &MainWindow::CreateRecipe_finished);
    Dialog->show();
}

void MainWindow::_removeRecipe(void)
{
    QItemSelectionModel* select = _mUi->tableView_Recipes->selectionModel();

    if(select->hasSelection() & !select->selectedRows().empty())
    {
        _mRecipesModel->removeRows(select->currentIndex().row(), 1);
    }
    else
    {
        QMessageBox::information(this, tr("Hinweis"), tr("Bitte mindenstens ein Rezept auswählen!"), QMessageBox::Ok);
    }
}

void MainWindow::_editRecipe(int Index)
{
    RecipeDialog* Dialog = new RecipeDialog(_mCategories, _mRecipes.at(Index), this);
    Dialog->setWindowTitle(tr("Rezept bearbeiten"));
    connect(Dialog, &QDialog::finished, this, &MainWindow::EditRecipe_finished);
    Dialog->show();
}

void MainWindow::_createDatabase(void)
{
    QString Path = QFileDialog::getSaveFileName(this, tr("Datenbank anlegen"), tr("Rezepte"), ("SQLite (*.sqlite3)"));
    if(Path.length() > 0)
    {
        if(_mDatabase.Create(Path))
        {
            _mIsEdited = true;
            _mUi->action_CloseDatabase->setEnabled(true);
            _mUi->action_OpenDatabase->setEnabled(false);
            _mStatus = Path;
        }
        else
        {
            qDebug() << "[Error] Unable to create database: " << _mDatabase.GetLastError();
            QMessageBox::critical(this, tr("Fehler"), tr("Datenbank kann nicht erzeugt werden!"), QMessageBox::Ok);
        }

        this->_updateStatusbar();
    }
}

void MainWindow::_openDatabase(void)
{
    QString Path = QFileDialog::getOpenFileName(this, tr("Datenbank öffnen"), tr("Rezepte"), ("SQLite (*.sqlite3)"));
    if(Path.length() > 0)
    {
        if(_mDatabase.Open(Path))
        {
            _mUi->action_CloseDatabase->setEnabled(true);
            _mUi->action_OpenDatabase->setEnabled(false);
            _mStatus = Path;
            _readRecipesFromDatabase();
        }
        else
        {
            qDebug() << "[Error] Unable to open database: " << _mDatabase.GetLastError();
            QMessageBox::critical(this, tr("Fehler"), tr("Datenbank kann nicht geöffnet werden!"), QMessageBox::Ok);
        }

        this->_updateStatusbar();
    }
}

void MainWindow::_closeDatabase(void)
{
    if(_mDatabase.IsOpen())
    {
        if(_mIsEdited)
        {
            QMessageBox MessageBox(QMessageBox::Question, tr("Datenbank speichern?"), tr("Soll die Datenbank gespeichert werden?"), QMessageBox::Yes | QMessageBox::No, this);
            MessageBox.setButtonText(QMessageBox::Yes, tr("Ja"));
            MessageBox.setButtonText(QMessageBox::No, tr("Nein"));
            if(MessageBox.exec() == QMessageBox::Yes)
            {
                this->_writeRecipesToDatabase();
            }
        }

        _mStatus = "";
        _mDatabase.Close();

        _mUi->action_CloseDatabase->setEnabled(false);
        _mUi->action_OpenDatabase->setEnabled(true);
    }

    this->_updateStatusbar();
}

void MainWindow::_readRecipesFromDatabase(void)
{
    if(_mDatabase.IsOpen())
    {
        this->_mRecipesModel->clear();
        this->_mRecipes.clear();

        if(_mDatabase.Read(_mRecipes))
        {
            _mRecipesModel->layoutChanged();
            _mUi->tableView_Recipes->scrollToBottom();
        }
        else
        {
            QMessageBox::critical(this, tr("Fehler"), tr("Datenbank kann nicht gelesen werden!"), QMessageBox::Ok);
        }
    }
}

void MainWindow::_writeRecipesToDatabase(void)
{
    if(_mDatabase.IsOpen())
    {
        if(!_mDatabase.Write(_mRecipes))
        {
            QMessageBox::critical(this, tr("Fehler"), tr("Datenbank kann nicht geschrieben werden!"), QMessageBox::Ok);
        }
        else
        {
            _mIsEdited = false;
        }
    }
}

void MainWindow::_updateStatusbar(void)
{
    QString FileName;

    if(_mDatabase.IsOpen())
    {
        FileName = ":/Icon/Ressources/Icons/icons8-connected-24.png";
    }
    else
    {
        FileName = ":/Icon/Ressources/Icons/icons8-disconnected-24.png";
    }

    QPixmap Pixmap;
    _mStatusLabel.setText(_mStatus);
    Pixmap.load(FileName);
    _mDatabaseState.setPixmap(Pixmap);
}

void MainWindow::_exportRecipe(void)
{
    QItemSelectionModel* Selection = _mUi->tableView_Recipes->selectionModel();

    if(Selection->hasSelection() & !Selection->selectedRows().empty())
    {
        QString File = QFileDialog::getSaveFileName(this, tr("Speicherort wählen"), tr("Rezepte"), ("JSON (*.json)"));
        if(File.length() > 0)
        {
            int First = Selection->selectedRows().first().row();
            int Last = First + Selection->selectedRows().size();
            QList<Recipe> SelectionList(_mRecipes.mid(First, Last));
            if(!_mRecipeExport.Export(File, SelectionList))
            {
                qDebug() << "[Error] Unable to write JSON database!";
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
    actionGroup->addAction(_mUi->action_German);
    actionGroup->addAction(_mUi->action_EnglishUS);
    actionGroup->setExclusive(true);

    connect(_mUi->action_German, &QAction::triggered, this, &MainWindow::languageChange);
    connect(_mUi->action_EnglishUS, &QAction::triggered, this, &MainWindow::languageChange);

    // Use german as default language
    _mUi->action_German->triggered(true);
}

void MainWindow::_switchLanguage(QString Language)
{
    QLocale::setDefault(QLocale(Language));

    qApp->removeTranslator(&_mTranslator);

    if(_mTranslator.load(QString(QApplication::applicationDirPath().append(QString("/Languages/FoodDatabase_%1.qm").arg(Language)))))
    {
        qApp->installTranslator(&_mTranslator);
    }
}

void MainWindow::_addCategory(QStringList Settings, QString Name)
{
    Settings.sort();
    _mCategories.insert(Name, Settings);
}
