#ifndef RECIPE_H
#define RECIPE_H

#include <QList>

#include "Ingredient/ingredient.h"

class Recipe
{
    public:
        Recipe(QString Name, QString Note, QString Link, QString Description, QString Timer1Name, QString Timer2Name, QString Category, uint Persons, uint CookingTime, uint Timer1Value, uint Timer2Value, QList<Ingredient> Ingredients);

        void Export(void);

        QString Name() const;
        void setName(const QString& Name);

        QString Note() const;
        void setNote(const QString& Note);

        QString Link() const;
        void setLink(const QString& Link);

        uint Persons() const;
        void setPersons(const uint& Persons);

        QString Description() const;
        void setDescription(const QString& Description);

        uint CookingTime() const;
        void setCookingTime(const uint& CookingTime);

        QString Timer1Name() const;
        void setTimer1Name(const QString& Timer1Name);

        QString Timer2Name() const;
        void setTimer2Name(const QString& Timer2Name);

        QString Category() const;
        void setCategory(const QString& Category);

        uint Timer1Value() const;
        void setTimer1Value(const uint& Timer1Value);

        uint Timer2Value() const;
        void setTimer2Value(const uint& Timer2Value);

        QList<Ingredient> Ingredients() const;
        void setIngredients(const QList<Ingredient>& Ingredients);

        friend QDebug operator<<(QDebug Stream, const Recipe& Recipe);
        friend QDataStream& operator<<(QDataStream& Stream, const Recipe& Recipe);

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

#endif // RECIPE_H
