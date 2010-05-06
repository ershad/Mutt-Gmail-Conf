/*
 * muttgconf.cpp
 *
 * Copyright (C) 2010  Ershad K <ershad92@gmail.com>
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 *  is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, 
 * Boston, MA  02110-1301  USA
 * 
 * See http://ershadk.wordpress.com for usage.
 */
 
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <string>

using namespace std;

class User
{                                               
     string GID; // Gmail ID eg:- john.example@gmail.com
     string GUsername; // Gmail login Username eg:- john.example
     string Password; // Gmail Password
     string Username; // System Username

public:

     void Header()
     {
          cout << "\n Welcome to MuttGconf V 1.0          www.ershadk.wordpress.com";
          cout << "\n -------------------------------------------------------------";
		  cout << "\n This program is based on -"
		       << "\n          http://www.andrews-corner.org/mutt.html";
		  cout << "\n -------------------------------------------------------------\n";
     }
     
     void GetDetails()
     {
          int i;

          cout << "\n Enter your Gmail ID [with @gmail.com] : ";
          getline (cin, GID);

          cout << "\n Enter Password [CAUTION: Password will be showed as you type] : ";
          getline (cin, Password);

          // Retrieving Gmail Login Username from Gmail ID
          i = GID.find_first_of ('@');
          GUsername = GID.substr (0, i);

          cout << "\n Enter Username of the system : ";
          getline (cin,Username);

          cout << "\n " << GUsername << "\n";

     }

	void SetCertificates();
	void WriteFetchmailConf();
	void SetProcmail();
	void WriteSendmailConf();
	void Writemuttrc();
};

void User::Writemuttrc()
{
	string muttrcPath;
	muttrcPath = "/home/" + Username + "/.muttrc";
	string shellcmd;
	shellcmd = "cp muttrc " + muttrcPath;
	string sed1;
	string sed2;
	
	sed1 = "sed -i 's/Username123/" + Username + "/g' " + muttrcPath;
	sed2 = "sed -i 's/GID123/" + GID + "/g' " + muttrcPath;
	
	system(shellcmd.c_str());
	system(sed1.c_str());
	system(sed2.c_str());
}

void User::WriteSendmailConf()
{
	string conf;
	string msmtprcPath;
	
	msmtprcPath = "/home/" + Username + "/.msmtprc";
	
	ofstream msmtprcfile (msmtprcPath.c_str());
	
	conf = "account default\nhost smtp.gmail.com\nport 587\nfrom " + GID;
	conf = conf + "\ntls on\ntls_starttls on";
	conf = conf + "\ntls_trust_file /home/"+ Username + "/mail/certs/Thawte_Premium_Server_CA.pem";
	conf = conf + "\n# tls_trust_file /home/" + Username +"/mail/certs/Equifax_Secure_CA.pem\nauth on";
	conf = conf + "\nuser " + GUsername + "\npassword " + Password + "\nlogfile ~/.msmtp.log";
	
	msmtprcfile << conf;
	system("chmod 600 ~/.msmtprc");
}

	
void User:: SetProcmail()
{
	string conf;
	string Bashpath;
	conf = "\nMAIL=/var/spool/mail/" + Username + " && export MAIL";

	Bashpath = "/home/" + Username + "/.bashrc";

	ofstream bashfile (Bashpath.c_str(), ios::app );
	bashfile << conf;

	string Pconf;
	string Ppath;
	
	Ppath = "/home/" + Username + "/.procmailrc";

	Pconf = "PATH=/bin:/usr/bin:/usr/local/bin\nVERBOSE=off\nDEFAULT=/var/spool/mail/" + Username;
	Pconf = Pconf + "\nMAILDIR=$HOME/mail\nLOGFILE=$HOME/.procmaillog\n# Recipes below this comment:\n\n:0:\n* ^TOmutt-user\n\nmutt";
	
	ofstream procmailrc (Ppath.c_str());
	procmailrc << Pconf;

}

void User::WriteFetchmailConf()
{
	string Fetchmailpath;
	Fetchmailpath = "/home/" + Username + "/.fetchmailrc" ;

	ofstream fetchmailrc (Fetchmailpath.c_str());
     fetchmailrc << "poll pop.gmail.com\n" 
                 << "with proto POP3\n" << "user '" << GID <<"' \n" 
                 << "there with password '" << Password << "' \n"
                 << "is '" << Username << "' here \n"
                 << "mda " << (char) 34 << "/usr/bin/procmail -d %T" << (char) 34 << "\n"
                 << "options\n"
                 << "no keep\n"
                 << "ssl\n"
                 << "sslcertck";

	system ("chmod 600 ~/.fetchmailrc");

}
void User::SetCertificates()
{ 
     system ("mkdir -pv $HOME/mail/certs");
     system ("cp Equifax_Secure_CA.pem $HOME/mail/certs/Equifax_Secure_CA.pem");
     system ("cp Thawte_Premium_Server_CA.pem $HOME/mail/certs/Thawte_Premium_Server_CA.pem");
     system ("c_rehash $HOME/mail/certs/");
}
     
int main()
{
     User ob;
     ob.Header();
     ob.GetDetails();
	 
	 cout << "\n .............................................................\n";
     ob.SetCertificates();
     ob.WriteFetchmailConf();
	 ob.SetProcmail();
	 ob.WriteSendmailConf();
	 ob.Writemuttrc();
	 cout << "\n .............................................................\n";
	 
	 cout << "\n Done :) Now start mutt, press '!' and type 'fetchmail -v' "
		  << "\n without quotes to get mails through POP3 and read it in "
		  << "\n mutt. Thank you for using MuttGconf. Nice day!";   
}
     
