#ifndef EDITRECIPEDIALOG_H
#define EDITRECIPEDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QAbstractButton>

#include "recipe.h"
#include "ingredientmodel.h"
#include "comboboxdelegate.h"
#include "numbersonlydelegate.h"

namespace Ui
{
    class EditRecipeDialog;
}

class EditRecipeDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit EditRecipeDialog(QStringList& RecipeCategories, QStringList& IngredientCategories, Recipe& OldRecipe, QWidget* parent = nullptr);
        ~EditRecipeDialog();

    private slots:
        void on_buttonBox_Close_clicked(QAbstractButton* button);
        void on_lineEdit_Name_textChanged(const QString& arg1);
        void on_comboBox_Category_currentIndexChanged(const QString& arg1);
        void on_lineEdit_Note_textEdited(const QString& arg1);
        void on_lineEdit_Link_textEdited(const QString& arg1);
        void on_spinBox_Persons_valueChanged(int arg1);
        void on_spinBox_CookingTime_valueChanged(int arg1);
        void on_spinBox_Timer1_valueChanged(int arg1);
        void on_lineEdit_Timer1_textEdited(const QString& arg1);
        void on_spinBox_Timer2_valueChanged(int arg1);
        void on_lineEdit_Timer2_textEdited(const QString& arg1);
        void on_plainTextEdit_Description_textChanged();
        void on_pushButton_AddIngredients_clicked();
        void on_pushButton_RemoveIngredients_clicked();

    private:
        Ui::EditRecipeDialog* _mUi;

        QList<Ingredient> _mNewIngredients;
        QStringList _mRecipeCategories;

        IngredientModel* _mIngredientsModel;
        ComboBoxDelegate* _mComboBoxDelegate;
        NumbersOnlyDelegate* _mNumbersOnlyDelegate;
        Recipe& _mOldRecipe;
        Recipe _mNewRecipe;
};

#endif // EDITRECIPEDIALOG_H
