//
//  main.cpp
//  LSXCipher
//
//  Created by Георгий Джаниашвили on 17/03/2021.
//  Copyright © 2021 Георгий Джаниашвили. All rights reserved.
//

#include <iostream>
#include <random>
#include <thread>
#define n 20 //размер блока в битах
#define l 5 //k число s боксов
#define m 4 //b битовый размер s бокса


std::random_device rd;
std::mt19937 g(rd());
//std::atomic<bool> complete;
uint16_t numbers[16] = {0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf};
uint16_t gf16[16][16] = { //неприводимый x^4+x+1
    {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},
    {0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf},
    {0x0, 0x2, 0x4, 0x6, 0x8, 0xa, 0xc, 0xe, 0x3, 0x1, 0x7, 0x5, 0xb, 0x9, 0xf, 0xd},
    {0x0, 0x3, 0x6, 0x5, 0xc, 0xf, 0xa, 0x9, 0xb, 0x8, 0xd, 0xe, 0x7, 0x4, 0x1, 0x2},
    {0x0, 0x4, 0x8, 0xc, 0x3, 0x7, 0xb, 0xf, 0x6, 0x2, 0xe, 0xa, 0x5, 0x1, 0xd, 0x9},
    {0x0, 0x5, 0xa, 0xf, 0x7, 0x2, 0xd, 0x8, 0xe, 0xb, 0x4, 0x1, 0x9, 0xc, 0x3, 0x6},
    {0x0, 0x6, 0xc, 0xa, 0xb, 0xd, 0x7, 0x1, 0x5, 0x3, 0x9, 0xf, 0xe, 0x8, 0x2, 0x4},
    {0x0, 0x7, 0xe, 0x9, 0xf, 0x8, 0x1, 0x6, 0xd, 0xa, 0x3, 0x4, 0x2, 0x5, 0xc, 0xb},
    {0x0, 0x8, 0x3, 0xb, 0x6, 0xe, 0x5, 0xd, 0xc, 0x4, 0xf, 0x7, 0xa, 0x2, 0x9, 0x1},
    {0x0, 0x9, 0x1, 0x8, 0x2, 0xb, 0x3, 0xa, 0x4, 0xd, 0x5, 0xc, 0x6, 0xf, 0x7, 0xe},
    {0x0, 0xa, 0x7, 0xd, 0xe, 0x4, 0x9, 0x3, 0xf, 0x5, 0x8, 0x2, 0x1, 0xb, 0x6, 0xc},
    {0x0, 0xb, 0x5, 0xe, 0xa, 0x1, 0xf, 0x4, 0x7, 0xc, 0x2, 0x9, 0xd, 0x6, 0x8, 0x3},
    {0x0, 0xc, 0xb, 0x7, 0x5, 0x9, 0xe, 0x2, 0xa, 0x6, 0x1, 0xd, 0xf, 0x3, 0x4, 0x8},
    {0x0, 0xd, 0x9, 0x4, 0x1, 0xc, 0x8, 0x5, 0x2, 0xf, 0xb, 0x6, 0x3, 0xe, 0xa, 0x7},
    {0x0, 0xe, 0xf, 0x1, 0xd, 0x3, 0x2, 0xc, 0x9, 0x7, 0x6, 0x8, 0x4, 0xa, 0xb, 0x5},
    {0x0, 0xf, 0xd, 0x2, 0x9, 0x6, 0x4, 0xb, 0x1, 0xe, 0xc, 0x3, 0x8, 0x7, 0x5, 0xa}
};

int** create_zero_table(int rows, int columns){
    int** a = new int*[rows];

    for(int i = 0; i < rows; i++){
        a[i] = new int[columns];
        for(int j = 0; j < columns; j++){
            a[i][j] = 0;
        }
    }
    return a;
}

void free_table(int** a, int rows){
    if(a){
        for(int i = 0; i < rows; i++){
            if(a[i]){
                delete[] a[i];
            }
        }
        delete[] a;
    }
}

void print_table(int** table, int rows, int columns){
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < columns; j++){
            std::cout << " " << table[i][j];
        }
        std::cout << "\n";
    }
}

int* create_x () {

    int* x = new int[l];

    std::shuffle(std::begin(numbers), std::end(numbers), g);
    std::cout << "x ";
    for(int i = 0; i < l; i++){
        x[i] = numbers[i];
        std::cout << x[i] << ' ';
    }
    
    std::cout << "\n";

    return x;

}

std::vector<int> create_s_box () {

    std::vector<int> s_box;

    std::shuffle(std::begin(numbers), std::end(numbers), g);
    std::cout << "sbox ";
    //random s_box
    for(int i = 0; i < 16; i++) {
        s_box.push_back(numbers[i]);
        std::cout << s_box[i] << ' ';
    }

    return s_box;

}

std::vector<int> inverse_s_box (std::vector<int> s_box) {
    
    std::vector<int> s_box_inv;

    //int i = 0;
    for(int i = 0; i < 16; i++) {
        for(int j = 0; j < 16; j++) {
            if(s_box[j] == i) {
                s_box_inv.push_back(j);
            }
        }
    }
    std::cout << "sbox inv ";
    for(int i = 0; i < 16; i++) {
        std::cout << s_box_inv[i] << ' ';
    }
    
    return s_box_inv;
    
}


int* s_box_sub (int* x, std::vector<int> s_box){

    int* x_after_s = new int[l];
    
    std::cout << "x after s ";
    for(int i = 0; i < l; i++){
        for(int  j = 0; j < 16; j++){
            if(x[i] == j) {
                x_after_s[i] = s_box[j];
            }
        }
        std::cout << x_after_s[i] << ' ';
    }
    
    return x_after_s;

}

int** create_key(int number_of_l_rounds){
    
    int** key_list = create_zero_table(number_of_l_rounds, l);
    
    for(int i = 0; i < number_of_l_rounds; i++){
        for(int j = 0; j < l; j++) {
        std::uniform_int_distribution<int> gen(0, 15); // uniform, unbiased
        int r = gen(g);
            key_list[i][j] = r;
        }
    }
    
    return key_list;
    
}

int* xor_x_and_key(int* x, int** key_list, int round){
    
    for(int i = 0; i < l; i++){
        x[i] ^= key_list[round][i];
    }
    
    return x;
    
}

//find inverse element, a * a^(-1) = b
int find_div_gf16(int a, int b){
    int k;
    for(int i = 0; i < 16; i++){
        if(gf16[a][i] == b) {
            k = i;
            break;
        }
    }
    return k;
}

int** gaussian_elimination_gf16(int** a){

    //upper triangular
    for(int i = 0; i < l-1; i++){
        for(int j = i+1; j < l; j++){
            int c = find_div_gf16(a[i][i], a[j][i]);
            for(int k = 0; k < 2*l; k++){
                a[j][k] = gf16[c][a[i][k]] ^ a[j][k];
            }
        }
    }
    
    //lower and upper triangular matrix
    for(int i = l - 1; i > 0; i--){
        for(int j = i-1; j >= 0; j--){
            int c = find_div_gf16(a[i][i], a[j][i]);
            for(int k = 2*l - 1; k > 0; k--){
                    a[j][k] = gf16[c][a[i][k]] ^ a[j][k];
            }
        }
    }
    
    //identity matrix
    for(int i = 0; i < l; i++){
        int c = find_div_gf16(a[i][i], 1);
        for(int j = 0; j < 2*l; j++){
            a[i][j] = gf16[c][a[i][j]];
        }
    }
    
    return a;
    
}

int expo_gf16(int number, int power){
    if(power == 1){
        return number;
    }
    else {
        int number_tmp = number;
        for(int i = 0; i < power - 1; i ++){
            number_tmp = gf16[number][number_tmp];
        }
        number = number_tmp;
    }
    return number;
}

int** create_l_table_over_gf16(){
    
    std::vector<int> v;
    std::vector<int> a;

    //create v
    for(int i = 0; i < 2*l; i++) {
        std::uniform_int_distribution<int> gen(1, 15); // uniform, unbiased
        int r = gen(g);
        v.push_back(r);
    }
        std::cout << "\n";
        std::cout << "v ";
    for(int i = 0; i < 2*l; i++){
        std::cout << v[i] << " ";
    }

    std::cout << "\n";
    std::cout << "\n";

    //create a
    std::shuffle(std::begin(numbers), std::end(numbers), g);
    for(int i = 0; i < 2*l; i++) {
        a.push_back(numbers[i]);
    }
    std::cout << "a ";
    for(int i = 0; i < 2*l; i++){
        std::cout << a[i] << " ";
    }
    
    std::cout << "\n";
    std::cout << "\n";
    //init matrix G and L
    int** G;
    int** L;
    G = create_zero_table(l, 2*l);
    L = create_zero_table(l, l);
    for(int i = 0; i < l; i++){
        for(int j = 0; j < 2*l; j++){
            if(i == 0)
                G[i][j] = v[j];
            else {
                G[i][j] = gf16[v[j]][expo_gf16(a[j], i)];
            }
        }
    }
    
    print_table(G, l, 2*l);
    std::cout << "\n";
    
    G = gaussian_elimination_gf16(G);
    
    //заполняем L
    for(int i = 0; i < l; i++){
        for(int j = 0; j < l; j++){
            L[i][j] = G[i][j+l];
        }
    }
    
    print_table(G, l, 2*l);
    std::cout << "\n";
    print_table(L, l, l);
    
    free_table(G, l);
    //free_table(L, l);
    
    return L;
    
}

int** inverse_l_table(int** a){

    int** inv = create_zero_table(l, 2*l);
    
    //init fill
    for(int i = 0; i < l; i++){
        for(int j = 0; j < l; j++){
            inv[i][j] = a[i][j];
        }
    }
    for(int i = 0; i < l; i++){
        inv[i][i+5] = 1;
    }
    
    //left and right upper triangular
    for(int i = 0 ; i < l-1; i++){
        int above = find_div_gf16(inv[i][i], 1);
        for(int z = 0; z < 2*l; z++)
            inv[i][z] = gf16[inv[i][z]][above];
        for(int j = i+1; j < l; j++){
            int below = inv[j][i];
            for(int k = 0; k < 2*l; k++){
                int tmp = gf16[inv[i][k]][below];
                inv[j][k] ^= tmp;

            }
        }
    }
    
    //left identity matrix right invers l_table
    for(int i = l-1 ; i > 0; i--){
        int below = find_div_gf16(inv[i][i], 1);
        for(int z = 2*l - 1; z > 0; z--)
            inv[i][z] = gf16[inv[i][z]][below];
        for(int j = i-1; j >= 0; j--){
            int above = inv[j][i];
            for(int k = 2*l-1; k > 0; k--){
                int tmp = gf16[inv[i][k]][above];
                inv[j][k] ^= tmp;

            }
        }
    }
    
    int** inv_true = create_zero_table(l, l);
    
    for(int i = 0; i < l; i++){
        for(int j = 0; j < l; j++){
            inv_true[i][j] = inv[i][j+l];
        }
    }
    
    print_table(inv_true, l, l);

    return inv_true;
}

int* mul_matrix (int* x, int** l_table){

    //init
    int* x_after_l_box  = new int[l];
    for(int i = 0; i < l; i++) {
        x_after_l_box[i] = 0;
    }

    for(int  i = 0; i < l; i++) {
        for(int j = 0; j < l; j++){
            int k = gf16[x[j]][l_table[i][j]];
            //std::cout << " l" << l_table[i][j];
            x_after_l_box[i] = x_after_l_box[i] ^ k;
        }
    }

    return x_after_l_box;
}

int** mul_matrix(int** l_table, int** l_inv){

    int** mult = create_zero_table(l, l);

    // Multiplying matrix a and b and storing in array mult.
    for(int i = 0; i < l; ++i)
        for(int j = 0; j < l; ++j)
            for(int k = 0; k < l; ++k) {
                mult[i][j] ^= gf16[l_table[i][k]][l_inv[k][j]];
            }
    return mult;
}

int main(int argc, const char * argv[]) {
    
    //init
    int* x = create_x();
    int** l_table = create_l_table_over_gf16();
    std::cout << '\n';
    int** l_table_inv = inverse_l_table(l_table);
    int** a = mul_matrix(l_table, l_table_inv);
    std::cout << '\n';
    print_table(a, l, l);
    std::vector<int> s_box = create_s_box();
    std::vector<int> s_box_inv = inverse_s_box(s_box);
    int** key_list = create_key(1);
    
   x = xor_x_and_key(x, key_list, 0);
    
    x = mul_matrix(x, l_table);
    for(int i = 0; i < l; i++)
        std::cout << x[i] << ' ';
    std::cout << '\n';
    
    x = s_box_sub(x, s_box);
    
    //шт
    
    x = s_box_sub(x, s_box_inv);
    
    x = mul_matrix(x, l_table_inv);
    
    x = xor_x_and_key(x, key_list, 0);
    
    std::cout << '\n';
    std::cout << '\n';
    for(int i = 0; i < l; i++)
        std::cout << x[i] << ' ';
    std::cout << '\n';
    
    //генерировать ключ и подмешивать в шифрование
    
    
    /*int A[64] = {0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f};*/
    
//    int* x = create_x();
//    std::cout<<"\n";
//    std::vector<uint16_t> s_box = create_s_box();
//    std::cout<<"\n";
//    int* x_after_s = x_s_box_sub(x, s_box);
//
//    int** a;
    //int** b;
    //int** c;
    
    //std::thread th1(create_l_table, a);
    //std::thread th2(create_l_table, b);
    //std::thread th3(create_l_table, c);
    
//    complete = false;
//    a = create_l_table();
    
    //print_table(a, n, n);
    
    //th1.join();
    //th2.join();
    //th3.join();
    
    //перемножение x на L
    //mul_matrix(x_after_s, a);
    
//    free_table(a, n);
//    delete [] x;
//    delete [] x_after_s;
    
    return 0;
}

//int** create_table(int rows, int columns){
//    int** a = new int*[rows];
//
//    for(int i = 0; i < rows; i++){             //fill random 0,1
//        a[i] = new int[columns];
//        for(int j = 0; j < columns; j++){
//            std::uniform_int_distribution<int> gen(0, 1); // uniform, unbiased
//            int r = gen(g);
//            a[i][j] = r;
//        }
//    }
//    return a;
//}

//bool is_upper_triangular_matrix(int** a) {
//
//    for(int i = 0; i < n; i++)
//        if(a[i][i] == 0)
//            return false;
//
//    for (int i = 1; i < n; i++)
//        for (int j = 0; j < i; j++)
//            if (a[i][j] != 0)
//                return false;
//    return true;
//}
//
//int** gaussian_elemination_for_zero_and_one(int** a) {
//
//    std::vector<int> v;
//
//    for(int i = 0; i < n; i++) {  //перевод каждой строки в int
//        std::string s = "";
//        for(int j = 0; j < n; j++) {
//            s += std::to_string(a[i][j]);
//        }
//        int k = std::stoi(s, nullptr, 2);
//        v.push_back(k);
//    }
//
//    sort(v.begin(), v.end(), std::greater<int>()); //сортировка
//
//    for(int i = 0; i < n; i++){           //заполняем матрицу в отсортированным виде
//        for(int j = 0; j < n; j++){
//            std::string s = std::bitset<n>(v[i]).to_string();
//            a[i][j] = s.at(j) - '0';
//        }
//    }
//
//    for(int i = 0; i < n; i++) {          //гаус приведение к треугольному виду
//        for(int j = i + 1; j < n; j++){
//            if(a[j][i] == 1){
//                for (int k = i; k < n; k++)
//                    a[j][k] ^= a[i][k];
//            }
//        }
//    }
//
//    return a;
//
//}
//
//int* create_x () {
//
//    int* x = new int[l];
//
//    std::shuffle(std::begin(numbers), std::end(numbers), g);
//    std::cout << "x ";
//    for(int i = 0; i < l; i++){
//        x[i] = numbers[i];
//        std::cout << x[i] << ' ';
//    }
//
//    return x;
//
//}
//
//std::vector<uint16_t> create_s_box () {
//
//    std::vector<uint16_t> s_box;
//
//    std::shuffle(std::begin(numbers), std::end(numbers), g);
//    std::cout << "sbox ";
//    //random s_box
//    for(int i = 0; i < 16; i++) {
//        s_box.push_back(numbers[i]);
//        std::cout << s_box[i] << ' ';
//    }
//
//    return s_box;
//
//}
//
//
//int* s_box_sub (int* x, std::vector<uint16_t> s_box){
//
//    int* x_after_s = new int[l];
//    std::vector<int> v;
//    int z = 0;
//
//    for(int i = 0; i < l; i++) {
//        std::string s = "";
//        for(int j = z; j < z + m; j++) {
//            s += std::to_string(x[j]);
//        }
//        z = z + m;
//        int k = std::stoi(s, nullptr, 2);
//        v.push_back(k);
//    }
//
//    std::cout << "x after s ";
//    for(int  i = 0; i < l; i++){
//        x_after_s[i] = s_box.at(v[i]);
//        std::cout << x_after_s[i] << ' ';
//    }
//
//    return x_after_s;
//
//}
//
//int* mul_matrix (int* x, int** l_box){
//
//    std::string s = "";
////    int* x_bit = new int[n];
//    int* x_after_l_box  = new int[n];
//
//    for(int i = 0; i < n; i++) {
//        x_after_l_box[i] = 0;
//    }
//
//    for(int i = 0; i < l; i++){
//        s  = s + std::bitset<m>(x[i]).to_string();
//    }
//
//    for(int  i = 0; i < n; i++) {
//        for(int j = 0; j < n; j++){
//            int k = (s.at(j) - '0') * l_box[j][i];
//            x_after_l_box[i] = x_after_l_box[i] ^ k;
//        }
//    }
//
//    std::cout << "x in bits " << s;
//
//    std::cout << "\n";
//
//    std::cout << "x in bits after lbox ";
//
//    for(int i = 0; i < n; i ++) {
//        std::cout << x_after_l_box[i] << ' ';
//    }
//
//    return x_after_l_box;
//
//}

//int** create_l_table_for_zero_and_one (){
//    int c = 0;
//    int** a;
//    int** b;
//    while(c != 1){
//        if (complete) {
//            break;
//        }
//        a = create_table(n, n);
//        b  = create_zero_table(n, n);
//        for(int i = 0; i < n; i++){
//            for(int j = 0; j < n; j++){
//                b[i][j] = a[i][j];
//            }
//        }
//        a = gaussian_elemination_for_zero_and_one(a);
//        if(is_upper_triangular_matrix(a)){
//            c = 1;
//            complete.store(true);
//            free_table(a, n);
//            break;
//        }
//        else {
//            free_table(a, n);
//            free_table(b, n);
//        }
//    }
//    return b;
//}
//
//int* crypt_asa_for_zero_and_one () {
//
//    int* x = create_x();
//    std::cout<<"\n";
//    std::vector<uint16_t> s_box = create_s_box();
//    std::cout<<"\n";
//    int** l_box;
//
//    complete = false;
//    //l_box = create_l_table();
//
//    x = mul_matrix(x, l_box);
//    std::cout<<"\n";
//    x = s_box_sub(x, s_box);
//
//
//    complete = false;
//    //l_box = create_l_table();
//    std::cout<<"\n";
//
//    x = mul_matrix(x, l_box);
//    std::cout<<"\n";
//
//    free(l_box);
//
//    return x;
//
//}
