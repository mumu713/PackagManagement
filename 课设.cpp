#include"pch.h"
#include <string>
#include <iostream>
#include <fstream> 
#include <atltime.h>
using namespace std;
#define N 1000
class package
{
public:
	string id;//单号
	string send;//发送
	string sendno;
	string recv;//接收
	string recvno;
	string name;
	string weight;
	float volume;
	float price;
	string state;

	void input(string _id, string _send, string _sendno, string _recv, string _recvno, string _name, string _weight, float _volume, float _price, string _state)//输入成员变量
	{
		id = _id;
		send = _send;
		sendno = _sendno;
		recv = _recv;
		recvno = _recvno;
		name = _name;
		weight = _weight;
		volume = _volume;
		price = _price;
		state = _state;
	}

	void show()//显示
	{
		cout << "单号：" << id
			<< " 发货方：" << send
			<< " 电话：" << sendno
			<< " 接收方：" << recv
			<< " 电话：" << recvno
			<< " 货物名：" << name
			<< " 重量：" << weight
			<< " 体积：" << volume
			<< " 价值：" << price
			<< " 状态：" << state << endl;
	}

	void input(ifstream & is)//存文件
	{
		is >> id >> send >> sendno >> recv >> recvno >> name >> weight >> volume >> price >> state;
	}

	void output(ofstream & os)//读文件
	{
		os << id
			<< " " << send
			<< " " << sendno
			<< " " << recv
			<< " " << recvno
			<< " " << name
			<< " " << weight
			<< " " << volume
			<< " " << price
			<< " " << state
			<< endl;
	}

};
class error
{
public:
	string id;//单号
	string state;//差错内容

	void input(string _id, string _state)
	{
		id = _id;
		state = _state;
	}

	void show()
	{
		cout << "单号：" << id
			<< " 差错内容：" << state << endl;
	}

	void input(ifstream & is)
	{
		is >> id >> state;
	}

	void output(ofstream & os)
	{
		os << id
			<< " " << state
			<< endl;
	}

};

class packagemanage
{
	package m_package[N];//包裹数组
	int m_count;//包裹数量
	error m_error[N];//差错数组
	int m_count2;//差错数量
	char m_code[32];//码号
public:
	packagemanage() {
		m_count = 0;
		m_count2 = 0;
		strcpy_s(m_code, "2019010001");
	}
	int packageexist(string id)//包裹是否存在
	{
		int ret = -1;
		for (int i = 0; i < m_count; i++)
		{
			if (id == m_package[i].id)//找到相同号码
			{
				ret = i;
				break;
			}
		}

		return ret;
	}

	int packagesend(string id)//包裹是不是发送中的
	{
		int ret = -1;
		for (int i = 0; i < m_count; i++)
		{
			if (id == m_package[i].id && m_package[i].state == "发送中 已结款")//找到发送中包裹
			{
				ret = i;
				break;
			}
		}

		return ret;
	}

	void insertpackage(string _id, string _send, string _sendno, string _recv, string _recvno, string _name, string _weight, float _volume, float _price, string _state)//添加
	{
		m_package[m_count].input(_id, _send, _sendno, _recv, _recvno, _name, _weight, _volume, _price, _state);
		m_count++;
	}

	int is_runnian(int iyear)//判断该年是否为闰年
	{
		if ((iyear % 4 == 0 && iyear % 100 != 0) || (iyear % 400 == 0))
			return 1;
		else
			return 0;
	}

	int sday(int num, int iyear)//switch语句来确定各个月份的天数
	{
		switch (num)
		{
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			return 31;
		case 4:
		case 6:
		case 9:
		case 11:
			return 30;
		case 2:
			if (is_runnian(iyear))
				return 29;
			else
				return 28;
		}
		return 0;
	}

	int GetWeek(int y, int m, int d)//根据年月日计算第几周
	{
		int i, sum = 0;
		for (i = 1; i <= m - 1; i++)
			sum += sday(i, y);
		sum += d;

		int weeks = sum / 7;
		return weeks;
	}

	void sendpackage(int index)//发送包裹，生成收货码发送给收货方
	{
		if (m_package[index].state == "发送中 已结款")
		{
			cout << "此包裹已经发送" << endl;
			return;
		}
		m_package[index].state = "发送中 已结款";//改包裹状态
		CTime today = CTime::GetCurrentTime();//计算当前时间
		int y1, w1, c1;
		y1 = today.GetYear();
		w1 = GetWeek(y1, today.GetMonth(), today.GetDay());
		int y2, w2, c2;
		sscanf_s(m_code, "%04d%02d%04d", &y2, &w2, &c2);//从之前保存的收货码中获取年周和第几个

		if (y1 == y2 && w1 == w2)//年和周相同增加，不相同则从1开始
		{
			c1 = c2 + 1;
		}
		else c1 = 1;
		sprintf_s(m_code, "%04d%02d%04d", y1, w1, c1);//组合收货码
		cout << "向手机号：" << m_package[index].recvno << "发送编号：" << m_code << endl;
	}

	void errpackage(int index)//设置包裹为差错
	{
		m_package[index].state = "差错";
	}

	void delpackage(int index)//删除包裹
	{
		for (int i = index; i < m_count; i++)
		{
			m_package[i] = m_package[i + 1];
		}

		m_count--;
	}

	void modifypackage(int index, string _id, string _send, string _sendno, string _recv, string _recvno, string _name, string _weight, float _volume, float _price, string _state)//修改包裹
	{
		m_package[index].input(_id, _send, _sendno, _recv, _recvno, _name, _weight, _volume, _price, _state);
	}

	void searchidpackage(string id)//根据单号查找包裹
	{
		for (int i = 0; i < m_count; i++)
		{
			if (id == m_package[i].id)
			{
				m_package[i].show();
				break;
			
			}
		}
	}

	void showallpackage()//显示全部包裹
	{
		for (int i = 0; i < m_count; i++)
		{
			m_package[i].show();
		}
	}


	void savepackage(string filesend)//保存全部包裹
	{
		ofstream write(filesend.c_str(), ios::out);
		write << m_code << endl;//保存收货码
		write << m_count << endl;
		for (int i = 0; i < m_count; i++)
		{
			m_package[i].output(write);
		}

		write.close();
	}

	void loadpackage(string filesend)//读取全部包裹
	{
		ifstream read(filesend.c_str(), ios::in);
		if (read.peek() == EOF)
		{
			read.close();
			return;
		}

		read >> m_code;//读取收货码
		read >> m_count;
		for (int i = 0; i < m_count; i++)
		{
			m_package[i].input(read);
		}

		read.close();
	}

	int errorexist(string id)//差错是否存在
	{
		int ret = -1;
		for (int i = 0; i < m_count2; i++)
		{
			if (id == m_error[i].id)//找到相同号码
			{
				ret = i;
				break;
			}
		}

		return ret;
	}

	void inserterror(string _id, string _state)//添加差错
	{
		m_error[m_count2].input(_id, _state);
		m_count2++;
	}


	void delerror(int index)//删除差错
	{
		for (int i = index; i < m_count2; i++)
		{
			m_error[i] = m_error[i + 1];
		}

		m_count2--;
	}

	void modifyerror(int index, string _id, string _state)//修改差错
	{
		m_error[index].input(_id, _state);
	}

	void searchiderror(string id)//根据单号查找差错
	{
		for (int i = 0; i < m_count2; i++)
		{
			if (id == m_error[i].id)
			{
				m_error[i].show();
				
			
			}
		}
	}

	void showallerror()//显示全部差错
	{
		for (int i = 0; i < m_count2; i++)
		{
			m_error[i].show();
		}
	}


	void saveerror(string filesend)//保存差错
	{
		ofstream write(filesend.c_str(), ios::out);
		write << m_code << endl;
		write << m_count2 << endl;
		for (int i = 0; i < m_count2; i++)
		{
			m_error[i].output(write);
		}

		write.close();
	}

	void loaderror(string filesend)//读取差错
	{
		ifstream read(filesend.c_str(), ios::in);
		if (read.peek() == EOF)
		{
			read.close();
			return;
		}

		read >> m_code;
		read >> m_count2;
		for (int i = 0; i < m_count2; i++)
		{
			m_error[i].input(read);
		}

		read.close();
	}
};

void packagemenu(packagemanage &manage)//包裹管理
{
	char n;
	int k = 1;
	int index;
	string id;
	string send;
	string sendno;
	string recv;
	string recvno;
	string name;
	string weight;
	float volume;
	float price;
	string state;
	string input;
	while (k == 1)
	{
		cout << "**********包裹管理**********" << endl;
		cout << "1   查询包裹信息" << endl
			<< "2   添加包裹信息" << endl
			<< "3   修改包裹信息" << endl
			<< "4   发送包裹" << endl
			<< "5   确认包裹收货" << endl
			<< "6   显示全部包裹信息" << endl
			<< "0   返回" << endl
			<< "选择:";
		cin >> n;
		switch (n)
		{
		case '1':
			cout << "请输入单号：";
			cin >> id;
			manage.searchidpackage(id);//根据单号查找包裹
			break;
		case '2':
			cout << "请输入单号：";
			cin >> id;
			index = manage.packageexist(id);//检查包裹是否存在
			if (index == -1)
			{
				cout << "请输入发送方：";
				cin >> send;
				cout << "请输入发送方电话：";
				cin >> sendno;
				cout << "请输入接受方：";
				cin >> recv;
				cout << "请输入接受方电话：";
				cin >> recvno;
				cout << "请输入货物名：";
				cin >> name;
				cout << "请输入重量：";
				cin >> weight;
				cout << "请输入体积：";
				cin >> volume;
				cout << "请输入价格：";
				cin >> price;

				manage.insertpackage(id, send, sendno, recv, recvno, name, weight, volume, price, "未发货 未付款");
			}//添加包裹信息
			else cout << "包裹已存在" << endl;
			break;
		case '3':
			cout << "请输入单号：";
			cin >> id;
			index = manage.packageexist(id);
			if (index != -1)
			{
				cout << "请输入发送方：";
				cin >> send;
				cout << "请输入发送方电话：";
				cin >> sendno;
				cout << "请输入接受方：";
				cin >> recv;
				cout << "请输入接受方电话：";
				cin >> recvno;
				cout << "请输入货物名：";
				cin >> name;
				cout << "请输入重量：";
				cin >> weight;
				cout << "请输入体积：";
				cin >> volume;
				cout << "请输入价格：";
				cin >> price;
				cout << "请输入状态：";
				cin >> state;
				manage.modifypackage(index, id, send, sendno, recv, recvno, name, weight, volume, price, state);
			}//修改包裹信息
			else cout << "包裹不存在" << endl;
			break;
		case '4':
			cout << "请输入单号：";
			cin >> id;
			index = manage.packageexist(id);
			if (index != -1) manage.sendpackage(index);
			else cout << "包裹不存在" << endl;
			break;
		case '5':
			cout << "请输入单号：";
			cin >> id;
			index = manage.packagesend(id);
			if (index != -1)
			{
				cout << "是否存在差错？(y/n)：";
				cin >> input;
				if (input == "y")
				{
					cout << "请输入差错内容：";
					cin >> state;
					manage.inserterror(id, state);//添加差错信息
					manage.errpackage(index);//设置包裹状态为差错
				}
				else manage.delpackage(index);//删除包裹信息
			}
			else cout << "非发送中包裹" << endl;
			break;
		case '6':
			manage.showallpackage();//显示包裹信息
			break;
		case '0':
			k = 0;
			break;
		default:cout << "输入错误请重新输入" << endl;
		}
	}
}
void errormenu(packagemanage &manage)//差错管理
{
	char n;
	int k = 1, index;
	string id;//单号
	string state;//差错
	while (k == 1)
	{
		cout << "**********差错管理**********" << endl;
		cout << "1   查询差错" << endl
			<< "2   显示全部差错" << endl
			<< "3   修改差错" << endl
			<< "4   解决差错" << endl
			<< "0   退出系统" << endl
			<< "选择:";
		cin >> n;
		switch (n)
		{
		case '1':
			cout << "请输入单号：";
			cin >> id;
			manage.searchiderror(id);
			break;
		case '2':
			manage.showallerror();
			break;
		case '3':
			cout << "请输入单号：";
			cin >> id;
			index = manage.errorexist(id);
			if (index != -1)
			{
				cout << "请输入修改的差错内容：";
				cin >> state;
				manage.modifyerror(index, id, state);
			}
			else cout << "差错不存在" << endl;
			break;
		case '4':
			cout << "请输入单号：";
			cin >> id;
			index = manage.errorexist(id);
			if (index != -1)
			{
				manage.delerror(index);
				index = manage.packageexist(id);
				manage.delpackage(index);
			}
			else cout << "差错不存在" << endl;
			break;
		case '0':
			k = 0;
			break;
		default:cout << "输入错误请重新输入" << endl;
		}
	}
}

int main()
{
	packagemanage manage;
	manage.loadpackage("package.txt");
	manage.loaderror("error.txt");
	char n;
	int k = 1;
	while (k == 1)
	{
		cout << "快递驿站管理系统" << endl;
		cout << "1   包裹管理" << endl
			<< "2   差错信息管理" << endl
			<< "0   退出系统" << endl
			<< "选择:";
		cin >> n;
		switch (n)
		{
		case '1':
			packagemenu(manage);
			break;
		case '2':
			errormenu(manage);
			break;
		case '0':
			k = 0;
			break;
		default:cout << "输入错误请重新输入" << endl;
		}
	}

	manage.savepackage("package.txt");
	manage.saveerror("error.txt");
	return 0;
}


