#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QDebug>
#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>

#include "About/aboutdialog.h"
#include "EditRecipe/editrecipedialog.h"
#include "CreateRecipe/createrecipedialog.h"

#include "recipe.h"
#include "database.h"
#include "recipesmodel.h"
#include "recipeexport.h"

/*
 *  ToDo:
 *      - Change column width in ingredient model
 *      - Implement column resize after editing for create recipe window
 *      - Delegate for category in create recipe window?
 *      - Implement sorting for recipes?
 *      - Add Category list
 */

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    protected slots:
        void languageChange(bool checked);

    protected:
        void changeEvent(QEvent* event) override;
        void closeEvent(QCloseEvent* event) override;
        void keyPressEvent(QKeyEvent* event) override;

    public slots:
        void on_CreateRecipeDialog_finished(int result);
        void on_EditRecipeDialog_finished(int result);
        void on_tableView_Recipes_doubleClicked(const QModelIndex& index);

    public:
        MainWindow(QWidget* parent = nullptr);
        ~MainWindow();

    private slots:
        void on_action_CreateDatabase_triggered();
        void on_action_OpenDatabase_triggered();
        void on_action_CloseDatabase_triggered();
        void on_action_WriteDatabase_triggered();
        void on_action_New_triggered();
        void on_action_Export_triggered();
        void on_action_About_triggered();
        void on_action_AboutQt_triggered();

        void on_pushButton_New_clicked();
        void on_pushButton_Export_clicked();
        void on_pushButton_RemoveRecipe_clicked();

    private:
        Ui::MainWindow* _mUi;

        QList<Recipe> _mRecipes;
        QLabel _mDatabaseState;
        QLabel _mStatusLabel;
        QTranslator _mTranslator;
        QStringList _mIngredientsCategorie;
        QString _mStatus;

        RecipesModel* _mRecipesModel;

        Database _mDatabase;
        RecipeExport _mRecipeExport;

        bool _mIsEdited;

        void _createRecipe(void);
        void _editRecipe(int Recipe);
        void _createDatabase(void);
        void _openDatabase(void);
        void _closeDatabase(void);
        void _readRecipesFromDatabase(void);
        void _writeRecipesToDatabase(void);
        void _updateStatusbar(void);
        void _exportRecipe(void);
        void _setLanguageMenu(void);
        void _switchLanguage(QString Language);
};
#endif // MAINWINDOW_H
