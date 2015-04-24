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
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Emotion_DetectClass
{
public:
    QWidget *centralWidget;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QSlider *video_status;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents_2;
    QHBoxLayout *horizontalLayout;
    QLabel *picshow;
    QPushButton *choose_path;
    QListWidget *fileList;
    QGroupBox *predict_control;
    QSplitter *splitter;
    QPushButton *but_loadlbf;
    QPushButton *but_start_detect;
    QPushButton *but_show_landmark;
    QPushButton *but_capture;
    QWidget *widget_res;
    QWidget *gridWidget_2;
    QGridLayout *gridLayout_2;
    QLabel *label_res_1;
    QLabel *label_res_3;
    QLabel *label_res_2;
    QLabel *label_res_4;
    QLabel *label_res_5;
    QLabel *label_res_0;
    QLabel *label_res_6;
    QLabel *label_res_7;
    QLabel *model_info;

    void setupUi(QMainWindow *Emotion_DetectClass)
    {
        if (Emotion_DetectClass->objectName().isEmpty())
            Emotion_DetectClass->setObjectName(QStringLiteral("Emotion_DetectClass"));
        Emotion_DetectClass->resize(567, 488);
        centralWidget = new QWidget(Emotion_DetectClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayoutWidget = new QWidget(centralWidget);
        gridLayoutWidget->setObjectName(QStringLiteral("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(10, 10, 531, 325));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setSpacing(0);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
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
        scrollArea->setMinimumSize(QSize(400, 300));
        scrollArea->setAcceptDrops(false);
        scrollArea->setAutoFillBackground(false);
        scrollArea->setSizeAdjustPolicy(QAbstractScrollArea::AdjustIgnored);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents_2 = new QWidget();
        scrollAreaWidgetContents_2->setObjectName(QStringLiteral("scrollAreaWidgetContents_2"));
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 398, 298));
        horizontalLayout = new QHBoxLayout(scrollAreaWidgetContents_2);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        picshow = new QLabel(scrollAreaWidgetContents_2);
        picshow->setObjectName(QStringLiteral("picshow"));
        picshow->setScaledContents(false);
        picshow->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        picshow->setWordWrap(false);

        horizontalLayout->addWidget(picshow);

        scrollArea->setWidget(scrollAreaWidgetContents_2);

        gridLayout->addWidget(scrollArea, 0, 0, 1, 1);

        choose_path = new QPushButton(gridLayoutWidget);
        choose_path->setObjectName(QStringLiteral("choose_path"));

        gridLayout->addWidget(choose_path, 1, 1, 1, 1);

        fileList = new QListWidget(gridLayoutWidget);
        fileList->setObjectName(QStringLiteral("fileList"));

        gridLayout->addWidget(fileList, 0, 1, 1, 1);

        predict_control = new QGroupBox(centralWidget);
        predict_control->setObjectName(QStringLiteral("predict_control"));
        predict_control->setEnabled(true);
        predict_control->setGeometry(QRect(400, 340, 141, 131));
        splitter = new QSplitter(predict_control);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setGeometry(QRect(30, 20, 91, 101));
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
        but_capture = new QPushButton(splitter);
        but_capture->setObjectName(QStringLiteral("but_capture"));
        splitter->addWidget(but_capture);
        widget_res = new QWidget(centralWidget);
        widget_res->setObjectName(QStringLiteral("widget_res"));
        widget_res->setGeometry(QRect(40, 340, 321, 131));
        gridWidget_2 = new QWidget(widget_res);
        gridWidget_2->setObjectName(QStringLiteral("gridWidget_2"));
        gridWidget_2->setGeometry(QRect(0, 19, 301, 100));
        gridWidget_2->setAutoFillBackground(false);
        gridWidget_2->setStyleSheet(QStringLiteral("background-color: rgb(216, 216, 216);"));
        gridLayout_2 = new QGridLayout(gridWidget_2);
        gridLayout_2->setSpacing(0);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        label_res_1 = new QLabel(gridWidget_2);
        label_res_1->setObjectName(QStringLiteral("label_res_1"));
        label_res_1->setAutoFillBackground(false);
        label_res_1->setStyleSheet(QLatin1String("border-width: 1px;\n"
"   border-style: solid;\n"
"   border-color: rgb(85, 85, 85,0);\n"
"border-right-color: rgb(85, 85, 85);\n"
""));
        label_res_1->setFrameShape(QFrame::Box);
        label_res_1->setFrameShadow(QFrame::Plain);
        label_res_1->setScaledContents(true);
        label_res_1->setAlignment(Qt::AlignCenter);
        label_res_1->setIndent(-1);

        gridLayout_2->addWidget(label_res_1, 1, 1, 1, 1);

        label_res_3 = new QLabel(gridWidget_2);
        label_res_3->setObjectName(QStringLiteral("label_res_3"));
        label_res_3->setStyleSheet(QLatin1String("border-width: 1px;\n"
"   border-style: solid;\n"
"   border-color: rgb(85, 85, 85,0);\n"
"\n"
""));
        label_res_3->setFrameShape(QFrame::Box);
        label_res_3->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_res_3, 1, 3, 1, 1);

        label_res_2 = new QLabel(gridWidget_2);
        label_res_2->setObjectName(QStringLiteral("label_res_2"));
        label_res_2->setStyleSheet(QLatin1String("border-width: 1px;\n"
"   border-style: solid;\n"
"   border-color: rgb(85, 85, 85,0);\n"
"border-right-color: rgb(85, 85, 85);\n"
""));
        label_res_2->setFrameShape(QFrame::Box);
        label_res_2->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_res_2, 1, 2, 1, 1);

        label_res_4 = new QLabel(gridWidget_2);
        label_res_4->setObjectName(QStringLiteral("label_res_4"));
        label_res_4->setStyleSheet(QString::fromUtf8("border-width: 1px;\n"
"   border-style: solid;\n"
"   border-color: rgb(85, 85, 85,0);\n"
"border-right-color: rgb(85, 85, 85);\n"
"border-bottom-color: rgb(85, 85, 85);\n"
"font: 75 14pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        label_res_4->setFrameShape(QFrame::Box);
        label_res_4->setPixmap(QPixmap(QString::fromUtf8("Resources/1_02.png")));
        label_res_4->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_res_4, 0, 0, 1, 1);

        label_res_5 = new QLabel(gridWidget_2);
        label_res_5->setObjectName(QStringLiteral("label_res_5"));
        label_res_5->setAutoFillBackground(false);
        label_res_5->setStyleSheet(QString::fromUtf8("border-width: 1px;\n"
"   border-style: solid;\n"
"   border-color: rgb(85, 85, 85,0);\n"
"border-right-color: rgb(85, 85, 85);\n"
"border-bottom-color: rgb(85, 85, 85);\n"
"font: 75 14pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        label_res_5->setFrameShape(QFrame::Box);
        label_res_5->setFrameShadow(QFrame::Plain);
        label_res_5->setPixmap(QPixmap(QString::fromUtf8("Resources/1_01.png")));
        label_res_5->setScaledContents(true);
        label_res_5->setAlignment(Qt::AlignCenter);
        label_res_5->setIndent(-1);

        gridLayout_2->addWidget(label_res_5, 0, 1, 1, 1);

        label_res_0 = new QLabel(gridWidget_2);
        label_res_0->setObjectName(QStringLiteral("label_res_0"));
        label_res_0->setStyleSheet(QLatin1String("border-width: 1px;\n"
"   border-style: solid;\n"
"   border-color: rgb(85, 85, 85,0);\n"
"border-right-color: rgb(85, 85, 85);"));
        label_res_0->setFrameShape(QFrame::Box);
        label_res_0->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_res_0, 1, 0, 1, 1);

        label_res_6 = new QLabel(gridWidget_2);
        label_res_6->setObjectName(QStringLiteral("label_res_6"));
        label_res_6->setStyleSheet(QString::fromUtf8("border-width: 1px;\n"
"   border-style: solid;\n"
"   border-color: rgb(85, 85, 85,0);\n"
"border-right-color: rgb(85, 85, 85);\n"
"border-bottom-color: rgb(85, 85, 85);\n"
"font: 75 14pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        label_res_6->setFrameShape(QFrame::Box);
        label_res_6->setPixmap(QPixmap(QString::fromUtf8("Resources/1_03.png")));
        label_res_6->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_res_6, 0, 2, 1, 1);

        label_res_7 = new QLabel(gridWidget_2);
        label_res_7->setObjectName(QStringLiteral("label_res_7"));
        label_res_7->setStyleSheet(QString::fromUtf8("border-width: 1px;\n"
"   border-style: solid;\n"
"   border-color: rgb(85, 85, 85,0);\n"
"\n"
"border-bottom-color: rgb(85, 85, 85);\n"
"font: 75 14pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        label_res_7->setFrameShape(QFrame::Box);
        label_res_7->setPixmap(QPixmap(QString::fromUtf8("Resources/1_04.png")));
        label_res_7->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_res_7, 0, 3, 1, 1);

        gridLayout_2->setRowStretch(0, 4);
        label_res_1->raise();
        label_res_2->raise();
        label_res_3->raise();
        label_res_0->raise();
        label_res_4->raise();
        label_res_5->raise();
        label_res_6->raise();
        label_res_7->raise();
        model_info = new QLabel(centralWidget);
        model_info->setObjectName(QStringLiteral("model_info"));
        model_info->setGeometry(QRect(303, 230, 101, 71));
        model_info->setFrameShape(QFrame::Box);
        Emotion_DetectClass->setCentralWidget(centralWidget);

        retranslateUi(Emotion_DetectClass);

        QMetaObject::connectSlotsByName(Emotion_DetectClass);
    } // setupUi

    void retranslateUi(QMainWindow *Emotion_DetectClass)
    {
        Emotion_DetectClass->setWindowTitle(QApplication::translate("Emotion_DetectClass", "Emotion_Detect", 0));
        choose_path->setText(QApplication::translate("Emotion_DetectClass", "Choose Path", 0));
        predict_control->setTitle(QApplication::translate("Emotion_DetectClass", "Predict", 0));
        but_loadlbf->setText(QApplication::translate("Emotion_DetectClass", "Load Model", 0));
        but_start_detect->setText(QApplication::translate("Emotion_DetectClass", "Start Detect", 0));
        but_show_landmark->setText(QApplication::translate("Emotion_DetectClass", "Show LandMark", 0));
        but_capture->setText(QApplication::translate("Emotion_DetectClass", "Stop/Start", 0));
        label_res_1->setText(QString());
        label_res_3->setText(QString());
        label_res_2->setText(QString());
        label_res_4->setText(QString());
        label_res_5->setText(QString());
        label_res_0->setText(QString());
        label_res_6->setText(QString());
        label_res_7->setText(QString());
        model_info->setText(QApplication::translate("Emotion_DetectClass", "Model Info", 0));
    } // retranslateUi

};

namespace Ui {
    class Emotion_DetectClass: public Ui_Emotion_DetectClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EMOTION_DETECT_H
