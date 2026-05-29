#include "Account.hpp"
#include <iostream>
#include <ctime>
#include <cstdio>

int Account::_nbAccounts = 0;
int Account::_totalAmount = 0;
int Account::_totalNbDeposits = 0;
int Account::_totalNbWithdrawals = 0;

int	Account::getNbAccounts( void ){ return _nbAccounts;};
int	Account::getTotalAmount( void ){return _totalAmount;};
int	Account::getNbDeposits( void ){return _totalNbDeposits;};
int	Account::getNbWithdrawals( void ){return _totalNbWithdrawals;};
void	Account::displayAccountsInfos( void ){
    _displayTimestamp();
    std::cout << " accounts:" <<  Account::_nbAccounts << ";total:" << Account::_totalAmount << ";deposits:" << Account::_totalNbDeposits << ";withdrawals:" << Account::_totalNbWithdrawals << "\n";

};

Account::Account( int initial_deposit ){
    _accountIndex = Account::_nbAccounts;
    _amount = initial_deposit;
	_nbDeposits = 0;
	_nbWithdrawals = 0;
    Account::_nbAccounts++;
    Account::_totalAmount += initial_deposit;
    _displayTimestamp();
    std::cout << " index:" << _accountIndex << ";amount:" << _amount << ";created\n";
    };

Account::~Account( void ){
    _displayTimestamp();
    std::cout << " index:" << _accountIndex << ";amount:" << _amount << ";closed\n";
};

void	Account::makeDeposit( int deposit ){
    _amount += deposit;
	_nbDeposits++;
    Account::_totalAmount += deposit;
    Account::_totalNbDeposits++;
    _displayTimestamp();
    std::cout << " index:" <<  _accountIndex << ";p_amount:" << _amount - deposit << ";deposit:" << deposit << ";amount:" << _amount << ";nb_deposits:" << _nbDeposits << "\n";
};

bool	Account::makeWithdrawal( int withdrawal ){
    _displayTimestamp();
    if (withdrawal <= _amount)
    {
        _amount -= withdrawal;
        _nbWithdrawals++;
        Account::_totalAmount -= withdrawal;
        Account::_totalNbWithdrawals++;
        std::cout << " index:" <<  _accountIndex << ";p_amount:" << _amount + withdrawal << ";withdrawal:" << withdrawal << ";amount:" << _amount << ";nb_withdrawals:" << _nbWithdrawals << "\n";
        return(true);
    }
    std::cout << " index:" <<  _accountIndex << ";p_amount:" << _amount << ";withdrawal:refused\n";
    return (false);
};

int		Account::checkAmount( void ) const{return _amount;};
void	Account::displayStatus( void ) const{ 
    _displayTimestamp();
    std::cout << " index:" <<  _accountIndex << ";amount:" << _amount << ";deposits:" << _nbDeposits << ";withdrawals:" << _nbWithdrawals << "\n";
};

void	Account::_displayTimestamp( void ){
std::time_t now = std::time(NULL);
    std::tm* now_tm = std::localtime(&now);

    char buffer[16];
    std::sprintf(buffer, "%04d%02d%02d_%02d%02d%02d",
                 now_tm->tm_year + 1900,
                 now_tm->tm_mon + 1,
                 now_tm->tm_mday,
                 now_tm->tm_hour,
                 now_tm->tm_min,
                 now_tm->tm_sec);

    std::cout << "[" << std::string(buffer) << "]";
    //std::cout << "[19920104_091532]";
};
