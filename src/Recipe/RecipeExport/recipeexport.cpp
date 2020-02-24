#include "recipeexport.h"

RecipeExport::RecipeExport()
{
}

bool RecipeExport::Export(QString FileName, QList<Recipe> Recipes)
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
        foreach(Recipe recipe, Recipes)
        {
            if(Categories.indexOf(recipe.Category()) == -1)
            {
                Categories.push_back(recipe.Category());
            }
        }

        QJsonObject root;
        QJsonArray RecipeCategories;

        foreach(QString category, Categories)
        {
            QJsonObject RecipeCategoryArray;
            RecipeCategoryArray.insert("name", category);

            QJsonArray RecipesArray;
            foreach(Recipe recipe, Recipes)
            {
                if(recipe.Category() == category)
                {
                    QJsonObject RecipeObject;
                    RecipeObject.insert("name", recipe.Name());
                    RecipeObject.insert("note", recipe.Note());
                    RecipeObject.insert("link", recipe.Link());
                    RecipeObject.insert("persons", QString("%1").arg(recipe.Persons()));
                    RecipeObject.insert("cooking_description", recipe.Description());
                    RecipeObject.insert("cooking_time", QString("%1").arg(recipe.CookingTime()));
                    RecipeObject.insert("timer1_name", recipe.Timer1Name());
                    RecipeObject.insert("timer2_name", recipe.Timer2Name());
                    RecipeObject.insert("timer1_seconds", QString("%1").arg(recipe.Timer1Value()));
                    RecipeObject.insert("timer2_seconds", QString("%1").arg(recipe.Timer2Value()));

                    // Create the ingredients list
                    QJsonArray IngredientsArray;
                    foreach(Ingredient ingredient, recipe.Ingredients())
                    {
                        QJsonObject Ingredient;
                        this->_writeIngredient(Ingredient, ingredient);
                        IngredientsArray.push_back(Ingredient);
                    }

                    // Write the ingredients list
                    RecipeObject.insert("ingredients", IngredientsArray);
                    RecipesArray.push_back(RecipeObject);
                }
            }

            RecipeCategoryArray.insert("recipes", RecipesArray);
            RecipeCategories.push_back(RecipeCategoryArray);
        }
        root.insert("recipeCategories", RecipeCategories);

        QJsonDocument RecipeExport(root);
        SaveFile.write(RecipeExport.toJson());
    }

    return true;
}

void RecipeExport::_writeIngredient(QJsonObject &JSON, Ingredient Ingredient) const
{
    JSON.insert("name", Ingredient.Name());
    JSON.insert("note", Ingredient.Note());
    JSON.insert("amount", Ingredient.Amount());
    JSON.insert("amount_type", Ingredient.Unit());
    JSON.insert("price", Ingredient.Price());
    JSON.insert("section", Ingredient.Section());
}
