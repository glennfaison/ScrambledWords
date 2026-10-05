#include "logindialog.h"
#include "ui_logindialog.h"

#include <QMessageBox>

#include "../core/scorestore.h"

LogInDialog::LogInDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LogInDialog)
{
    ui->setupUi(this);
}

LogInDialog::~LogInDialog()
{
    delete ui;
}

void LogInDialog::on_okButton_clicked()
{
    const QString name = ui->nameLineEdit->text();
    if (name.isEmpty()) {
        ui->enterNameLabel->setText(QStringLiteral("<b style='color:red;'>*</b>"
                                                   "Enter your name below:"
                                                   "<b style='color:red;'>Cannot be empty!</b>"));
        return;
    }

    ScoreStore store;
    if (store.hasPlayer(name)) {
        QMessageBox::information(this, tr("Log In Successful"),
                                 tr("You have successfully logged in"));
        emit userLoggedIn(name);
    } else {
        QMessageBox::warning(this, tr("Invalid User Name!"),
                             tr("The user name you entered is invalid.\n"
                                "Check your spelling and try again."));
    }
}

void LogInDialog::on_nameLineEdit_textChanged(const QString &text)
{
    ui->nameLineEdit->setText(text.toLower());
}
