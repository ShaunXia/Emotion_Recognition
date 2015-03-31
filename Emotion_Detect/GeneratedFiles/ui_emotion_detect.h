/********************************************************************************
** Form generated from reading UI file 'emotion_detect.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EMOTION_DETECT_H
#define UI_EMOTION_DETECT_H

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
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Emotion_DetectClass
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
    QPushButton *but_capture;
    QGroupBox *predict_control;
    QSplitter *splitter;
    QPushButton *but_loadlbf;
    QPushButton *but_start_detect;
    QPushButton *but_show_landmark;
    QGroupBox *group_result;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *result_layout;

    void setupUi(QMainWindow *Emotion_DetectClass)
    {
        if (Emotion_DetectClass->objectName().isEmpty())
            Emotion_DetectClass->setObjectName(QStringLiteral("Emotion_DetectClass"));
        Emotion_DetectClass->resize(555, 637);
        centralWidget = new QWidget(Emotion_DetectClass);
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
        but_capture = new QPushButton(centralWidget);
        but_capture->setObjectName(QStringLiteral("but_capture"));
        but_capture->setGeometry(QRect(210, 490, 75, 61));
        predict_control = new QGroupBox(centralWidget);
        predict_control->setObjectName(QStringLiteral("predict_control"));
        predict_control->setEnabled(true);
        predict_control->setGeometry(QRect(10, 470, 161, 161));
        splitter = new QSplitter(predict_control);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setGeometry(QRect(30, 20, 101, 101));
        splitter->setOrientation(Qt::Vertical);
        but_loadlbf = new QPushButton(splitter);
        but_loadlbf->setObjectName(QStringLiteral("but_loadlbf"));
        splitter->addWidget(but_loadlbf);
        but_start_detect = new QPushButton(splitter);
        but_start_detect->setObjectName(QStringLiteral("but_start_detect"));
        splitter->addWidget(but_start_detect);
        but_show_landmark = new QPushButton(splitter);
        but_show_landmark->setObjectName(QStringLiteral("but_show_landmark"));
        splitter->addWidget(but_show_landmark);
        group_result = new QGroupBox(centralWidget);
        group_result->setObjectName(QStringLiteral("group_result"));
        group_result->setGeometry(QRect(390, 470, 131, 161));
        group_result->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        verticalLayoutWidget = new QWidget(group_result);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(10, 10, 111, 141));
        result_layout = new QVBoxLayout(verticalLayoutWidget);
        result_layout->setSpacing(6);
        result_layout->setContentsMargins(11, 11, 11, 11);
        result_layout->setObjectName(QStringLiteral("result_layout"));
        result_layout->setContentsMargins(0, 0, 0, 0);
        Emotion_DetectClass->setCentralWidget(centralWidget);

        retranslateUi(Emotion_DetectClass);

        QMetaObject::connectSlotsByName(Emotion_DetectClass);
    } // setupUi

    void retranslateUi(QMainWindow *Emotion_DetectClass)
    {
        Emotion_DetectClass->setWindowTitle(QApplication::translate("Emotion_DetectClass", "Emotion_Detect", 0));
        choose_path->setText(QApplication::translate("Emotion_DetectClass", "Choose Path", 0));
        but_capture->setText(QApplication::translate("Emotion_DetectClass", "Capture", 0));
        predict_control->setTitle(QApplication::translate("Emotion_DetectClass", "Predict", 0));
        but_loadlbf->setText(QApplication::translate("Emotion_DetectClass", "Load Model", 0));
        but_start_detect->setText(QApplication::translate("Emotion_DetectClass", "Start Detect", 0));
        but_show_landmark->setText(QApplication::translate("Emotion_DetectClass", "Show LandMark", 0));
        group_result->setTitle(QApplication::translate("Emotion_DetectClass", "Result", 0));
    } // retranslateUi

};

namespace Ui {
    class Emotion_DetectClass: public Ui_Emotion_DetectClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EMOTION_DETECT_H
