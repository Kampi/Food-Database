#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMap>
#include <QLabel>
#include <QKeyEvent>
#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QWidgetAction>

#include "aboutdialog.h"
#include "recipedialog.h"
#include "settingsdialog.h"

#include "recipe.h"
#include "database.h"
#include "recipesmodel.h"
#include "recipeexport.h"
#include "recipesproxy.h"

#ifdef QT_DEBUG
    #include <QtDebug>
#endif

/*
 *  ToDo:
 *      - Change column width in ingredient model
 *      - Implement column resize after editing for create recipe window
 *      - Implement sorting for recipes?
 *      - Tab flow for recipe table
 *      - Set value for ingredient cost to 0 after deleting
 *      - Validator for amount field in ingredient table
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
        void CreateRecipe_finished(int result);
        void EditRecipe_finished(int result);
        void Settings_finished(int result);
        void on_tableView_Recipes_doubleClicked(const QModelIndex& index);

    public:
        MainWindow(QWidget* parent = nullptr);
        ~MainWindow(void);

    private slots:
        void on_action_CreateDatabase_triggered(void);
        void on_action_OpenDatabase_triggered(void);
        void on_action_CloseDatabase_triggered(void);
        void on_action_WriteDatabase_triggered(void);
        void on_action_NewRecipe_triggered(void);
        void on_action_RemoveRecipe_triggered(void);
        void on_action_ExportRecipe_triggered(void);
        void on_action_Settings_triggered(void);
        void on_action_About_triggered(void);
        void on_action_AboutQt_triggered(void);
        void on_pushButton_NewRecipe_clicked(void);
        void on_pushButton_ExportRecipe_clicked(void);
        void on_pushButton_RemoveRecipe_clicked(void);

    private:
        Ui::MainWindow* _mUi;
        QLabel* _mDatabaseState;

        QTranslator _mTranslator;
        QMap<QString, QStringList> _mCategories;

        RecipesModel* _mRecipesModel;

        RecipesProxy* _mRecipesProxy;

        Database _mDatabase;
        RecipeExport _mRecipeExport;

        bool _mIsEdited;

        void _saveSettings(void);
        bool _loadSettings(void);
        void _createRecipe(void);
        void _removeRecipe(void);
        void _editRecipe(QModelIndex Index);
        void _createDatabase(void);
        void _openDatabase(void);
        void _closeDatabase(void);
        void _readRecipesFromDatabase(void);
        void _writeRecipesToDatabase(void);
        void _updateStatusbar(void);
        void _exportRecipe(void);
        void _setLanguageMenu(void);
        void _switchLanguage(QString Language);
        void _addCategory(QStringList Settings, QString Name);
};
#endif // MAINWINDOW_H
