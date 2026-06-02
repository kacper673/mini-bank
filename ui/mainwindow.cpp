#include "mainwindow.h"
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QMessageBox>
#include <sstream>
#include <iomanip>

static QString formatAmount(long long amountGr) {
    std::ostringstream oss;
    oss << amountGr / 100 << "."
        << std::setw(2) << std::setfill('0') << (amountGr % 100)
        << " PLN";
    return QString::fromStdString(oss.str());
}

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("Mini Bank");
    setMinimumSize(700, 440);
    setupUI();
}

void MainWindow::setupUI() {
    auto* central    = new QWidget(this);
    auto* rootLayout = new QVBoxLayout(central);
    rootLayout->setContentsMargins(12, 12, 12, 12);
    rootLayout->setSpacing(10);
    setCentralWidget(central);

    auto* contentRow = new QHBoxLayout();
    contentRow->setSpacing(12);

    auto* leftBox    = new QGroupBox("Konta");
    auto* leftLayout = new QVBoxLayout(leftBox);
    accountList_ = new QListWidget();
    accountList_->setMinimumWidth(220);
    accountList_->setSpacing(2);
    leftLayout->addWidget(accountList_);

    auto* rightBox    = new QGroupBox("Szczegóły konta");
    auto* rightLayout = new QVBoxLayout(rightBox);

    auto* form = new QFormLayout();
    form->setSpacing(8);
    lblNumber_  = new QLabel("—");
    lblOwner_   = new QLabel("—");
    lblPesel_   = new QLabel("—");
    lblDate_    = new QLabel("—");
    lblBalance_ = new QLabel("—");
    lblBalance_->setStyleSheet("font-weight: bold; font-size: 15px;");

    form->addRow("Numer konta:",    lblNumber_);
    form->addRow("Właściciel:",     lblOwner_);
    form->addRow("PESEL:",          lblPesel_);
    form->addRow("Data urodzenia:", lblDate_);
    form->addRow("Saldo:",          lblBalance_);

    auto* btnRow = new QHBoxLayout();
    depositBtn_  = new QPushButton("⬆  Wpłać");
    withdrawBtn_ = new QPushButton("⬇  Wypłać");
    depositBtn_ ->setEnabled(false);
    withdrawBtn_->setEnabled(false);
    depositBtn_ ->setMinimumHeight(32);
    withdrawBtn_->setMinimumHeight(32);
    btnRow->addWidget(depositBtn_);
    btnRow->addWidget(withdrawBtn_);

    rightLayout->addLayout(form);
    rightLayout->addStretch();
    rightLayout->addLayout(btnRow);

    contentRow->addWidget(leftBox);
    contentRow->addWidget(rightBox, 1);

    auto* newAccountBtn = new QPushButton("＋  Nowe konto");
    newAccountBtn->setMinimumHeight(38);
    newAccountBtn->setStyleSheet("font-weight: bold;");

    rootLayout->addLayout(contentRow, 1);
    rootLayout->addWidget(newAccountBtn);

    connect(accountList_, &QListWidget::currentItemChanged,
            this, &MainWindow::onAccountSelected);

    connect(newAccountBtn, &QPushButton::clicked, this, &MainWindow::onNewAccount);
    connect(depositBtn_,   &QPushButton::clicked, this, &MainWindow::onDeposit);
    connect(withdrawBtn_,  &QPushButton::clicked, this, &MainWindow::onWithdraw);
}

void MainWindow::refreshAccountList() {
    long long selectedNum = -1;
    if (auto* item = accountList_->currentItem())
        selectedNum = item->data(Qt::UserRole).toLongLong();

    accountList_->clear();

    for (long long num : bank_.getAccountNumbers()) {
        const Account& acc = bank_.getAccount(num);

        QString text = QString("%1\n%2   |   %3")
            .arg(num)
            .arg(QString::fromStdString(acc.owner.name))
            .arg(formatAmount(acc.getMoneyGr()));

        auto* item = new QListWidgetItem(text);
        item->setData(Qt::UserRole, static_cast<qlonglong>(num));
        accountList_->addItem(item);

        if (num == selectedNum)
            accountList_->setCurrentItem(item);
    }
}

void MainWindow::updateDetailsPanel(long long num) {
    try {
        const Account& acc = bank_.getAccount(num);
        lblNumber_ ->setText(QString::number(num));
        lblOwner_  ->setText(QString::fromStdString(acc.owner.name));
        lblPesel_  ->setText(QString::fromStdString(acc.owner.pesel));
        lblDate_   ->setText(QString::fromStdString(acc.owner.date_of_birth));
        lblBalance_->setText(formatAmount(acc.getMoneyGr()));
        depositBtn_ ->setEnabled(true);
        withdrawBtn_->setEnabled(true);
    }
    catch (const std::exception& e) {
        QMessageBox::warning(this, "Błąd", e.what());
    }
}

void MainWindow::clearDetailsPanel() {
    lblNumber_ ->setText("—");
    lblOwner_  ->setText("—");
    lblPesel_  ->setText("—");
    lblDate_   ->setText("—");
    lblBalance_->setText("—");
    depositBtn_ ->setEnabled(false);
    withdrawBtn_->setEnabled(false);
}

void MainWindow::onAccountSelected(QListWidgetItem* current, QListWidgetItem*) {
    if (!current) { clearDetailsPanel(); return; }
    updateDetailsPanel(current->data(Qt::UserRole).toLongLong());
}

void MainWindow::onNewAccount() {
    QDialog dlg(this);
    dlg.setWindowTitle("Nowe konto");
    dlg.setMinimumWidth(340);

    auto* form      = new QFormLayout(&dlg);
    auto* nameEdit  = new QLineEdit();
    auto* peselEdit = new QLineEdit();
    auto* dateEdit  = new QLineEdit();

    peselEdit->setMaxLength(11);
    dateEdit ->setPlaceholderText("DD.MM.RRRR");

    form->addRow("Imię i nazwisko:", nameEdit);
    form->addRow("PESEL:",           peselEdit);
    form->addRow("Data urodzenia:",  dateEdit);

    auto* btns = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    form->addRow(btns);
    connect(btns, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(btns, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() != QDialog::Accepted) return;

    try {
        long long num = bank_.createAccount(
            nameEdit ->text().trimmed().toStdString(),
            peselEdit->text().trimmed().toStdString(),
            dateEdit ->text().trimmed().toStdString()
        );
        refreshAccountList();
        QMessageBox::information(this, "Sukces",
            QString("Konto utworzone pomyślnie.\nNumer: %1").arg(num));
    }
    catch (const std::exception& e) {
        QMessageBox::warning(this, "Błąd tworzenia konta", e.what());
    }
}

static bool getAmount(QWidget* parent, const QString& title, long long& outGr) {
    QDialog dlg(parent);
    dlg.setWindowTitle(title);
    dlg.setMinimumWidth(260);

    auto* form       = new QFormLayout(&dlg);
    auto* zloteEdit  = new QLineEdit("0");
    auto* groszeEdit = new QLineEdit("0");
    form->addRow("Złote:",         zloteEdit);
    form->addRow("Grosze (0–99):", groszeEdit);

    auto* btns = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    form->addRow(btns);
    QObject::connect(btns, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    QObject::connect(btns, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() != QDialog::Accepted) return false;

    bool ok1, ok2;
    long long z = zloteEdit ->text().toLongLong(&ok1);
    long long g = groszeEdit->text().toLongLong(&ok2);

    if (!ok1 || !ok2 || z < 0 || g < 0 || g > 99) {
        QMessageBox::warning(parent, "Błąd", "Nieprawidłowa kwota.");
        return false;
    }
    outGr = z * 100 + g;
    return true;
}

void MainWindow::onDeposit() {
    auto* item = accountList_->currentItem();
    if (!item) return;
    long long num = item->data(Qt::UserRole).toLongLong();

    long long amount = 0;
    if (!getAmount(this, "Wpłata", amount)) return;

    try {
        bank_.deposit(num, amount);
        updateDetailsPanel(num);
        refreshAccountList();
    }
    catch (const std::exception& e) {
        QMessageBox::warning(this, "Błąd wpłaty", e.what());
    }
}

void MainWindow::onWithdraw() {
    auto* item = accountList_->currentItem();
    if (!item) return;
    long long num = item->data(Qt::UserRole).toLongLong();

    long long amount = 0;
    if (!getAmount(this, "Wypłata", amount)) return;

    try {
        bank_.withdraw(num, amount);
        updateDetailsPanel(num);
        refreshAccountList();
    }
    catch (const std::exception& e) {
        QMessageBox::warning(this, "Błąd wypłaty", e.what());
    }
}
