#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QSystemTrayIcon>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private:
    Ui::Dialog *ui;
    QSystemTrayIcon* poSysTrayIcon;
    QMenu* poSysTrayContextMenu;

    QAction* poCloseAppAction;
    QAction* poShowDialogAction;
    QAction* poHideDialogAction;

    void initSystemTray(void);
    void loadSerialPortList(void);

    //overrides
    void hideEvent(QHideEvent *event);
    void showEvent(QShowEvent *event);
};

#endif // DIALOG_H
