#include <iostream>
#include <fstream>
#include <string>
#include <map>
using namespace std;

class MySection
{
private:
	string section; 						//每个section的名字
	map<string,string> mapKey;              //键－值组合

public:
	MySection( string section )
	{ 
		this->section = section;
	}

	bool AddKeyValue( string key, string value )
	{
		pair<string,string> p(key,value);   	//加入一对键－值
		mapKey.insert(p);
		
		return true;
	}

	void show( ostream & os )
	{
		os<<section<<endl;
		map<string,string>::iterator it = mapKey.begin();

		while( it != mapKey.end() )
		{
			os<<"\t"<<(*it).first<<"="<<(*it).second<<endl;
			it++;
		}
	}

	string FindValue( string key )							//根据键，查找值
	{
		map<string,string>::iterator it = mapKey.find(key);
		return (*it).second;
	}

};

class MySectionCollect
{
private:
	map<string,MySection> mapSection;

public:
	bool AddSection( string strSection )
	{
		pair<string,MySection> p( strSection, MySection(strSection) );
		mapSection.insert(p);

		return true;
	}

	MySection * GetSection( string strSection )
	{
		map<string,MySection>::iterator it = mapSection.find(strSection);
		return &((*it).second);
	}

	void show( ostream & os )
	{
		map<string,MySection>::iterator it = mapSection.begin();

		while( it != mapSection.end() )
		{
			((*it).second).show(os);
			it++;
		}
	}

	string FindValue( string section, string key )				//根据section和键，查找值
	{
		MySection * pSection = GetSection(section);
		return pSection->FindValue(key);
	}
};

class ReadIni
{
private:
	string strPath;
	MySectionCollect & collect;

public:
	ReadIni( string strPath, MySectionCollect & collect ):strPath(strPath),collect(collect){}
	
	void Trim( string & s )			//删除多余的空格
	{
		if( s != "" )
		{
			s.erase(0, s.find_first_not_of(" "));   	//删除左空格
			if( s != "" )
				s.erase(s.find_last_not_of(" ") + 1);	//删除右空格
		}
	}

	string GetSection( string strText )
	{
		strText.erase(0, strText.find_first_not_of("["));  //删除左'['
		strText.erase(strText.find_last_not_of("]") + 1);  //删除右']'
		return strText;
	}

	void GetPair( string strText, string &key,string &value)
	{
		int pos = strText.find("=");
		key = strText.substr(0,pos);
		value = strText.substr(pos+1, strText.length()-pos-1);
		Trim(key);
		Trim(value);
	}

	void Process()
	{
		string strLine = "";
		string strSection = "";
		string strKey = "";
		string strValue = "";

		MySection * pSection = NULL;

		ifstream in( strPath.c_str() );
		while( !in.eof() )
		{
			getline(in, strLine);
			Trim(strLine);
			if(strLine == "" ) continue;

			if( strLine.at(0) == '[' )
			{
				strSection = GetSection(strLine);
				collect.AddSection(strSection);
				pSection = collect.GetSection(strSection);
			}

			if( strLine.at(0) != '[' ) 			//若是键－值对
			{
				GetPair(strLine, strKey, strValue);
				pSection->AddKeyValue(strKey, strValue);	//添加键－值对
			}
		}
		in.close();
	}

};

int main()
{
	string path = "test.ini";
	string key;
	string section;

	MySectionCollect collect;
	ReadIni ri(path, collect);

	ri.Process();
//	collect.show(cout);

	cout<<"Please input section and key\n";
	cin>>section>>key;

	cout<<"The value is : "<<collect.FindValue(section, key)<<endl;
	
	return 0;
}
