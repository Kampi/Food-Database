#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget* parent) : QDialog(parent),
                                            _mUi(new Ui::AboutDialog)
{
    this->_mUi->setupUi(this);
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    this->setWindowFlags(this->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);

    // Replacement arguments for the HTML file
    QString Application = QFileInfo(QCoreApplication::applicationFilePath()).fileName().split(".")[0];
    QList<QString> Args = QList<QString>({QString("%1").arg(VERSION_MAJOR),
                                          QString("%1").arg(VERSION_MINOR),
                                          QString("%1").arg(VERSION_BUILD),
                                          Application});

    QFile File(":/About/Ressources/About/aboutauthor.html");
    File.open(QFile::ReadOnly | QFile::Text);
    QString Text = QTextStream(&File).readAll();

    // Replace the place holder
    int PlaceHolder = Text.count("%%");
    for(int i = 0; i < PlaceHolder; i++)
    {
        Text.replace("%%" + QString("%1").arg(i), Args.at(i));
    }

    this->_mUi->textBrowser_AboutAuthor->setToolTip(QString(tr("Über") + " " + Application));
    this->_mUi->textBrowser_AboutAuthor->setHtml(Text);

    QFile License(":/About/Ressources/About/LICENSE");
    License.open(QFile::ReadOnly);
    this->_mUi->textBrowser_License->setPlainText(QTextStream(&License).readAll());

    this->_mUi->buttonBox_Close->button(QDialogButtonBox::Close)->setText(tr("Schließen"));
}

AboutDialog::~AboutDialog()
{
    delete this->_mUi;
}
