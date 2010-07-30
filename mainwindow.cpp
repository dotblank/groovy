/*
 * This file is part of groovy
 *
 * Copyright (C) 2010 Robin Burchell <viroteck@viroteck.net>
 *
 * This software, including documentation, is protected by copyright.
 * All rights are reserved.
 *
 * Copying, including reproducing, storing, adapting or translating, any or
 * all of this material requires prior written consent of the author.
 *
 * This material may also contain confidential information which may not be
 * disclosed to others without the prior written consent of the author.
 */

#include <QMessageBox>
#include <QDebug>

#include "grooveclient.h"
#include "groovesearchmodel.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow),
    m_searchModel(new GrooveSearchModel(this))
{
    GrooveClient::instance()->establishConnection();
    connect(GrooveClient::instance(), SIGNAL(connected()), SLOT(onConnected()));

    m_ui->setupUi(this);
    m_ui->searchResults->setModel(m_searchModel);

    qDebug() << connect(m_ui->pushButton, SIGNAL(clicked()), this, SLOT(onSearchButtonPress()));
}

MainWindow::~MainWindow()
{
    delete m_ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::onConnected()
{
   // QMessageBox::information(NULL, "Connected!", "Groovy is now connected.");
}

void MainWindow::onSearchButtonPress()
{
    qDebug() << Q_FUNC_INFO << "Searching for " << m_ui->lineEdit->text();
    m_searchModel->searchByArtist(m_ui->lineEdit->text());
}
