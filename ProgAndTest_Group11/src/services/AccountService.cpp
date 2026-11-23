#include "AccountService.h"
#include <functional>
#include <cctype>
#include <sstream>
std::string AccountService::hashPassword(const std::string& p){ std::ostringstream s; s<<std::hex<<std::hash<std::string>{}(p); return s.str(); }
bool AccountService::validEmail(const std::string& e){ auto at=e.find('@'); return at!=std::string::npos && at>0 && e.find('.',at)!=std::string::npos && e.find(' ') == std::string::npos; }
bool AccountService::registerUser(const std::string& n,const std::string& e,const std::string& p,const std::string& c,const std::string& ph,std::string& err){
 if(n.empty()||e.empty()||p.empty()||ph.empty()){err="Vui long nhap day du thong tin.";return false;}
 if(!validEmail(e)){err="Email khong hop le.";return false;} if(p.size()<8){err="Mat khau phai co it nhat 8 ky tu.";return false;} if(p!=c){err="Mat khau xac nhan khong khop.";return false;} if(repo.emailExists(e)){err="Email da duoc dang ky.";return false;}
 User u;u.name=n;u.email=e;u.passwordHash=hashPassword(p);u.phone=ph;u.role="customer";repo.add(u);return true; }
bool AccountService::login(const std::string& e,const std::string& p,Auth& a,std::string& err){ const User* u=repo.findByEmail(e); if(!u||u->role!="customer"||u->passwordHash!=hashPassword(p)){err="Email hoac mat khau khong dung.";return false;}a.login(*u);return true; }
bool AccountService::updateProfile(Auth& a,const std::string& n,const std::string& ph,std::string& err){ if(!a.isLoggedIn()||a.role()!="customer"){err="Can dang nhap tai khoan khach hang.";return false;} if(n.empty()||ph.empty()){err="Ten va so dien thoai khong duoc bo trong.";return false;} User* u=repo.findByEmail(a.user()->email); if(!u){err="Khong tim thay tai khoan.";return false;}u->name=n;u->phone=ph;User updated=*u;a.login(updated);return true; }
bool AccountService::recoverPassword(const std::string& e,const std::string& p,std::string& err){ User* u=repo.findByEmail(e); if(!u){err="Email khong ton tai.";return false;} if(p.size()<8){err="Mat khau moi phai co it nhat 8 ky tu.";return false;}u->passwordHash=hashPassword(p);return true; }
bool AccountService::adminLogin(const std::string& e,const std::string& p,Auth& a,std::string& err){const User* u=repo.findByEmail(e);if(!u||u->role!="admin"||u->passwordHash!=hashPassword(p)){err="Thong tin admin khong hop le.";return false;}a.login(*u);return true;}

