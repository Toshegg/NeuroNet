#include <iostream>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <ctime>
using namespace std;

class Net {
      
      int *X, a1_num, a2_num, x_num, number; //массив входных чисел, размер массива А1, размер массива А2, размер массива Х, число, которое должно получиться
      double **W1, **W2, **W3, *A1[2], *A2[2], R[10][2];//Массив весов между Х и А1 слоем, между А1 и А2, между А2 и R
      
      public:
             Net (int x, int a1, int a2, char * input, char * weight_1, char * weight_2, char * weight_3)
             {
                 ifstream inp(input); 
                 
                 a1_num = a1;
                 a2_num = a2;
                 x_num = x;
                
                 X = new int[x];
                
                 for (int i = 0; i < 2; i++)
                     {
                          A1[i] = new double[a1];
                          A2[i] = new double[a2];
                          for (int j = 0; j < 10; j++)
                            R[j][i] = 0;
                     }
                 for (int i = 0; i < a1_num; i++)
                     for (int j = 0; j < 2; j++)
                     A1[j][i] = 0;
                 for (int i = 0; i < a2_num; i++)
                     for (int j = 0; j < 2; j++)
                         A2[j][i] = 0;
                
                 W1 = new double*[x];
                 for (int i = 0; i < x; i++) W1[i] = new double[a1];
                 W2 = new double*[a1];
                 for (int i = 0; i < a1; i++) W2[i] = new double[a2];
                 W3 = new double*[a2];
                 for (int i = 0; i < a2; i++) W3[i] = new double[10];
                 
                 inp >> number;
                 for (int i = 0; i < x; i++) inp >> X[i]; //получаем входные данные
                 
                 if (!FileExists(weight_1)) create(weight_1, x, a1); //проверяем, созданы ли файлы с весами
                 if (!FileExists(weight_2)) create(weight_2, a1, a2);
                 if (!FileExists(weight_3)) create(weight_3, a2, 10);
                 
                 ifstream w1(weight_1), w2(weight_2), w3(weight_3);
                 
                 for (int i = 0; i < x; i++)  //вводим веса
                     for (int j = 0; j < a1; j++)
                         w1 >> W1[i][j];
                 for (int i = 0; i < a1; i++)
                     for (int j = 0; j < a2; j++)
                         w2 >> W2[i][j];
                 for (int i = 0; i < a2; i++)
                    for (int j = 0; j < 10; j++)
                         w3 >> W3[i][j];
                    inp.close();
                    w1.close();
                    w2.close();
                    w3.close();
             }
             void create (char * file, int param_1, int param_2) //создание файла с весами
             {
                  ofstream out (file);
                  for (int i = 0; i < param_1*param_2; i++)
                  out << (double) (rand () % 1000) / 1000 << ' ';
                  out.close();
             }
                      
             bool FileExists(const char *fname) //проверка файла на существование
             {
			       return ifstream(fname) != NULL;
             }
             
             double Activation (double e) { //функция активации
                 return (double)(1/(1+exp(-e)));
             }
             
             double DifActivation (double e) { //дифференцированная функция активации (нах не нужна, но пусть будет)
                    return Activation(e)*(1-Activation(e));       
             }
             
             void counting(int layer) { //подсчет значений функции каждого нейрона в зависимости от слоя
                     switch (layer) {
                            case 1:
                                 for (int i = 0; i < a1_num; i++)
                                     for (int j = 0; j < x_num; j++)
                                         A1[0][i] += X[j]*W1[j][i];
                                 for (int i = 0; i < a1_num; i++) A1[0][i] = Activation(A1[0][i]);
                               
                                 break;
                            case 2:
                                 for (int i = 0; i < a2_num; i++)
                                     for (int j = 0; j < a1_num; j++)
                                         A2[0][i] += A1[0][j]*W2[j][i];
                                 for (int i = 0; i < a2_num; i++) A2[0][i] = Activation(A2[0][i]);
                               
                                 break;
                            case 3:
                                 for (int i = 0; i < 10; i++) 
                                    {
                                        for (int j = 0; j < a2_num; j++)
                                            R[i][0] += A2[0][j]*W3[j][i];
                                        R[i][0] = Activation (R[i][0]);
                                    }
                               
                                 break;
                            }         
                  }
            void delta () { //подсчет коэффициента дельта для каждого нейрона
                for (int i = 0; i < 10; i++) 
                {
                    if (i == number) {
                        R[i][1] = 1-R[i][0];
                        continue;
                        } 
                    R[i][1] = 0 - R[i][0];
                }
                for (int i = 0; i < a2_num; i++)
                    for (int j = 0; j < 10; j++)
                        A2[1][i] += W3[i][j]*R[j][1];
                for (int i = 0; i < a1_num; i++)
                    for (int j = 0; j < a2_num; j++)
                        A1[1][i] += W2[i][j]*A2[1][j];
            }

            void learning (double n) { // обучение сети
                for (int i = 0; i < x_num; i++)
                    for (int j = 0; j < a1_num; j++)
                        W1[i][j] += n*A1[0][j]*(1-A1[0][j])*A1[1][j]*X[i];
                for (int i = 0; i < a1_num; i++)
                    for (int j = 0; j < a2_num; j++)
                        W2[i][j] += n*A2[0][j]*(1-A2[0][j])*A2[1][j]*A1[0][i];
                for (int i = 0; i < a2_num; i++)
                    for (int j = 0; j < 10; j++)
                        W3[i][j] += n*R[j][0]*(1-R[j][0])*R[j][1]*A2[0][i];    

            }
            void saving_results (char * file1, char * file2, char * file3) //сохранение результатов в файл
            {
                //remove(file1);
                //remove(file2);
                //remove(file3);
                ofstream out1(file1), out2(file2), out3(file3);
                for (int i = 0; i < x_num; i++)
                    for (int j = 0; j < a1_num; j++)
                         out1 << W1[i][j] << ' ';
                for (int i = 0; i < a1_num; i++)
                    for (int j = 0; j < a2_num; j++)
                         out2 << W2[i][j] << ' ';
                for (int i = 0; i < a2_num; i++)
                    for (int j = 0; j < 10; j++)
                         out3 << W3[i][j] << ' ';    
                out1.close();
                out2.close();
                out3.close();
            }

            bool answer_is_correct() { //правилен ли ответ?
                int ans;
                double max = -10000;
                for (int i = 0; i < 10; i++)
                    if (R[i][0] > max) {
                        max = R[i][0];
                        ans = i;
                    }
                return ans==number;
            }
            void give_me_answer_please() {
                 int ans;
                 double max = -10000;
                 for (int i = 0; i < 10; i++)
                    if (R[i][0] > max) {
                        max = R[i][0];
                        ans = i;
                    }
                 cout << "Number: " << ans << '\n';
                 }
            void print_x () {
                for (int i = 0; i < x_num; i++)
                    cout << X[i] << ' ';
            }
            void print_w1 () {
                 for (int i = 0; i < x_num; i++)
                 {
                     for (int j = 0; j < a1_num; j++)
                         cout << W1[i][j] << ' ';
                     cout << '\n';
                         }
                 }
            void print_num() {
                 cout << number;
                 }
            void print_a1 () {
                 for (int i = 0; i < a1_num; i++)
                    cout << A1[0][i] << ' ';
                 }
            void print_a2 () {
                 for (int i = 0; i < a2_num; i++)
                    cout << A2[0][i] << ' ';
                 }
            void print_r () {
                 for (int i = 0; i < 10; i++)
                    cout << R[i][0] << ' ';
                 }
            void print_delta_r () {
                 for (int i = 0; i < 10; i++)
                    cout << R[i][1] << ' ';
                 }
            void print_delta_a1 () {
                 for (int i = 0; i < a1_num; i++)
                    cout << A1[1][i] << ' ';
                 }
            void print_delta_a2 () {
                 for (int i = 0; i < a2_num; i++)
                    cout << A2[1][i] << ' ';
                 }
            void set_to_zero () {
                 for (int i = 0; i < a1_num; i++)
                     for (int j = 0; j < 2; j++)
                         A1[j][i] = 0;
                 for (int i = 0; i < a2_num; i++)
                     for (int j = 0; j < 2; j++)
                         A2[j][i] = 0;
                 for (int i = 0; i < 10; i++)
                     for (int j = 0; j < 2; j++)
                         R[i][j] = 0;
                 }
};

int main ()
{
    Net n (20, 10, 15, "one.txt", "weight_1.txt", "weight_2.txt", "weight_3.txt");
    double nu = 1000;
    for (int i = 0; i < 3; i++)
        n.counting(i+1);
        n.print_w1();     
    
    if (!n.answer_is_correct()) {
        n.delta();
        n.learning(nu);
        n.saving_results("weight_1.txt", "weight_2.txt", "weight_3.txt");
        n.print_w1();   
    }
    
    n.set_to_zero();
    for (int i = 0; i < 3; i++)
        n.counting(i+1);
    n.give_me_answer_please();
    
    system ("pause");
    return 0;
    }
