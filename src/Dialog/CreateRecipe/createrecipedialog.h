#ifndef CREATERECIPEDIALOG_H
#define CREATERECIPEDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QAbstractButton>

#include "recipe.h"
#include "ingredientmodel.h"
#include "comboboxdelegate.h"

namespace Ui
{
    class CreateRecipe;
}

class CreateRecipeDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit CreateRecipeDialog(QList<Recipe>& Recipes, QWidget* parent = nullptr);
        ~CreateRecipeDialog();

    private slots:
        void on_pushButton_AddIngredients_clicked();
        void on_pushButton_RemoveIngredients_clicked();
        void on_buttonBox_Close_clicked(QAbstractButton* button);
        void on_spinBox_Timer1_valueChanged(int arg1);
        void on_spinBox_Timer2_valueChanged(int arg1);

    private:
        Ui::CreateRecipe* _mUi;

        IngredientModel* _mIngredientsModel;
        ComboBoxDelegate _mComboBoxDelegate;

        QList<Ingredient> _mIngredientsList;
        QList<Recipe>& _mRecipes;

        void _addIngredient(QString Name = "<Name>", QString Note = "<Note>", uint Amount = 0, QString Unit = "<Unit>", double Price = 0.0, QString Section = "<Section>");
};

#endif // CREATERECIPEDIALOG_H
