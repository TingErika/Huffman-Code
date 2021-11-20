// Huffman Code.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <unordered_map>
#include <queue>
#include <stack>
#include <fstream>
#include <bitset>
#define MAX 100000
using namespace std;

unordered_map<char, int> probability;
unordered_map<char, string> huffmanCode;
unordered_map<string, char> inverseHuffmanCode;
string result = "";

template<class T>
//最小堆类定义
class MinHeap {
private:
    T* heapArray;       //存放数据的数组
    int CurrentSize;    //当前堆中元素数目
    int MaxSize;        //最大元素数目
    void swap(int pos_x, int pos_y);    //交换位置x和y的元素
    void BuildHeap();   //建堆

public:
    MinHeap(const int n);   //构造函数，参数n为堆的最大元素数目
    //析构函数
    virtual ~MinHeap() {
        delete[]heapArray;
    };
    bool isEmpty();                 //判断堆是否为空
    bool isLeaf(int pos) const;     //判断是否是叶结点
    int LeftChild(int pos) const;   //返回左孩子位置
    int RightChild(int pos) const;  //返回右孩子位置
    int Parent(int pos) const;      //返回父结点位置
    bool Remove(int pos, T& node);  //删除给定下标的元素
    bool Insert(const T& newNode);  //向堆中插入新元素newNode
    T& RemoveMin();                 //从堆顶删除最小值
    void SiftUp(int position);      //从position开始向上调整
    void SiftDown(int left);        //从left开始向下筛选
};

template<class T>
MinHeap<T>::MinHeap(const int n) {
    if (n <= 0) {
        return;
    }
    CurrentSize = 0;
    MaxSize = n;                //最大元素数目赋值为n
    heapArray = new T[MaxSize]; //创建堆空间
    BuildHeap();                //进行堆元素的赋值工作
}

template<class T>
bool MinHeap<T>::isEmpty() {
    return CurrentSize == 0;
}

template<class T>
bool MinHeap<T>::isLeaf(int pos) const {
    return (pos >= CurrentSize / 2) && (pos < CurrentSize);
}

template<class T>
void MinHeap<T>::swap(int pos_x, int pos_y) {
    T temp = heapArray[pos_x];
    heapArray[pos_x] = heapArray[pos_y];
    heapArray[pos_y] = temp;
}

template<class T>
void MinHeap<T>::BuildHeap() {
    for (int i = CurrentSize / 2 - 1; i >= 0; i--) {
        SiftDown(i);
    }
}

template<class T>
int MinHeap<T>::LeftChild(int pos) const {
    return 2 * pos + 1;
}

template<class T>
int MinHeap<T>::RightChild(int pos) const {
    return 2 * pos + 2;
}

template<class T>
int MinHeap<T>::Parent(int pos) const {
    return (pos - 1) / 2;
}

template<class T>
bool MinHeap<T>::Insert(const T& newNode) {
    //如果堆已满则返回false
    if (CurrentSize == MaxSize) {
        return false;
    }
    heapArray[CurrentSize] = newNode;   //新元素newNode放到堆的末尾
    SiftUp(CurrentSize);    //向上调整
    CurrentSize++;          //堆的当前元素数+1
    return true;
}

template<class T>
T& MinHeap<T>::RemoveMin() {
    //判断堆非空
    if (CurrentSize == 0) {
        cout << "Can't Delete";
        exit(1);
    }
    else {
        swap(0, --CurrentSize); //交换堆顶和堆末尾的元素
        //若当前元素数大于1则从堆顶开始向下筛选
        if (CurrentSize > 1) {
            SiftDown(0);
        }
        return heapArray[CurrentSize];
    }
}

template<class T>
bool MinHeap<T>::Remove(int pos, T& node) {
    if ((pos < 0) || (pos >= CurrentSize)) {
        return false;
    }
    node = heapArray[pos];  //记录删除的元素
    heapArray[pos] = heapArray[--CurrentSize];  //用最后的元素值替代被删除元素
    //当前元素小于父结点，需要上升调整
    if (heapArray[Parent(pos)] > heapArray[pos]) {
        SiftUp(pos);
    }
    //当前元素大于父结点，向下筛选
    else {
        SiftDown(pos);
    }
    return true;
}

template<class T>
void MinHeap<T>::SiftUp(int position) {
    int temppos = position;
    T temp = heapArray[temppos];
    while ((temppos > 0) && (heapArray[Parent(temppos)] > temp)) {
        heapArray[temppos] = heapArray[Parent(temppos)];
        temppos = Parent(temppos);
    }
    heapArray[temppos] = temp;
}

template<class T>
void MinHeap<T>::SiftDown(int left) {
    int i = left;           //标识父结点
    int j = LeftChild(i);   //用于记录关键值较小的子结点
    T temp = heapArray[i];  //保存父结点
    //过筛
    while (j < CurrentSize) {
        //若有右子结点且小于左子结点
        if ((j < CurrentSize - 1) && (heapArray[j] > heapArray[j + 1])) {
            j++;    //j指向右子结点
        }
        //如果父结点大于子结点的值则交换位置
        if (temp > heapArray[j]) {
            heapArray[i] = heapArray[j];
            i = j;
            j = LeftChild(j);
        }
        //堆序性满足时则跳出
        else {
            break;
        }
    }
    heapArray[i] = temp;
}

template<class T> class HuffmanTree;
template<class T>
class HuffmanTreeNode {
    friend class HuffmanTree<T>;
private:
    char info;
    T weight;
    HuffmanTreeNode<T>* parent;
    HuffmanTreeNode<T>* left;
    HuffmanTreeNode<T>* right;
public:
    HuffmanTreeNode<T>* leftchild() {
        return left;
    }
    HuffmanTreeNode<T>* rightchld() {
        return right;
    }
    //重载运算符
    bool operator > (HuffmanTreeNode<T>& HN) {
        return weight > HN.weight;
    }
    bool operator < (HuffmanTreeNode<T>& HN) {
        return weight < HN.weight;
    }
    bool operator == (HuffmanTreeNode<T>& HN) {
        return weight == HN.weight;
    }
};

template<class T>
class HuffmanTree {
private:
    HuffmanTreeNode<T>* root;   //Huffman树的根节点
    //把以ht1和ht2为根的两棵Huffman树合并成一棵以parent为根的二叉树
    void MergeTree(HuffmanTreeNode<T>& ht1, HuffmanTreeNode<T>& ht2, HuffmanTreeNode<T>* parent);
    //删除Huffman树或其子树
    void DeleteTree(HuffmanTreeNode<T>* root);
    //在Huffman树的基础上构建Huffman编码
    void HuffmanCoding(HuffmanTreeNode<T>* root);
public:
    //构造Huffman树，参数weight为权值数组，n为数组长度
    HuffmanTree();
    //析构函数
    virtual ~HuffmanTree() {
        DeleteTree(root);
    };
};

//引用传递：形参相当于是实参的“别名”，对形参的操作其实就是对实参的操作
//在引用传递过程中，被调函数的形式参数虽然也作为局部变量在栈中开辟了内存空间，但是这时存放的是由主调函数放进来的实参变量的地址
//被调函数对形参的任何操作都被处理成间接寻址，即通过栈中存放的地址访问主调函数中的实参变量。正因为如此，被调函数对形参做的任何操作都影响了主调函数中的实参变量。
//指针传递：形参为指向实参地址的指针，当对形参的指向操作时，就相当于对实参本身进行的操作
template<class T>
void HuffmanTree<T>::MergeTree(HuffmanTreeNode<T>& ht1, HuffmanTreeNode<T>& ht2, HuffmanTreeNode<T>* parent) {
    HuffmanTreeNode<T>* left = new HuffmanTreeNode<T>();
    HuffmanTreeNode<T>* right = new HuffmanTreeNode<T>();
    *left = ht1; // 不能写为parent->left = &ht1，应开辟新空间，或者应用拷贝构造函数
    *right = ht2;
    parent->info = '\0';
    parent->weight = ht1.weight + ht2.weight;
    parent->parent = NULL;
    parent->left = left;   
    parent->right = right;
}

template<class T>
void HuffmanTree<T>::DeleteTree(HuffmanTreeNode<T>* root) {
    if (root) {
        DeleteTree(root->left);
        DeleteTree(root->right);
        delete root;
    }
}

template<class T>
HuffmanTree<T>::HuffmanTree() {
    int i = 0;
    int size = probability.size();
    MinHeap<HuffmanTreeNode<T>> heap(size);    //最小值堆
    HuffmanTreeNode<T>* parent, firstchild, secondchild;
    HuffmanTreeNode<T>* NodeList = new HuffmanTreeNode<T>[size];
    //初始化
    for (auto iter = probability.begin(); iter != probability.end(); iter++) {
        NodeList[i].info = iter->first;
        NodeList[i].weight = iter->second;
        NodeList[i].parent = NodeList[i].left = NodeList[i].right = NULL;
        heap.Insert(NodeList[i]);   //向堆中添加元素
    }
    //通过n-1次合并建立Huffman树
    for (i = 0; i < size - 1; i++) {
        parent = new HuffmanTreeNode<T>;    //申请一个分支结点
        firstchild = heap.RemoveMin();      //选择权值最小的结点
        secondchild = heap.RemoveMin();    //选择权值次小的结点
        MergeTree(firstchild, secondchild, parent); //将权值最小的两棵树合并到parent树
        heap.Insert(*parent);               //把parent插入到堆中
        root = parent;                      //Huffman树的根节点赋为parent
    }
    delete[]NodeList;
    HuffmanCoding(root);
}

//由Huffman树得到Huffman编码
template<class T>
void HuffmanTree<T>::HuffmanCoding(HuffmanTreeNode<T>* root) {
    HuffmanTreeNode<T>* curNode;
    queue<HuffmanTreeNode<T>* > nodeQueue;
    stack<char> codeStack;
    nodeQueue.push(root);
    while (!nodeQueue.empty()) {
        curNode = nodeQueue.front();
        nodeQueue.pop();
        if (!curNode->left && !curNode->right) {
            HuffmanTreeNode<T>* tmpNode = curNode;
            while (tmpNode->parent) {
                if (tmpNode->parent->left == tmpNode) {
                    codeStack.push('0');
                }
                else if (tmpNode->parent->right == tmpNode) {
                    codeStack.push('1');
                }
                tmpNode = tmpNode->parent;
            }
            string code = "";
            while (!codeStack.empty()) {
                code += codeStack.top();
                codeStack.pop();
            }
            huffmanCode[curNode->info] = code;
        }
        if (curNode->left) {
            curNode->left->parent = curNode;
            nodeQueue.push(curNode->left);
        }
        if (curNode->right) {
            curNode->right->parent = curNode;
            nodeQueue.push(curNode->right);
        }
    }
}

//另一种由Huffman树得到Huffman编码的方法
template<class T>
void createCodeList(HuffmanTreeNode<T>* root, string s) {
    if (!root->left && !root->right) {
        huffmanCode[root->info] = s;
        return;
    }
    if (root->left) {
        createCodeList(root->left, s + '0');
    }
    if (root->right) {
        createCodeList(root->right, s + '1');
    }
}

//读取原文件，统计频率并加入map中
void readCount(string filename) {
    char info;
    ifstream in;
    in.open(filename, ios::in);
    if (in.good()) {
        while (!in.eof()) {
            in.get(info);
            //该判断用于避免读取不存在的下一位
            if (!in.eof()) {
                probability[info]++;
            }
        }
    }
    else {
        cout << "Fail to open the file " << filename << endl;
        return;
    }
    in.close();
}

void compress(string filename, string zipname) {
    //打开输入文件
    ifstream in;
    in.open(filename);
    //判断文件是否正常打开
    if (!in.good()) {
        cout << "Fail to open the file " << filename << endl;
        return;
    }

    //以二进制形式打开输出文件，且如果文件已存在，则清空后再写入
    ofstream out;
    out.open(zipname, ios_base::trunc|ios_base::binary);
    //判断文件是否正常打开
    if (!out.good()) {
        cout << "Fail to open the file " << zipname << endl;
        return;
    }
    
    //写入Huffman编码个数
    out << huffmanCode.size() << " ";
    //遍历HuffmanCode中Huffman结点并写入文件
    for (auto iter = huffmanCode.begin(); iter != huffmanCode.end(); iter++) {
        //将编码的key与value写入文件
        out << iter->first << iter->second << " ";
    }
    char info;
    unsigned long ulong;
    unsigned char byte;
    string cs = "";
    int length = cs.length();
    string str = "";
    
    while (!in.eof()) {
        // MAX 防止出现string长度超出限制。当文件很大时，必须要有此句
        while (length < MAX && !in.eof()) {
            //通过Huffman编码表将原文转成相应的编码
            in.get(info);
            //防止读取文件末尾不存在的一位
            if (!in.eof()) {
                cs += huffmanCode[info];
                length = cs.length();
            }
        }
        //将转化后的二进制编码串写入文件
        while (length >= 8) {
            //取前8b
            str = cs.substr(0, 8);
            bitset<8> bits(str);
            ulong = bits.to_ulong(); //转换为long类型
            byte = ulong;            //转换为char类型
            out << byte;           //写入文件
            cs = cs.substr(8, length - 8);  //取出剩下的二进制串
            length = cs.length();
        }
    }
    //假如剩余不足8位时，补足0并写入文件
    if (length != 0) {
        str = cs.substr(0, length);
        int n = 0;
        //补0
        while (n < 8 - length) {
            str += '0';
            n++;
        }
        bitset<8> bits(str);
        ulong = bits.to_ulong(); //转换为long类型
        byte = ulong;    //转换为char类型
        out << byte;   //写入文件
    }
    //写入补0的个数，如果没有则写入0
    char p = (char)(8 - length) % 8;
    out << p;
    out.close();   //关闭文件
    in.close();     //关闭文件
}

//将char转成string二进制串
string turnachar(unsigned char c)
{
    string s = "";
    int j = 128;    //后八位为 1000_0000
    for (int i = 0; i < 8; i++)
    {
        //判断原char该位数是0或1
        s += (unsigned char)(bool)(c & j) + '0';
        j >>= 1;    //将1右移
    }
    return s;
}

//获取Huffman译码表并进行译码
void decompress(string zipname, string filename) {
//1、获取Huffman译码表
    //以二进制的形式打开编码后的文件
    ifstream in;
    in.open(zipname, ios_base::binary);
    //假如文件打开失败
    if (in.fail()) {
        cout << "Fail to open the file " << zipname << endl;
        return;
    }

    //要以文本文件形式打开并写入输出文件，不然回车换行不能正常显示
    ofstream out(filename, ios_base::trunc);
    //假如文件打开失败
    if (out.fail()) {
        cout << "Fail to open the file " << filename << endl;
        return;
    }

    int size;
    char info;
    char c;
    string value;
    //如果只是>>这种的话，会读取不到\n，然后会出错
    in >> size;
    in.get(c);      //读取掉空格
    while (size > 0) {
        in.get(info);   //读取info
        in >> value;    //读取value
        in.get(c);      //读取掉空格
        inverseHuffmanCode[value] = info; //将key与value写入invertHuffmanCode
        size--;
        cout << value << " " << info << endl;
    }

//2、开始译码
    unsigned char byte;
    string str = "";
    int length;
    while (!in.fail()) {
        result.clear();
        str.clear();
        length = str.length();
        while (length < MAX && !in.eof()) {
            in.get(c);  //读取每一个char
            //读取到文件末尾时起效，保证读取正常
            if (!in.eof()) {
                byte = (unsigned char)c;  //转成无符号的
                str += turnachar(byte);    //转化成原本的二进制串
                length = str.length();
            }
        }
        //当文件结束时，去除补的0及记录个数的char
        if (in.fail()) {
            int num = (int)c;   //num代表补0个数
            str = str.substr(0, str.length() - 8 - num);
        }

        string cs = "";
        int i = 1;
        bool check = false;
        while (str.length() > 0) {
            //开始解码
            cs = str.substr(0, i);
            //假如在Huffman表中找不到，继续读取下一位
            while (inverseHuffmanCode.find(cs) == inverseHuffmanCode.end()) {
                i++;
                length = str.length();
                //判断是否超过原字符串大小，避免报错
                if (i > length) {
                    check = true;
                    break;
                }
                cs = str.substr(0,i);
            }
            //用于退出两层循环
            if (check == true) {
                break;
            }
            //解码
            result += inverseHuffmanCode[cs];
            //去除已解码的部分，继续解码
            str = str.substr(i, str.length() - i);
            i = 1;
        }
        //将解码后的结果写入文件
        out << result;
    }
    //关闭文件
    in.close();
    out.close();
}

int main()
{
    string filename = "..\\1.txt";
    string zipname = "..\\1.zip";
    string unzipname = "..\\2.txt";
    readCount(filename);
    //pair<char, int> infoWeight[] = { {'a',15},{'b',2}, {'c',6}, {'d',5}, {'e',20}, {'f',10}, {'g',18}};
    HuffmanTree<int> ht = HuffmanTree<int>();
    for (auto i = huffmanCode.begin(); i != huffmanCode.end(); i++) {
        cout << i->first << " " << i->second << endl;
    }
    compress(filename, zipname);
    decompress(zipname, unzipname);
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
