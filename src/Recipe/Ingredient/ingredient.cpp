#include "ingredient.h"

Ingredient::Ingredient(void)
{
}

Ingredient::Ingredient(QString Name, QString Note, double Amount, QString Unit, double Price, QString Section) : _mName(Name),
                                                                                                                 _mNote(Note),
                                                                                                                 _mUnit(Unit),
                                                                                                                 _mSection(Section),
                                                                                                                 _mAmount(Amount),
                                                                                                                 _mPrice(Price)
{
}

QString Ingredient::Note() const
{
    return _mNote;
}

void Ingredient::setNote(const QString& Note)
{
    _mNote = Note;
}

QString Ingredient::Unit() const
{
    return _mUnit;
}

void Ingredient::setUnit(const QString& Unit)
{
    _mUnit = Unit;
}

QString Ingredient::Section() const
{
    return _mSection;
}

void Ingredient::setSection(const QString& Section)
{
    _mSection = Section;
}

double Ingredient::Amount() const
{
    return _mAmount;
}

void Ingredient::setAmount(double Amount)
{
    _mAmount = Amount;
}

double Ingredient::Price() const
{
    return _mPrice;
}

void Ingredient::setPrice(double Price)
{
    _mPrice = Price;
}

QString Ingredient::Name() const
{
    return _mName;
}

void Ingredient::setName(const QString& Name)
{
    _mName = Name;
}

QDebug operator<<(QDebug Stream, const Ingredient& Ingredient)
{
    Stream << "Name: " << Ingredient._mName << endl;
    Stream << "Note: " << Ingredient._mNote << endl;
    Stream << "Amount: " << Ingredient._mAmount << endl;
    Stream << "Unit: " << Ingredient._mUnit << endl;
    Stream << "Price: " << Ingredient._mPrice << endl;
    Stream << "Section: " << Ingredient._mSection << endl;

    return Stream;
}

QTextStream& operator<<(QTextStream& Stream, const Ingredient& Ingredient)
{
    Stream << "Name: " << Ingredient._mName << endl;
    Stream << "Note: " << Ingredient._mNote << endl;
    Stream << "Amount: " << Ingredient._mAmount << endl;
    Stream << "Unit: " << Ingredient._mUnit << endl;
    Stream << "Price: " << Ingredient._mPrice << endl;
    Stream << "Section: " << Ingredient._mSection << endl;

    return Stream;
}
