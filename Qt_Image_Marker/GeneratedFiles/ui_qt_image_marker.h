/********************************************************************************
** Form generated from reading UI file 'qt_image_marker.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QT_IMAGE_MARKER_H
#define UI_QT_IMAGE_MARKER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>
#include "customwidgets.h"
#include "qtglimageviewer.h"

QT_BEGIN_NAMESPACE

class Ui_Qt_Image_MarkerClass
{
public:
    QAction *actionOpenFile;
    QAction *actionClose;
    QWidget *centralWidget;
    QGridLayout *gridLayout_4;
    QtGlImageviewer *imageViewerWidget;
    QListView *listView_file;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QDockWidget *dockWidget;
    QWidget *dockWidgetContents;
    QHBoxLayout *horizontalLayout;
    QToolButton *toolButton_SwitchMark;
    QToolButton *toolButton_SwitchResult;
    QLabel *label;
    LineEdit_K *lineEdit_strokeWidth;
    QRadioButton *radioButton_markForeground;
    QRadioButton *radioButton_markBackground;
    QRadioButton *radioButton_markClean;
    QToolButton *toolButton_cleanAllMark;
    QSpacerItem *horizontalSpacer;
    QToolButton *toolButton_runGrabCut;

    void setupUi(QMainWindow *Qt_Image_MarkerClass)
    {
        if (Qt_Image_MarkerClass->objectName().isEmpty())
            Qt_Image_MarkerClass->setObjectName(QStringLiteral("Qt_Image_MarkerClass"));
        Qt_Image_MarkerClass->resize(926, 790);
        actionOpenFile = new QAction(Qt_Image_MarkerClass);
        actionOpenFile->setObjectName(QStringLiteral("actionOpenFile"));
        actionClose = new QAction(Qt_Image_MarkerClass);
        actionClose->setObjectName(QStringLiteral("actionClose"));
        centralWidget = new QWidget(Qt_Image_MarkerClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout_4 = new QGridLayout(centralWidget);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        imageViewerWidget = new QtGlImageviewer(centralWidget);
        imageViewerWidget->setObjectName(QStringLiteral("imageViewerWidget"));

        gridLayout_4->addWidget(imageViewerWidget, 0, 1, 1, 1);

        listView_file = new QListView(centralWidget);
        listView_file->setObjectName(QStringLiteral("listView_file"));
        listView_file->setMaximumSize(QSize(300, 16777215));
        listView_file->setEditTriggers(QAbstractItemView::AllEditTriggers);

        gridLayout_4->addWidget(listView_file, 0, 0, 1, 1);

        Qt_Image_MarkerClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(Qt_Image_MarkerClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 926, 25));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        Qt_Image_MarkerClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(Qt_Image_MarkerClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        Qt_Image_MarkerClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(Qt_Image_MarkerClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        Qt_Image_MarkerClass->setStatusBar(statusBar);
        dockWidget = new QDockWidget(Qt_Image_MarkerClass);
        dockWidget->setObjectName(QStringLiteral("dockWidget"));
        dockWidget->setFeatures(QDockWidget::DockWidgetVerticalTitleBar);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QStringLiteral("dockWidgetContents"));
        horizontalLayout = new QHBoxLayout(dockWidgetContents);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        toolButton_SwitchMark = new QToolButton(dockWidgetContents);
        toolButton_SwitchMark->setObjectName(QStringLiteral("toolButton_SwitchMark"));
        toolButton_SwitchMark->setMinimumSize(QSize(0, 30));
        toolButton_SwitchMark->setMaximumSize(QSize(100, 16777215));
        toolButton_SwitchMark->setFocusPolicy(Qt::TabFocus);
        toolButton_SwitchMark->setCheckable(true);

        horizontalLayout->addWidget(toolButton_SwitchMark);

        toolButton_SwitchResult = new QToolButton(dockWidgetContents);
        toolButton_SwitchResult->setObjectName(QStringLiteral("toolButton_SwitchResult"));
        toolButton_SwitchResult->setMinimumSize(QSize(0, 30));
        toolButton_SwitchResult->setMaximumSize(QSize(100, 16777215));
        toolButton_SwitchResult->setCheckable(true);

        horizontalLayout->addWidget(toolButton_SwitchResult);

        label = new QLabel(dockWidgetContents);
        label->setObjectName(QStringLiteral("label"));
        label->setMaximumSize(QSize(100, 16777215));

        horizontalLayout->addWidget(label);

        lineEdit_strokeWidth = new LineEdit_K(dockWidgetContents);
        lineEdit_strokeWidth->setObjectName(QStringLiteral("lineEdit_strokeWidth"));
        lineEdit_strokeWidth->setMaximumSize(QSize(50, 16777215));
        lineEdit_strokeWidth->setMaxLength(5);
        lineEdit_strokeWidth->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(lineEdit_strokeWidth);

        radioButton_markForeground = new QRadioButton(dockWidgetContents);
        radioButton_markForeground->setObjectName(QStringLiteral("radioButton_markForeground"));
        radioButton_markForeground->setChecked(true);

        horizontalLayout->addWidget(radioButton_markForeground);

        radioButton_markBackground = new QRadioButton(dockWidgetContents);
        radioButton_markBackground->setObjectName(QStringLiteral("radioButton_markBackground"));

        horizontalLayout->addWidget(radioButton_markBackground);

        radioButton_markClean = new QRadioButton(dockWidgetContents);
        radioButton_markClean->setObjectName(QStringLiteral("radioButton_markClean"));

        horizontalLayout->addWidget(radioButton_markClean);

        toolButton_cleanAllMark = new QToolButton(dockWidgetContents);
        toolButton_cleanAllMark->setObjectName(QStringLiteral("toolButton_cleanAllMark"));
        toolButton_cleanAllMark->setMinimumSize(QSize(0, 30));

        horizontalLayout->addWidget(toolButton_cleanAllMark);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        toolButton_runGrabCut = new QToolButton(dockWidgetContents);
        toolButton_runGrabCut->setObjectName(QStringLiteral("toolButton_runGrabCut"));
        toolButton_runGrabCut->setMinimumSize(QSize(0, 30));

        horizontalLayout->addWidget(toolButton_runGrabCut);

        dockWidget->setWidget(dockWidgetContents);
        Qt_Image_MarkerClass->addDockWidget(static_cast<Qt::DockWidgetArea>(4), dockWidget);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionOpenFile);

        retranslateUi(Qt_Image_MarkerClass);

        QMetaObject::connectSlotsByName(Qt_Image_MarkerClass);
    } // setupUi

    void retranslateUi(QMainWindow *Qt_Image_MarkerClass)
    {
        Qt_Image_MarkerClass->setWindowTitle(QApplication::translate("Qt_Image_MarkerClass", "Qt_Image_Marker", 0));
        actionOpenFile->setText(QApplication::translate("Qt_Image_MarkerClass", "Open Folder", 0));
        actionClose->setText(QApplication::translate("Qt_Image_MarkerClass", "Close", 0));
        menuFile->setTitle(QApplication::translate("Qt_Image_MarkerClass", "File", 0));
        toolButton_SwitchMark->setText(QApplication::translate("Qt_Image_MarkerClass", "Show Mark (Q)", 0));
        toolButton_SwitchResult->setText(QApplication::translate("Qt_Image_MarkerClass", "Show Result (W)", 0));
        label->setText(QApplication::translate("Qt_Image_MarkerClass", "Strock Width(E)", 0));
        lineEdit_strokeWidth->setText(QApplication::translate("Qt_Image_MarkerClass", "5", 0));
        radioButton_markForeground->setText(QApplication::translate("Qt_Image_MarkerClass", "foreground", 0));
        radioButton_markBackground->setText(QApplication::translate("Qt_Image_MarkerClass", "background", 0));
        radioButton_markClean->setText(QApplication::translate("Qt_Image_MarkerClass", "clean", 0));
        toolButton_cleanAllMark->setText(QApplication::translate("Qt_Image_MarkerClass", "Clean Mark", 0));
        toolButton_runGrabCut->setText(QApplication::translate("Qt_Image_MarkerClass", "GrabCut(R)", 0));
    } // retranslateUi

};

namespace Ui {
    class Qt_Image_MarkerClass: public Ui_Qt_Image_MarkerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QT_IMAGE_MARKER_H
