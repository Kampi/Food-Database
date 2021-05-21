#ifndef RECIPE_H
#define RECIPE_H

#include <QList>
#include <QDebug>
#include <QSqlRecord>
#include <QTextStream>

#include "ingredient.h"

#ifdef QT_DEBUG
    #include <QtDebug>
#endif

class Recipe
{
    public:
        Recipe(void);
        Recipe(const Recipe& obj);
        Recipe(QString Name, QString Note, QString Link, QString Description, QString Timer1Name, QString Timer2Name, QString Category, uint Persons, uint CookingTime, uint Timer1Value, uint Timer2Value, QList<Ingredient> Ingredients);

        void Export(void);

        QString Name(void) const;
        void setName(const QString& Name);

        QString Note(void) const;
        void setNote(const QString& Note);

        QString Link(void) const;
        void setLink(const QString& Link);

        uint Persons(void) const;
        void setPersons(const uint& Persons);

        QString Description(void) const;
        void setDescription(const QString& Description);

        uint CookingTime(void) const;
        void setCookingTime(const uint& CookingTime);

        QString Timer1Name(void) const;
        void setTimer1Name(const QString& Timer1Name);

        QString Timer2Name(void) const;
        void setTimer2Name(const QString& Timer2Name);

        QString Category(void) const;
        void setCategory(const QString& Category);

        uint Timer1Value(void) const;
        void setTimer1Value(const uint& Timer1Value);

        uint Timer2Value(void) const;
        void setTimer2Value(const uint& Timer2Value);

        QList<Ingredient> Ingredients(void) const;
        void setIngredients(const QList<Ingredient>& Ingredients);

        #ifdef QT_DEBUG
                friend QDebug operator<<(QDebug Stream, const Recipe& Recipe);
        #endif

        friend QTextStream& operator<<(QTextStream& Stream, const Recipe& Recipe);

        friend bool operator==(const Recipe& First, const Recipe& Second);

    private:
        QString _mName;
        QString _mNote;
        QString _mLink;
        QString _mDescription;
        QString _mTimer1Name;
        QString _mTimer2Name;
        QString _mCategory;

        uint _mPersons;
        uint _mCookingTime;
        uint _mTimer1Value;
        uint _mTimer2Value;

        QList<Ingredient> _mIngredients;
};

Q_DECLARE_METATYPE(Recipe)

#endif // RECIPE_H
