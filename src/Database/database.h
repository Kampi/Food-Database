#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql>
#include <QString>
#include <QFileDialog>

#include "recipe.h"
#include "ingredient.h"

class Database
{
    public:
        Database();

        bool Create(QString FileName);
        bool Open(QString FileName);
        bool Read(QList<Recipe>& Recipes);
        bool Write(QList<Recipe>& Recipes);
        bool IsOpen(void);

        QString GetLastError(void);

        void Close(void);

    private:
        QSqlDatabase _mDatabase;
};

#endif // DATABASE_H