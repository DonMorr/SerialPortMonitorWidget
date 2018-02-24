#include "dialog.h"
#include "ui_dialog.h"
#include <QSystemTrayIcon>
#include <QMenu>
#include <QSerialPortInfo>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QMessageBox>

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

    initTableModel();

    loadSerialPortList();
}

/*!
 * \brief Dialog::refreshButtonPressed Handler called when the refresh button is pressed.
 */
void Dialog::refreshButtonPressed(void)
{
    loadSerialPortList();
}

/*!
 * \brief Dialog::about Displays an about Message Box.
 */
void Dialog::about(void)
{
    /*
    QMessageBox msgBox;
    msgBox.setText(QString("Serial Port Monitor Widget v%1 (%2)\n https://github.com/DonMorr/SerialPortMonitorWidget").arg(APP_VERSION).arg(GIT_VERSION));
    msgBox.exec();
    */
    QMessageBox::information(0, "About", QString("Serial Port Monitor Widget v%1 (%2)\nSee the project on GitHub: \nhttps://github.com/DonMorr/SerialPortMonitorWidget").arg(APP_VERSION).arg(GIT_VERSION));
}

/*!
 * \brief Dialog::initTableModel Initialise the model for the main table view.
 */
void Dialog::initTableModel(void)
{
    oTableStandardItemModel = new QStandardItemModel(1,3,this);
    ui->comPortTableIview->setModel(oTableStandardItemModel);
}

/*!
 * \brief Dialog::loadSerialPortList Search and save to the table model any available serial ports.
 */
void Dialog::loadSerialPortList(void)
{
    // Get a list of the available serial ports.
    QList<QSerialPortInfo> aoPorts = QSerialPortInfo::availablePorts();

    // Clear the current model
    oTableStandardItemModel->clear();


    oTableStandardItemModel->setHorizontalHeaderItem(0, new QStandardItem(QString("Name")));
    oTableStandardItemModel->setHorizontalHeaderItem(1, new QStandardItem(QString("Description")));
    oTableStandardItemModel->setHorizontalHeaderItem(2, new QStandardItem(QString("In Use?")));


    // Finally stretch the table to fit the parent.
    for (int c = 0; c < ui->comPortTableIview->horizontalHeader()->count(); ++c)
    {
        ui->comPortTableIview->horizontalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);
    }

    // If there are any serial ports, add them to the model.
    if(aoPorts.length() > 0)
    {
        // Now iterate through the list and add the items through the model.
        QList<QSerialPortInfo>::iterator i;
        for(i = aoPorts.begin(); i != aoPorts.end(); ++i)
        {
#ifdef Q_OS_MACOS
            if(i->portName().contains("tty.usbserial"))
#endif
            {
                qDebug("Serial Port: %s - %s - Is Busy? %s",
                       i->portName().toUtf8().data(),
                       i->description().toUtf8().data(),
                       i->isBusy()?"yes":"No");

                // Create a list of pointers to QStandardItems
                QList<QStandardItem*> oRowList;
                QStandardItem* poRowItem = new QStandardItem(i->portName());
                poRowItem->setEditable(false);
                oRowList << poRowItem;
                poRowItem = new QStandardItem(i->description());
                poRowItem->setEditable(false);
                oRowList << poRowItem;
                poRowItem = new QStandardItem((i->isBusy()?"yes":"No"));
                poRowItem->setEditable(false);
                oRowList << poRowItem;

                // Now add the rowlist
                oTableStandardItemModel->appendRow(oRowList);
            }
        }
    }
    else
    {
        qDebug("No Serial ports found...");
    }
}

/*!
 * \brief Dialog::initSystemTray initialise the system tray.
 */
void Dialog::initSystemTray(void)
{
    if(QSystemTrayIcon::isSystemTrayAvailable())
    {
        // Assemble the contect m      enu actions
        this->poShowDialogAction = new QAction(tr("&Show"), this);
        connect(this->poShowDialogAction, SIGNAL(triggered(bool)), this, SLOT(show()));
        this->poShowDialogAction->setVisible(false);

        this->poHideDialogAction = new QAction(tr("&Hide"), this);
        connect(this->poHideDialogAction, SIGNAL(triggered(bool)), this, SLOT(hide()));

        this->poRefreshDialogAction = new QAction(tr("&Refresh"), this);
        connect(this->poRefreshDialogAction, SIGNAL(triggered(bool)), this, SLOT(refreshButtonPressed()));

        this->poCloseAppAction = new QAction(tr("&Quit"), this);
        connect(this->poCloseAppAction, SIGNAL(triggered(bool)), this, SLOT(accept(void)));

        this->poAboutAction = new QAction(tr("&About"), this);
        connect(this->poAboutAction, SIGNAL(triggered(bool)), this, SLOT(about(void)));

        // Assemble the context menu
        this->poSysTrayContextMenu = new QMenu(this);
        this->poSysTrayContextMenu->addAction(poShowDialogAction);
        this->poSysTrayContextMenu->addAction(poHideDialogAction);
        this->poSysTrayContextMenu->addAction(poRefreshDialogAction);
        this->poSysTrayContextMenu->addSeparator();
        this->poSysTrayContextMenu->addAction(poAboutAction);
        this->poSysTrayContextMenu->addAction(this->poCloseAppAction);


        // Create the system tray icon
        this->poSysTrayIcon = new QSystemTrayIcon(this);
        this->poSysTrayIcon->setContextMenu(this->poSysTrayContextMenu);

        // Connect the click events so we can open the dialog when the tray icon is pressed.
        connect(this->poSysTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

        this->poSysTrayIcon->setIcon(QIcon(":/icons/settings.svg"));
        this->poSysTrayIcon->show();
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("System Tray Unavailable!");
        msgBox.exec();
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
    // Refresh the list when we are shown.
    loadSerialPortList();
}

/*!
 * \brief Dialog::iconActivated Hanles when the system tray icon has been clicked on.
 * \param reason
 */
void Dialog::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        this->show();
        break;
    case QSystemTrayIcon::MiddleClick:
        break;
    default:
        ;
    }
}

Dialog::~Dialog()
{
    delete ui;
}
