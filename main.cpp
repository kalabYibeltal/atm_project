#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <conio.h>
#include <time.h>
#include <Windows.h>
//#include <curses.h>
//#include <graphics.h>

using namespace std;

time_t now;

string pinc(){//fun_for alarm and making the code in astrix
   int i=0;
    char new1[4]; 
    while( i<10){int c;
        new1[i]=getch();
        c=new1[i];
		if(c==13){break;
        }
        else if(c==8&&i>0){
            cout<<"\b \b";
            --i;--i;
        }
        else{
            cout<<"\a*";
        }
        i++;
    }
    ofstream pas;
	pas.open("imagin.txt");
	pas<<new1[0]<<new1[1]<<new1[2]<<new1[3]<<endl;
	pas.close();
	ifstream pus;
	string re;
	pus.open("imagin.txt");
	pus>>re;
	pus.close();
	return re;
}
string date(time_t now){//fun_for time in usual format
    string mon, tot;
    tm nowlocal{};
    now = time(nullptr);
    nowlocal =* localtime(&now);
    switch (nowlocal.tm_mon){
        case 0:
            mon = "Jan";
            break;
        case 1:
            mon = "Feb";
            break;
        case 2:
            mon = "Mar";
            break;
        case 3:
            mon = "Apr";
            break;
        case 4:
            mon = "May";
            break;
        case 5:
            mon = "Jun";
            break;
        case 6:
            mon = "Jul";
            break;
        case 7:
            mon = "Aug";
            break;
        case 8:
            mon = "Sept";
            break;
        case 9:
            mon = "Oct";
            break;
        case 10:
            mon = "Nov";
            break;
        case 11:
            mon = "Dec";
            break;
    }
//    tot += mon;
    tot = mon + " " +  to_string(nowlocal.tm_mday)+ " " + to_string(nowlocal.tm_year + 1900)  + ","  + to_string(nowlocal.tm_hour) + ":" + to_string(nowlocal.tm_min) + ":" + to_string(nowlocal.tm_sec);
    return tot;
}
// atm and customer clases for respective object which initialize when the
// porgram starts
class atm{
private:
    bool stat;
    unsigned int total_balance;
public:
    atm(){
        ifstream aatm;
        aatm.open("atm.csv");
        if (aatm.is_open()) {
            string x;
            aatm >> x;
            stat = (x == "true");
            x.clear();
            aatm >> x;
            total_balance = stoi(x);
            x.clear();
        } else{
            cerr << "couldn't open file." << endl;
            exit(1);
        }
        aatm.close();
    }

    void atm_withdraw(unsigned int x){
        total_balance -= x;
        ofstream aatm;
        aatm.open("atm.csv");
        if (aatm.is_open()){
            aatm << (stat?"true":"false") << endl;
            aatm << total_balance << endl;
        } else{
            cerr << "error." << endl;
            exit(1);
        }
        aatm.close();
    }
    unsigned int balance(){
        return total_balance;
    }
};

class cus{
private:
    string account_no;
    bool stat;
    string pin;
    string user_name;
    unsigned int balance;
    string filename;
public:
    cus(const string& accno){
        bool gr = false;
        ifstream bn;
        bn.open("accounts.csv");
        if (bn.is_open()){
            string x;
            while (bn.good()){
                bn >> x;
                if (x == accno){
                    gr = true;
                    break;
                }
            }
        } else{
            cerr << "error." << endl;
            exit(1);
        }
        bn.close();

        if (gr){
            string x;
            x = "accounts/" + accno + ".csv";
            filename = x;
//            x.clear();
            ifstream nu;
            nu.open(x);
            x.clear();
            if (nu.is_open()){
                getline(nu, x, ',');
                account_no = x;
                x.clear();
                getline(nu, x, ',');
                stat = (x == "true");
                x.clear();
                if (stat){
                    getline(nu, x, ',');
                    pin = x;
                    x.clear();
                    getline(nu, x, ',');
                    balance = stoi(x);
                    x.clear();
                    getline(nu, x, ',');
                    user_name = x;
                    x.clear();
                } else{
                	cout<<"Blocked_Account_Trai_Login wait 24hrs"<<endl;
                    //log
                    ofstream lo("log.csv", ios::app);
                    if (lo.is_open()){
                        lo<< date(now) << "," << account_no <<"," << "Blocked_Account_Trai_Login" << "," <<"--"<< "," << "--"<<"," <<endl;
                    } else{
                        cerr << "error." << endl;
                    }
                    lo.close();
//                    cout << "error." << endl;
                    exit(2);
                }
            } else{
                cerr << "error." << endl;
                exit(1);
            }
            nu.close();
        } else{
        	cout<<"unsuccesful trail or unknown account";
            //log
            ofstream lo("log.csv", ios::app);
            if (lo.is_open()){
                lo<< date(now) << "," <<"--"<<"," << "Unsuccesful_Trai_Login" << "," <<"--"<< "," << "--"<<"," <<endl;
            } else{
                cerr << "error." << endl;
            }
            lo.close();
            cerr << "error." << endl;
            exit(3);
        }
    }

    void withdraw(unsigned int x){
        balance -= x;
        ofstream nu;
        nu.open(filename);
        if (nu.is_open()){
            nu << account_no << "," << (stat? "true":"false") << "," << pin << "," << balance << "," << user_name << endl;
        } else{
            cerr << "error" << endl;
            exit(1);
        }
        nu.close();
    }
    void accept(unsigned int x){
        balance += x;
        ofstream nu;
        nu.open(filename);
        if (nu.is_open()){
            nu << account_no << "," << (stat? "true":"false") << "," << pin << "," << balance << "," << user_name << endl;
        } else{
            cerr << "error" << endl;
            exit(1);
        }
        nu.close();
    }
    void pinchange(const string& np){
        const string& x = np;
        pin = x;
        ofstream nu;
        nu.open(filename);
        if (nu.is_open()){
            string a = pin;
            nu << account_no << "," << (stat? "true":"false") << "," << x << "," << balance << "," << user_name << endl;
        } else{
            cerr << "error" << endl;
            exit(1);
        }
        nu.close();
    }
    unsigned int getbalance(){
        return balance;
    }
    bool validate(string st){
        return (st == pin);
    }
    string getusername(){
        return user_name;
    }
    void lockdown(){
        stat = false;
        ofstream nu;
        nu.open(filename);
        if (nu.is_open()){
            nu << account_no << "," << (stat? "true":"false") << "," << pin << "," << balance << "," << user_name << endl;
        } else{
            cerr << "error" << endl;
            exit(1);
        }
        nu.close();
    }
};

int main(){
//	int gmode= DETECT ,gm;
//	initgraph(&gmode,&gm," ");
//	readimagefile("Capture.jpg",100,100,300,300);
//	getch();
//	closegraph();
    atm gh;
    time_t i = 0;
    string accno, pin;;
    char a;
    t:
	system("cls");
	system("color 0a");
    
	cout << "\t" << "ATM Simulator program" << endl;
    cout << "Enter your card no: ";
    cin >> accno;
    cus uno(accno);
    l:
    cout << "\nEnter your pin here: ";
	pin = pinc();
    system("cls");
    if (uno.validate(pin)){
        goto y;
    } else{
       static int c = 3;system("color c");
        while (c > 1 && !uno.validate(pin)){
        	
            cout << "Incorrect pin. you have " << --c << " trials left." << endl;
             //cout << "Enter your pin here: "<<endl;
				goto l;
   			// pin = pinc();
            
        }
        if (c <= 1){
            cout << "TO much incorrect attempt, ACCOUNT LOCKED!" << endl;
            uno.lockdown();
            goto t;
        }
    }
    y:
    system("cls");
    system("color 43");
    cout << "welcome " << uno.getusername() << endl;
    cout << endl << "Choose your next action carefully : " << endl;
    cout<<setw(80)<<setfill('-') << "-"<<endl;
    cout << setw(40) << setfill(' ')  <<  "1) withdraw" << setw(30)  << setfill(' ') << "4) transfer" << endl;
    cout << setw(40) << setfill(' ')  << "2) check balance" << setw(30) << setfill(' ') << " 5) Change Pincode" << endl;
    cout << setw(40) << setfill(' ')  << "3) Fast Cash 100" << endl;
    cout<<setw(80)<<setfill('-') << "-"<<endl;
    cout << "Enter your prefered action: ";
    cin >> a;

    if (a == '1'){
        z:
        unsigned int al;
        system("cls");
        
        cout << endl << "Choose your next action carefully : " << endl;
    cout<<setw(80)<<setfill('-') << "-"<<endl;
    cout << setw(40) << setfill(' ')  <<  "1) 200" << setw(30)  << setfill(' ') << "4) 2000" << endl;
    cout << setw(40) << setfill(' ')  << "2) 500" << setw(30) << setfill(' ') << " 5) 5000" << endl;
    cout << setw(40) << setfill(' ')  << "3) 1000" << setw(30) << setfill(' ') << " 6) for other" << endl;
    cout<<setw(80)<<setfill('-') << "-"<<endl;
    int qp;
    cin>>qp;
    system("cls");
    if(qp==1){
    	if (200 < gh.balance() && 200 < uno.getbalance()){
            gh.atm_withdraw(200);
            uno.withdraw(200);
            //transaction
            ofstream tr;
            tr.open("transaction.csv", ios::app);
            if (tr.is_open()){
                tr << date(i) << "," << accno << "," << "withdraw" << "," << 100 << "," << endl;
            } else{
                cerr << "error." << endl;
            }
            tr.close();
            //log
            ofstream lo("log.csv", ios::app);
            if (lo.is_open()){
                lo << date(i) << "," << accno << "," << "withdraw" << "," << 100 << "," << endl;
            } else{
                cerr << "error." << endl;
            }
            lo.close();
            cout<<"\nwithdrawing";
            Sleep(0500);
            cout<<".";
            Sleep(0500);
            cout<<".";
            Sleep(0500);
            cout<<".";
             system("cls");
             system("color 3a");
            cout << "Here is your receit." << endl;
            cout<<setw(50)<<setfill('*') << "*"<<endl;
            cout << setw(25) << setfill(' ')  <<  "Location : " << setw(10)  << setfill(' ') << "AAiT Amist kilo" << endl;
            cout << setw(25) << setfill(' ')  <<  "Date and Time : " << setw(10)  << setfill(' ') << date(i) << endl;
            cout << setw(25) << setfill(' ')  << "Account Number : " << setw(10) << setfill(' ') << accno << endl;
            cout << setw(25) << setfill(' ')  << "Amount withdrawn : " << setw(10) << setfill(' ') << 200 << " BIRR ONLY" << endl;
            cout << setw(50) << setfill('*') << "*" << endl;
            cout << "Transaction successful." << endl;
        } else{
            //log
            cout<<"unsuccesful Trial"<<endl;
            ofstream lo("log.csv", ios::app);
            if (lo.is_open()){
                lo << date(i) << "," << accno << "," << "Unsuccesful_Trial" << "," << "--" << "," << "--" << "," << endl;
            } else{
                cerr << "error." << endl;
            }
            lo.close();
            goto z;
	}}
	if(qp==2){
		if (500 < gh.balance() && 500 < uno.getbalance()){
            gh.atm_withdraw(500);
            uno.withdraw(500);
            //transaction
            ofstream tr;
            tr.open("transaction.csv", ios::app);
            if (tr.is_open()){
                tr << date(i) << "," << accno << "," << "withdraw" << "," << 100 << "," << endl;
            } else{
                cerr << "error." << endl;
            }
            tr.close();
            //log
            ofstream lo("log.csv", ios::app);
            if (lo.is_open()){
                lo << date(i) << "," << accno << "," << "withdraw" << "," << 100 << "," << endl;
            } else{
                cerr << "error." << endl;
            }
            lo.close();
            cout<<"\nwithdrawing";
            Sleep(0500);
            cout<<".";
            Sleep(0500);
            cout<<".";
            Sleep(0500);
            cout<<".";
             system("cls");
             system("color 3a");
            cout << "Here is your receit." << endl;
            cout<<setw(50)<<setfill('*') << "*"<<endl;
            cout << setw(25) << setfill(' ')  <<  "Location : " << setw(10)  << setfill(' ') << "AAiT Amist kilo" << endl;
            cout << setw(25) << setfill(' ')  <<  "Date and Time : " << setw(10)  << setfill(' ') << date(i) << endl;
            cout << setw(25) << setfill(' ')  << "Account Number : " << setw(10) << setfill(' ') << accno << endl;
            cout << setw(25) << setfill(' ')  << "Amount withdrawn : " << setw(10) << setfill(' ') << 500 << " BIRR ONLY" << endl;
            cout << setw(50) << setfill('*') << "*" << endl;
            cout << "Transaction successful." << endl;
        } else{
            //log
            cout<<"unsuccesful Trial"<<endl;
            ofstream lo("log.csv", ios::app);
            if (lo.is_open()){
                lo << date(i) << "," << accno << "," << "Unsuccesful_Trial" << "," << "--" << "," << "--" << "," << endl;
            } else{
                cerr << "error." << endl;
            }
            lo.close();
            goto z;
	}
}
	
	if(qp==3){
		if (1000 < gh.balance() && 1000 < uno.getbalance()){
            gh.atm_withdraw(1000);
            uno.withdraw(1000);
            //transaction
            ofstream tr;
            tr.open("transaction.csv", ios::app);
            if (tr.is_open()){
                tr << date(i) << "," << accno << "," << "withdraw" << "," << 100 << "," << endl;
            } else{
                cerr << "error." << endl;
            }
            tr.close();
            //log
            ofstream lo("log.csv", ios::app);
            if (lo.is_open()){
                lo << date(i) << "," << accno << "," << "withdraw" << "," << 100 << "," << endl;
            } else{
                cerr << "error." << endl;
            }
            lo.close();
            cout<<"\nwithdrawing";
            Sleep(0500);
            cout<<".";
            Sleep(0500);
            cout<<".";
            Sleep(0500);
            cout<<".";
             system("cls");
             system("color 3a");
            cout << "Here is your receit." << endl;
            cout<<setw(50)<<setfill('*') << "*"<<endl;
            cout << setw(25) << setfill(' ')  <<  "Location : " << setw(10)  << setfill(' ') << "AAiT Amist kilo" << endl;
            cout << setw(25) << setfill(' ')  <<  "Date and Time : " << setw(10)  << setfill(' ') << date(i) << endl;
            cout << setw(25) << setfill(' ')  << "Account Number : " << setw(10) << setfill(' ') << accno << endl;
            cout << setw(25) << setfill(' ')  << "Amount withdrawn : " << setw(10) << setfill(' ') << 1000 << " BIRR ONLY" << endl;
            cout << setw(50) << setfill('*') << "*" << endl;
            cout << "Transaction successful." << endl;
        } else{
            //log
            cout<<"unsuccesful Trial"<<endl;
            ofstream lo("log.csv", ios::app);
            if (lo.is_open()){
                lo << date(i) << "," << accno << "," << "Unsuccesful_Trial" << "," << "--" << "," << "--" << "," << endl;
            } else{
                cerr << "error." << endl;
            }
            lo.close();
            goto z;
	}}
	if(qp==4){
		if (2000 < gh.balance() && 2000 < uno.getbalance()){
            gh.atm_withdraw(2000);
            uno.withdraw(2000);
            //transaction
            ofstream tr;
            tr.open("transaction.csv", ios::app);
            if (tr.is_open()){
                tr << date(i) << "," << accno << "," << "withdraw" << "," << 100 << "," << endl;
            } else{
                cerr << "error." << endl;
            }
            tr.close();
            //log
            ofstream lo("log.csv", ios::app);
            if (lo.is_open()){
                lo << date(i) << "," << accno << "," << "withdraw" << "," << 100 << "," << endl;
            } else{
                cerr << "error." << endl;
            }
            lo.close();
            cout<<"\nwithdrawing";
            Sleep(0500);
            cout<<".";
            Sleep(0500);
            cout<<".";
            Sleep(0500);
            cout<<".";
             system("cls");
             system("color 3a");
            cout << "Here is your receit." << endl;
            cout<<setw(50)<<setfill('*') << "*"<<endl;
            cout << setw(25) << setfill(' ')  <<  "Location : " << setw(10)  << setfill(' ') << "AAiT Amist kilo" << endl;
            cout << setw(25) << setfill(' ')  <<  "Date and Time : " << setw(10)  << setfill(' ') << date(i) << endl;
            cout << setw(25) << setfill(' ')  << "Account Number : " << setw(10) << setfill(' ') << accno << endl;
            cout << setw(25) << setfill(' ')  << "Amount withdrawn : " << setw(10) << setfill(' ') << 2000 << " BIRR ONLY" << endl;
            cout << setw(50) << setfill('*') << "*" << endl;
            cout << "Transaction successful." << endl;
        } else{
            //log
            cout<<"unsuccesful Trial"<<endl;
            ofstream lo("log.csv", ios::app);
            if (lo.is_open()){
                lo << date(i) << "," << accno << "," << "Unsuccesful_Trial" << "," << "--" << "," << "--" << "," << endl;
            } else{
                cerr << "error." << endl;
            }
            lo.close();
            goto z;
	}
	}
	if(qp==5){
		if (5000 < gh.balance() && 5000 < uno.getbalance()){
            gh.atm_withdraw(5000);
            uno.withdraw(5000);
            //transaction
            ofstream tr;
            tr.open("transaction.csv", ios::app);
            if (tr.is_open()){
                tr << date(i) << "," << accno << "," << "withdraw" << "," << 100 << "," << endl;
            } else{
                cerr << "error." << endl;
            }
            tr.close();
            //log
            ofstream lo("log.csv", ios::app);
            if (lo.is_open()){
                lo << date(i) << "," << accno << "," << "withdraw" << "," << 100 << "," << endl;
            } else{
                cerr << "error." << endl;
            }
            lo.close();
            cout<<"\nwithdrawing";
            Sleep(0500);
            cout<<".";
            Sleep(0500);
            cout<<".";
            Sleep(0500);
            cout<<".";
             system("cls");
             system("color 3a");
            cout << "Here is your receit." << endl;
            cout<<setw(50)<<setfill('*') << "*"<<endl;
            cout << setw(25) << setfill(' ')  <<  "Location : " << setw(10)  << setfill(' ') << "AAiT Amist kilo" << endl;
            cout << setw(25) << setfill(' ')  <<  "Date and Time : " << setw(10)  << setfill(' ') << date(i) << endl;
            cout << setw(25) << setfill(' ')  << "Account Number : " << setw(10) << setfill(' ') << accno << endl;
            cout << setw(25) << setfill(' ')  << "Amount withdrawn : " << setw(10) << setfill(' ') << 5000 << " BIRR ONLY" << endl;
            cout << setw(50) << setfill('*') << "*" << endl;
            cout << "Transaction successful." << endl;
        } else{
            //log
            cout<<"unsuccesful Trial"<<endl;
            ofstream lo("log.csv", ios::app);
            if (lo.is_open()){
                lo << date(i) << "," << accno << "," << "Unsuccesful_Trial" << "," << "--" << "," << "--" << "," << endl;
            } else{
                cerr << "error." << endl;
            }
            lo.close();
            goto z;
	}}
	if(qp==6){
		        cout << "Enter the amount of money you want to withdraw: ";
        cin >> al;
        system("cls");
        if (al < gh.balance() && al < uno.getbalance() && al <= 5000 && al >= 50){
            gh.atm_withdraw(al);
            uno.withdraw(al);
            //transaction
            ofstream tr;
            tr.open("transaction.csv", ios::app);
            if (tr.is_open()){
                tr << date(i) << "," << accno << "," << "withdraw" << "," << al << "," << endl;
            } else{
                cerr << "error." << endl;
            }
            tr.close();
            //log
            ofstream lo("log.csv", ios::app);
            if (lo.is_open()){
                lo << date(i) << "," << accno << "," << "withdraw" << "," << al << "," << endl;
            } else{
                cerr << "error." << endl;
            }
            lo.close();
            cout<<"\nwithdrawing";
            
            cout<<".";
            Sleep(0500);
			
            cout<<".";
            Sleep(0500);
            cout<<".";
            
            cout << "Here is your receit." << endl;
            cout<<setw(50)<<setfill('*') << "*"<<endl;
            cout << setw(25) << setfill(' ')  <<  "Location : " << setw(10)  << setfill(' ') << "AAiT Amist kilo" << endl;
            cout << setw(25) << setfill(' ')  <<  "Date and Time : " << setw(10)  << setfill(' ') << date(i) << endl;
            cout << setw(25) << setfill(' ')  << "Account Number : " << setw(10) << setfill(' ') << accno << endl;
            cout << setw(25) << setfill(' ')  << "Amount withdrawn : " << setw(10) << setfill(' ') << al << " BIRR ONLY" << endl;
            cout << setw(50) << setfill('*') << "*" << endl;
            cout << "Transaction successful." << endl;
        } else{
            //log
            cout<<"you have requsted more than or less than you are supposed to get"<<endl;
            ofstream lo("log.csv", ios::app);
            if (lo.is_open()){
                lo << date(i) << "," << accno << "," << "Unsuccesful_Trial" << "," << "--" << "," << "--" << "," << endl;
            } else{
                cerr << "error." << endl;
            }
            lo.close();
            goto z;
        }
	}
     
    
 

    } else if (a == '2'){
    	 cout<<"\nchecking balance";
            Sleep(0500);
            cout<<".";
            Sleep(0500);
            cout<<".";
            Sleep(0500);
            cout<<".";
             system("cls");
             system("color 2a");
    	
        cout << "Your remaining balance is " << uno.getbalance() << endl;
        //log
        ofstream lo("log.csv", ios::app);
        if (lo.is_open()){
            lo << date(i) << "," << accno << "," << "Checking_Balance" << "," << "--" << "," << "--" << "," << endl;
        } else{
            cerr << "error." << endl;
            exit(1);
        }
        lo.close();
    } else if (a == '3'){
        if (100 < gh.balance() && 100 < uno.getbalance()){
            gh.atm_withdraw(100);
            uno.withdraw(100);
            //transaction
            ofstream tr;
            tr.open("transaction.csv", ios::app);
            if (tr.is_open()){
                tr << date(i) << "," << accno << "," << "withdraw" << "," << 100 << "," << endl;
            } else{
                cerr << "error." << endl;
            }
            tr.close();
            //log
            ofstream lo("log.csv", ios::app);
            if (lo.is_open()){
                lo << date(i) << "," << accno << "," << "withdraw" << "," << 100 << "," << endl;
            } else{
                cerr << "error." << endl;
            }
            lo.close();
            cout<<"\nwithdrawing";
            Sleep(0500);
            cout<<".";
            Sleep(0500);
            cout<<".";
            Sleep(0500);
            cout<<".";
             system("cls");
             system("color 3a");
            cout << "Here is your receit." << endl;
            cout<<setw(50)<<setfill('*') << "*"<<endl;
            cout << setw(25) << setfill(' ')  <<  "Location : " << setw(10)  << setfill(' ') << "AAiT Amist kilo" << endl;
            cout << setw(25) << setfill(' ')  <<  "Date and Time : " << setw(10)  << setfill(' ') << date(i) << endl;
            cout << setw(25) << setfill(' ')  << "Account Number : " << setw(10) << setfill(' ') << accno << endl;
            cout << setw(25) << setfill(' ')  << "Amount withdrawn : " << setw(10) << setfill(' ') << 100 << " BIRR ONLY" << endl;
            cout << setw(50) << setfill('*') << "*" << endl;
            cout << "Transaction successful." << endl;
        } else{
            //log
            cout<<"unsuccesful Trial"<<endl;
            ofstream lo("log.csv", ios::app);
            if (lo.is_open()){
                lo << date(i) << "," << accno << "," << "Unsuccesful_Trial" << "," << "--" << "," << "--" << "," << endl;
            } else{
                cerr << "error." << endl;
            }
            lo.close();
            goto z;
        }
    } else if (a == '4'){
        string r;
        unsigned int ak;
        cout << "Enter the account no you want to transfer to: ";
        cin >> r;
        cus dos(r);
        cout << "Enter the amonunt of money you want to transfer: ";
        cin >> ak;
        if (ak < uno.getbalance()){
            uno.withdraw(ak);
            dos.accept(ak);
            //transaction
            ofstream tr("transaction.csv", ios::app);
            if(tr.is_open()){
                tr << date(i) << "," << accno << "," << "transfered" << "," << ak << "," << r << "," << endl;
            }else{
                cout<<"erorr"<<endl;
            }
            tr.close();
            //log
            ofstream lo("log.csv", ios::app);
            if (lo.is_open()){
                lo << date(i) << "," << accno << "," << "transfered" << "," << ak << "," << "to" << "," << r << "," << endl;
            } else{
                cerr << "error." << endl;
            }
            lo.close();
            cout<<"\nTransfering";
            Sleep(0500);
            cout<<".";
            Sleep(0500);
            cout<<".";
            Sleep(0500);
            cout<<".";
            
            cout << "Here is your receit." << endl;
            cout<<setw(50)<<setfill('*') << "*"<<endl;
            cout << setw(25) << setfill(' ')  <<  "Location : " << setw(10)  << setfill(' ') << "AAiT Amist kilo" << endl;
            cout << setw(25) << setfill(' ')  <<  "Date and Time : " << setw(10)  << setfill(' ') << date(i) << endl;
            cout << setw(25) << setfill(' ')  << "Sender Account Number : " << setw(10) << setfill(' ') << accno << endl;
            cout << setw(25) << setfill(' ')  << "receivers Account Number : " << setw(10) << setfill(' ') << r << endl;
            cout << setw(25) << setfill(' ')  << "Amount transfered : " << setw(10) << setfill(' ') << ak << " BIRR ONLY" << endl;
            cout << setw(50) << setfill('*') << "*" << endl;
            cout << "Transaction complete." << endl;
        } else{
            cout << "couldn't transfer money-insufficent balance." << endl;
            //log
            ofstream lo("log.csv", ios::app);
            if (lo.is_open()){
                lo << date(i) << "," << accno << "," << "Unsuccesful_Trial_to_Transfer_Money" << "," << ak << "," << r << "," << endl;
            } else{
                cerr << "error." << endl;
            }
            lo.close();
        }

    }else if (a == '5'){
        string ay, au;
        cout << "Enter a new pin: ";
     	ay =  pinc();
        cout << "Confirm your pin again: ";
        au = pinc();
        if (au == ay){
            uno.pinchange(au);
            cout << "Pin change successful." << endl;
            //log
            ofstream lo("log.csv", ios::app);
            if (lo.is_open()){
                lo << date(i) << "," << accno << "," << "password_change_sucessfully" << "," << endl;
            } else{
                cerr << "error." << endl;
            }
            lo.close();

        } else{
            cout << "you entered different password." << endl;
            //log
            ofstream lo("log.csv", ios::app);
            if (lo.is_open()){
                lo << date(i) << "," << accno << "," << "unsuccesful_trial_to_change_password" << "," << "----" << "---" << "," << endl;
            } else{
                cerr << "error." << endl;
            }
            lo.close();

        }
    } else{
        goto y;
    }
    Sleep(1000);
 goto t;
    return 0;
}



//#include <iostream>
//#include <fstream>
//#include <string>
//using namespace std;
//
//int main() {
//    ifstream go;
//    go.open("accounts.csv");
//    if (go.is_open()){
//        int x;
//        while (go.good()){
//            go >> x;
//            if (x == 1300){
//                cout << x << endl;
//            }
//        }
//    } else{
//        cerr << "error." << endl;
//        return 1;
//    }
//    go.close();
//    cout << "file closed." << endl;
//
////    string x;
////    for (int i = 0; i < 351; ++i) {
////        x = "accounts/" + to_string(v[i]) + ".csv";
////        ofstream nml;
////        nml.open(x);
////        if(nml.is_open()){
////            nml << v[i] << "," << "true" << "," << v[i]*4 << "," << 20000 << "," << endl;
////            cout << v[i] << endl;
////        } else{
////            cerr << "error." << endl;
////        }
////        nml.close();
////    }
//    return 0;
//}

