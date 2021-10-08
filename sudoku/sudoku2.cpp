#include<memory.h>
#include<iostream>
#include<fstream>

using namespace std;

//COL AND ROW
#define N 9
//one cell COL and ROW
#define M 3
//minimal Map when search the solution
#define MIN_Map 17

//link node
struct node
{
    char row;              //row
    char col;              //col
    char candidate[N];   //candidate array
    int num;             //the candidate num
    struct node* prev;   //the prev node
    struct node* next;   //the next node
};

class Sudoku
{
private:
    char Map[N][N];      //Map
    char solution[N][N];  //solution
    char solution_tmp[N][N];      //tmp solution
    unsigned long num;    //solution num
    unsigned long n;      //nth solution
    struct node *head;    //the head of link
    struct node *end;     //the tail of link
    int flag;             //flag

private:
    // init map link
    bool initMapLink();
    // delete Map
    void delMap();
    // delete Map2
    bool renounce();
    //check repeated
    bool isRepeated(char, int, int, char[N][N]);
    //get solution
    bool recursion(struct node *);

public:
    Sudoku(char Map[N][N]);
    ~Sudoku();
    // init Map
    void initMap(char Map[N][N]);
    // output the result
    char (* output(char output[N][N], bool who = true))[N];
    // check
    int check();
    // solve
    unsigned long solve(unsigned long n = 0ul);
    void showMap(char Map[N][N],int n);
};

int main(int argc,char** argv){
    char Map[N][N];
    if(argc != 2) return 0;
    char buf[BUFSIZ];
    bool readFile = false;

    ifstream ifile;
    ifile.open(argv[1],ios::in);
    int row = 0;
    cout << "Input File Name: " << argv[1] << endl << endl;
    cout << "Board Position" << endl;
    while(ifile.getline(buf,BUFSIZ)){
        readFile = true;
        int j = 0;
        cout << buf << endl;
        for(int i = 0; buf[i] != '\0'; i++){
            if(buf[i] == ' ') continue;
            if(buf[i] == 'X' || buf[i] == 'x')
                Map[row][j] = 0;
            else Map[row][j] = (buf[i] - '0');
            j++;
        }
        row++;
    }

    ifile.close();
    if(readFile == false) return 0;
    // showMap(Map);

    Sudoku* record = new Sudoku(Map);
    record->solve();


    return 0;
}

Sudoku::Sudoku(char Map[N][N])
{
    head = end = nullptr;
    num = n = 0;
    initMap(Map);
}

Sudoku::~Sudoku()
{
    delMap();
}

//init map
void Sudoku::initMap(char Map[N][N] /*= nullptrptr*/)
{
    if (Map)
    {
        memcpy(this->Map, Map, sizeof(this->Map));
        flag = check();
    }
    else
    {
        memset(this->Map, 0, sizeof(this->Map));
        flag = 0;
    }
    memcpy(solution, this->Map, sizeof(solution));
}

char (* Sudoku::output(char output[N][N], bool who /*= true*/))[N]
{
    if(who)
        memcpy(output, solution, sizeof(solution));
    else
        memcpy(output, Map, sizeof(Map));
    return output;
}

//-1 illega number -2:sudoku not initial -3 repeat
int Sudoku::check()
{
    int valid_cell_num = 0;
    // if (this->Map == nullptr)
    //     return -2;
    for (int i=0; i<N; i++)
    {
        for (int j=0; j<N; j++)
        {
            if(Map[i][j]<0 || Map[i][j]>N)
                return -1;
            if(Map[i][j]>0 && Map[i][j]<=N)
            {
                if(isRepeated(Map[i][j],i,j,Map))
                    return -3;
                valid_cell_num++;
            }
        }
    }
    if (valid_cell_num>=MIN_Map)
        return 1;    //if the number of cell which large than 0 >= MIN_Map RETURN TRUE 
    return 0;
}

bool Sudoku::initMapLink()
{
    struct node *tmp_node = nullptr;
    head = end = new struct node;
    head->prev = nullptr;
    head->next = nullptr;
    for (int i=0; i<N; i++)
    {
        for (int j=0; j<N; j++)
        {
            if(Map[i][j] != 0)
                continue;
            tmp_node = new struct node;
            tmp_node->row = i;
            tmp_node->col = j;
            tmp_node->num = N;
            for(int k=0; k<N; k++)
                tmp_node->candidate[k] = k+1;
            tmp_node->prev = end;
            tmp_node->next = nullptr;
            end->next = tmp_node;
            end = tmp_node;
        }
    }
    tmp_node = head;
    head = head->next;
    delete tmp_node;
    if(head == nullptr)
        return false;
    else
        head->prev = nullptr;
    return true;
}

void Sudoku::delMap()
{
    struct node *p = head;
    while(head)
    {
        p = head->next;
        delete head;
        head = p;
    }
    end = nullptr;
}

//delete link
bool Sudoku::renounce()
{
    struct node *tmp_node = head;
    char rowp, colp;
    while(tmp_node)
    {
        rowp = tmp_node->row;
        colp = tmp_node->col;
        //line delete
        for (int j=0; j<N; j++)
        {
            if(Map[rowp][j] && tmp_node->candidate[Map[rowp][j]-1])
            {
                tmp_node->candidate[Map[rowp][j]-1] = 0;
                tmp_node->num--;
            }
        }
        //col delete
        for (int i=0; i<N; i++)
        {
            if(Map[i][colp] && tmp_node->candidate[Map[i][colp]-1])
            {
                tmp_node->candidate[Map[i][colp]-1] = 0;
                tmp_node->num--;
            }
        }
        //cell delete
        for (int i=rowp-rowp%M; i<rowp-rowp%M+M; i++)
        {
            for (int j=colp-colp%M; j<colp-colp%M+M; j++)
                if(Map[i][j] && tmp_node->candidate[Map[i][j]-1])
                {
                    tmp_node->candidate[Map[i][j]-1] = 0;
                    tmp_node->num--;
                }
        }

        if(tmp_node->num<=0)
            return false;
        tmp_node=tmp_node->next;
    }
    return true;
}

//check is repeat or not
bool Sudoku::isRepeated(char a, int rowp, int colp, char array[N][N])
{
    //check row
    for (int j=0; j<N; j++)
    {
        if(array[rowp][j] == a && j!=colp)
            return true;
    }
    //check col
    for (int i=0; i<N; i++)
    {
        if(array[i][colp] == a && i!=rowp)
            return true;
    }
    //check cell
    for (int i=rowp-rowp%M; i<rowp-rowp%M+M; i++)
    {
        for (int j=colp-colp%M; j<colp-colp%M+M; j++)
            if(array[i][j] == a && i!=rowp && j!=colp)
                return true;
    }
    return false;
}


void Sudoku::showMap(char Map[9][9],int n){
    cout << endl;
    cout << "Solution #" << n << endl;
    cout << "Board Position" << endl;
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            cout << (int)Map[i][j];
            if(j < 8) cout << " ";
        }
        cout << endl;
    }
}

//get all solution when n == 0
//else get the nth solution
bool Sudoku::recursion(struct node *tmp_node)
{
    // terminal condition
    if(tmp_node==nullptr)
    {
        // solution number + 1
        num++;
        // solution
        memcpy(solution, solution_tmp, sizeof(solution));
        showMap(solution,num);
        // determine this is the nth solution
        if(num == n)
        {
            return true;
        }
        // find next solution
        else
            return false;
    }
    // recurive start
    for(int i=0; i<N; i++)
    {
        if(tmp_node->candidate[i]==0)
            continue;
        if(isRepeated(tmp_node->candidate[i],tmp_node->row,tmp_node->col,solution_tmp))
            continue;
        solution_tmp[tmp_node->row][tmp_node->col]=tmp_node->candidate[i];
        if(recursion(tmp_node->next))
            return true;
        else
            solution_tmp[tmp_node->row][tmp_node->col]=0;
    }
    // backtrace
    return false;
}

//if n is 0 return all solution
unsigned long Sudoku::solve(unsigned long n)
{
    //1 success else not valid
    if (flag < 0)
        return 0;
    //initial link
    if(!initMapLink())
        return 0;
    //delte link
    renounce();
    //empty solution num
    num = 0;
    this->n = n;
    //initial solution status
    memcpy(solution_tmp, Map, sizeof(solution_tmp));
    memcpy(solution, Map, sizeof(solution));
    //get the solution
    recursion(head);
    //get the number of solution
    return num;
}
