#ifndef RECIPEEXPORT_H
#define RECIPEEXPORT_H

#include <QString>
#include <QSqlRecord>
#include <QJsonArray>
#include <QFileDialog>
#include <QJsonObject>
#include <QJsonDocument>

#ifdef QT_DEBUG
    #include <QtDebug>
#endif

#include "recipe.h"

class RecipeExport
{
    public:
        RecipeExport();

        bool Export(QString FileName, QList<QSqlRecord> Recipes);

    private:
        void _writeIngredient(QJsonObject& JSON, Ingredient Ingredient) const;
};

#endif // RECIPEEXPORT_H
