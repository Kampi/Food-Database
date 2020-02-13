VERSION_MAJOR = 1
VERSION_MINOR = 1
VERSION_BUILD = 0

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 file_copies

COPIES += languageFiles

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS \
           "VERSION_MAJOR=$$VERSION_MAJOR" \
           "VERSION_MINOR=$$VERSION_MINOR" \
           "VERSION_BUILD=$$VERSION_BUILD"

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

RESOURCES += \
    Ressources.qrc

INCLUDEPATH += \
    Recipe/ \
    Recipe/Ingredient/ \
    Recipe/RecipeExport/ \
    Model/ \
    Model/IngredientModel/ \
    Model/RecipesModel/ \
    Model/ComboBoxDelegate/ \
    Model/NumbersOnlyDelegate/ \
    Dialog/ \
    Database/ \

SOURCES += \
    Database/database.cpp \
    Dialog/About/aboutdialog.cpp \
    Dialog/CreateRecipe/createrecipedialog.cpp \
    Dialog/EditRecipe/editrecipedialog.cpp \
    Model/ComboBoxDelegate/comboboxdelegate.cpp \
    Model/IngredientModel/ingredientmodel.cpp \
    Model/NumbersOnlyDelegate/numbersonlydelegate.cpp \
    Model/RecipesModel/recipesmodel.cpp \
    Recipe/Ingredient/ingredient.cpp \
    Recipe/RecipeExport/recipeexport.cpp \
    Recipe/recipe.cpp \
    main.cpp \
    mainwindow.cpp \

HEADERS += \
    Database/database.h \
    Dialog/About/aboutdialog.h \
    Dialog/CreateRecipe/createrecipedialog.h \
    Dialog/EditRecipe/editrecipedialog.h \
    Model/ComboBoxDelegate/comboboxdelegate.h \
    Model/IngredientModel/ingredientmodel.h \
    Model/NumbersOnlyDelegate/numbersonlydelegate.h \
    Model/RecipesModel/recipesmodel.h \
    Recipe/Ingredient/ingredient.h \
    Recipe/RecipeExport/recipeexport.h \
    Recipe/recipe.h \
    mainwindow.h

FORMS += \
    Dialog/About/aboutdialog.ui \
    Dialog/CreateRecipe/createrecipedialog.ui \
    Dialog/EditRecipe/editrecipedialog.ui \
    mainwindow.ui

TRANSLATIONS += \
    Languages/FoodDatabase_de_DE.ts \
    Languages/FoodDatabase_en_US.ts

# Target version
VERSION = $${VERSION_MAJOR}.$${VERSION_MINOR}.$${VERSION_BUILD}

# Copy language files
languageFiles.files = $$files($$PWD/Languages/*.qm)
CONFIG(debug, debug|release) {
    languageFiles.path = $${OUT_PWD}/debug/Languages
} else {
    languageFiles.path = $${OUT_PWD}/release/Languages
}

# Deployment rules
DEPLOY_COMMAND = windeployqt
DEPLOY_OPTIONS = "--no-system-d3d-compiler --no-opengl --no-angle --no-opengl-sw"
CONFIG(debug, debug|release) {
    DEPLOY_TARGET = $$shell_quote($$shell_path($${OUT_PWD}/debug/$${TARGET}.exe))
    DEPLOY_OPTIONS += "--debug"
} else {
    DEPLOY_TARGET = $$shell_quote($$shell_path($${OUT_PWD}/release/$${TARGET}.exe))
    DEPLOY_OPTIONS += "--release"
}

QMAKE_POST_LINK = $${DEPLOY_COMMAND} $${DEPLOY_OPTIONS} $${DEPLOY_TARGET}
