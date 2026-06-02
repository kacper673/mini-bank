#pragma once
#include <QMainWindow>
#include <QListWidget>
#include <QLabel>
#include <QPushButton>
#include "bank.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void onAccountSelected(QListWidgetItem* current, QListWidgetItem* previous);
    void onNewAccount();
    void onDeposit();
    void onWithdraw();

private:
    void setupUI();
    void refreshAccountList();                      // odswieza liste kont po lewej
    void updateDetailsPanel(long long accountNumber); // wypelnia panel szczegolów
    void clearDetailsPanel();                       // czysci panel gdy nic nie wybrano

    Bank bank_;

    // Widgety — przechowujemy wskazniki zeby moc je aktualizowac w slotach
    QListWidget* accountList_;
    QLabel*      lblNumber_;
    QLabel*      lblOwner_;
    QLabel*      lblPesel_;
    QLabel*      lblDate_;
    QLabel*      lblBalance_;
    QPushButton* depositBtn_;
    QPushButton* withdrawBtn_;
};
