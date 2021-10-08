//
//  main.cpp
//  sudoku_IE523
//
//  Created by Zhiyi Ye on 9/12/20.
//  Copyright © 2020 zhiyi_uiuc. All rights reserved.
//

#include<iostream>
#include<fstream>

using namespace std;

bool check(int Map[9][9],int row,int col,int n){
    for(int i = 0; i < 9; i++){
        if((Map[row][i] == n && i != col) || (Map[i][col] == n && i != row)){
            return false;  //check row and col
        }
    }

    int start_row = (row / 3) * 3;
    int start_col = (col / 3) * 3;
    for(int i = start_row; i < start_row + 3; i++){
        for(int j = start_col; j < start_col + 3; j++){
            if(i == row && j == col) continue;
            if(Map[i][j] == n){
                return false;
            }
        }
    }

    return true;
}

void showMap(int Map[9][9]){
    cout << "Final Solution" << endl;
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            cout << Map[i][j];
            if(j < 8) cout << " ";
        }
        cout << endl;
    }
}

bool solve(int Map[9][9],int row,int col){
    bool solved = true;
    int i,j;
    for(i = 0; i < 9; i++){
        for(j = 0; j < 9; j++){
            if(Map[i][j] == 0){
                solved = false;
                break;
            }
        }
        if(solved == false) break;
    }

    if(solved){ //get Solution
        showMap(Map);
        return true;
    }

    for(int k = 1; k <= 9; k++){
        Map[i][j] = k;  //assign value

        if(check(Map,i,j,k) == true && solve(Map,i,j) == true){
            return true;
        }
    }
    Map[i][j] = 0;
    return false;
}

int main(int argc,char** argv){
    int Map[9][9];
    if(argc != 2) return 0;
    char buf[BUFSIZ];

    ifstream ifile;
    ifile.open(argv[1],ios::in);
    int row = 0;
    cout << "Input File Name: " << argv[1] << endl << endl;
    cout << "Initial Board Position" << endl;
    while(ifile.getline(buf,BUFSIZ)){
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

    cout << endl;
    // showMap(Map);
    solve(Map,0,0);
    return 0;
}
