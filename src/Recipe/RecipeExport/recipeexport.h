#ifndef RECIPEEXPORT_H
#define RECIPEEXPORT_H

#include <QString>
#include <QJsonArray>
#include <QFileDialog>
#include <QJsonObject>
#include <QJsonDocument>

#include "recipe.h"

#ifdef QT_DEBUG
    #include <QtDebug>
#endif

class RecipeExport
{
    public:
        RecipeExport();

        bool Export(QString FileName, QList<Recipe> Recipes);

    private:
        void _writeIngredient(QJsonObject& JSON, Ingredient Ingredient) const;
};

#endif // RECIPEEXPORT_H
