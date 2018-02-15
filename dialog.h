#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QSystemTrayIcon>
#include <QStandardItemModel>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

public slots:
    void refreshButtonPressed(void);

private slots:
    void iconActivated(QSystemTrayIcon::ActivationReason reason);

private:
    Ui::Dialog *ui;
    QSystemTrayIcon* poSysTrayIcon;
    QMenu* poSysTrayContextMenu;

    QAction* poCloseAppAction;
    QAction* poShowDialogAction;
    QAction* poHideDialogAction;
    QAction* poRefreshDialogAction;

    QStandardItemModel *oTableStandardItemModel;

    void initSystemTray(void);
    void loadSerialPortList(void);
    void initTableModel(void);

    //overrides
    void hideEvent(QHideEvent *event);
    void showEvent(QShowEvent *event);





};

#endif // DIALOG_H
