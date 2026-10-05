#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

namespace Ui {
class LogInDialog;
}

class LogInDialog : public QDialog
{
    Q_OBJECT
public:
    explicit LogInDialog(QWidget *parent = nullptr);
    ~LogInDialog() override;

signals:
    void userLoggedIn(const QString &name);

private slots:
    void on_okButton_clicked();
    void on_nameLineEdit_textChanged(const QString &text);

private:
    Ui::LogInDialog *ui;
};

#endif // LOGINDIALOG_H
