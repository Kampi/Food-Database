#include "database.h"

Database::Database() : _mDatabase(QSqlDatabase::addDatabase("QSQLITE"))
{
}

bool Database::Create(QString FileName)
{
    if(FileName.length() > 0)
    {
        bool Result;
        QSqlQuery Query;

        _mDatabase.setDatabaseName(FileName);
        if(!_mDatabase.open())
        {
            return false;
        }

        Result = Query.exec("CREATE TABLE Recipes (ID INTEGER primary key, "
                                                  "Name VARCHAR(50), "
                                                  "Note VARCHAR(50), "
                                                  "Link MEDIUMTEXT, "
                                                  "Persons INTEGER, "
                                                  "Cooking_Description MEDIUMTEXT, "
                                                  "Category VARCHAR(50), "
                                                  "Cooking_Time INTEGER, "
                                                  "Timer1_Name VARCHAR(50), "
                                                  "Timer2_Name VARCHAR(50), "
                                                  "Timer1_Value INTEGER, "
                                                  "Timer2_Value INTEGER, "
                                                  "IngredientsStart INTEGER, "
                                                  "IngredientsCount INTEGER)"
                            );

        Result &= Query.exec("CREATE TABLE Ingredients (ID INTEGER primary key, "
                                                       "Name VARCHAR(50), "
                                                       "Note VARCHAR(50), "
                                                       "Amount INTEGER, "
                                                       "Unit VARCHAR(50), "
                                                       "Price REAL, "
                                                       "Section VARCHAR(50))"
                             );

        return Result;
    }

    return false;
}

bool Database::Open(QString FileName)
{
    if(FileName.length() > 0)
    {
        _mDatabase.setDatabaseName(FileName);
        return _mDatabase.open();
    }

    return false;
}

bool Database::Read(QList<Recipe>& Recipes)
{
    if(_mDatabase.isOpen())
    {
        QSqlQuery Query;
        QList<Ingredient> Ingredients;

        // First get the ingredients from the database
        if(Query.exec("SELECT ID, Name, Note, Amount, Unit, Price, Section FROM Ingredients"))
        {
            while(Query.next())
            {
                Ingredients.push_back(Ingredient(Query.value(1).toString(),
                                                 Query.value(2).toString(),
                                                 Query.value(3).toDouble(),
                                                 Query.value(4).toString(),
                                                 Query.value(5).toDouble(),
                                                 Query.value(6).toString()
                                                 ));
            }
        }
        else
        {
            qDebug() << "[Error] Unable to read ingredients table: " << _mDatabase.lastError();

            return false;
        }

        // Then get the recipes from the database
        if(Query.exec("SELECT ID, Name, Note, Link, Cooking_Description, Timer1_Name, Timer2_Name, Category, Persons, Cooking_Time, Timer1_Value, Timer2_Value, IngredientsStart, IngredientsCount FROM Recipes"))
        {
            while(Query.next())
            {
                QList<Ingredient> RecipeIngredients(Ingredients.mid(Query.value(12).toInt(), Query.value(13).toInt()));
                Recipes.push_back(Recipe(Query.value(1).toString(),
                                         Query.value(2).toString(),
                                         Query.value(3).toString(),
                                         Query.value(4).toString(),
                                         Query.value(5).toString(),
                                         Query.value(6).toString(),
                                         Query.value(7).toString(),
                                         Query.value(8).toUInt(),
                                         Query.value(9).toUInt(),
                                         Query.value(10).toUInt(),
                                         Query.value(11).toUInt(),
                                         RecipeIngredients
                                      )
                                  );
            }

            return true;
        }
        else
        {
            qDebug() << "[Error] Unable to read recipes table: " << _mDatabase.lastError();

            return false;
        }
    }

    return false;
}

bool Database::Write(QList<Recipe>& Recipes)
{
    if(_mDatabase.isOpen())
    {
        bool Result;
        QSqlQuery DeleteQuery;
        if(!DeleteQuery.exec("DELETE FROM Recipes") & DeleteQuery.exec("DELETE FROM Ingredients"))
        {
            qDebug() << "[Error] Unable to clear table: " << _mDatabase.lastError();

            return false;
        }

        int IngredientsStart = 0;
        foreach(Recipe recipe, Recipes)
        {
            QSqlQuery RecipeQuery;

            // Write the ingredients first
            foreach(Ingredient ingredient, recipe.Ingredients())
            {
                QSqlQuery IngredientQuery;

                IngredientQuery.prepare("INSERT INTO Ingredients (Name, Note, Amount, Unit, Price, Section) VALUES (:name, :note, :amount, :unit, :price, :section)");
                IngredientQuery.bindValue(":name", ingredient.Name());
                IngredientQuery.bindValue(":note", ingredient.Note());
                IngredientQuery.bindValue(":amount", ingredient.Amount());
                IngredientQuery.bindValue(":unit", ingredient.Unit());
                IngredientQuery.bindValue(":price", ingredient.Price());
                IngredientQuery.bindValue(":section", ingredient.Section());

                Result = IngredientQuery.exec();
                if(!Result)
                {
                    qDebug() << "[Error] Unable to write ingredients table: " << _mDatabase.lastError();

                    return false;
                }
            }

            RecipeQuery.prepare("INSERT INTO Recipes (Name, Note, Link, Cooking_Description, Timer1_Name, Timer2_Name, Category, Persons, Cooking_Time, Timer1_Value, Timer2_Value, IngredientsStart, IngredientsCount) "
                                "VALUES (:name, :note, :link, :description, :timer1_name, :timer2_name, :category, :persons, :cooking_time, :timer1_value, :timer2_value, :ingredientsstart, :ingredientscount)");

            RecipeQuery.bindValue(":name", recipe.Name());
            RecipeQuery.bindValue(":note", recipe.Note());
            RecipeQuery.bindValue(":link", recipe.Link());
            RecipeQuery.bindValue(":description", recipe.Description());
            RecipeQuery.bindValue(":timer1_name", recipe.Timer1Name());
            RecipeQuery.bindValue(":timer2_name", recipe.Timer2Name());
            RecipeQuery.bindValue(":category", recipe.Category());
            RecipeQuery.bindValue(":persons", recipe.Persons());
            RecipeQuery.bindValue(":cooking_time", recipe.CookingTime());
            RecipeQuery.bindValue(":timer1_value", recipe.Timer1Value());
            RecipeQuery.bindValue(":timer2_value", recipe.Timer2Value());
            RecipeQuery.bindValue(":ingredientsstart", IngredientsStart);
            RecipeQuery.bindValue(":ingredientscount", recipe.Ingredients().size());

            IngredientsStart += recipe.Ingredients().size();

            Result = RecipeQuery.exec();
            if(!Result)
            {
                qDebug() << "[Error] Unable to write recipes table: " << _mDatabase.lastError();

                return false;
            }
        }

        return true;
    }

    return false;
}

bool Database::IsOpen(void)
{
    return _mDatabase.isOpen();
}

QString Database::GetLastError(void)
{
    return _mDatabase.lastError().text();
}

void Database::Close(void)
{
    if(_mDatabase.isOpen())
    {
        _mDatabase.close();
    }
}
