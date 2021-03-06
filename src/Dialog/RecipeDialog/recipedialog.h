#ifndef RECIPEDIALOG_H
#define RECIPEDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QAbstractButton>

#include "recipe.h"
#include "ingredientmodel.h"
#include "comboboxdelegate.h"
#include "numbersonlydelegate.h"

#ifdef QT_DEBUG
    #include <QtDebug>
#endif

namespace Ui
{
    class RecipeDialog;
}

class RecipeDialog : public QDialog
{
    Q_OBJECT

    signals:
        void RecipeChanged(int Result);

    public:
        explicit RecipeDialog(int ID, QMap<QString, QStringList> Categories, Recipe CurRecipe, QWidget* parent = nullptr);
        explicit RecipeDialog(QMap<QString, QStringList> Categories, Recipe CurRecipe, QWidget* parent = nullptr);
        ~RecipeDialog();

        Recipe recipe() const;
        uint id() const;

    private slots:
        void on_pushButton_AddIngredients_clicked();
        void on_pushButton_RemoveIngredients_clicked();
        void on_lineEdit_Name_textChanged(const QString& arg1);
        void on_comboBox_Category_currentTextChanged(const QString& arg1);
        void on_lineEdit_Note_textEdited(const QString& arg1);
        void on_lineEdit_Link_textEdited(const QString& arg1);
        void on_spinBox_Persons_valueChanged(int arg1);
        void on_spinBox_CookingTime_valueChanged(int arg1);
        void on_lineEdit_Timer1_textEdited(const QString& arg1);
        void on_timeEdit_Timer1_userTimeChanged(const QTime& time);
        void on_lineEdit_Timer2_textEdited(const QString& arg1);
        void on_timeEdit_Timer2_userTimeChanged(const QTime& time);
        void on_plainTextEdit_Description_textChanged();
        void on_buttonBox_Close_clicked(QAbstractButton* button);

    private:
        Ui::RecipeDialog* _mUi;

        uint _mID;

        QMap<QString, QStringList> _mCategories;

        Recipe _mRecipe;
        IngredientModel* _mIngredientsModel;
        ComboBoxDelegate* _mSectionsComboBox;
        ComboBoxDelegate* _mUnitComboBox;
        NumbersOnlyDelegate* _mNumbersOnlyDelegate;
};

#endif // RECIPEDIALOG_H
