#include "signupdialog.h"
#include "ui_signupdialog.h"

#include <QMessageBox>

#include "../core/scorestore.h"

SignUpDialog::SignUpDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SignUpDialog)
{
    ui->setupUi(this);
}

SignUpDialog::~SignUpDialog()
{
    delete ui;
}

void SignUpDialog::on_okButton_clicked()
{
    const QString name = ui->nameLineEdit->text();
    const QString email = ui->emailLineEdit->text();

    if (name.isEmpty()) {
        ui->enterNameLabel->setText(QStringLiteral("<b style='color:red;'>*</b>"
                                                   "Enter your name below:"
                                                   "<b style='color:red;'>Cannot be empty!</b>"));
        return;
    }

    ScoreStore store;
    if (store.hasPlayer(name)) {
        ui->enterNameLabel->setText(QStringLiteral("Enter your name below:"
                                                   "<b style='color:red;'>User already exists!</b>"));
        return;
    }

    store.saveScores(name, QList<int>());

    QMessageBox::information(this, tr("Sign Up Successful"),
                             tr("<b style='color:blue;'>Name :</b>%1<br>"
                                "<b style='color:blue;'>Email:</b>%2<br>")
                                 .arg(name, email));
    emit successfulSignUp(name);
}

void SignUpDialog::on_nameLineEdit_textChanged(const QString &text)
{
    ui->nameLineEdit->setText(text.toLower());
}
