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

    _mUi->statusbar->addPermanentWidget(&_mDatabaseState);
    _mUi->statusbar->showMessage(QString("%1.%2.%3").arg(VERSION_MAJOR).arg(VERSION_MINOR).arg(VERSION_BUILD));
    this->_updateStatusbar();

    // Create some dummy data
    QList<Ingredient> Ingredients({Ingredient("First", "Note", 1, "kg", 1.55, "Things"), Ingredient("Second", "More", 1, "l", 1.0, "Another")});
    _mRecipes.push_back(Recipe("First recipe", "Some note", "A link", "Short description", "", "", "Category 1", 1, 30, 0, 0, Ingredients));
    _mRecipes.push_back(Recipe("Second recipe", "Some note", "A link", "Short description", "", "", "Category 2", 1, 10, 0, 0, Ingredients));
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
    QApplication::aboutQt();
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

void MainWindow::changeEvent(QEvent* event)
{
    if(event->type())
    {
        switch(event->type())
        {
            case QEvent::LanguageChange:
            {
                qDebug() << "Language changed!";
                break;
            }
            case QEvent::LocaleChange:
            {
                qDebug() << "Locale changed!";
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
    CreateRecipeDialog* _mCreateDialog = new CreateRecipeDialog(_mRecipes, this);
    connect(_mCreateDialog, &QDialog::finished, this, &MainWindow::on_CreateRecipeDialog_finished);
    _mCreateDialog->show();
}

void MainWindow::_editRecipe(int Recipe)
{
    EditRecipeDialog* _mEditDialog = new EditRecipeDialog(_mRecipes[Recipe], this);
    connect(_mEditDialog, &QDialog::finished, this, &MainWindow::on_EditRecipeDialog_finished);
    _mEditDialog->show();
}

void MainWindow::_createDatabase(void)
{
    if(_mDatabase.Create(QFileDialog::getSaveFileName(this, tr("Datenbank anlegen"), tr("Rezepte"), ("SQLite (*.sqlite3)"))))
    {
        _mIsEdited = true;
        _mUi->action_CloseDatabase->setEnabled(true);
        _mUi->action_OpenDatabase->setEnabled(false);
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
    if(_mDatabase.Open(QFileDialog::getOpenFileName(this, tr("Datenbank öffnen"), tr("Rezepte"), ("SQLite (*.sqlite3)"))))
    {
        _mUi->action_CloseDatabase->setEnabled(true);
        _mUi->action_OpenDatabase->setEnabled(false);

        this->_updateStatusbar();
        _readRecipesFromDatabase();
    }
    else
    {
        qDebug() << "[Error] Unable to open database: " << _mDatabase.GetLastError();
        QMessageBox::critical(this, tr("Fehler"), tr("Datenbank kann nicht geöffnet werden!"), QMessageBox::Ok);
    }
}

void MainWindow::_closeDatabase(void)
{
    if(_mDatabase.IsOpen())
    {
        if(_mIsEdited)
        {
            QMessageBox::StandardButton reply = QMessageBox::question(this, tr("Datenbank speichern?"), tr("Soll die Datenbank gespeichert werden?"), QMessageBox::Yes | QMessageBox::No);
            if(reply == QMessageBox::Yes)
            {
                this->_writeRecipesToDatabase();
            }
        }

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
    }
}
