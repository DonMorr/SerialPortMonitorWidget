#include "dialog.h"
#include "ui_dialog.h"
#include <QSystemTrayIcon>
#include <QMenu>
#include <QSerialPortInfo>
#include <QStandardItemModel>
#include <QStandardItem>

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
    qDebug("refreshButtonPressed");
    loadSerialPortList();
}


/*!
 * \brief Dialog::initTableModel Initialise the model for the main table view.
 */
void Dialog::initTableModel(void)
{
    oTableStandardItemModel = new QStandardItemModel(0,3,this); //2 Rows and 3 Columns
    oTableStandardItemModel->setHorizontalHeaderItem(0, new QStandardItem(QString("Name")));
    oTableStandardItemModel->setHorizontalHeaderItem(1, new QStandardItem(QString("Description")));
    oTableStandardItemModel->setHorizontalHeaderItem(2, new QStandardItem(QString("In Use?")));
    ui->comPortTableIview->setModel(oTableStandardItemModel);
}


/*!
 * \brief Dialog::loadSerialPortList Search and save to the model the available serial ports.
 */
void Dialog::loadSerialPortList(void)
{
    QList<QSerialPortInfo> aoPorts = QSerialPortInfo::availablePorts();
    QList<QSerialPortInfo>::iterator i;

    // Clear the current model
    oTableStandardItemModel->clear();

    if(aoPorts.length() == 0)
    {
        qDebug("No Serial ports found...");
    }
    else
    {
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
                oRowList << new QStandardItem(i->portName())
                         << new QStandardItem(i->description())
                         << new QStandardItem((i->isBusy()?"yes":"No"));

                // Now add the rowlist
                oTableStandardItemModel->appendRow(oRowList);
            }
        }
    }

    // Finally stretch the table to fit the parent.
    for (int c = 0; c < ui->comPortTableIview->horizontalHeader()->count(); ++c)
    {
        ui->comPortTableIview->horizontalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);
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
