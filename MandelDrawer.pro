QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Qt\QtMain.cpp \
    Qt\mainwindow.cpp \
    Source/Color.cpp \
    Source/Formulas/Expression.cpp \
    Source/Formulas/Formula.cpp \
    Source/Formulas/FormulaCompiler.cpp \
    Source/Formulas/FormulaInterpreter.cpp \
    Source/Formulas/Parser.cpp \
    Source/Main.cpp

HEADERS += \
    Qt\mainwindow.h \
    Source/Animation.h \
    Source/Color.h \
    Source/CommonTools.h \
    Source/Formulas/Common.h \
    Source/Formulas/Expression.h \
    Source/Formulas/Formula.h \
    Source/Formulas/FormulaCompiler.h \
    Source/Formulas/FormulaInterpreter.h \
    Source/Formulas/Functions.h \
    Source/Formulas/OpCodeDefs.h \
    Source/Formulas/OpCodes.h \
    Source/Formulas/Operators.h \
    Source/Formulas/Parser.h \
    Source/Image.h \
    Source/MandelDrawer.h \
    Source/MandelDrawerSettings.h \
    Source/Types.h \
    Source/Vector2D.h
	
SUBDIRS += \
	Source@

FORMS += \
    Qt\mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
