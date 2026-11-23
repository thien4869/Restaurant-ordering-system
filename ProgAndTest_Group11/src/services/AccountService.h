#ifndef ACCOUNTSERVICE_H
#define ACCOUNTSERVICE_H
#include "../repositories/UserRepository.h"
#include "../models/Auth.h"
class AccountService {
    UserRepository& repo;
    static std::string hashPassword(const std::string& password);
    static bool validEmail(const std::string& email);
public:
    explicit AccountService(UserRepository& r):repo(r){}
    bool registerUser(const std::string& name,const std::string& email,const std::string& password,const std::string& confirm,const std::string& phone,std::string& error);
    bool login(const std::string& email,const std::string& password,Auth& auth,std::string& error);
    bool updateProfile(Auth& auth,const std::string& name,const std::string& phone,std::string& error);
    bool recoverPassword(const std::string& email,const std::string& newPassword,std::string& error);
    bool adminLogin(const std::string& email,const std::string& password,Auth& auth,std::string& error);
};
#endif

