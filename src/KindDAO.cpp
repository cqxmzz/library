// KindDAO.cpp
// Author: Chen Qiming
// Date: 2010/12/27

#include "KindDAO.h"
#include "BookDAO.h"
#include <string>

std::vector<class Kind *> KindDAO::AllKinds;

std::string KindDAO::wtos(const std::wstring &w)
{
	int	len = WideCharToMultiByte(GetACP(), 0, w.c_str(), -1, NULL, 0, NULL, NULL);
	char *buf = new char[len];
	WideCharToMultiByte(GetACP(), 0, w.c_str(), -1, buf, len, NULL, NULL);
	std::string s(buf);
	delete[] buf;
	return s;
}

std::wstring KindDAO::stow(const std::string &s)
{
	int len = MultiByteToWideChar(GetACP(), 0, s.c_str(), -1, NULL, 0);
	wchar_t*buf = new wchar_t[len];
	MultiByteToWideChar(GetACP(), 0, s.c_str(), -1, buf, len);
	std::wstring w(buf);
	delete[] buf;
	return w;
}

bool KindDAO::file_exists(std::string const &path)
{
	std::fstream f(path.c_str());
	bool exists = f.is_open();
	f.close();
	return exists;
}

bool KindDAO::loadAll()
{
	KindDAO::AllKinds.clear();
	Kind::setKindNumberNow(0);
	std::wstring path = System::getWorkingDirectory() + L"\\" + System::getKindFileName();
	if (file_exists(wtos(path)))
	{
		
		std::wifstream wfin(wtos(path));
		wfin.imbue(std::locale("chs"));
		int kindnumbernow;
		wfin >> kindnumbernow;
		std::wstring ISBN;
		std::wstring Name;
		std::wstring Authors;
		std::wstring Index;
		int kindnumber;
		std::wstring tempstring;
		while (1)
		{
			std::getline(wfin, tempstring);//把fin没读进去的回车读掉
			std::getline(wfin, tempstring);
			if(wtos(tempstring) == "////") 
				return true;
			std::getline(wfin, Name);
			std::getline(wfin, ISBN);
			std::getline(wfin, Authors);
			std::getline(wfin, Index);
			wfin >> kindnumber;
			Kind* TempKind;
			TempKind = new Kind(wtos(ISBN), Name, Authors, Index);
			(*TempKind).setKindNumber(kindnumber);
			KindDAO::AllKinds.push_back(TempKind);
		}
		Kind::setKindNumberNow(kindnumbernow);
	}
	else
		return false;
	return true;
}

bool KindDAO::saveAll()
{
	std::wstring path = System::getWorkingDirectory() + L"\\" + System::getKindFileName();
	if (file_exists(wtos(path)))
	{
		std::wofstream wfout (wtos(path));
		wfout.imbue(std::locale("chs"));
		wfout << Kind::getKindNumberNow() << std::endl;
		for (int i = 0; i < (int)KindDAO::AllKinds.size(); i++)
		{
			wfout << L"//" << std::endl;
			wfout << KindDAO::AllKinds[i]->getName() << std::endl;
			wfout << stow(KindDAO::AllKinds[i]->getISBN()) << std::endl;
			for (int j = 0; j < (int)KindDAO::AllKinds[i]->getAuthors().size(); j++)
			{
		//		if (KindDAO::AllKinds[i]->getAuthors()[j] != L"")
					wfout << KindDAO::AllKinds[i]->getAuthors()[j] << L",";
			}
			wfout << std::endl;
			wfout << KindDAO::AllKinds[i]->getIndex() << std::endl;
			wfout << KindDAO::AllKinds[i]->getKindNumber() << std::endl;
		}
		wfout << L"////";
	}
	else
		return false;
	return true;
}

void KindDAO::create(std::string const &isbn, std::wstring const &name, std::wstring const &authors, std::wstring const &index)
{
	Kind* TempKind;
	TempKind = new Kind(isbn, name, authors, index);
	Kind::setKindNumberNow(Kind::getKindNumberNow());
	(*TempKind).setKindNumber(Kind::getKindNumberNow());
	KindDAO::AllKinds.push_back(TempKind);
}

void KindDAO::remove(std::string const &isbn)
{
	for (int i = 0; i < (int)KindDAO::AllKinds.size(); i++)
	{
		if (KindDAO::AllKinds[i]->getISBN() == isbn)
		{
			for (int j = 0; j < (int)KindDAO::AllKinds[i]->getBooks().size(); j++)
			{
				BookDAO::remove(* KindDAO::AllKinds[j]->getBooks()[j]);
			}
			KindDAO::AllKinds.erase(KindDAO::AllKinds.begin()+i);
		}
	}
}
