#ifndef SIGNUPDIALOG_H
#define SIGNUPDIALOG_H

#include <QDialog>

namespace Ui {
class SignUpDialog;
}

class SignUpDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SignUpDialog(QWidget *parent = nullptr);
    ~SignUpDialog() override;

signals:
    void successfulSignUp(const QString &name);

private slots:
    void on_okButton_clicked();
    void on_nameLineEdit_textChanged(const QString &text);

private:
    Ui::SignUpDialog *ui;
};

#endif // SIGNUPDIALOG_H
