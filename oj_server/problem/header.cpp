#include<bits/stdc++.h>
using namespace std;

ostream& operator<<(ostream& out, const vector<int>& v)
{
    out << '[';
    for(size_t i = 0; i < size(v); ++i) out << v[i] << ",]"[i == size(v)-1];
    return out;
}

template<typename...Args>
void input(Args...args)
{
    int _[]{(cout << args << ' ', 0)...};
    cout << '\n';
}

template<typename T1, typename T2, typename...Args>
bool judge(T1&& res, T2&& ans, Args...args)
{
    if(res == ans) return true;
    cout << "Wrong Answer!\n";
    cout << "   input:  "; input(args...);
    cout << "   output: " << res << '\n';
    cout << "   expect: " << ans << '\n';
    return false;
}
