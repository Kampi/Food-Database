#ifndef CREATERECIPEDIALOG_H
#define CREATERECIPEDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QAbstractButton>

#include "recipe.h"
#include "ingredientmodel.h"
#include "comboboxdelegate.h"
#include "numbersonlydelegate.h"

namespace Ui
{
    class CreateRecipe;
}

class CreateRecipeDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit CreateRecipeDialog(QStringList& RecipeCategories, QStringList& IngredientCategories, QList<Recipe>& Recipes, QWidget* parent = nullptr);
        ~CreateRecipeDialog();

    private slots:
        void on_pushButton_AddIngredients_clicked();
        void on_pushButton_RemoveIngredients_clicked();
        void on_buttonBox_Close_clicked(QAbstractButton* button);
        void on_spinBox_Timer1_valueChanged(int arg1);
        void on_spinBox_Timer2_valueChanged(int arg1);

    private:
        Ui::CreateRecipe* _mUi;

        QList<Ingredient> _mIngredientsList;
        QList<Recipe>& _mRecipes;
        QStringList _mRecipeCategories;

        IngredientModel* _mIngredientsModel;
        ComboBoxDelegate* _mComboBoxDelegate;
        NumbersOnlyDelegate* _mNumbersOnlyDelegate;

        void _addIngredient(QString Name = tr("<Name>"), QString Note = tr("<Note>"), uint Amount = 0, QString Unit = tr("<Unit>"), double Price = 0.0, QString Section = tr("<Section>"));
};

#endif // CREATERECIPEDIALOG_H
