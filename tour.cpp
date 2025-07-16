#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include <malloc.h>
#include <cstdio>
#include <cstdlib>//清屏函数头文件
#include <sstream>
#include <fstream>
#include <windows.h>
#include <stack>
#include <unordered_map>
#include <map>
#include <vector>
#include <string>
#include <queue>
#include <unordered_map>
#include <iomanip>
#include <cmath>
#include<conio.h>
#include<string>
#include <string.h>
using namespace std;
#include <algorithm>

#define M 200
#define INF 999666333
#define Path "tour.txt"

using namespace std;

std::map<std::string, std::string> userCredentials = {
        {"aa", "111"},
        {"bb", "222"},
        {"cc", "333"},
        {"dd","444"},
        {"ee","555"},
        {"ff","666"},
        {"gg","777"},
        {"hh","888"},
        {"ii","999"},
        {"jj","000"}
};
string tour_id = "123456";
string tour_password = "000000";
string admin_id = "456789";
string admin_password = "111111";

typedef struct Node {
    string name;//名称
    string intro;//简介
    float  value;//评价

}Node;//单个景点
struct Node1 {
    char data;
    unsigned freq;
    Node1* left;
    Node1* right;
    Node1(char data, unsigned freq) : data(data), freq(freq), left(nullptr), right(nullptr) {}
};
struct Point {
    string name;
    string intro;
    string key;
    string value;
    string heart;
};
struct FoodNode {
    string name;   // 美食名称
    string intro;  // 美食简介
    float value;  // 美食评分
};
typedef struct Map
{
    int count;//景点总数量
    int edge;//道路数量
    int m[M][M];//景点邻接矩阵
    Node Pname[M];//各个景点的信息
    vector<Point> points;
}Map;
typedef struct FoodMap {
    int count;         // 美食总数量
    FoodNode foods[M];// 美食信息数组
    vector<Point> points;
}FoodMap;
#define MAX_BUFFER_SIZE 4096
#define ESC 27        //退出键
#define Enter 13      //回车键
#define BUFFER_SIZE 4096
/*定义日记结构体*/
typedef struct Link1
{
    char title[40];         //标题
    char content[1000];     //日记内容
    char keyword[40];
    double star;
    int times;
    int  tag;             //用来标记该日记是否满足查阅要求，是为0，否为-1，初始值为0；
    struct Link1* next;
} Diary;

Diary* Init_Diary(Diary* D1);/*初始化日记表*/
void Initial_Tag(Diary* D1, int tag);
int Open_Diary(Diary* D1, Diary* D);
int Printf_DiaryList(Diary* D1);/*显示日记列表*/
void Print_Menu1(int* choose1);/*进入菜单*/
void Print_Menu2(int* choose2);/*进入日记菜单*/
void Save_Diary(Diary* D1, int i);/*保存日记*/
Diary* Write_Diary(Diary* D1);/* 写新日记*/
void Torder(Diary* A);
void Sorder(Diary* A);
Diary* Filter_word(Diary* D1, char cmpword[], int n);
int Search_Diary(Diary* D1);
void diary_system();
int search_title(char* a, char* b);
int search_keyword(char* a, char* b);
void read_graph_diary(Diary* D);
char* strstr2(char* str, char* strSearch);
void compress(Diary* D);
// 初始化points向量
void initialize_points(Map* p) {
    p->points.clear();  // 清空points向量

    for (int i = 0; i < p->count; i++) {
        Point point;
        point.name = p->Pname[i].name;
        point.intro = p->Pname[i].intro;
        point.value = to_string(p->Pname[i].value);
        p->points.push_back(point);
    }
}
//初始美食points变量
void initialize_points_food(FoodMap* f) {
    f->points.clear();  // 清空points向量

    for (int i = 0; i < f->count; i++) {
        Point point;
        point.name = f->foods[i].name;
        point.intro = f->foods[i].intro;

        point.value = to_string(f->foods[i].value);
        f->points.push_back(point);
    }
}
//读出数据
void read_graph(Map* p) {
    ifstream fin("tour.txt", ios::in);
    if (fin.is_open() == false)
    {
        cout << "打开文件" << "tour.txt" << "失败\n";
        return;
    }
    string temp;
    fin >> temp;
    int t = stoi(temp);
    p->count = stoi(temp);
    fin >> temp;
    p->edge = stoi(temp);
    for (int i = 1; i <= p->count; i++)
        for (int j = 1; j <= p->count; j++)
        {
            fin >> temp;
            p->m[i][j] = stoi(temp);
        }
    for (int i = 1; i <= p->count; i++)
    {
        fin >> temp;
        p->Pname[i].name = temp;
        fin >> temp;
        p->Pname[i].intro = temp;
        fin >> temp;
        p->Pname[i].value = stoi(temp);
    }
    fin.close();
}
void read_graph_food(FoodMap* f) {

    // 读取美食信息
    ifstream fin("Food.txt", ios::in);
    if (fin.is_open() == false) {
        cout << "打开文件" << "Food.txt" << "失败\n";
        return;
    }
    fin >> f->count;
    for (int i = 1; i <= f->count; i++) {
        fin >> f->foods[i].name >> f->foods[i].intro >> f->foods[i].value;
    }
    fin.close();
}
//写入数据
void write_graph(Map* p) {
    ofstream fout(Path, ios::out);
    if (fout.is_open() == false)
    {
        cout << "打开文件" << Path << "失败\n";
        return;
    }
    fout << p->count << "\n";
    fout << p->edge << "\n";
    for (int i = 1; i <= p->count; i++)
        for (int j = 1; j <= p->count; j++)
            fout << p->m[i][j] << "\n";
    for (int i = 1; i <= p->count; i++)
    {
        fout << p->Pname[i].name << "\n";
        fout << p->Pname[i].intro << "\n";
        fout << p->Pname[i].value << "\n";
    }
    fout.close();
}

//输出景点信息
void print_graph(Map* p)
{
    cout << "景点个数：" << p->count << "\n";
    cout << "道路条数：" << p->edge << "\n";
    cout << "\n";
    cout << "景点路径信息：(邻接矩阵)\n";//输出路径信息
    cout << "          ";
    for (int i = 1; i <= p->count; i++)//通过printf语句控制输出信息所占空格个数
        printf("%-10s", p->Pname[i].name.c_str());//-：左对齐
    cout << "\n";
    for (int i = 1; i <= p->count; i++)
    {
        printf("%-10s", p->Pname[i].name.c_str());//.c_str()：转化为char数组，便于调整输出格式
        for (int j = 1; j <= p->count; j++)
            printf("%-10d", p->m[i][j]);
        cout << "\n";
    }
    cout << "\n";
    cout << "      ***景点信息***\n";//输出景点信息
    for (int i = 1; i <= p->count; i++)
    {
        cout << "景点编号：" << i << "\n";
        cout << "\t名称：" << p->Pname[i].name << "\n";
        cout << "\t简介：" << p->Pname[i].intro << "\n";
        cout << "\t评价：" << p->Pname[i].value << "\n";
        cout << "\t---------------------------------------\n";
    }
    cout << "                        输出成功\n";
    cout << "----------------------------------------------------------\n";
}
void print_food(FoodMap* f)
{
    cout << "美食个数：" << f->count << "\n";
    cout << "\n";
    cout << "      ***美食信息***\n";
    for (int i = 1; i <= f->count; i++)
    {
        cout << "美食编号：" << i << "\n";
        cout << "\t名称：" << f->foods[i].name << "\n";
        cout << "\t简介：" << f->foods[i].intro << "\n";
        cout << "\t评分：" << f->foods[i].value << "\n";
        cout << "\t---------------------------------------\n";
    }
    cout << "                        输出成功\n";
    cout << "----------------------------------------------------------\n";
}
//查找景点(辅助函数)1
int search_num(Map* p, string name)//返回查找到的景点编号
{
    for (int i = 1; i <= p->count; i++)
        if (p->Pname[i].name == name) return i;
    return -1;
}
// 查找景点(辅助函数)2
vector<int> search_points(Map* p, string keyword) {
    initialize_points(p);
    vector<int> foundPoints;
    for (int i = 0; i < p->count; i++) {
        if (p->points[i].name.find(keyword) != string::npos) {
            foundPoints.push_back(i);
        }
    }
    return foundPoints;
}
//查找食物(辅助函数)1
int search_num_food(FoodMap* f, string name)//返回查找到的景点编号
{
    for (int i = 1; i <= f->count; i++)
        if (f->foods[i].name == name) return i;
    return -1;
}
// 查找食物(辅助函数)2
vector<int> search_points_food(FoodMap* f, string keyword) {
    initialize_points_food(f);
    vector<int> foundPoints;
    for (int i = 0; i < f->count; i++) {
        if (f->points[i].name.find(keyword) != string::npos) {
            foundPoints.push_back(i);
        }
    }
    return foundPoints;
}
void tour_search_graph(Map* p) {
    string keyword;
    cout << "请输入关键字：";
    cin >> keyword;

    vector<int> foundPoints = search_points(p, keyword);

    if (!foundPoints.empty()) {
        // 按评价值升序排序
        sort(foundPoints.begin(), foundPoints.end(), [&](int a, int b) {
            return p->points[a].value < p->points[b].value;
            });

        cout << "找到以下景点：" << endl;
        for (const auto& i : foundPoints) {
            cout << "景点编号：" << i << endl;
            cout << "\t名称：" << p->points[i].name << endl;
            cout << "\t简介：" << p->points[i].intro << endl;
            float floatValue = stof(p->points[i].value);
            stringstream ss;
            ss << fixed << setprecision(1) << floatValue;
            string formattedValue = ss.str();
            cout << "\t评价：" << formattedValue << endl;

            cout << "---------------------------------------" << endl;
        }
    }
    else {
        cout << "未找到含有关键字的景点" << endl;
    }
    cout << "查找结束" << endl;
    cout << "--------------------------------------------" << endl;
}
// 游客查找食物
void tour_search_graph_food(FoodMap* f) {
    string keyword;
    cout << "请输入关键字：";
    cin >> keyword;

    vector<int> foundPoints = search_points_food(f, keyword);

    if (!foundPoints.empty()) {
        // 按评价值升序排序
        sort(foundPoints.begin(), foundPoints.end(), [&](int a, int b) {
            return f->points[a].value < f->points[b].value;
            });

        cout << "找到以下食物：" << endl;
        for (const auto& i : foundPoints) {
            cout << "食物编号：" << i << endl;
            cout << "\t名称：" << f->points[i].name << endl;
            cout << "\t简介：" << f->points[i].intro << endl;
            float floatValue = stof(f->points[i].value);
            stringstream ss;
            ss << fixed << setprecision(1) << floatValue;
            string formattedValue = ss.str();
            cout << "\t评价：" << formattedValue << endl;

            cout << "---------------------------------------" << endl;
        }
    }
    else {
        cout << "未找到含有关键字的食物" << endl;
    }
    cout << "查找结束" << endl;
    cout << "--------------------------------------------" << endl;
}
//展示附近景点
void show_around(Map* p)
{
    string name;
    cout << "请输入当前景点名称：";
    cin >> name;
    int n = search_num(p, name);
    if (n != -1)
    {
        cout << "\t***当前景点：" << "\n";
        cout << "名称：" << p->Pname[n].name << "\n";
        cout << "简介：" << p->Pname[n].intro << "\n";
        cout << "评价：" << p->Pname[n].value << "\n";
        cout << "\t***附近景点：" << "\n";
        for (int i = 1; i <= p->count; i++)//遍历单行或单列
        {
            if (p->m[n][i] != 0 && p->m[n][i] != INF)//排除距离为0（本身），距离为INF（无穷）的节点
            {
                cout << "名称：" << p->Pname[i].name << "\n";
                cout << "简介：" << p->Pname[i].intro << "\n";
                cout << "评价：" << p->Pname[i].value << "\n";
                cout << "距离：" << p->m[n][i] << "\n";
                cout << "\n";
            }
        }

    }
}

//迪杰斯特拉算法
void dijkstra(int m[M][M], int s, int n, int* dist, int* path)//通过dist数组和path数组传递路径信息
{
    int final[M];//标记是否找到最短路径
    int v, w, i, j, min;
    //初始化：final置0；path置-1(默认开始都无前驱)；dist置INF（无穷）；
    //第一步：遍历出发点对应的行，更新dist（直达路径），同时把（直达路径）path(前驱节点)设置为出发点编号
    for (v = 1; v <= n; v++)//初始化和第一步同时进行
    {
        final[v] = 0;
        path[v] = -1;
        dist[v] = m[s][v];
        if (dist[v] != INF && dist[v] != 0)
        {
            path[v] = s;
        }
    }
    final[s] = 1;//第一步：出发点dist[s]=0肯定已经是最短路径，final[s]=1(标记为已找到)
    dist[s] = 0;
    //循环遍历，循环n-1次每次找出dist最短同时未找到最短路径的节点，该节点已找到最短路径，同时更新与该节点直接相连节点的最短路径
    for (i = 1; i < n; ++i)
    {
        min = INF;
        for (w = 1; w <= n; ++w)//找出dist最短同时未找到最短路径的节点
            if (!final[w] && dist[w] < min)
            {
                v = w;
                min = dist[w];
            }
        final[v] = 1;//标记为已找到最短路径
        for (w = 1; w <= n; ++w)//更新与该节点直接相连节点的最短路径
        {
            if (!final[w] && min < INF && m[v][w] < INF && (min + m[v][w] < dist[w]))//如若修改dist，则同时也要修改path(前驱节点)
            {
                dist[w] = min + m[v][w];
                path[w] = v;//修改path(前驱节点)
            }
        }
    }
    //n-1遍循环结束，各个节点都找到最短路径
}

//单一最短路径
void shortone_graph(Map* p)
{
    int dist[M], path[M];//两个辅助数组
    cout << "请输入起点：";
    string name;
    cin >> name;
    int t = search_num(p, name);
    if (t != -1)
    {
        dijkstra(p->m, t, p->count, dist, path);
        cout << "\t    ***最短路径信息***\n";//直接输出迪杰斯特拉算法得出的距离和前驱景点
        cout << "\t景点名称 最短路径  前驱景点  \n";
        for (int i = 1; i <= p->count; i++)
        {
            if (i == t)
                printf("\t%-8s %-8d 出发景点\n", p->Pname[i].name.c_str(), dist[i]);
            else
                printf("\t%-8s %-8d %-8s\n", p->Pname[i].name.c_str(), dist[i], p->Pname[path[i]].name.c_str());

        }
        cout << "\n";
        //输出具体路径，通过栈“先进后出”的特点来帮助我们进行实现
        cout << "请输入终点：";
        cin >> name;
        int n = search_num(p, name);
        if (n != -1)//找到终点
        {
            stack<int> s;
            s.push(n);//终点编号入栈
            while (n != t)//依次压入前驱景点编号，直到压入起点景点编号
            {
                s.push(path[n]);
                n = path[n];
            }
            cout << "最短路径：\n";
            n = s.top();//返回栈顶景点编号，即起点编号，不删除
            s.pop();//弹出栈顶元素，删除栈顶
            cout << p->Pname[n].name;//起点特殊处理
            while (!s.empty())//依次弹栈，直到栈为空，同时通过t和m记录前后两个编号，便于我们输出路径长度
            {
                n = s.top();
                s.pop();
                cout << "--" << p->m[t][n] << "->";//输出路径长度
                cout << p->Pname[n].name;
                t = n;//让t等于刚刚弹出编号，下一次循环n接收栈顶元素，这样就很好记录了路径前后两个编号，便于输出路径长度
            }
            cout << "\n";
        }
        else//未找到终点
            cout << "未找到该景点。\n";
    }
    else
        cout << "未找到该景点。\n";
    cout << "--------------------------------------------\n";
}

//弗洛伊德算法
void floyd(int m[M][M], int path[M][M], int n)
{
    for (int i = 1; i <= n; i++)
        for (int j = 0; j < n; j++)
            path[i][j] = -1;
    for (int k = 1; k <= n; k++)
    {
        for (int i = 1; i <= n; i++)
        {
            for (int j = 1; j <= n; j++)
            {
                if (m[i][j] > m[i][k] + m[k][j])
                {
                    m[i][j] = m[i][k] + m[k][j];
                    path[i][j] = k;
                }
            }
        }
    }
}

//全局最短路径
void shortall_graph(Map* p)
{
    int n[M][M], path[M][M];
    for (int i = 1; i <= p->count; i++)
        for (int j = 1; j <= p->count; j++)
            n[i][j] = p->m[i][j];
    floyd(n, path, p->count);
    for (int i = 1; i <= p->count; i++)
    {
        for (int j = 1; j <= p->count; j++)
            printf("%15d", n[i][j]);
        cout << "\n";
    }
    cout << "---------------------------------------------------------------------------\n";
    for (int i = 1; i <= p->count; i++)
    {
        for (int j = 1; j <= p->count; j++)
            printf("%15d", path[i][j]);
        cout << "\n";
    }
}

void all_paths(Map* p, int start, int end, int* visit, int* path, int path_idx, int path_len, int& path_count)
{
    // 将当前节点标记为已访问，将当前节点添加到路径中，并更新当前路径长度
    visit[start] = 1;
    path[path_idx] = start;
    path_idx++;
    if (path_idx >= 2) {
        path_len += p->m[path[path_idx - 2]][start]; // 更新路径长度的条件检查
    }

    if (start == end) // 如果当前节点是目标节点，则打印当前路径和路径长度
    {
        cout << "Path: ";
        if (path_idx > 1) {
            for (int i = 0; i < path_idx - 1; i++) {
                cout << p->Pname[path[i]].name << "--" << p->m[path[i]][path[i + 1]] << "->";
            }
            cout << p->Pname[path[path_idx - 1]].name << endl;
        }
        cout << "Distance: " << path_len << endl;
        cout << endl;

        // 增加已找到的路径数量
        path_count++;
        if (path_count == 10) {
            // 打印了前十个最短路径，结束递归
            return;
        }
    }
    else // 如果当前节点不是目标节点
    {
        for (int i = 1; i <= p->count; i++) {
            if (p->m[start][i] != INF && visit[i] == 0) {
                int* visit_copy = new int[p->count + 1]; // 创建 visit 数组的副本
                memcpy(visit_copy, visit, sizeof(int) * (p->count + 1));
                all_paths(p, i, end, visit_copy, path, path_idx, path_len, path_count);
                delete[] visit_copy; // 释放 visit 数组的副本内存

                if (path_count == 10) {
                    // 已经打印了前十个最短路径，结束循环
                    break;
                }
            }
        }
    }

    // 将当前节点的访问状态重新标记为未访问，并将当前节点从路径中删除，同时更新路径长度
    visit[start] = 0;
    path_idx--;
    if (path_idx >= 1) {
        path_len -= p->m[path[path_idx - 1]][start]; // 更新路径长度的条件检查
    }
}
//所有简单路径以及最短简单路径
void all_shortpath(Map* p)
{
    string start, end;

    int t, n;
    cout << "请输入起点：";
    cin >> start;
    t = search_num(p, start);
    if (t != -1)
    {
        cout << "请输入终点：";
        cin >> end;
        n = search_num(p, end);
        if (n != -1)
        {
            int visit[M] = { 0 };
            int path1[M] = { 0 };
            int path_count = 0;
            cout << "所有简单路径：\n";
            all_paths(p, t, n, visit, path1, 0, 0, path_count);
            int dist[M];
            int path2[M];
            cout << "最短简单路径：\n";
            dijkstra(p->m, t, p->count, dist, path2);
            stack<int> s;
            s.push(n);//终点编号入栈
            while (n != t)//依次压入前驱景点编号，直到压入起点景点编号
            {
                s.push(path2[n]);
                n = path2[n];
            }
            cout << "最短路径：\n";
            n = s.top();//返回栈顶景点编号，即起点编号，不删除
            s.pop();//弹出栈顶元素，删除栈顶
            cout << p->Pname[n].name;//起点特殊处理
            while (!s.empty())//依次弹栈，直到栈为空，同时通过t和m记录前后两个编号，便于我们输出路径长度
            {
                n = s.top();
                s.pop();
                cout << "--" << p->m[t][n] << "->";//输出路径长度
                cout << p->Pname[n].name;
                t = n;//让t等于刚刚弹出编号，下一次循环n接收栈顶元素，这样就很好记录了路径前后两个编号，便于输出路径长度
            }
            cout << "\n";
        }
        else//未找到终点
            cout << "未找到该景点。\n";

    }
    else//未找到终点
        cout << "未找到该景点。\n";
    cout << "-----------------------------------------------\n";
}

//最优路线规划
void best_path(Map* p)
{
    string name;
    cout << "请输入出发景点：";
    cin >> name;
    int start = search_num(p, name);
    if (start != -1)
    {
        int dist[M], path[M], visit[M];//两个辅助数组
        int sum = 0;
        dijkstra(p->m, start, p->count, dist, path);
        int v, min;
        for (int i = 1; i <= p->count; i++)
            visit[i] = 0;
        cout << "最优路径：\n";
        cout << p->Pname[start].name;//起点特殊处理
        visit[start] = 1;
        while (1)
        {
            min = INF;
            for (int i = 1; i <= p->count; i++)//找到最短的路径
            {
                if (!visit[i] && min > dist[i])
                {
                    v = i;
                    min = dist[i];
                }
            }
            int t = start, n = v;
            stack<int> s;
            s.push(n);//终点编号入栈
            while (n != t)//依次压入前驱景点编号，直到压入起点景点编号
            {
                s.push(path[n]);
                n = path[n];
            }
            t = s.top();//返回栈顶景点编号，即起点编号，不删除
            s.pop();//弹出栈顶元素，删除栈顶
            while (!s.empty())//依次弹栈，直到栈为空，同时通过t和m记录前后两个编号，便于我们输出路径长度
            {
                n = s.top();
                s.pop();
                cout << "--" << p->m[t][n] << "->";//输出路径长度
                sum += p->m[t][n];
                cout << p->Pname[n].name;
                t = n;//让t等于刚刚弹出编号，下一次循环n接收栈顶元素，这样就很好记录了路径前后两个编号，便于输出路径长度
            }
            start = v;
            visit[start] = 1;
            int flag = 1;
            for (int i = 1; i <= p->count; i++)
                if (visit[i] == 0) flag = 0;
            if (flag == 1) break;
            dijkstra(p->m, start, p->count, dist, path);
        }
        cout << "\n";
        cout << "最优路径长度：" << sum << "\n";
    }
    else
        cout << "未找到该景点。\n";
    cout << "-----------------------------------------------\n";

}

// 计算两个特征向量的余弦相似度
float cosine_similarity(const vector<float>& v1, const vector<float>& v2) {
    float dot_product = 0.0;
    float norm_v1 = 0.0;
    float norm_v2 = 0.0;

    for (int i = 0; i < v1.size(); i++) {
        dot_product += v1[i] * v2[i];
        norm_v1 += v1[i] * v1[i];
        norm_v2 += v2[i] * v2[i];
    }

    if (norm_v1 == 0 || norm_v2 == 0) {
        return 0.0;
    }
    else {
        return dot_product / (sqrt(norm_v1) * sqrt(norm_v2));
    }
}

// 根据用户喜欢的景点名称推荐相似的景点
vector<string> recommend_similar_points(const Map& map, const string& liked_point) {
    int liked_index = -1;
    for (int i = 0; i < 14; i++) {
        if (map.Pname[i].name == liked_point) {
            liked_index = i;
            break;
        }
    }

    if (liked_index == -1) {
        cout << "抱歉，未找到喜欢的景点" << endl;
        return vector<string>();
    }

    vector<float> liked_features;  // 喜欢的景点的特征向量
    for (int i = 0; i < 14; i++) {
        liked_features.push_back(map.m[liked_index][i]);
    }

    vector<pair<string, float>> point_similarity_pairs;  // 存储景点及其相似度的对

    for (int i = 0; i < 14; i++) {
        if (i != liked_index) {
            vector<float> current_features;
            for (int j = 0; j < 14; j++) {
                current_features.push_back(map.m[i][j]);
            }

            float similarity = cosine_similarity(liked_features, current_features);
            point_similarity_pairs.push_back(make_pair(map.Pname[i].name, similarity));
        }
    }

    // 按相似度降序排序
    sort(point_similarity_pairs.begin(), point_similarity_pairs.end(),
        [](const pair<string, float>& a, const pair<string, float>& b) {
            return a.second > b.second;
        });

    // 只推荐前三个相似的景点
    vector<string> recommended_points;
    for (int i = 0; i < min(3, static_cast<int>(point_similarity_pairs.size())); i++) {
        recommended_points.push_back(point_similarity_pairs[i].first);
    }

    return recommended_points;
}

void handleUserInput(const Map& map) {
    string liked_point;
    cout << "请输入您喜欢的景点名称：";
    cin >> liked_point;

    // 根据输入的喜欢的景点名称推荐相似的景点
    vector<string> recommended_points = recommend_similar_points(map, liked_point);

    // 输出推荐的相似景点
    cout << "为您推荐的相似景点有：" << endl;
    for (const auto& point : recommended_points) {
        cout << point << endl;
    }
}
void tour_mainface(Map* p, FoodMap* f, Diary* D1, int choose1, char tempch)
{
    system("cls");
    while (1)
    {
        cout << "\t***************************************************************\n";
        cout << "\t--------------------欢迎进入景区景点管理系统-------------------\n";
        cout << "\t    *                  主菜单                             *    \n";
        cout << "\t    *          【1】展示所有景点                          *    \n";
        cout << "\t    *          【2】查找景点                              *    \n";
        cout << "\t    *          【3】展示当前景点附近景点                  *    \n";
        cout << "\t    *          【4】两景点最短路径规划                    *    \n";
        cout << "\t    *          【5】两景点所有简单路径和最短简单路径      *    \n";
        cout << "\t    *          【6】景区最佳游览路径规划                  *    \n";
        cout << "\t    *          【7】写入数据                              *    \n";
        cout << "\t    *          【8】推荐景点                              *    \n";
        cout << "\t    *          【9】推荐美食                              *    \n";
        cout << "\t    *          【10】查找美食                              *    \n";
        cout << "\t    *          【11】日记系统                              *    \n";
        cout << "\t    *          【12】清屏                                  *    \n";
        cout << "\t    *          【0】清屏                                  *    \n";
        cout << "\t提示：本系统只会在开始时读出文件，退出系统以后写入文件，中间可以用过指令完成写入\n";
        cout << "\t******************************************************\n";
        cout << "\n\t请选择您的操作 (0-14):";
        int t;
        cin >> t;
        cout << "操作" << t << "\n";
        while (!(t >= 0 && t <= 13))//检测输入数据
        {
            cout << "*输入有误，请重新输入：";
            cin >> t;
            cout << "操作" << t << "\n";
        }
        switch (t)
        {
        case 1:
            print_graph(p); break;
        case 2:
            tour_search_graph(p); break;
        case 3:
            show_around(p); break;
        case 4:
            shortone_graph(p); break;
        case 5:
            all_shortpath(p); break;
        case 6:
            best_path(p); break;
        case 7:
            write_graph(p); break;
        case 8:
            handleUserInput(*p); break;
        case 9:
            print_food(f); break;
        case 10:
            tour_search_graph_food(f); break;
        case 11:
            diary_system(); break;
        case 12:
            system("cls");; break;
        case 0:
            cout << "\n\t\t\t*按任意键关闭本系统*\n";
            exit(0);
            break;
        }
    }
}

//登录账号

void log_in(Diary* D1, int choose1, char tempch)
{
    Map p; FoodMap f;
    read_graph(&p);
    read_graph_food(&f);
    string id, password;
    int flag1;
    flag1 = 1;//辅助变量，便于退出循环，以免卡入死循环
    while (flag1)//多层嵌套循环实现不同界面之间的转化
    {
        system("cls");
        cout << "             景区登陆界面\n";
        cout << "*************************************\n";
        cout << "            1. 游客登陆\n";
        cout << "            2. 退出\n";
        cout << "请输入选项编号：";
        int option;
        cin >> option;
        switch (option)
        {
        case 1:
        {
            std::cout << "游客登陆\n";
            std::cout << " 账号：";
            std::cin >> id;

            std::cout << " 密码：";
            std::cin >> password;

            if (userCredentials.find(id) != userCredentials.end() && userCredentials[id] == password)
            {
                std::cout << " 游客登陆成功，欢迎您！\n";
                std::cout << "提示：系统将在6秒后跳转界面\n";
                std::cout << "主界面进入中\n";
                Sleep(6 * 1000);
                tour_mainface(&p, &f, D1, choose1, tempch);
                return;
            }
            else
            {
                std::cout << "账号或密码错误\n";
                Sleep(7 * 1000);
            }

            flag1 = 0;
            break;
        }

        case 2:
            cout << "退出成功!\n";
            Sleep(4 * 1000);
            return;
        default:
            cout << "输入错误，请正确输入!\n";
            break;
        }
    }
}

//账户设置
void Account(Diary* D1, int choose1, char tempch)
{
    system("color 70");
    system("cls");
    int flag;
    flag = 1;
    while (flag)
    {
        system("cls");
        cout << "             景区管理系统\n";
        cout << "*************************************\n";
        cout << "            1. 登陆系统\n";
        cout << "            2. 退出系统\n";
        string tempt;
        cout << "请输入选项编号：";
        cin >> tempt;
        int option;
        option = stoi(tempt);
        switch (option)
        {
        case 1:
            log_in(D1, choose1, tempch);
            flag = 0;
            break;
        case 2:
            cout << "退出成功!" << endl;
            flag = 0;
            break;
        default:
            cout << "输入错误，请正确输入!" << endl;
            break;
        }
    }
}

//欢迎界面
void Welcome(Diary* D1, int choose1, char tempch) {
    cout << "\n\n\t\t******欢迎使用景区旅游信息管理系统******\n\n";
    cout << " \t \t \t 按任意键进入系统... ...";
    getchar();
    Account(D1, choose1, tempch);
}

//主函数
int main()
{
    system("chcp 65001");
    Diary* D1 = NULL;
    int choose1 = 0;
    char tempch = '1', FileName[20];
    D1 = Init_Diary(D1);
    Welcome(D1, choose1, tempch);
    return 0;
}
/*初始化日记表*/
Diary* Init_Diary(Diary* D1)
{
    D1 = (Diary*)malloc(sizeof(Diary));  //为用户D1申请空间
    D1->next = NULL;
    return D1;
}
//读取日记信息
void read_graph_diary(Diary* D) {
    FILE* fp = fopen("diary.txt", "r");
    char ch, number[5];
    int r = 0, i, j;
    for (i = 0; i < 5; i++)
    {
        number[i] = '0';
    }
    if (fp == NULL)
    {
        fclose(fp);
        fflush(stdin);
    }
    else
    {
        rewind(fp);//将指针重置到第一行
        while (!feof(fp)) {  //检测文件一共多少行
            ch = fgetc(fp);
            if (ch == '\n') {
                r++;
            }
        }
        rewind(fp);//将指针重置到第一行
        int end1 = 0;
        Diary* D1, * fd;
        D1 = D;
        for (i = 0; i < r / 5; i++)
        {
            fd = (Diary*)malloc(sizeof(Diary));
            fd->next = NULL;
            fgets(fd->keyword, 40, fp);
            fgets(fd->content, 1000, fp);
            fgets(fd->title, 40, fp);
            fgets(number, 10, fp);
            fd->star = number[0] - '0' + (number[2] - '0') * 0.1;
            fgets(number, 10, fp);
            j = 0;
            fd->times = 0;
            while (number[j] <= '9' && number[j] >= '0')
            {
                fd->times = (fd->times) * 10 + (number[j] - '0'); j++;
            }
            D1->next = fd;
            D1 = D1->next;
            D1->next = NULL;
        }

    }
    fflush(stdin);
    fclose(fp);
}
/* 将所有日记记录的标记tag值置为0或-1*/
void Initial_Tag(Diary* D1, int tag)
{
    Diary* q;
    q = D1->next;
    while (q != NULL)
    {
        q->tag = tag;
        q = q->next;
    }
}
/* 打开日记*/
int Open_Diary(Diary* D1, Diary* D)
{
    double k;
    printf("\n\t--------------------------------------------------\n");
    printf("\n\ttitle==>%s", D1->title);
    printf("\n\n\tdestination==>%s", D1->keyword);
    printf("\n\t--------------------------------------------------\n");
    printf("\n\n\tcontent:\n");
    printf("\n\t%s", D1->content);
    D1->times++;
    printf("\n\n\theat==>%d", D1->times);
    printf("\n\n\tscore==>%.2lf", D1->star);
    printf("\n\n\tplease score from 1 to 5:");
    scanf("%lf", &k);
    D1->star = (D1->star * (D1->times - 1) + k) / D1->times; Save_Diary(D, 1);
    printf("\n\n\tcompress and store this diary：1、Yes   2、No");
    int r;
    scanf("%d", &r);
    if (r == 1)
        compress(D1);
    return 1;
}
/*进入日记菜单并选择操作*/
void diary_system()
{
    printf("\n\t\t\t           “diary_system”－－－－"
        ""
        ""
        "－－－－－(0->return)\n");
    printf("\t|----------------------------------------------------------------------|\n");
    printf("\t|  1、Write the diary                                                  |\n");
    printf("\t|  2、Diary recommendation                                             |\n");
    printf("\t|  3、Diary search                                                     |\n");
    printf("\t|----------------------------------------------------------------------|\n");
    int op, opt;
    Diary* D = NULL;
    D = Init_Diary(D);
    read_graph_diary(D);
    printf("\n\tPlease select:\t");
    scanf("%d", &op);
    switch (op)
    {
    case 1:
        D = Write_Diary(D); break;
    case 2:
        printf("Please select the recommendation criteria:\n1、heat\t2、score\n");
        scanf("%d", &opt);
        if (opt == 1)
        {
            Torder(D);
        }
        else if (opt == 2)
            Sorder(D);
        else
            printf("error。");
        Printf_DiaryList(D);
        break;
    case 3:
        Search_Diary(D);
        break;
    }

    rewind(stdin);         /*清空键盘缓冲区里的无用字符*/
}
/* 日记保存*/
void Save_Diary(Diary* D1, int m)
{
    Diary* fd;
    FILE* p = fopen("diary.txt", "w");
    if (p == NULL)
    {
        printf("Nothing");
        getchar();
        exit(1);
    }
    fd = D1->next;
    while (fd)
    {
        if (m == 0)
        {
            fprintf(p, "%s\r\n%s\r\n%s\r\n%.1lf\r\n%d\r\n", fd->keyword, fd->content, fd->title, fd->star, fd->times);
            m = 1; fd = fd->next;
            continue;
        }
        fprintf(p, "%s%s%s%.1lf\r\n%d\r\n", fd->keyword, fd->content, fd->title, fd->star, fd->times);
        fd = fd->next;
    }
    fflush(stdin);
    fclose(p);
}

/*显示日记列表*/
int Printf_DiaryList(Diary* D1)
{
    int count = 0, m = 0, i;
    Diary* D;
    D = D1;
    Initial_Tag(D, 0);
    printf("\n");
    if (D->next == NULL)
        printf("\tNo diary\n");
    else
    {
        while (D->next != NULL)
        {
            D = D->next;
            if (D->tag == 0)
            {
                count++;
                printf("\tNo.%2d==>   TITLE:%-20s\n", count, D->title);
                printf("\tDESTINATION：%-s\n", D->keyword);
                printf("\tSCORE：%.2lf\tHEAT：%d\n", D->star, D->times);
            }
        }
    }
    D = D1;
    m = 1;
    while (m != 0)
    {
        printf("\n\n\tEnter the serial number of the journal to be viewed. If exiting, enter 0:\n");
        scanf_s("%d", &m);
        if (m != 0)
        {
            i = 0;
            while (D != NULL)
            {
                D = D->next;
                if (D->tag == 0)
                    i++;
                if (D->tag == 0 && i == m)
                    break;
            }
            Open_Diary(D, D1);
        }
    }
    return count;
}
/* 写新的日记*/
Diary* Write_Diary(Diary* D1)
{
    int temp, i;
    Diary* p1 = NULL;
    p1 = Init_Diary(p1);
    p1 = (Diary*)malloc(sizeof(Diary));
    getchar();
    printf("\n\tPlease enter the title:\t");
    gets_s(p1->title);
    printf("\n\tPlease enter the diary content:\n\n\t");
    gets_s(p1->content);
    printf("\n\tplease enter the study tour destination:\t");
    gets_s(p1->keyword);
    p1->tag = 0;
    p1->star = 0;
    p1->times = 0;
    p1->next = D1->next;
    D1->next = p1;
    Save_Diary(D1, 0);
    return D1;
}
//用户主界面


void Print_search(int* choose)
{
    printf("\n\t\t                      “Search diary”－－－－－(0->return)\n");
    printf("\t|---------------------------------------------------------------------------------------------|\n");
    printf("\t|   1.Search by title     2.Search by study tour destination   3.Search by  the diary content |\n");
    printf("\t|---------------------------------------------------------------------------------------------|\n");
    do {
        printf("\n\tPlease select:\t");
        scanf("%d", choose);
    } while (*choose < 0 || *choose>5);
    rewind(stdin);         /*清空键盘缓冲区里的无用字符*/
}

int Search_Diary(Diary* D1)
{
    char tempch = '1';
    char word[20];
    int choose;
    Diary* p;
    while (tempch != ESC)
    {
        Print_search(&choose);
        if (choose == 0)
        {
            return 0;
        }
        p = D1;
        if (p->next == NULL)
            printf("\n\tNo diary.\n");
        else
        {
            switch (choose)
            {
            case 0: return 0;
            case 1: {
                printf("\n\tPlease enter the journal title:\t");
                gets_s(word);
                Filter_word(D1, word, 1);
            }break;     //按标题查阅
            case 2: {
                printf("\n\tPlease enter the study tour destination:\t");
                gets_s(word);
                Filter_word(D1, word, 2);
            }break;     //按关键字查阅
            case 3: {
                printf("\n\tPlease enter the diary content:\t");
                gets_s(word);
                Filter_word(D1, word, 3);
            }break;
            }

        }
        Initial_Tag(D1, 0);
        tempch = _getch();
        rewind(stdin);
    }
    return 1;
}
int compare_title(char* a, char* b)
{
    int la = strlen(a) - 1, lb = strlen(b);
    int i;
    if (la == lb)
        for (i = 0; i < la; i++)
        {
            if (a[i] != b[i])
                break;
        }
    if (i == la)
        return 0;
    else
        return 1;
}
Diary* Filter_word(Diary* D1, char word[], int n)
{
    int tmp = 1, i, m, op, k = 0;
    Diary* D, * D2;
    char* temp = NULL;
    Initial_Tag(D1, 0);
    D = D1;
    while (D->next != NULL)
    {
        if (n == 1)
            tmp = compare_title(D->next->title, word);
        else {
            if (n == 2)
                temp = strstr(D->next->keyword, word);
            else if (n == 3)
                temp = strstr(D->next->content, word);
            if (temp == NULL)
                D->next->tag = -1;
            else
                k++;
        }
        if (tmp == 0)
        {
            Open_Diary(D->next, D1); break;
        }
        D = D->next; //比较下一个
    }
    if (tmp == 1)
    {
        if (k == 0)
            printf("No diary that meets the requirements\n");
        else {
            if (k > 1)
            {
                printf("\n\n\taccording to which order：\n\n\t1、heat\t2、score\n");
                scanf("%d", &op);
                if (op == 1)
                {
                    Torder(D1);
                }
                else
                    Sorder(D1);
            }
            printf("\n\n\tThe following diaries meet the search requirements:\n");
            Printf_DiaryList(D1);
        }
    }
    Initial_Tag(D1, 0);
    return D1;
}
void Torder(Diary* A)
{
    Diary* tmp, * temp, * A1, * p, * preA1;
    Diary* head, * pre, * cur, * next, * end;
    head = A;
    end = NULL;
    //从链表头开始将较大值往后沉
    while (head->next != end)
    {
        for (pre = head, cur = pre->next, next = cur->next; next != end; pre = pre->next, cur = cur->next, next = next->next)
        {
            //相邻的节点比较
            if (cur->times < next->times)
            {
                cur->next = next->next;
                pre->next = next;
                next->next = cur;
                temp = next;
                next = cur;
                cur = temp;
            }
        }
        end = cur;
    }
}

void Sorder(Diary* A)
{
    Diary* tmp, * temp, * A1, * p, * preA1;
    Diary* head, * pre, * cur, * next, * end;
    head = A;
    end = NULL;
    //从链表头开始将较大值往后沉
    while (head->next != end)
    {
        for (pre = head, cur = pre->next, next = cur->next; next != end; pre = pre->next, cur = cur->next, next = next->next)
        {
            //相邻的节点比较
            if (cur->star < next->star)
            {
                cur->next = next->next;
                pre->next = next;
                next->next = cur;
                temp = next;
                next = cur;
                cur = temp;
            }
            printf("%d\n", cur->times);
        }
        end = cur;
    }
}
//用哈夫曼编码对一串小写字母字符串进行压缩
typedef struct blo
{
    int fre;
    char ch;
    int hufflong = 0;
}blo, * bloo;
typedef struct link
{
    blo node;
    int parent;
    int lchild;
    int rchild;
    char* code;
}link, * huffman;
void initblo(bloo& node)
{
    int i;
    node = (bloo)malloc(sizeof(blo) * 55);
    for (i = 0; i < 55; i++)
    {
        node[i].fre = 0;
    }
    for (i = 0; i < 26; i++)
    {
        node[i].ch = 'a' + i;
    }
    for (i = 0; i < 26; i++)
    {
        node[i + 26].ch = 'A' + i;
    }
    node[52].ch = ',';
    node[53].ch = '.';
    node[54].ch = ' ';
}
void huffsort(huffman& ht, int& s1, int& s2, int n)
{
    int min;
    int i;
    for (i = 1; i <= n; i++)
    {
        if (ht[i].parent == 0)
        {
            min = i; break;
        }
    }
    for (i = min + 1; i <= n; i++)
    {
        if (ht[i].parent == 0 && ht[i].node.fre < ht[min].node.fre)
        {
            min = i;
        }
    }
    s1 = min;
    for (i = 1; i <= n; i++)
    {
        if (ht[i].parent == 0 && i != s1)
        {
            min = i; break;
        }
    }
    for (i = min + 1; i <= n; i++)
    {
        if (ht[i].parent == 0 && ht[i].node.fre < ht[min].node.fre && i != s1)
        {
            min = i;
        }
    }
    s2 = min;
}

void creathuff(huffman& ht, bloo& node)
{
    int n = 55;
    int m = 2 * n - 1;
    int i = 1;
    ht = (huffman)malloc(sizeof(link) * (m + 1));
    for (i = 0; i <= m; i++)
    {
        if (i <= n && i != 0)
        {
            ht[i].node.ch = node[i - 1].ch;
            ht[i].node.fre = node[i - 1].fre;
        }
        else
        {
            ht[i].node.ch = '#';
            ht[i].node.fre = 0;
        }
        ht[i].parent = 0;
        ht[i].lchild = 0;
        ht[i].rchild = 0;
    }
    for (i = n + 1; i <= m; i++)
    {
        int s1, s2;
        huffsort(ht, s1, s2, i - 1);
        ht[s1].parent = i;
        ht[s2].parent = i;
        ht[i].lchild = s1;
        ht[i].rchild = s2;
        ht[i].node.fre = ht[s1].node.fre + ht[s2].node.fre;
    }
}
void huffcode(huffman& ht)
{
    for (int i = 1; i <= 55; i++)
    {
        char* cd = (char*)malloc(sizeof(char) * 55);
        cd[55 - 1] = '\0';
        int start = 55 - 1;
        int c = i;
        int p = ht[c].parent;
        while (p != 0)
        {
            if (ht[p].lchild == c)
            {
                cd[--start] = '0';
            }
            if (ht[p].rchild == c)
            {
                cd[--start] = '1';
            }
            c = p; p = ht[p].parent;
        }
        ht[i].node.hufflong = 0;
        for (int j = start; j < 55; j++)
        {
            ht[i].node.hufflong++;
        }
        ht[i].node.hufflong--;

        ht[i].code = (char*)malloc(sizeof(char) * (ht[i].node.hufflong + 1));
        for (int j = start; j < 55; j++)
        {
            ht[i].code = &cd[start];
        }
    }
}
void number(bloo& node, huffman& ht, int& size)
{

    size = 0; int i;
    for (i = 0; i < 55; i++)
    {
        size = size + node[i].fre * ht[i + 1].node.hufflong;
    }
}
void decompress(char* str, huffman& ht, int size, int huffsize)
{
    char result[2000] = "";
    for (int i = 0; i < size; i++)
    {
        for (int j = 1; j <= 55; j++)
        {
            if (str[i] == ht[j].node.ch)
            {
                printf("%s", ht[j].code);
                strcat(result, ht[j].code);
                break;
            }
        }
    }
    int i, p = 0;
    printf("\nDecompress or not：1、Yes 2、No\n");
    scanf("%d", &p);
    if (p == 1)
    {
        for (int end = 0, start = 0; end < huffsize; end++)
        {
            int count = 0;
            int nodelength = end - start + 1;
            for (i = 1; i <= 55; i++)
            {
                if (nodelength == ht[i].node.hufflong)
                {
                    int whetherfind = 0;
                    for (int j = 0, count = 0; j < nodelength; j++)
                    {
                        if (result[start + j + 1] == ht[i].code[j])
                        {
                            count++;
                        }
                        if (count == nodelength)
                        {
                            printf("%c", ht[i].node.ch);
                            end++;
                            start = end;
                            whetherfind = 1;
                            break;
                        }
                    }
                    if (whetherfind == 1)
                    {
                        break;
                    }
                }
            }
        }
    }
}
void compress(Diary* D) {
    bloo node;
    char* str = D->content;
    int size = strlen(str) - 1;
    initblo(node);
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < 55; j++)
        {
            if (str[i] == node[j].ch)
                node[j].fre++;
        }
    }
    huffman ht;
    creathuff(ht, node);
    huffcode(ht);
    int huffsize = 0;
    number(node, ht, huffsize);
    printf("\n\nOriginal string compressed as:\n");
    decompress(str, ht, size, huffsize);
}