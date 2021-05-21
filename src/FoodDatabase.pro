VERSION_MAJOR = 2
VERSION_MINOR = 0
VERSION_BUILD = 0

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 file_copies

COPIES += languageFiles settingsFile packageSetup configSetup

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS \
           "VERSION_MAJOR=$$VERSION_MAJOR" \
           "VERSION_MINOR=$$VERSION_MINOR" \
           "VERSION_BUILD=$$VERSION_BUILD" \

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
    Model/ComboBoxDelegate/ \
    Model/NumbersOnlyDelegate/ \
    Model/RecipesProxy/ \
    Model/RecipesModel/ \
    Dialog/ \
    Dialog/AboutDialog \
    Dialog/RecipeDialog \
    Dialog/SettingsDialog \
    Widget/CategoryList \

SOURCES += \
    Dialog/AboutDialog/aboutdialog.cpp \
    Dialog/RecipeDialog/recipedialog.cpp \
    Dialog/SettingsDialog/settingsdialog.cpp \
    Model/ComboBoxDelegate/comboboxdelegate.cpp \
    Model/IngredientModel/ingredientmodel.cpp \
    Model/NumbersOnlyDelegate/numbersonlydelegate.cpp \
    Model/RecipesProxy/recipesproxy.cpp \
    Model/RecipesModel/recipesmodel.cpp \
    Recipe/Ingredient/ingredient.cpp \
    Recipe/RecipeExport/recipeexport.cpp \
    Recipe/recipe.cpp \
    Widget/CategoryList/categorylist.cpp \
    main.cpp \
    mainwindow.cpp \

HEADERS += \
    Dialog/AboutDialog/aboutdialog.h \
    Dialog/RecipeDialog/recipedialog.h \
    Dialog/SettingsDialog/settingsdialog.h \
    Model/ComboBoxDelegate/comboboxdelegate.h \
    Model/IngredientModel/ingredientmodel.h \
    Model/NumbersOnlyDelegate/numbersonlydelegate.h \
    Model/RecipesProxy/recipesproxy.h \
    Model/RecipesModel/recipesmodel.h \
    Recipe/Ingredient/ingredient.h \
    Recipe/RecipeExport/recipeexport.h \
    Recipe/recipe.h \
    Widget/CategoryList/categorylist.h \
    mainwindow.h

FORMS += \
    Dialog/AboutDialog/aboutdialog.ui \
    Dialog/RecipeDialog/recipedialog.ui \
    Dialog/SettingsDialog/settingsdialog.ui \
    Widget/CategoryList/categorylist.ui \
    mainwindow.ui

TRANSLATIONS += \
    Languages/FoodDatabase_de_DE.ts \
    Languages/FoodDatabase_en_US.ts \

# Target version
VERSION = $${VERSION_MAJOR}.$${VERSION_MINOR}.$${VERSION_BUILD}

# Set the application icon
win32:RC_ICONS += Ressources/Icons/icons8-food-96.ico

# Set the build directories
CONFIG(debug, debug|release) {
    DESTDIR = debug
} else {
    DESTDIR = ../packages/com.kampis-elektroecke.FoodDatabase/data
}

# Copy language files
languageFiles.files = $$files($${PWD}/Languages/*.qm)
CONFIG(debug, debug|release) {
    languageFiles.path = $$shell_quote($${OUT_PWD}/debug/Languages)
} else {
    languageFiles.path = $$shell_quote($${DESTDIR}/Languages)
}

# Copy settings files
settingsFile.files = $$files(Settings.ini)
CONFIG(debug, debug|release) {
    settingsFile.path = $$shell_quote($${OUT_PWD}/debug)
} else {
    settingsFile.path = $$shell_quote($${DESTDIR})
}

# Copy package and configuration files
CONFIG(debug, debug|release) {
    packageSetup.path = $$shell_quote($${OUT_PWD}/debug)
    configSetup.path = $$shell_quote($${OUT_PWD}/debug)
} else {
    packageSetup.path = $$shell_quote($${DESTDIR})
    configSetup.path = $$shell_quote(../config)
}

packageSetup.files += $$shell_quote(Ressources/Icons/icons8-food-96.ico) \
                      $$shell_quote(Ressources/Icons/icons8-food-96.png) \

configSetup.files += $$shell_quote(Ressources/Icons/icons8-food-96.ico) \
                     $$shell_quote(Ressources/Icons/icons8-food-96.png) \

# Deployment rules
DEPLOY_COMMAND = $$(QTDIR)/bin/windeployqt
DEPLOY_OPTIONS = "--no-system-d3d-compiler --no-opengl --no-angle --no-opengl-sw"
DEPLOY_TARGET = $$shell_quote($$shell_path($${DESTDIR}/$${TARGET}.exe))
CONFIG(debug, debug|release) {
    DEPLOY_OPTIONS += "--debug"
} else {
    DEPLOY_OPTIONS += "--release"
}

QMAKE_POST_LINK = $${DEPLOY_COMMAND} $${DEPLOY_OPTIONS} $${DEPLOY_TARGET}
