#ifndef INGREDIENT_H
#define INGREDIENT_H

#include <QString>
#include <QTextStream>

#ifdef QT_DEBUG
    #include <QtDebug>
#endif

class Ingredient
{
    public:
        Ingredient(void);
        Ingredient(QString Name, QString Note, double Amount, QString Unit, double Price, QString Section);

        QString Name() const;
        void setName(const QString& Name);

        QString Note() const;
        void setNote(const QString& Note);

        QString Unit() const;
        void setUnit(const QString& Unit);

        QString Section() const;
        void setSection(const QString& Section);

        double Amount() const;
        void setAmount(double Amount);

        double Price() const;
        void setPrice(double Price);

        #ifdef QT_DEBUG
            friend QDebug operator<<(QDebug Stream, const Ingredient& Ingedient);
        #endif
        friend QTextStream& operator<<(QTextStream& Stream, const Ingredient& Ingedient);

    private:
        QString _mName;
        QString _mNote;
        QString _mUnit;
        QString _mSection;

        double _mAmount;
        double _mPrice;
};

Q_DECLARE_METATYPE(Ingredient)

#endif // INGREDIENT_H
