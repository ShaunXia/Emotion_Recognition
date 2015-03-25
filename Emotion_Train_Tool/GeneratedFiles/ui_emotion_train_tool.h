/********************************************************************************
** Form generated from reading UI file 'emotion_train_tool.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EMOTION_TRAIN_TOOL_H
#define UI_EMOTION_TRAIN_TOOL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSlider>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Emotion_Train_ToolClass
{
public:
    QWidget *centralWidget;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QPushButton *choose_path;
    QListWidget *fileList;
    QSlider *video_status;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents_2;
    QHBoxLayout *horizontalLayout;
    QLabel *picshow;
    QGroupBox *group_emotion;
    QPushButton *but_capture;
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QPushButton *but_sleepy;
    QPushButton *but_normal;
    QPushButton *but_happy;
    QGroupBox *group_facs;
    QPlainTextEdit *log_facs_info;
    QPushButton *but_facs_info;
    QWidget *widget1;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *but_svm_or_facs;
    QPushButton *but_load_model;
    QPushButton *but_detect_trigger;

    void setupUi(QMainWindow *Emotion_Train_ToolClass)
    {
        if (Emotion_Train_ToolClass->objectName().isEmpty())
            Emotion_Train_ToolClass->setObjectName(QStringLiteral("Emotion_Train_ToolClass"));
        Emotion_Train_ToolClass->resize(851, 488);
        centralWidget = new QWidget(Emotion_Train_ToolClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayoutWidget = new QWidget(centralWidget);
        gridLayoutWidget->setObjectName(QStringLiteral("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(10, 10, 531, 451));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setSpacing(0);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        choose_path = new QPushButton(gridLayoutWidget);
        choose_path->setObjectName(QStringLiteral("choose_path"));

        gridLayout->addWidget(choose_path, 1, 1, 1, 1);

        fileList = new QListWidget(gridLayoutWidget);
        fileList->setObjectName(QStringLiteral("fileList"));

        gridLayout->addWidget(fileList, 0, 1, 1, 1);

        video_status = new QSlider(gridLayoutWidget);
        video_status->setObjectName(QStringLiteral("video_status"));
        video_status->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(video_status, 1, 0, 1, 1);

        scrollArea = new QScrollArea(gridLayoutWidget);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(scrollArea->sizePolicy().hasHeightForWidth());
        scrollArea->setSizePolicy(sizePolicy);
        scrollArea->setMinimumSize(QSize(400, 400));
        scrollArea->setAcceptDrops(false);
        scrollArea->setAutoFillBackground(false);
        scrollArea->setSizeAdjustPolicy(QAbstractScrollArea::AdjustIgnored);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents_2 = new QWidget();
        scrollAreaWidgetContents_2->setObjectName(QStringLiteral("scrollAreaWidgetContents_2"));
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 398, 424));
        horizontalLayout = new QHBoxLayout(scrollAreaWidgetContents_2);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        picshow = new QLabel(scrollAreaWidgetContents_2);
        picshow->setObjectName(QStringLiteral("picshow"));
        picshow->setScaledContents(false);
        picshow->setWordWrap(false);

        horizontalLayout->addWidget(picshow);

        scrollArea->setWidget(scrollAreaWidgetContents_2);

        gridLayout->addWidget(scrollArea, 0, 0, 1, 1);

        gridLayout->setColumnStretch(0, 3);
        gridLayout->setColumnStretch(1, 1);
        group_emotion = new QGroupBox(centralWidget);
        group_emotion->setObjectName(QStringLiteral("group_emotion"));
        group_emotion->setEnabled(true);
        group_emotion->setGeometry(QRect(560, 70, 251, 171));
        but_capture = new QPushButton(group_emotion);
        but_capture->setObjectName(QStringLiteral("but_capture"));
        but_capture->setGeometry(QRect(20, 60, 71, 61));
        widget = new QWidget(group_emotion);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(120, 40, 91, 101));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        but_sleepy = new QPushButton(widget);
        but_sleepy->setObjectName(QStringLiteral("but_sleepy"));

        verticalLayout->addWidget(but_sleepy);

        but_normal = new QPushButton(widget);
        but_normal->setObjectName(QStringLiteral("but_normal"));

        verticalLayout->addWidget(but_normal);

        but_happy = new QPushButton(widget);
        but_happy->setObjectName(QStringLiteral("but_happy"));

        verticalLayout->addWidget(but_happy);

        group_facs = new QGroupBox(centralWidget);
        group_facs->setObjectName(QStringLiteral("group_facs"));
        group_facs->setGeometry(QRect(560, 290, 251, 171));
        log_facs_info = new QPlainTextEdit(group_facs);
        log_facs_info->setObjectName(QStringLiteral("log_facs_info"));
        log_facs_info->setGeometry(QRect(110, 20, 131, 141));
        but_facs_info = new QPushButton(group_facs);
        but_facs_info->setObjectName(QStringLiteral("but_facs_info"));
        but_facs_info->setGeometry(QRect(10, 20, 75, 23));
        widget1 = new QWidget(centralWidget);
        widget1->setObjectName(QStringLiteral("widget1"));
        widget1->setGeometry(QRect(560, 20, 247, 25));
        horizontalLayout_2 = new QHBoxLayout(widget1);
        horizontalLayout_2->setSpacing(10);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        but_svm_or_facs = new QPushButton(widget1);
        but_svm_or_facs->setObjectName(QStringLiteral("but_svm_or_facs"));

        horizontalLayout_2->addWidget(but_svm_or_facs);

        but_load_model = new QPushButton(widget1);
        but_load_model->setObjectName(QStringLiteral("but_load_model"));

        horizontalLayout_2->addWidget(but_load_model);

        but_detect_trigger = new QPushButton(widget1);
        but_detect_trigger->setObjectName(QStringLiteral("but_detect_trigger"));

        horizontalLayout_2->addWidget(but_detect_trigger);

        Emotion_Train_ToolClass->setCentralWidget(centralWidget);

        retranslateUi(Emotion_Train_ToolClass);

        QMetaObject::connectSlotsByName(Emotion_Train_ToolClass);
    } // setupUi

    void retranslateUi(QMainWindow *Emotion_Train_ToolClass)
    {
        Emotion_Train_ToolClass->setWindowTitle(QApplication::translate("Emotion_Train_ToolClass", "Emotion_Train_Tool", 0));
        choose_path->setText(QApplication::translate("Emotion_Train_ToolClass", "Choose Path", 0));
        group_emotion->setTitle(QApplication::translate("Emotion_Train_ToolClass", "SVM TRAIN", 0));
        but_capture->setText(QApplication::translate("Emotion_Train_ToolClass", "Capture", 0));
        but_sleepy->setText(QApplication::translate("Emotion_Train_ToolClass", "Sleepy", 0));
        but_normal->setText(QApplication::translate("Emotion_Train_ToolClass", "Normal", 0));
        but_happy->setText(QApplication::translate("Emotion_Train_ToolClass", "Happy", 0));
        group_facs->setTitle(QApplication::translate("Emotion_Train_ToolClass", "FACS TRAIN INFO", 0));
        but_facs_info->setText(QApplication::translate("Emotion_Train_ToolClass", "Get Info", 0));
        but_svm_or_facs->setText(QApplication::translate("Emotion_Train_ToolClass", "SVM/FACS", 0));
        but_load_model->setText(QApplication::translate("Emotion_Train_ToolClass", "Load Model", 0));
        but_detect_trigger->setText(QApplication::translate("Emotion_Train_ToolClass", "Off/Detect", 0));
    } // retranslateUi

};

namespace Ui {
    class Emotion_Train_ToolClass: public Ui_Emotion_Train_ToolClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EMOTION_TRAIN_TOOL_H
