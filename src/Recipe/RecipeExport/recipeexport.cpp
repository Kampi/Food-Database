#include "recipeexport.h"

RecipeExport::RecipeExport()
{
}

bool RecipeExport::Export(QString FileName, QList<QSqlRecord> Recipes)
{
    if(FileName.length() > 0)
    {
        QFile SaveFile(FileName);
        QList<QString> Categories;

        if(!SaveFile.open(QIODevice::WriteOnly))
        {
            return false;
        }

        // Get the different categories
        foreach(QSqlRecord recipe, Recipes)
        {
            if(Categories.indexOf(recipe.value("Category").toString()) == -1)
            {
                Categories.push_back(recipe.value("Category").toString());
            }
        }

        QJsonObject Root;
        QJsonArray RecipeCategoriesArray;
        foreach(QString category, Categories)
        {
            QJsonObject RecipeCategoryObject;
            RecipeCategoryObject.insert("name", category);

            QJsonArray RecipesArray;
            foreach(QSqlRecord recipe, Recipes)
            {
                if(recipe.value("Category").toString() == category)
                {
                    QJsonObject RecipeObject;
                    QJsonArray IngredientsArray;

                    RecipeObject.insert("name", recipe.value("Name").toString());
                    RecipeObject.insert("note", recipe.value("Note").toString());
                    RecipeObject.insert("link", recipe.value("Link").toString());
                    RecipeObject.insert("persons", recipe.value("Persons").toString());
                    RecipeObject.insert("cooking_description", recipe.value("Description").toString());
                    RecipeObject.insert("cooking_time",recipe.value("Cooking_Time").toString());
                    RecipeObject.insert("timer1_name", recipe.value("Timer1_Name").toString());
                    RecipeObject.insert("timer2_name", recipe.value("Timer2_Name").toString());
                    RecipeObject.insert("timer1_seconds", recipe.value("Timer1_Value").toString());
                    RecipeObject.insert("timer2_seconds", recipe.value("Timer2_Value").toString());

                    // Write the ingredients array
                    QJsonDocument IngredientDoc = QJsonDocument::fromJson(recipe.value("Ingredients").toString().toUtf8());
                    QJsonObject JSON = IngredientDoc.object();

                    foreach(const QString& Ingredient, JSON.keys())
                    {
                        QJsonObject IngredientObject;

                        IngredientObject.insert("name", Ingredient);

                        IngredientObject.insert("amount", JSON.value(Ingredient).toObject().value("Amount").toDouble());
                        IngredientObject.insert("amount_type", JSON.value(Ingredient).toObject().value("Unit").toString());
                        IngredientObject.insert("note", JSON.value(Ingredient).toObject().value("Note").toString());
                        IngredientObject.insert("price", JSON.value(Ingredient).toObject().value("Price").toDouble());
                        IngredientObject.insert("section", JSON.value(Ingredient).toObject().value("Section").toString());

                        IngredientsArray.push_back(IngredientObject);
                    }

                    RecipeObject.insert("ingredients", IngredientsArray);
                    RecipesArray.push_back(RecipeObject);
                }
            }

            RecipeCategoryObject.insert("recipes", RecipesArray);
            RecipeCategoriesArray.push_back(RecipeCategoryObject);
        }
        Root.insert("recipeCategories", RecipeCategoriesArray);

        QJsonDocument RecipeExport(Root);
        SaveFile.write(RecipeExport.toJson());

        return true;
    }

    return false;
}

void RecipeExport::_writeIngredient(QJsonObject& JSON, Ingredient Ingredient) const
{
    JSON.insert("name", Ingredient.Name());
    JSON.insert("note", Ingredient.Note());
    JSON.insert("amount", Ingredient.Amount());
    JSON.insert("amount_type", Ingredient.Unit());
    JSON.insert("price", Ingredient.Price());
    JSON.insert("section", Ingredient.Section());
}
