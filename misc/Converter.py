import json
import sqlite3

OLD_DB = ""
NEW_DB = ""

if(__name__ == "__main__"):
    Connection = sqlite3.connect(OLD_DB)
    Cursor = Connection.cursor()

    Recipes = Cursor.execute("SELECT * FROM Recipes").fetchall()
    Ingredients = Cursor.execute("SELECT * FROM Ingredients").fetchall()

    Cursor.close()
    Connection.close()
    
    print("Reading {} recpies".format(len(Recipes)))
    print("Reading {} ingredients".format(len(Ingredients)))

    Connection = sqlite3.connect(NEW_DB)
    Cursor = Connection.cursor()

    for Recipe in Recipes:
        Start = Recipe[12]
        Count = Recipe[13]

        Data = {}
        for i in range(Start, Start + Count):
            Info = {}

            Info["Amount"] = Ingredients[i][3]
            Info["Note"] = Ingredients[i][2]
            Info["Price"] = Ingredients[i][5]
            Info["Section"] = Ingredients[i][6]
            Info["Unit"] = Ingredients[i][4]
            Data[Ingredients[i][1]] = Info

        Ingredients_JSON = json.dumps(Data, ensure_ascii = False)
        SQL = '''INSERT INTO Recipes (Name, Note, Link, Persons, Cooking_Description, Category, Cooking_Time, Timer1_Name, Timer2_Name, Timer1_Value, Timer2_Value, Ingredients)
                        VALUES ('{}', '{}', '{}', '{}', '{}', '{}', '{}', '{}', '{}', '{}', '{}', '{}')'''.format(Recipe[1], Recipe[2], Recipe[3], Recipe[4], Recipe[5], Recipe[6], Recipe[7], Recipe[8], Recipe[9], Recipe[10], Recipe[11], Ingredients_JSON)
        Cursor.execute(SQL)

    Connection.commit()
    Cursor.close()
    Connection.close()
