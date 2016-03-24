#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "clientsrc.h"

std::string host = "";
int port = DEFAULT_PORT;
std::string nickname = "";
QString imagePath = NULL;
bool isPicSet = false;
font usrFont;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    usrFont.color = "#333333";
    usrFont.style = QFont("Arial", 12);

    conn = new Connection(this);
    setCentralWidget(conn);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::set_app_title(QString str)
{
    setWindowTitle(str);
}

void MainWindow::set_style()
{
    QFile File("style/test.qss");
    File.open(QFile::ReadOnly);
    QString StyleSheet = QLatin1String(File.readAll());

    qApp->setStyleSheet(StyleSheet);
}

void MainWindow::on_actionProfile_Pic_triggered()
{
    imagePath = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("JPEG (*.jpg *.jpeg);;PNG (*.png)" ));
    isPicSet = true;
    clnt->drawProfilePic();
}

void MainWindow::on_actionConnect_triggered()
{
    conn = new Connection(this);
    setCentralWidget(conn);
}

void MainWindow::on_actionFont_Color_triggered()
{
    QColor color = QColorDialog::getColor(Qt::black, this, "Text Color",  QColorDialog::DontUseNativeDialog);
    if(color.isValid()) {
        usrFont.color = color.name();
    }
}

void MainWindow::on_actionFont_Style_triggered()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, QFont("Arial", 12), this);
    if (ok) {
        usrFont.style = font;
    }
}


void MainWindow::client_start()
{
    clnt = new Client(this);
    setCentralWidget(clnt);
}

void MainWindow::closeEvent (QCloseEvent *event)
{
    clnt->client_quit();
}
