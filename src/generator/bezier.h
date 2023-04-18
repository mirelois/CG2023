#include <vector>
#include <tuple>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

vector<tuple<float,float,float>>* generate_bezier(char *fileName, int tessellationLevel);
