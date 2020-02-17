#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) :   QMainWindow(parent),
                                            _mUi(new Ui::MainWindow),
                                            _mDatabaseState(new QLabel()),
                                            _mRecipesModel(new RecipesModel(_mRecipes)),
                                            _mIsEdited(false)
{
    _mUi->setupUi(this);
    this->setWindowFlags(this->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);

    _mUi->tableView_Recipes->setModel(_mRecipesModel);
    _mUi->tableView_Recipes->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    _mUi->tableView_Recipes->horizontalHeader()->setSectionsClickable(false);

    this->_setLanguageMenu();

    _mUi->statusbar->addWidget(&_mStatusLabel);
    _mUi->statusbar->addPermanentWidget(&_mDatabaseState);
    this->_updateStatusbar();

    _mIngredientsCategorie = QStringList() << tr("1") << tr("2") << tr("3") << tr("4") << tr("5");

    // Create some dummy data
    QList<Ingredient> Ingredients({Ingredient(tr("First"), tr("Note"), 1, "kg", 1.55, _mIngredientsCategorie.at(0)), Ingredient(tr("Second"), tr("More"), 1, "l", 1.0, _mIngredientsCategorie.at(1))});
    _mRecipes.push_back(Recipe(tr("First recipe"), tr("Some note"), tr("Link to first recipe"), tr("Short description for recipe one."), "", "", tr("Category 1"), 1, 30, 0, 0, Ingredients));
    _mRecipes.push_back(Recipe(tr("Second recipe"), tr("Some note"), ("Link to second recipe"), tr("Short description for recipe two."), "", "", tr("Category 2"), 1, 10, 0, 0, Ingredients));
    _mRecipesModel->layoutChanged();
    _mUi->tableView_Recipes->scrollToBottom();
}

MainWindow::~MainWindow()
{
    delete _mRecipesModel;
    delete _mUi;
}

void MainWindow::on_CreateRecipeDialog_finished(int result)
{
    if(result == 1)
    {
        _mIsEdited = true;
        _mRecipesModel->insertRow(_mRecipesModel->rowCount(QModelIndex()));
        _mRecipesModel->layoutChanged();
        _mUi->tableView_Recipes->scrollToBottom();
    }
}

void MainWindow::on_EditRecipeDialog_finished(int result)
{
    if(result == 1)
    {
        _mIsEdited = true;
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

void MainWindow::on_action_New_triggered()
{
    this->_createRecipe();
}

void MainWindow::on_action_Export_triggered()
{
    this->_exportRecipe();
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

void MainWindow::on_pushButton_New_clicked()
{
    this->_createRecipe();
}

void MainWindow::on_pushButton_Export_clicked()
{
    this->_exportRecipe();
}

void MainWindow::on_pushButton_RemoveRecipe_clicked()
{
    QItemSelectionModel* select = _mUi->tableView_Recipes->selectionModel();

    if(select->hasSelection() & !select->selectedRows().empty())
    {
        _mRecipesModel->removeRows(select->currentIndex().row(), 1);
    }
}

void MainWindow::languageChange(bool)
{
    QAction* Sender = qobject_cast<QAction*>(QObject::sender());
    Sender->setChecked(true);
    this->_switchLanguage(Sender->iconText());
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

void MainWindow::_createRecipe(void)
{
    CreateRecipeDialog* _mCreateDialog = new CreateRecipeDialog(_mIngredientsCategorie, _mRecipes, this);
    connect(_mCreateDialog, &QDialog::finished, this, &MainWindow::on_CreateRecipeDialog_finished);
    _mCreateDialog->show();
}

void MainWindow::_editRecipe(int Recipe)
{
    EditRecipeDialog* _mEditDialog = new EditRecipeDialog(_mIngredientsCategorie, _mRecipes[Recipe], this);
    connect(_mEditDialog, &QDialog::finished, this, &MainWindow::on_EditRecipeDialog_finished);
    _mEditDialog->show();
}

void MainWindow::_createDatabase(void)
{
    QString Path = QFileDialog::getSaveFileName(this, tr("Datenbank anlegen"), tr("Rezepte"), ("SQLite (*.sqlite3)"));
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

void MainWindow::_openDatabase(void)
{
    QString Path = QFileDialog::getOpenFileName(this, tr("Datenbank öffnen"), tr("Rezepte"), ("SQLite (*.sqlite3)"));
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
    QPixmap PixMap(16, 16);

    if(_mDatabase.IsOpen())
    {
        PixMap.fill(Qt::green);
    }
    else
    {
        PixMap.fill(Qt::red);
    }

    _mStatusLabel.setText(_mStatus);
    _mDatabaseState.setPixmap(PixMap);
}

void MainWindow::_exportRecipe(void)
{
    QItemSelectionModel* select = _mUi->tableView_Recipes->selectionModel();

    if(select->hasSelection() & !select->selectedRows().empty())
    {
        int First = select->selectedRows().first().row();
        int Last = First + select->selectedRows().size();
        QList<Recipe> SelectionList(_mRecipes.mid(First, Last));
        if(!_mRecipeExport.Export(QFileDialog::getSaveFileName(this, tr("Speicherort wählen"), tr("Rezepte"), ("JSON (*.json)")),  SelectionList))
        {
            qDebug() << "[Error] Unable to write JSON database!";
            QMessageBox::critical(this, tr("Fehler"), tr("Export fehlgeschlagen!"), QMessageBox::Ok);
        }
    }    else
    {
        QMessageBox::information(this, tr("Information"), tr("Please select at least one recipe!"), QMessageBox::Ok);
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
