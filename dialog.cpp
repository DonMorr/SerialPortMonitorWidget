#include "dialog.h"
#include "ui_dialog.h"
#include <QSystemTrayIcon>
#include <QMenu>
#include <QSerialPortInfo>

/*!
 * \brief Dialog::Dialog Constructor for creating our dialog.
 * \param parent pointer to parent QWidget object.
 */
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    initSystemTray();

    loadSerialPortList();


}


void Dialog::loadSerialPortList(void)
{
    //ui->comPortListIview;

    QList<QSerialPortInfo> aoPorts = QSerialPortInfo::availablePorts();
    QList<QSerialPortInfo>::iterator i;

    for(i = aoPorts.begin(); i != aoPorts.end(); ++i)
    {
        if(i->portName().contains("tty.usbserial"))
        {
            qDebug("Serial Port: %s - %s - Is Busy? %s",
                   i->portName().toUtf8().data(),
                   i->description().toUtf8().data(),
                   i->isBusy()?"yes":"No");
        }
    }

}


/*!
 * \brief Dialog::initSystemTray initialise the system tray.
 */
void Dialog::initSystemTray(void)
{
    if(QSystemTrayIcon::isSystemTrayAvailable())
    {
        qDebug("System Tray Available!");


        //! Assemble the contect menu actions
        this->poShowDialogAction = new QAction(tr("&Show"), this);
        connect(this->poShowDialogAction, SIGNAL(triggered(bool)), this, SLOT(show()));
        this->poShowDialogAction->setVisible(false);

        this->poHideDialogAction = new QAction(tr("&Hide"), this);
        connect(this->poHideDialogAction, SIGNAL(triggered(bool)), this, SLOT(hide()));

        this->poCloseAppAction = new QAction(tr("&Quit"), this);
        connect(this->poCloseAppAction, SIGNAL(triggered(bool)), this, SLOT(accept(void)));

        //! Assemble the context menu
        this->poSysTrayContextMenu = new QMenu(this);
        this->poSysTrayContextMenu->addAction(poShowDialogAction);
        this->poSysTrayContextMenu->addAction(poHideDialogAction);
        this->poSysTrayContextMenu->addSeparator();
        this->poSysTrayContextMenu->addAction(this->poCloseAppAction);

        //! Create the system tray icon
        this->poSysTrayIcon = new QSystemTrayIcon(this);
        this->poSysTrayIcon->setContextMenu(this->poSysTrayContextMenu);

        this->poSysTrayIcon->setIcon(QIcon(":/icons/724094.png"));
        this->poSysTrayIcon->show();
    }
    else
    {
        qDebug("System Tray Unavailable!");
    }
}

/*!
 * \brief Dialog::hideEvent Overriding the hide event.
 * \param event
 */
void Dialog::hideEvent(QHideEvent *event)
{
    (void) event;
    this->poShowDialogAction->setVisible(true);
    this->poHideDialogAction->setVisible(false);
}

/*!
 * \brief Dialog::showEvent Override the show event.
 * \param event
 */
void Dialog::showEvent(QShowEvent *event)
{
    (void) event;
    this->poShowDialogAction->setVisible(false);
    this->poHideDialogAction->setVisible(true);
}

Dialog::~Dialog()
{
    delete ui;
}
