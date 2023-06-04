#include <vector>
#include <tuple>
#include <fstream>
#include <string>
#include <sstream>
#include <map>

using namespace std;

tuple<vector<float>*,vector<float>*, vector<float>*, vector<unsigned int>*> generate_bezier(char *fileName, float tessellationLevel);
