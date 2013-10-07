#include <iostream>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <ctime>
using namespace std;

class Net {
      
      int *X, a1_num, a2_num, x_num;
      double **W1, **W2, *W3, *A1[2], *A2[2], R[2];
      
      public:
             Net (int x, int a1, int a2, char * input, char * weight_1, char * weight_2, char * weight_3)
             {
                 ifstream inp(input), w1(weight_1), w2(weight_2), w3(weight_3);
                 
                 a1_num = a1;
                 a2_num = a2;
                 x_num = x;
                
                 X = new int[x];
                
                 for (int i = 0; i < 2; i++)
                     {
                          A1[i] = new double[a1];
                          A1[i] = 0;
                          A2[i] = new double[a2];
                          A2[i] = 0;
                          R[i] = 0;
                     }
                
                 W1 = new double*[x];
                 for (int i = 0; i < x; i++) W1[i] = new double[a1];
                 W2 = new double*[a1];
                 for (int i = 0; i < a1; i++) W2[i] = new double[a2];
                 W3 = new double[a2];
                 
                 for (int i = 0; i < x; i++) inp >> X[i];
                 
                 if (!FileExists(weight_1)) create(weight_1, x, a1);
                 if (!FileExists(weight_2)) create(weight_2, a1, a2);
                 if (!FileExists(weight_3)) create(weight_3, a2);
                 
                 for (int i = 0; i < x; i++)
                     for (int j = 0; j < a1; j++)
                         w1 >> W1[i][j];
                 for (int i = 0; i < a1; i++)
                     for (int j = 0; j < a2; j++)
                         w2 >> W2[i][j];
                 for (int i = 0; i < a2; i++)
                         w3 >> W3[i];
             }
             void create (char * file, int param_1, int param_2)
             {
                  ofstream out (file);
                  for (int i = 0; i < param_1*param_2; i++)
                  out << (double) (rand () % 1000) / 1000 << ' ';
             }
             void create (char * file, int param)
             {
                  ofstream out (file);
                  for (int i = 0; i < param; i++)
                  out << (double) (rand () % 1000) / 1000 << ' ';
             }
             
             bool FileExists(const char *fname)
             {
			       return ifstream(fname) != NULL;
             }
             
             double Activation (double e) {
                 return 1/(1-exp(-e));
             }
             
             double DifActivation (double e) {
                    return Activation(e)*(1-Activation(e));       
             }
             
             void counting(int layer) {
                     switch (layer) {
                            case 1:
                                 for (int i = 0; i < a1_num; i++)
                                     for (int j = 0; j < x_num; i++)
                                         A1[i][0] += X[j]*W1[j][i];
                                 for (int i = 0; i < a1_num; i++) A1[i][0] = Activation(A1[i][0]);
                               
                                 break;
                            case 2:
                                 for (int i = 0; i < a2_num; i++)
                                     for (int j = 0; j < a1_num; i++)
                                         A2[i][0] += A1[j][0]*W2[j][i];
                                 for (int i = 0; i < a2_num; i++) A2[i][0] = Activation(A2[i][0]);
                               
                                 break;
                            case 3:
                                 for (int i = 0; i < a2_num; i++) R[0] += A2[i][0]*W3[i];
                                 R[0] = Activation (R[0]);
                               
                                 break;
                            }         
                  }
};

int main ()
{
    return 0;
    }
