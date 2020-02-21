#include "recipe.h"

Recipe::Recipe()
{
}

Recipe::Recipe(const Recipe& obj) : _mName(obj.Name()),
                                    _mNote(obj.Note()),
                                    _mLink(obj.Link()),
                                    _mDescription(obj.Description()),
                                    _mTimer1Name(obj.Timer1Name()),
                                    _mTimer2Name(obj.Timer2Name()),
                                    _mCategory(obj.Category()),
                                    _mPersons(obj.Persons()),
                                    _mCookingTime(obj.CookingTime()),
                                    _mTimer1Value(obj.Timer1Value()),
                                    _mTimer2Value(obj.Timer2Value()),
                                    _mIngredients(obj.Ingredients())
{
}

Recipe::Recipe(QString Name, QString Note, QString Link, QString Description, QString Timer1Name, QString Timer2Name, QString Category, uint Persons, uint CookingTime, uint Timer1Value, uint Timer2Value, QList<Ingredient> Ingredients) :  _mName(Name),
                                                                                                                                                                                                                                              _mNote(Note),
                                                                                                                                                                                                                                              _mLink(Link),
                                                                                                                                                                                                                                              _mDescription(Description),
                                                                                                                                                                                                                                              _mTimer1Name(Timer1Name),
                                                                                                                                                                                                                                              _mTimer2Name(Timer2Name),
                                                                                                                                                                                                                                              _mCategory(Category),
                                                                                                                                                                                                                                              _mPersons(Persons),
                                                                                                                                                                                                                                              _mCookingTime(CookingTime),
                                                                                                                                                                                                                                              _mTimer1Value(Timer1Value),
                                                                                                                                                                                                                                              _mTimer2Value(Timer2Value),
                                                                                                                                                                                                                                              _mIngredients(Ingredients)
{
}

QString Recipe::Name() const
{
    return _mName;
}

void Recipe::setName(const QString& Name)
{
    _mName = Name;
}

QString Recipe::Note() const
{
    return _mNote;
}

void Recipe::setNote(const QString& Note)
{
    _mNote = Note;
}

QString Recipe::Link() const
{
    return _mLink;
}

void Recipe::setLink(const QString& Link)
{
    _mLink = Link;
}

uint Recipe::Persons() const
{
    return _mPersons;
}

void Recipe::setPersons(const uint& Persons)
{
    _mPersons = Persons;
}

QString Recipe::Description() const
{
    return _mDescription;
}

void Recipe::setDescription(const QString& Description)
{
    _mDescription = Description;
}

uint Recipe::CookingTime() const
{
    return _mCookingTime;
}

void Recipe::setCookingTime(const uint& CookingTime)
{
    _mCookingTime = CookingTime;
}

QString Recipe::Timer1Name() const
{
    return _mTimer1Name;
}

void Recipe::setTimer1Name(const QString& Timer1Name)
{
    _mTimer1Name = Timer1Name;
}

QString Recipe::Timer2Name() const
{
    return _mTimer2Name;
}

void Recipe::setTimer2Name(const QString& Timer2Name)
{
    _mTimer2Name = Timer2Name;
}

QString Recipe::Category() const
{
    return _mCategory;
}

void Recipe::setCategory(const QString& Category)
{
    _mCategory = Category;
}

uint Recipe::Timer1Value() const
{
    return _mTimer1Value;
}

void Recipe::setTimer1Value(const uint& Timer1Value)
{
    _mTimer1Value = Timer1Value;
}

uint Recipe::Timer2Value() const
{
    return _mTimer2Value;
}

void Recipe::setTimer2Value(const uint& Timer2Value)
{
    _mTimer2Value = Timer2Value;
}

QList<Ingredient> Recipe::Ingredients() const
{
    return _mIngredients;
}

void Recipe::setIngredients(const QList<Ingredient>& Ingredients)
{
    _mIngredients = Ingredients;
}

QDebug operator<<(QDebug Stream, const Recipe& Recipe)
{
    Stream << "Name: " << Recipe._mName << endl;
    Stream << "Note: " << Recipe._mNote << endl;
    Stream << "Link: " << Recipe._mLink << endl;
    Stream << "Persons: " << Recipe._mPersons << endl;
    Stream << "Cooking description: " << Recipe._mDescription << endl;
    Stream << "Cooking time: " << Recipe._mCookingTime << endl;
    Stream << "Timer 1: " << Recipe._mTimer1Name << " - " << Recipe._mTimer1Value << "s" << endl;
    Stream << "Timer 2: " << Recipe._mTimer2Name << " - " << Recipe._mTimer2Value << "s" << endl;
    Stream << "Ingredients: " << endl;

    foreach(Ingredient ingredient, Recipe.Ingredients())
    {
        Stream << ingredient << endl;
    }

    return Stream;
}

QTextStream& operator<<(QTextStream& Stream, const Recipe& Recipe)
{
    Stream << "Name: " << Recipe._mName << endl;
    Stream << "Note: " << Recipe._mNote << endl;
    Stream << "Link: " << Recipe._mLink << endl;
    Stream << "Persons: " << Recipe._mPersons << endl;
    Stream << "Cooking description: " << Recipe._mDescription << endl;
    Stream << "Cooking time: " << Recipe._mCookingTime << endl;
    Stream << "Timer 1: " << Recipe._mTimer1Name << " - " << Recipe._mTimer1Value << "s" << endl;
    Stream << "Timer 2: " << Recipe._mTimer2Name << " - " << Recipe._mTimer2Value << "s" << endl;
    Stream << "Ingredients: " << endl;

    foreach(Ingredient ingredient, Recipe.Ingredients())
    {
        Stream << ingredient << endl;
    }

    return Stream;
}
